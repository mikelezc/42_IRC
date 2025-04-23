/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:32:17 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/02 14:06:04 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client()
{
	this->_clientNick = "";
	this->_clientUser = "";
	this->_clientFd = -1;
	this->_clientIsRegistered = false;
	this->_clientIsLoged = false;
	this->_clientBuffer = "";
	this->_clientIpAddress = "";
}

Client::Client(std::string nick, std::string user, int fd) 
{
	this->_clientNick = nick;
	this->_clientUser = user;
	this->_clientFd = fd;
	this->_clientIsRegistered = false;
	this->_clientIsLoged = false;
	this->_clientBuffer = "";
	this->_clientIpAddress = "";
}

Client::Client(Client const &src)
{
	*this = src;
}

Client &Client::operator=(Client const &src)
{
	if (this != &src)
	{
		this->_clientNick = src._clientNick;
		this->_clientUser = src._clientUser;
		this->_clientFd = src._clientFd;
		this->_clientIsRegistered = src._clientIsRegistered;
		this->_clientIsLoged = src._clientIsLoged;
		this->_clientChannelsBook = src._clientChannelsBook;
		this->_clientBuffer = src._clientBuffer;
		this->_clientIpAddress = src._clientIpAddress;
	}
	return *this;
}

Client::~Client()
{

}

///////////////////////////////////////////////////////////////////////////////

/* Main functions */

void Client::clearClientBuffer()
{
	_clientBuffer.clear();
}

void Client::addChannelInvite(std::string &channelName)
{
	_clientChannelsBook.push_back(channelName);
}

void Client::eraseChannelInvite(std::string &channelName)
{
	for (size_t i = 0; i < this->_clientChannelsBook.size(); i++)
	{
		if (this->_clientChannelsBook[i] == channelName)
			{
				this->_clientChannelsBook.erase(this->_clientChannelsBook.begin() + i);
				return;
			}
	}
}
