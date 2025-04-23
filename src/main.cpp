/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 18:18:00 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/03 12:59:56 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

static bool	parsePasswrd(const char* password)
{
    int length = std::strlen(password);
    if (length == 0 || length > 20)										//password should be between 1 and 20 characters
        return false;

    for (int i = 0; i < length; ++i)									//password should be printable and not contain any space (only one word)
    {
        if (!std::isprint(password[i]) || std::isspace(password[i]))
            return false;
    }

    return true;
}

static bool	parsePortNbr(const char* port)
{
    for (int i = 0; port[i] != '\0'; ++i)
    {
        if (port[i] < '0' || port[i] > '9')
            return false;
    }

    int portNumber = std::atoi(port);

	if (portNumber < 1024 || portNumber > 65535)
		return false;
	
	return true;
}
/**
 * - Ports 0 to 1023 are reserved for specific services and protocols, 
 * such as HTTP (port 80), FTP (port 21), and SSH (port 22). 
 * They require administrative privileges to use.
 * 
 * - Ports 1024 to 49151 can be registered for specific services by software developers.
 * 
 * - Ports 49152 to 65535 are used by client programs and are allocated dynamically by the operating system, 
 * so they are known as ephemeral ports. But they can be used by software developers as well.
 */

static void	signalManager()
{
	signal(SIGINT, Server::signalReceived);								//catch signals to close the server properly (ctrl + c or ctrl + \)
	signal(SIGQUIT, Server::signalReceived);		
	signal(SIGTERM, Server::signalReceived);			
	signal(SIGPIPE, SIG_IGN);											//ignore SIGPIPE signal to avoid crashing when writing to a closed socket
}

////////////////////////////////////////////////////////////////////////////////

int		main(int argc, char **argv)
{
	Server server;
	
	if (argc != 3)
	{
		std::cout	<< RED << "Error: Invalid number of arguments. Usage: ./ircserv <port> <password>" << RESET << std::endl;
		return EXIT_FAILURE;
	}
	
	std::cout	<<  "Server is starting..." << std::endl << std::endl;
	
	try
	{
		signalManager();
		
		if (!parsePortNbr(argv[1]))
		{
			std::cout	<< RED << "Error: Invalid Port number, it should be between 1024 and 65535" << std::endl;
			return EXIT_FAILURE;
		}
		if (!parsePasswrd(argv[2]))
		{
			std::cout	<< RED << "Error: Invalid Password, it should be one word with 1 to 20 valid characters" << std::endl;
			return EXIT_FAILURE;
		}
		
		server.go(std::atoi(argv[1]), argv[2]);
	}
	catch(const std::exception &excpt)
	{
		server.disconnectServer();
		std::cerr	<< RED << "Error: " << excpt.what() << RESET << std::endl;
	}
	
	std::cout	<< RED << "Server disconnected" << RESET << std::endl;
	return EXIT_SUCCESS;
}
