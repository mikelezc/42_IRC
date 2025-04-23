/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:45:51 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/11 08:04:30 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server()
{
	this->_serverSocketFd = -1;
}

Server::Server(Server const &src)
{
	*this = src;
}

Server &Server::operator=(Server const &src)
{
	if (this != &src)
	{
		this->_port = src._port;
		this->_serverSocketFd = src._serverSocketFd;
		this->_pass = src._pass;
		this->_clientsList = src._clientsList;
		this->_channelsList = src._channelsList;
		this->_pollFdList = src._pollFdList;
		this->isBotfull = src.isBotfull;
	}
	return *this;
}

Server::~Server()
{
}

//////////////////////////////////////////////////////////////////////////////////

/* Static variables */

bool Server::_signal	= false;
bool Server::isBotfull	= false;

void Server::processPollEvents()
{
	for (size_t i = 0; i < _pollFdList.size(); i++)  					// Iterate over the array of fds
	{
		if (_pollFdList[i].revents & POLLIN)  							// If there is a new event to read in fd i (any of the fds) & POLLIN = data can be read
		{
			if (_pollFdList[i].fd == _serverSocketFd)  					// If the event occurs in the server socket: a new client is accepted
				this->addNewClient();
			else  														// If the event occurs in a client socket: new data is received
				this->inputDataProcess(_pollFdList[i].fd);
		}
	}
}

/* INPUT 
* We need to iterate over the array of fds to find out which one has the event.
* If poll() returns a value greater than 0, it means that an event has occurred in one or more fds.
*/

void Server::pollEventsLoop()
{
	while (!Server::_signal)
	{
		int pollResult = poll(&_pollFdList[0], _pollFdList.size(), -1);
		
		if (pollResult == -1 && Server::_signal == false)
			throw std::runtime_error("poll() failed");

		processPollEvents();
	}
}
/*
* Main loop that monitors and processes poll events
* poll() monitor changes in the status of file descriptors
* -1 means that poll() will wait indefinitely, and will only advance when an event occurs in one of the fds.
* if poll() detects an event in any of the fds, it will return the number of fds with events.
* If poll() fails, it will return -1.
*/

//////////////////////////////////////////////////////////////////////////////////

/* main function */
void Server::go(int port, std::string pass)
{
	/* set args and initialize socket */
	setArgs(port, pass);

	/* initialize server socket */
	initServerSocket();

	/* main poll loop */
	pollEventsLoop();

	/* disconnect server */
	disconnectServer();
}
