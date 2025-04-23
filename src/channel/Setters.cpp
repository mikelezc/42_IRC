/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Setters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:48:15 by mlezcano          #+#    #+#             */
/*   Updated: 2024/09/28 19:12:30 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

void Channel::setInviteOnly(bool channelInviteOnly)
{
	this->_isInviteOnly = channelInviteOnly;
}

void Channel::setTopicCreatedTime(std::string time)
{
	this->_timeTopicWasCreated = time;
}

void Channel::setUsersLimit(int channelUserLimit)
{
	this->_channelUsersLimit = channelUserLimit;
}

void Channel::setChannelTopic(std::string _channelTopic)
{
	this->_channelTopic = _channelTopic;
}

void Channel::setChannelKey(std::string channelKey)
{
	this->_channelKey = channelKey;
}

void Channel::setChannelName(std::string channelName)
{
	this->_channelName = channelName;
}

void Channel::setTopicRestricted(bool isTopicRestricted)
{
	this->_isTopicRestricted = isTopicRestricted;
}

void Channel::setChannelMode(size_t index, bool mode)
{
	_channelModes[index].second = mode;
}

void Channel::setTimeChannelWasCreated()	
{
	std::time_t _time = std::time(NULL);
	std::ostringstream oss;
	oss << _time;
	this->_timeChannelWasCreated = std::string(oss.str());
}
