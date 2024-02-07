/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/07 15:39:57 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

/*
	steps for creating contections:

	-	Create socket
	-	Bind socket
	-	Listen socket
	-	Accept and receive data
	-	Disconnect
 */

void createConection(std::string str)
{
	// Create socket
	Socket socketClass;
	Server server;
	int socketVal = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketVal == -1)
		raiseError("error creating socket");

	// Reset socket to reuse address
	int reuseAddr = 1;
	if (setsockopt(socketVal, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) == -1)
	{
		close(socketVal);
		raiseError("Error setting socket option");
	}

	// Bind socket
	sockaddr_in serverScruct;
	serverScruct.sin_family = AF_INET;
	serverScruct.sin_addr.s_addr = INADDR_ANY;
	server.setPort(str);
	server.setName(str);
	server.setRoot(str);
	serverScruct.sin_port = htons(server.getPort());

	if (bind(socketVal, (struct sockaddr *)&serverScruct, sizeof(serverScruct)) == -1)
	{
		close(socketVal);
		raiseError("error binding socket");
	}

	// Listen socket
	if (listen(socketVal, 10) == -1)
	{
		close(socketVal);
		raiseError("error socket listening");
	}

	// Accept and recive data
	std::vector<pollfd> clients;
	while (true)
	{
		server.setActions(str);
		// registering a new client
		sockaddr_in clientAddress;
		socklen_t clientAddrSize = sizeof(clientAddress);
		int clientSocket = accept(socketVal, (struct sockaddr *)&clientAddress, &clientAddrSize);
		if (clientSocket == -1)
			raiseError("error accepting connection");
		else
		{
			pollfd pollFd;
			pollFd.fd = clientSocket;
			pollFd.events = POLLIN;
			clients.push_back(pollFd);
		}

		// polling data from clients
		int pollVal = poll(clients.data(), clients.size(), -1);
		if (pollVal == -1)
			raiseError("error polling data");
		else if (pollVal > 0)
		{
			// iterate through the clients and remove connection if no read value
			std::vector<pollfd>::iterator it = clients.begin();
			while (it != clients.end())
			{
				if (it->revents == POLLIN)
				{
					char buffer[1024];
					int readVal = read(it->fd, buffer, 1024);
					if (readVal == -1)
						raiseError("error reading data");
					else if (readVal == 0)
					{
						close(it->fd);
						it = clients.erase(it);
						continue;
					}
					Request req = parseReq(buffer, socketClass);
					int action = setAction(buffer);
					std::string aux = buffer;
					socketClass.setDirectory(aux.substr(aux.find("/"), aux.find(" HTTP") - aux.find(" ") - 1)); // Now we should check if the action can be performed in the chosen directory, if not thwrow error ¿405?
					socketClass.setActions(server, socketClass.getDirectory(), str);
					socketClass.setForbidden(socketClass.getDirectory(), str);
					std::string act;
					if (action < 3)
						act = socketClass.getActionsArray(action);
					else
						act = "";
					if (act.length() > 0)
					{
						if (!isAllowed(server, act, socketClass.getActions(), socketClass.getForbidden()))
						{
							socketClass.setResponse("<html>\n<head><title>405 Not Allowed</title></head>\n<body>\n<center><h1>405 Not Allowed</h1></center>\n<hr><center>" + server.getName() + "</center>\n</body>\n</html>");
							socketClass.setContentLength(socketClass.getResponse());
							socketClass.setHeader("HTTP/1.1 405 Method Not Allowed\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\n");
						}
						else
							handleRequests(socketClass, buffer, server, str, req);
					}
					else
					{
						socketClass.setResponse("<html>\n<head><title>501 Not Implemented</title></head>\n<body>\n<center><h1>501 Not Implemented</h1></center>\n<hr><center>" + server.getName() + "</center>\n</body>\n</html>");
						socketClass.setContentLength(socketClass.getResponse());
						socketClass.setHeader("HTTP/1.1 501 Not Implemented\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\n\n");
					}
					std::string resp = socketClass.generateHttpResponse();
					int writeVal = write(it->fd, resp.c_str(), resp.length());
					if (writeVal == -1)
						raiseError("error writing data");
					close(it->fd);
					it = clients.erase(it);
					server.emptyActions();
					socketClass.emptyActions();
				}
			}
		}
	}
}

