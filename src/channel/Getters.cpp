/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:45:09 by mlezcano          #+#    #+#             */
/*   Updated: 2024/09/29 19:17:05 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

int Channel::getInviteOnly()
{
	return this->_isInviteOnly;
}

std::string Channel::getTopicCreatedTime()
{
	return this->_timeTopicWasCreated;
}

int Channel::getUsersLimit()
{
	return this->_channelUsersLimit;
}

std::string Channel::getChannelTopic()
{
	return this->_channelTopic;
}

std::string Channel::getPassword()
{
	return this->_channelKey;
}

std::string Channel::getChannelName()
{
	return this->_channelName;
}

bool Channel::getTopicRestricted() const
{
	return this->_isTopicRestricted;
}

bool Channel::getChannelMode(size_t index)
{
	return _channelModes[index].second;
}

std::string Channel::getTimeChannelWasCreated()
{
	return _timeChannelWasCreated;
}

/////////////////////////////////////////////////////////////////////////////

int Channel::getTotalNbrClients()
{
	return this->_channelClientsBook.size() + this->_channelAdminsBook.size();
}

std::string Channel::getConfiguredModeFromChannel()						// it gets the modes of the channel when it is created
{
	std::string mode;
	
	for(size_t i = 0; i < _channelModes.size(); i++)
	{
		if(_channelModes[i].first != 'o' && _channelModes[i].second)	// if the mode is not 'o' (admin) and is set to true (second)
			mode.push_back(_channelModes[i].first);						// add the mode to the string
	}
	
	if(!mode.empty())													// if the string is not empty
		mode.insert(mode.begin(),'+');									// add a '+' at the beginning of the string
	
	return mode;
}



bool Channel::isClientInChannel(std::string &nick)
{
	for(size_t i = 0; i < _channelClientsBook.size(); i++)
	{
		if(_channelClientsBook[i].getNick() == nick)
			return true;
	}
	
	for(size_t i = 0; i < _channelAdminsBook.size(); i++)
	{
		if(_channelAdminsBook[i].getNick() == nick)
			return true;
	}
	
	return false;
}

std::string Channel::whoIsInChannel()
{
	std::string list;
	
	for(size_t i = 0; i < _channelAdminsBook.size(); i++)				// iterate over the admins book
	{
		list += "@" + _channelAdminsBook[i].getNick();					// add the nick of the admin with '@' at the beginning
		if((i + 1) < _channelAdminsBook.size())							// if it is not the last admin
			list += " ";												// add a space
	}
	
	if(_channelClientsBook.size())										// if there are clients in the channel
		list += " ";													// add a space to separate the admins from the clients
		
	for(size_t i = 0; i < _channelClientsBook.size(); i++)
	{
		list += _channelClientsBook[i].getNick();						// add the nick of the client
		if((i + 1) < _channelClientsBook.size())						// if it is not the last client
			list += " ";												// add a space
	}
	return list;
}

Client *Channel::getClientFromChannel(int fd)
{
	for (std::vector<Client>::iterator it = _channelClientsBook.begin(); it != _channelClientsBook.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client *Channel::getAdminFromChannel(int fd)
{
	for (std::vector<Client>::iterator it = _channelAdminsBook.begin(); it != _channelAdminsBook.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client* Channel::getClientOrAdminFromChannel(std::string nick)
{
	for (std::vector<Client>::iterator it = _channelClientsBook.begin(); it != _channelClientsBook.end(); ++it)
	{
		if (it->getNick() == nick)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = _channelAdminsBook.begin(); it != _channelAdminsBook.end(); ++it)
	{
		if (it->getNick() == nick)
			return &(*it);
	}
	return NULL;
}
