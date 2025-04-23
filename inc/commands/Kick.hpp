/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:50:13 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:58:16 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
#define KICK_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>

class Server;

class Kick : public Command
{
	private:
	
	public:
		Kick(Server *server);
		~Kick();

		Kick(const Kick &other);
		Kick &operator=(const Kick &other);

		std::string parseKickCommand(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
		void 		execute(std::string cmd, int fd);
};
#endif

