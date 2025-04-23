/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetSet.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 08:08:01 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/03 12:59:31 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

/* Getters */

std::string	Server::getPassword()
{
	return this->_pass;
}

int	Server::getFd()
{
	return this->_serverSocketFd;
}

Client* Server::getClient(int fd)
{
	for (size_t i = 0; i < this->_clientsList.size(); i++)
	{
		if (this->_clientsList[i].getFd() == fd)
			return &this->_clientsList[i];
	}
	return NULL;
}

Client* Server::getClientNick(std::string nickname)
{
	for (size_t i = 0; i < this->_clientsList.size(); i++)
	{
		if (this->_clientsList[i].getNick() == nickname)
			return &this->_clientsList[i];
	}
	return NULL;
}

Channel* Server::getChannel(std::string name)
{
	for (size_t i = 0; i < this->_channelsList.size(); i++)
	{
		if (this->_channelsList[i].getChannelName() == name)
			return &_channelsList[i];
	}
	return NULL;
}

std::vector<Channel> &Server::getChannels(void)
{
	return this->_channelsList;
}

//////////////////////////////////////////////////////////////////////////////////

/* Setters */

void Server::setArgs(int port, std::string password)
{
	this->_port = port;
	this->_pass = password;
}

void Server::setFd(int fd)
{
	this->_serverSocketFd = fd;
}
