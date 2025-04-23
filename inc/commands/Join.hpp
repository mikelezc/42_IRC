/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:12 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/16 19:26:12 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef JOIN_HPP
#define JOIN_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
#include <sstream>
#include <string>

class Server;

class Join : public Command
{
	public:
		Join	(Server* server);
		~Join();

		Join(const Join &other);
    	Join &operator=(const Join &other);
		
		void	execute(std::string cmd, int fd);
		int		SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd);
		void	JoinExistingChannel(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd);
		void	CreateChannelIfNotExists(std::vector<std::pair<std::string, std::string> >&token, int i, int fd);
		void 	ProcessChannels(const std::string& ChStr, std::vector<std::pair<std::string, std::string> >& token);
    	void 	ProcessPasswords(const std::string& PassStr, std::vector<std::pair<std::string, std::string> >& token);
		int		SearchForClients(std::string nickname);
};

#endif