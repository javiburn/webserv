/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 20:09:02 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/28 15:45:35 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

// orthodox canonical form

Request::Request() {}

Request::Request(const Request &copy)
{
	*this = copy;
}

Request &Request::operator=(const Request &assign)
{
	if (this != &assign)
	{
		this->method = assign.method;
		this->uri = assign.uri;
		this->version = assign.version;
		this->host = assign.host;
		this->user_agent = assign.user_agent;
		this->accept = assign.accept;
		this->connection = assign.connection;
	}
	return *this;
}

Request::~Request() {}

// setters and getters

std::string	Request::getMethod()
{
	return this->method;
}

void	Request::setMethod(std::string method)
{
	this->method = method;
}

void	Request::setExtension()
{
	this->extension = this->uri.substr(this->uri.rfind(".") + 1, this->uri.length() - this->uri.rfind("."));
	return;
}

std::string	Request::getExtension()
{
	return this->extension;
}

std::string Request::getContentType()
{
	return this->contentType;
}

void Request::setContentType(std::string type)
{
	this->contentType = type;
}

std::string	Request::getUri()
{
	return this->uri;
}

void	Request::setUri(std::string uri)
{
	this->uri = uri;
}

std::string	Request::getVersion()
{
	return this->version;
}

void	Request::setVersion(std::string version)
{
	this->version = version;
}

std::string	Request::getHost()
{
	return this->host;
}

void	Request::setHost()
{
	size_t pos = this->reqBuffer.find("Host:") + 5;
	std::string temp;
	if (pos < 5 || pos > this->reqBuffer.length() + 5)
		this->host = "localhost";
	else{
		while (isspace(this->reqBuffer[pos]))
			pos++;
		while (this->reqBuffer[pos] != '\n' && this->reqBuffer[pos] != ':' && this->reqBuffer[pos] != '\r'){
			temp.push_back(this->reqBuffer[pos]);
			pos++;
		}
	}
	this->host = temp;
}

std::string	Request::getUser_agent()
{
	return this->user_agent;
}

void	Request::setUser_agent(std::string user_agent)
{
	this->user_agent = user_agent;
}

std::string	Request::getAccept()
{
	return this->accept;
}

void	Request::setAccept(std::string accept)
{
	this->accept = accept;
}

std::string	Request::getConnection()
{
	return this->connection;
}

void	Request::setConnection(std::string connection)
{
	this->connection = connection;
}

void	Request::setAbsPath(Server& server)
{
	this->absPath = server.getRoot();
	if (this->uri[0] == '/' && absPath[absPath.length() - 1] == '/')
		this->absPath.pop_back();
	if (this->uri.find("?") < this->uri.length() && this->uri.find("?") >= 0){
		this->absPath += this->uri.substr(0, this->uri.find("?"));
		this->getArgs = this->uri.substr(this->uri.find("?"), this->uri.length() - this->uri.find("?"));
		return;
	}
	this->absPath += this->uri;
}

std::string	Request::getterGetArgs()
{
	return this->getArgs;
}

std::string	Request::getAbsPath()
{
	return this->absPath;
}
// methods

/**
 * Parses the request and stores the information in the Request object.
*/
Request Request::parseReq(std::string passedReq)
{
	std::istringstream iss(passedReq);
	std::string line;
	int count = 0;
	Request req;

	while (std::getline(iss, line))
	{
		std::istringstream iss2(line);
		std::string word;
		while (std::getline(iss2, word, ' '))
		{
			switch (count)
			{
			case 0:
				req.setMethod(word);
				break;
			case 1:
				req.setUri(word);
				break;
			case 2:
				req.setVersion(word);
				break;
			}
			count++;
		}
	}
	return req;
}


void	Request::setContentLength()
{
	int pos = this->reqBuffer.find("Content-Length:") + 15;
	if (pos >= static_cast<int>(this->reqBuffer.length()) || pos < 15){
		this->contentLength  = 0;
		return;
	}
	std::istringstream temp(this->reqBuffer.substr(pos, this->reqBuffer.length() - pos));
	std::string num;
	std::getline(temp, num);
	this->contentLength = std::atoi(num.c_str());
}
int	Request::getContentLength()
{
	return this->contentLength;
}

void	Request::setReqBuffer(std::string buffer)
{
	this->reqBuffer = buffer;
}

std::string	Request::getReqBuffer()
{
	return this->reqBuffer;
}

int	Request::getClientBodySize()
{
	return this->clientBodySize;
}

void	Request::setClientBodySize(int maxSize)
{
	this->clientBodySize = maxSize;
}

void	Request::handleSlash()
{
	this->absPath.pop_back();
}

void	Request::setPort()
{
	size_t pos = this->reqBuffer.find(this->host) + this->host.length();
	if (pos < this->host.length() || pos > this->reqBuffer.length()){
		raiseError("Unexpected error");
	}
	std::string num;
	while (this->reqBuffer[pos] == ':')
		pos++;
	while (pos < this->reqBuffer.length()){
		if (isdigit(this->reqBuffer[pos]))
			num.push_back(this->reqBuffer[pos]);
		else
			break;
		pos++;
	}
	this->port = std::atoi(num.c_str());
}

int	Request::getPort()
{
	return this->port;
}

void	Request::newSetPort(unsigned int nport)
{
	this->port = nport;
}
