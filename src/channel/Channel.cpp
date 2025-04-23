/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:34:41 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/03 11:10:27 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel()
{
	this->_isInviteOnly = false;
	this->_channelUsersLimit = 0;
	this->_isTopicRestricted = false;
	this->_channelName = "";
	this->_channelTopic = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		_channelModes.push_back(std::make_pair(charaters[i],false));
	this->_timeChannelWasCreated = "";
}

Channel::Channel(Channel const &src)
{
	*this = src;
}

Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
	{
		this->_isInviteOnly = src._isInviteOnly;
		this->_channelUsersLimit = src._channelUsersLimit;
		this->_isTopicRestricted = src._isTopicRestricted;
		this->_channelName = src._channelName;
		this->_channelKey = src._channelKey;
		this->_timeChannelWasCreated = src._timeChannelWasCreated;
		this->_channelTopic = src._channelTopic;
		this->_channelClientsBook = src._channelClientsBook;
		this->_channelAdminsBook = src._channelAdminsBook;
		this->_channelModes = src._channelModes;
	}
	return *this;
}

Channel::~Channel()
{
	
}

///////////////////////////////////////////////////////////////////////////////

/* Manage clients in channel */

void Channel::channelAddClient(Client nick)
{
	_channelClientsBook.push_back(nick);
}

void Channel::channelEraseClient(int fd)
{
	for (std::vector<Client>::iterator it = _channelClientsBook.begin(); it != _channelClientsBook.end(); ++it)
	{
		if (it->getFd() == fd)
			{
				_channelClientsBook.erase(it); 
				break;
			}
	}
}

void Channel::channelAddAdmin(Client nick)
{
	_channelAdminsBook.push_back(nick);
}


void Channel::channelEraseAdmin(int fd)
{
	for (std::vector<Client>::iterator it = _channelAdminsBook.begin(); it != _channelAdminsBook.end(); ++it)
	{
		if (it->getFd() == fd)
			{
				_channelAdminsBook.erase(it);
				break;
			}
	}
}

bool Channel::clientToAdminUpgrd(std::string& nick)
{
	size_t i = 0;
	
	for(; i < _channelClientsBook.size(); i++)						// find the client in the clients book
	{
		if(_channelClientsBook[i].getNick() == nick)				// if is found
			break;													// break the loop and keep the index
	}
	
	if(i < _channelClientsBook.size())								// if client is found (i < book when the loop is broken)
	{
		_channelAdminsBook.push_back(_channelClientsBook[i]);		// add client to the admins book
		_channelClientsBook.erase(i + _channelClientsBook.begin());	// erase client from the clients book
		return true;
	}
	return false;													// if the client is not found...
}

bool Channel::adminToClientDwngrd(std::string& nick)
{
	size_t i = 0;
	
	for(; i < _channelAdminsBook.size(); i++)
	{
		if(_channelAdminsBook[i].getNick() == nick)
			break;
	}
	
	if(i < _channelAdminsBook.size())
	{
		_channelClientsBook.push_back(_channelAdminsBook[i]);
		_channelAdminsBook.erase(i + _channelAdminsBook.begin());
		return true;
	}
	return false;
}