void handleRequests(Socket &socketClass, char *buffer, Server &server, std::string str, Request req)
{
	(void)buffer;
	(void)req;
	socketClass.setAutoIndex(isAutoindex(str, socketClass));
	std::string finalRoute;
	if (socketClass.getDirectory()[0] != '/' || server.getRoot()[server.getRoot().length() - 1] != '/')
		finalRoute = server.getRoot() + socketClass.getDirectory();
	else
		finalRoute = server.getRoot() + socketClass.getDirectory().substr(1, socketClass.getDirectory().length() - 1);
	if (finalRoute[finalRoute.length() - 1] == '/')
		finalRoute.pop_back();
	if (socketClass.getAutoIndex() == true)
	{
		socketClass.generateAutoIndex(server, socketClass.getDirectory(), socketClass);
		socketClass.setContentLength(socketClass.getResponse());
		socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\nContent-Type: text/html; charset=utf-8\n");
	}
	else // ! this code should be changed but it will serve as backup for now
	{
		struct stat s;
		if (stat(finalRoute.c_str(), &s) == 0 && s.st_mode & S_IFREG)
		{
			//exit(0);
			socketClass.setResponse(getFile(finalRoute));
			socketClass.setContentLength(socketClass.getResponse());
			std::string extension = finalRoute.substr(finalRoute.rfind(".") + 1, finalRoute.length() - finalRoute.rfind("."));
			socketClass.setContentType(parseContentType(extension));
			socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\nContent-Type: " + socketClass.getContentType() + "; charset=utf-8\n");

		}
		else if (!access(finalRoute.c_str(), F_OK))
		{
			socketClass.setResponse(getFile(finalRoute));
			socketClass.setContentLength(socketClass.getResponse());
			socketClass.setContentType(parseContentType(req.uri));
			socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\nContent-Type: " + socketClass.getContentType() + "; charset=utf-8\n");
		}
		/* else if (!access(finalRoute.c_str(), F_OK))
		{
			socketClass.setResponse(getFile(finalRoute));
			socketClass.setContentLength(socketClass.getResponse());
			socketClass.setContentType(getContentType("html"));
			socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\ncharset=utf-8\n");
		}
		else if (socketClass.getDirectory().compare("/favicon.ico") == 0)
		{
			socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\ncharset=utf-8\n\n");
			socketClass.setContentType(getContentType("jpeg"));
			socketClass.setResponse(getFile("pages/images/favicon.ico"));
		}
		else if (std::string(buffer).find("GET /info HTTP/1.1") != std::string::npos)
		{
			socketClass.setResponse(getFile("pages/info/geco.html"));
			socketClass.setContentLength(socketClass.getResponse());
			socketClass.setContentType(getContentType("html"));
			socketClass.setHeader("HTTP/1.1 200 OK\nServer: " + server.getName() + "\ncharset=utf-8\n");
		}
		else if (std::string(buffer).find("GET /teapot HTTP/1.1") != std::string::npos)
		{
			socketClass.setResponse(getFile("pages/teapot.html"));
			socketClass.setContentLength(socketClass.getResponse());
			socketClass.setContentType(getContentType("html"));
			socketClass.setHeader("HTTP/1.1 418 I'm a teapot\nServer: " + server.getName() + "\ncharset=utf-8\n");
		} */
		else
		{
			socketClass.setResponse(getFile("pages/error_404.html"));
			socketClass.setContentLength(socketClass.getResponse());
			socketClass.setContentType(parseContentType("html"));
			socketClass.setHeader("HTTP/1.1 404 Not Found\nServer: " + server.getName() + "\nContent-Type: " + socketClass.getContentType() + "; charset=utf-8\n");
		}
	}
}

int main(int argc, char **argv)
{
	std::string file;

	if (argc > 2)
		raiseError("Too many arguments");
	else if (argc == 2)
	{
		parseConfigFile(argv[1]);
		file = configToString(argv[1]);
	}
	else
	{
		char *temp = strdup("webserv.conf");
		parseConfigFile(temp);
		file = configToString(temp);
		free(temp);
	}
	while (true)
	{
		createConection(file);
		sleep(1);
	}
	return 0;
}
