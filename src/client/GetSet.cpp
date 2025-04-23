/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetSet.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:38:06 by mlezcano          #+#    #+#             */
/*   Updated: 2024/09/28 11:35:18 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

/* Getters */

int Client::getFd()
{
	return this->_clientFd;
}

std::string Client::getNick()
{
	return this->_clientNick;
}

std::string Client::getUser()
{
	return this->_clientUser;
}

std::string Client::getBuffer()
{
	return _clientBuffer;
}

std::string Client::getIpAddress()
{
	return _clientIpAddress;
}

std::string Client::getHostname()
{
	return this->getNick() + "!" + this->getUser();
}

//////////////////////////////////////////////////////////////////////////////

/* Setters */
void Client::setFd(int clientFd)
{
	this->_clientFd = clientFd;
}

void Client::setNick(std::string& clientNick)
{
	this->_clientNick = clientNick;
}

void Client::setLogin(bool clientIsLoged)
{
	this->_clientIsLoged = clientIsLoged;
}

void Client::setUser(std::string& clientUser)
{
	this->_clientUser = clientUser;
}

void Client::setBuffer(std::string src)
{
	_clientBuffer += src;
}

void Client::setRegistered(bool clientIsRegistered)
{
	_clientIsRegistered = clientIsRegistered;
}

void Client::setIpAddress(std::string clientIpAddress)
{
	this->_clientIpAddress = clientIpAddress;
}

//////////////////////////////////////////////////////////////////////////////

/* Boolean */

bool Client::isClientRegistered()
{
	return _clientIsRegistered;
}

bool Client::isInvitedChannel(std::string& channelName)
{
	for (size_t i = 0; i < this->_clientChannelsBook.size(); i++)
	{
		if (this->_clientChannelsBook[i] == channelName)
			return true;
	}
	return false;
}

bool Client::isLogged()
{
	return this->_clientIsLoged;
}
