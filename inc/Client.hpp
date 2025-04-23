/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:37:04 by mlezcano          #+#    #+#             */
/*   Updated: 2024/09/28 11:31:41 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
	private:
		
		std::string					_clientNick;
		std::string					_clientUser;

		bool						_clientIsRegistered;
		bool						_clientIsLoged;
		
		int							_clientFd;
		std::string					_clientBuffer;
		std::string					_clientIpAddress;
		
		std::vector<std::string>	_clientChannelsBook;

	public:
	
		/* Client.cpp */
		
			/* Cannonical form */
			
			Client();
			Client(std::string clientNick, std::string clientUser, int clientFd);
			Client(Client const &src);
			Client &operator=(Client const &src);
			~Client();
			
			/* Main functions */
			
			void			clearClientBuffer();
			void			addChannelInvite(std::string& channelName);
			void			eraseChannelInvite(std::string& channelName);
			
		/* GetSet.cpp */
		
			/* Getters */
			
			int				getFd();
			std::string		getNick();
			std::string		getUser();
			std::string		getIpAddress();
			std::string		getBuffer();
			std::string		getHostname();
			
			/* Setters */
			
			void			setFd(int clientFd);
			void			setNick(std::string& clientNick);
			void			setLogin(bool clientIsLoged);
			void			setUser(std::string& clientUser);
			void			setBuffer(std::string src);
			void			setRegistered(bool clientIsRegistered);
			void			setIpAddress(std::string ip);

			/* Boolean */
			
			bool			isClientRegistered();
			bool			isInvitedChannel(std::string& channelName);
			bool			isLogged();
};

#endif