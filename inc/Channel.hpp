/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 18:31:49 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/03 11:11:17 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;

class Channel
{
	private:

		std::string							_channelName;
		std::string							_timeChannelWasCreated;
		
		std::vector<std::pair<char, bool> >	_channelModes;	// i, t, k, o, l
		
		bool								_isInviteOnly;
		bool								_isTopicRestricted;
		std::string							_channelKey;
		int									_channelUsersLimit;
		
		
		std::string							_channelTopic;
		std::string							_timeTopicWasCreated;
		
		std::vector<Client>					_channelAdminsBook;
		std::vector<Client>					_channelClientsBook;

	public:
	
		/* Channel.cpp */

			/* Cannonical form */

			Channel();
			~Channel();
			Channel(Channel const &src);
			Channel &operator=(Channel const &src);

			/* Manage clients in channel */

			void			channelAddClient(Client nick);
			void			channelAddAdmin(Client nick);
			void			channelEraseClient(int fd);
			void			channelEraseAdmin(int fd);
			bool			clientToAdminUpgrd(std::string& nick);
			bool			adminToClientDwngrd(std::string& nick);

		
		/* SendToAll.cpp */

			void			allMembersSender(std::string message, int fd);

		/* Getters.cpp */

			std::string		getChannelName();
			std::string 	getTimeChannelWasCreated();
			
			bool			getChannelMode(size_t index);
			std::string		getConfiguredModeFromChannel();
			
			int				getInviteOnly();
			bool			getTopicRestricted() const;
			std::string		getPassword();
			int				getUsersLimit();
			
			std::string 	getChannelTopic();
			std::string 	getTopicCreatedTime();
			
			int				getTotalNbrClients();
			bool			isClientInChannel(std::string &nick);
			std::string		whoIsInChannel();
			
			Client*			getClientFromChannel(int fd);
			Client*			getAdminFromChannel(int fd);
			Client*			getClientOrAdminFromChannel(std::string nick);

		
		/* Setters.cpp */

			void			setChannelName(std::string channelName);
			void			setTimeChannelWasCreated();
			
			void			setChannelMode(size_t index, bool mode);
			
			void			setInviteOnly(bool channelInviteOnly);
			void			setTopicRestricted(bool isTopicRestricted);
			void			setChannelKey(std::string channelKey);
			void			setUsersLimit(int channelUsersLimit);
			
			void			setChannelTopic(std::string channelTopic);
			void			setTopicCreatedTime(std::string time);
};

#endif
