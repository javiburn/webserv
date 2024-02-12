/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsarabia <jsarabia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 18:22:38 by alaparic          #+#    #+#             */
/*   Updated: 2024/02/12 12:47:49 by jsarabia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"

/**
 * Manages the behabour that the server will have with DELETE requests
*/
void deleteMethod(Server &server, Request &req, Response &resp)
{
	if (access(req.getAbsPath().c_str(), X_OK) == 0)
	{
		const int result = remove(req.getAbsPath().c_str());
		if (result == 0)
		{
			resp.setResponse("<html>\n<body>\n<h1>File deleted.</h1>\n</body>\n</html>\n");
			resp.setContentLength(resp.getResponse());
			resp.generateHeader(200, server);
			req.setContentType(parseContentType("html"));
		}
		else
			resp.setResponseNotFound();
			resp.setContentLength(resp.getResponse());
			req.setContentType(parseContentType("html"));
			resp.generateHeader(404, server);
	}
	else
	{
		resp.setResponseForbidden();
		resp.setContentLength(resp.getResponse());
		req.setContentType(parseContentType("html"));
		resp.generateHeader(403, server);	// Sure? I am pretty sure the error code should be 405
	}
}
