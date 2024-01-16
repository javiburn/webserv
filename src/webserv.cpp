/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaparic <alaparic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 10:42:26 by alaparic          #+#    #+#             */
/*   Updated: 2024/01/16 12:50:13 by alaparic         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/webserv.hpp"

int main(int argc, char **argv)
{
	((void)argc, (void)argv);
	std::cout << "Weberv!" << std::endl;

	int socket_fd;

	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "error: socket creation" << std::endl;
		exit(1);
	}

	std::cout << "Socket connection created!!" << std::endl;
	return 0;
}
