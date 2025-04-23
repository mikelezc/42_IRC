/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:45:40 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/11 08:07:05 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>

#include "Client.hpp"
#include "Channel.hpp"
#include "Macros.hpp"

class Client;
class Channel;

class Server
{
	private:

		static bool 				_signal;
		
		int							_port;
		std::string					_pass;
		
		int 						_serverSocketFd;

		std::vector<Client> 		_clientsList;
		std::vector<Channel> 		_channelsList;
		std::vector<struct pollfd>	_pollFdList;	

		struct sockaddr_in			_serverSocketAddr;
		struct sockaddr_in			_clientSocketAddr;
		struct pollfd				_pollFdClient;		// structure used for monitoring file descriptors for I/O events.

	public:
		
		static bool					isBotfull;

		/* Cannonical form */

		Server();
		Server(Server const &src);
		Server &operator=(Server const &src);
		~Server();

	///////////////////////////////////////////////////////////////////////////////////

	/* Server.cpp */

		void						go(int port, std::string pass);
		void						processPollEvents();
		void						pollEventsLoop();

	/* Signal.cpp */
	
		static void					signalReceived(int sigNbr);
	//  static because it's a signal handler and can't be a member function but it needs to access _signal

	/* ServerSocket.cpp */
		
		void						printSocketInfo();
		void						initServerSocket();
		void						configureServerAddress();
		void						createServerSocket();
		void						setSocketOptions();
		void						setNonBlockingMode();
		void						bindServerSocket();
		void						listenForConnections();
		void						addServerSocketToPollFd();

	/* AddClient.cpp */
		void						createNewClient(Client &client, int clientAcceptedFd);
		void						addClientToPoll(int clientAcceptedFd);
		bool						setClientNonBlocking(int clientAcceptedFd);
		int							acceptClientConnection();
		void						clearClientAddress();
		void						addNewClient();
	
	/* Input.cpp */

		std::vector<std::string>	inputBufferSplitter(std::string buffer);
		void						inputDataProcess(int fd);
		
	/* GetSet.cpp */
			
		/* Getters */
		
		std::string					getPassword();
		int							getFd();
		Client*						getClient(int fd);
		Client*						getClientNick(std::string nickname);
		Channel*					getChannel(std::string name);
		std::vector<Channel>&		getChannels(void);

		/* Setters */

		void						setFd(int fd);
		void						setArgs(int port, std::string password);

	/* Erase.cpp */
		
		void						eraseFromClientList(int fd);
		void						eraseFromPollFdList(int fd);
		void						eraseChannels(int fd);
		void						eraseAll(int fd);

	/* Disconnect.cpp */
		
		void						disconnectServer();
		
		
	///////////////////////////////////////////////////////////////////////////////////
		
	/* CmdParser.cpp */

		std::vector<std::string>	split_cmd(std::string &str);
		bool						isClientRegistered(int fd);
		void						parse_exec_cmd(std::string &cmd, int fd);
    	void						senderror(int code, const std::string& clientname, int fd, \
									const std::string& msg, const std::string& channelname = "");
		void						_sendResponse(std::string response, int fd);

};

#endif