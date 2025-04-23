/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Erase.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:59:00 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/11 08:04:30 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

/* Remove a file descriptor from the pollFdList */

void	Server::eraseFromPollFdList(int fd)
{
	for (size_t i = 0; i < this->_pollFdList.size(); ++i)
	{
		if (this->_pollFdList[i].fd == fd)
		{
			this->_pollFdList.erase(this->_pollFdList.begin() + i);
			return;	// Exit after removing the FD
		}
	}
}

/* Remove a client from the clientsList */

void	Server::eraseFromClientList(int fd)
{
	for (size_t i = 0; i < this->_clientsList.size(); ++i)
	{
		if (this->_clientsList[i].getFd() == fd)
		{
			this->_clientsList.erase(this->_clientsList.begin() + i);
			return;	// Exit after removing the client
		}
	}
}

/* Remove a client from all channels and erase the channel if it becomes empty */

void	Server::eraseChannels(int fd)
{
	for (size_t i = 0; i < this->_channelsList.size(); ++i)
	{
		bool clientRemoved = false;
		
		// Remove client from the channel if they are a member
		if (_channelsList[i].getClientFromChannel(fd))
		{
			_channelsList[i].channelEraseClient(fd);
			clientRemoved = true;
		}
		// Remove client if they are the admin
		else if (_channelsList[i].getAdminFromChannel(fd))
		{
			_channelsList[i].channelEraseAdmin(fd);
			clientRemoved = true;
		}
		
		// If the channel becomes empty, remove the channel
		if (_channelsList[i].getTotalNbrClients() == 0)
		{
			_channelsList.erase(_channelsList.begin() + i);
			--i; // Adjust index of amount of channels
			continue; // Skip to the next iteration
		}
		
		// Notify all members if the client was removed
		if (clientRemoved)
		{
			std::string quitMessage = ":" + getClient(fd)->getNick() + \
			"!~" + getClient(fd)->getUser() + "@localhost QUIT Quit\r\n";
			_channelsList[i].allMembersSender(quitMessage, -1);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////

/* Main function */

void	Server::eraseAll(int fd)
{
	eraseChannels(fd);
	eraseFromClientList(fd);
	eraseFromPollFdList(fd);
	close(fd);
}
