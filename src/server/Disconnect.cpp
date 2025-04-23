/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Disconnect.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:38:44 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/03 12:59:23 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void Server::disconnectServer()
{
	for (size_t i = 0; i < _clientsList.size(); i++)
	{
		std::cout << RED << "Client (" << _clientsList[i].getFd() << ") " << _clientsList[i].getNick() << " was disconnected"
				  << RESET << std::endl
				  << std::endl;
		close(_clientsList[i].getFd());
	}

	if (_serverSocketFd != -1)
	{
		std::cout << "Server is shutting down..." << std::endl;
		close(_serverSocketFd);
	}
}
