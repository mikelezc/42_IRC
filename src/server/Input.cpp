/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:34:12 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/11 08:10:10 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

std::vector<std::string> Server::inputBufferSplitter(std::string buffer)
{
	std::vector<std::string> cmds;
	std::istringstream iss(buffer);
	std::string line;
	
	while (std::getline(iss, line))													// While there are lines to read...
	{
		size_t pos = line.find_first_of(CRLF);										// Find \r\n characters (end of the line)
		if (pos != std::string::npos)												// If the characters were found...
			line = line.substr(0, pos);												// Get the substring before the newline character
		cmds.push_back(line);														// Add the line to the vector
	}
	
	return cmds;																	// Return the vector of commands
}

void Server::inputDataProcess(int fd)
{
	std::vector<std::string> cmds; 													// Vector to store the commands ()
	
	char buffer[1024];			  													// To store the received data (1024 bytes is the maximum size of IRC protocol)
	memset(buffer, 0, sizeof(buffer)); 												// Clear the buffer before receiving data
	
	/* Receive data from the client */
	Client *client = getClient(fd);	  												// Get the client that sent the data
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); 						// Receive data from the client (returns the number of bytes received)
	if (bytes <= 0) 																// If no data was received...
	{
		std::cout << RED << "Client fd: " << client->getFd() << " nick: " << client->getNick() << " disconnected"
				  << RESET << std::endl
				  << std::endl;

		eraseAll(fd);																// Erase the client from the server
	}
	else 																			// If data was received...
	{
		client->setBuffer(buffer); 													// Set the buffer of the client

		if (client->getBuffer().find_first_of(CRLF) == std::string::npos)			// If the buffer doesn't contain a newline or carriage return (CRLF)...
			return;																	// Return (wait for the rest of the data)

		cmds = inputBufferSplitter(client->getBuffer()); 							// Split the buffer into commands (lines between \r\n)

		for (size_t i = 0; i < cmds.size(); i++) 									// Iterate over the commands
			this->parse_exec_cmd(cmds[i], fd);										// Parse and execute the command

		if (getClient(fd))				 											// If the client still exists...
			getClient(fd)->clearClientBuffer(); 									// Clear his buffer to receive new data again
	}
}
