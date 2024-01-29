/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getFile.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:57:01 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/29 16:58:11 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

int	setAction(std::string str)
{
	std::string	word = str.substr(0, str.find(" "));
	if (word == "GET")
		return 0;
	else if (word == "POST")
		return 1;
	else if (word == "DELETE")
		return 2;
	return 3;
}

std::string getFile(std::string fileAdr)
{
	std::ifstream file(fileAdr);
	std::string content;

	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		content = buffer.str();
		file.close();
	}

	return content;
}
