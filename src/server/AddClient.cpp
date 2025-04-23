/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:42:40 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/18 17:01:37 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void Server::createNewClient(Client& client, int clientAcceptedFd)
{
	client.setFd(clientAcceptedFd);
	client.setIpAddress(inet_ntoa((_clientSocketAddr.sin_addr)));
	_clientsList.push_back(client);
	std::cout << GREEN << "Client fd: " << clientAcceptedFd << " connected"
			  << RESET << std::endl
			  << std::endl;
}
/**
 * ADDING THE NEW CLIENT
 * - We set a new object of the Client class with the file descriptor and IP address of the client.
 * - We add the client to the vector of clients and the pollfd structure to the vector of pollfd structures.
 * - We print a message to the console to inform that a new client has connected.
 */

void Server::addClientToPoll(int clientAcceptedFd)
{
	_pollFdClient.fd = clientAcceptedFd;
	_pollFdClient.events = POLLIN;
	_pollFdClient.revents = 0;
	_pollFdList.push_back(_pollFdClient);
}
/**
 * ADDING THE NEW CLIENT
 * - _pollFdClient -> structure used for monitoring file descriptors for I/O events.
 * - POLLIN -> to set the event to POLLIN (data can be read from the client).
 * - revents -> to initialize the returned events to 0 (no event has occurred yet).
 */

bool Server::setClientNonBlocking(int clientAcceptedFd)
{
	if (fcntl(clientAcceptedFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << RED << "fcntl() failed" << std::endl;
		return false;
	}
	return true;
}
/**
 * CLIENT SOCKET - NON-BLOCKING MODE
 * -F_SETFL -> set file status flags, 
 * -O_NONBLOCK -> non-blocking mode
 * We set the client socket to non-blocking mode to avoid blocking the server when reading data from the client.
 * If we don't set the client socket to non-blocking mode, 
 * the server will block while waiting for data from the client.
 */

int Server::acceptClientConnection()
{
	socklen_t clientAddressLen = sizeof(_clientSocketAddr);
	int clientAcceptedFd = accept(_serverSocketFd, (sockaddr *)&(_clientSocketAddr), &clientAddressLen);
	if (clientAcceptedFd == -1)
		std::cout << RED << "accept() failed" << std::endl;
	return clientAcceptedFd;
}
/**
 * IMPORTANT - ACCEPT FUNCTION
 * accept() extracts the first connection request on the queue of pending connections for the listening socket,
 * it creates a new connected socket, and returns a NEW file descriptor referring to that socket.
 * The newly created socket is NOT in the listening state, 
 * and the original socket is unaffected by this call (it remains open in listening mode).
 */

void Server::clearClientAddress()
{
	memset(&_clientSocketAddr, 0, sizeof(_clientSocketAddr));
}

///////////////////////////////////////////////////////////////////////////////////

/* main function */

void Server::addNewClient()
{
	Client client;

	clearClientAddress();
	int clientAcceptedFd = acceptClientConnection();

	if (clientAcceptedFd == -1)
		return;

	if (!setClientNonBlocking(clientAcceptedFd))
		return;

	addClientToPoll(clientAcceptedFd);

	createNewClient(client, clientAcceptedFd);
}
