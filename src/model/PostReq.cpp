/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostReq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:22:53 by jsarabia          #+#    #+#             */
/*   Updated: 2024/03/18 18:35:29 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

// orthodox canonical form

PostReq::PostReq() {}

PostReq::PostReq(const PostReq &copy)
{
	*this = copy;
}

PostReq &PostReq::operator=(const PostReq &assign)
{
	if (this != &assign)
	{
		this->fileName = assign.fileName;
	}
	return *this;
}

PostReq::~PostReq() {}

// setters and getters

void PostReq::setContentTextPlain(std::string str)
{
	this->content = str;
}

void PostReq::setFileName(std::string request)
{
	int pos = request.find("filename=\"") + 10;
	if (pos >= static_cast<int>(request.length()) || pos < 10)
	{
		this->fileName = "";
		return;
	}
	std::string temp = request.substr(pos, request.length() - pos);
	this->fileName = temp.substr(0, temp.find("\""));
}

std::string PostReq::getFileName()
{
	return this->fileName;
}

void PostReq::setFileContent(std::string request)
{
	int pos = request.find("Content-Type:") + 14;
	if (pos >= static_cast<int>(request.length()) || pos < 14)
	{
		this->content = "";
		return;
	}
	std::string reqcpy = request.substr(pos, request.length() - pos);
	pos = reqcpy.find("Content-Type:") + 14;
	if (pos >= static_cast<int>(reqcpy.length()) || pos < 14)
	{
		this->content = "";
		return;
	}
	reqcpy = reqcpy.substr(pos, request.length() - pos);
	reqcpy = reqcpy.substr(reqcpy.find("\n"), reqcpy.length() - reqcpy.find("\n"));
	if (boundary[boundary.length() - 1] == '\n' || boundary[boundary.length() - 1] == '\r')
		boundary = boundary.substr(0, boundary.length() - 1);
	std::size_t pos2 = reqcpy.find(boundary);
	std::size_t check = reqcpy.rfind(boundary);
	if (pos2 < reqcpy.length() && pos2 < check && check < reqcpy.length())
	{
		this->content = reqcpy.substr(pos2 + boundary.length(), reqcpy.length() - pos2 - boundary.length() - check - 2);
	}
	else
		this->content = reqcpy.substr(0, pos2 - 2);
	this->content.push_back('\0');
	unsigned int i = 0;
	while (i < this->content.length()){
		if (isspace(this->content[i]) || this->content[i] == '\0')
			this->content = this->content.substr(1, this->content.length() - 1);
		else
			break;
		i++;
	}
	while (isspace(this->content.back()) || this->content.back() == '\0')
		this->content.pop_back();
}

std::string PostReq::getFileContent()
{
	return this->content;
}

void PostReq::setPostType(std::string type)
{
	this->postType = type;
}

std::string PostReq::getPostType()
{
	return this->postType;
}

void PostReq::setBoundary(std::string request)
{
	int pos = request.find("boundary=") + 9;
	if (pos >= static_cast<int>(request.length()) || pos < 9)
	{
		this->boundary = "";
		return;
	}
	size_t d = request.find("\n");
	this->boundary = request.substr(pos, d - pos);
}

std::string PostReq::getBoundary()
{
	return this->boundary;
}