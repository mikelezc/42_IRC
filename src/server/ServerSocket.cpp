/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:08:22 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/11 08:07:38 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Server.hpp"

void	Server::printSocketInfo()
{
	std::cout << GREEN << "Server fd: " << _serverSocketFd << " is running on port: " << _port << RESET << std::endl
			  << "Waiting for incoming connections..." << std::endl
			  << std::endl;
}

void	Server::addServerSocketToPollFd()
{
	_pollFdClient.fd = _serverSocketFd;
	_pollFdClient.events = POLLIN;
	_pollFdClient.revents = 0;
	
	_pollFdList.push_back(_pollFdClient);
}

/** 
 * struct pollfd {						-> (poll is a system call that monitors file descriptors for I/O events)
 *    int     fd;    				 	-> file descriptor to poll
 *    short   events;					-> requested events (POLLIN: data can be read, POLLOUT: data can be written...)
 *    short   revents;					-> returned events 
 * };
 */

void	Server::listenForConnections()
{
	if (listen(_serverSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));
}
/** 
 * - _serverSocketFd is the file descriptor of the socket.
 * - SOMAXCONN is the maximum length to which the queue of pending connections for _serverSocketFd may grow.
*/

void	Server::bindServerSocket()
{
	if (bind(_serverSocketFd, (struct sockaddr *)&_serverSocketAddr, sizeof(_serverSocketAddr)) == -1)
		throw(std::runtime_error("faild to bind socket"));
}
/**
 * - _serverSocketFd is the file descriptor of the socket.
 * - _serverSocketAddr is the address of the server.
 * - sizeof(_serverSocketAddr) to know how many bytes to read.
 */

void	Server::setNonBlockingMode()
{
	if (fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
}
/**
 * - _serverSocketFd is the file descriptor of the socket.
 * - F_SETFL is used to set the file status flags and allow to change the file status flags in next argument.
 * - O_NONBLOCK sets the file status flags to non-blocking mode 
 * 		->(this means that the read() and write() functions will return immediately without waiting for data to be read or written).
 */

void	Server::setSocketOptions()
{
	int enable = 1;

	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
}
/**
 * - _serverSocketFd is the file descriptor of the socket.
 * - SOL_SOCKET socket layer itself, used for manipulating options for the socket itself. (can be other levels: ip, tcp, udp, etc.)
 * - SO_REUSEADDR allows the server to bind to an address that is already in use by another socket.
 * 		-> This is useful when the server is restarted and we want to bind to the same address without waiting for the TIME_WAIT state to expire.
 * - enable because we want to enable the option (this option allows the server to bind to an address that is already in use by another socket).
 * - sizeof(enable) to know how many bytes to read.
 */

void	Server::createServerSocket()
{
	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd == -1)
		throw(std::runtime_error("faild to create socket"));
}
/**
 * - AF_INET is the address family (IPv4).
 * - SOCK_STREAM is the type of socket (TCP).
 * - 0 is the protocol (default protocol for the given address family and socket type).
 */

void	Server::configureServerAddress()
{
	_serverSocketAddr.sin_family = AF_INET;
	_serverSocketAddr.sin_port = htons(_port);
	_serverSocketAddr.sin_addr.s_addr = INADDR_ANY;
}
/**
 * struct sockaddr_in {
 *    short            sin_family;		-> address family. For IPv4, this is typically set to AF_INET (IPv6 is AF_INET6).
 *    unsigned short   sin_port;		->PORT (htons converts the port number to big-endian notation)
 *    struct in_addr   sin_addr;		->ADDRESS (INADDR_ANY Accept "any" incoming messages of any IP address on the server)
 *    char             sin_zero[8];		-> Not used, and should be zeroed.
 * };
 */

/////////////////////////////////////////////////////////////////////////////////////

/* main function */
void	Server::initServerSocket()
{
	configureServerAddress();
	createServerSocket();
	setSocketOptions();
	setNonBlockingMode();
	bindServerSocket();
	listenForConnections();
	addServerSocketToPollFd();
	printSocketInfo();
}
