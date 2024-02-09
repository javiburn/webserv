/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:39 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/09 20:34:23 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <sys/stat.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>

#include "enums.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "PostReq.hpp"
using namespace std;

int			setAction(std::string);
Request		parseReq(std::string passedReq);
std::string	configToString(char *str);
void		raiseError(const char *msg);
std::string	getFile(const std::string &fileAdr);
int			parseConfigFile(std::string file_name);
int			isAllowed(Server &server, const std::string str, std::list<std::string> actions, std::list<std::string> forbidden);
void		handleRequests(Location &location, Server &server, Request &req, Response &response);
std::string	parseContentType(std::string extension);
void		createConection(std::string str);
void		handlePost(Location &location, Server &server, Request &req, Response &response);
std::string	deleteFirstElement(std::string str);
void		deleteMethod(Server &server, Request &req, Response &resp);

#endif
