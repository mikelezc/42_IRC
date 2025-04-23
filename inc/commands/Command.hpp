/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:29:47 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/03 20:29:49 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Macros.hpp"

class Server;

class Command
{
	protected:
		Server *server;
	public:
	Command(Server *server);
	Command(const Command &cmd);
	Command &operator= (const Command &cmd);
	virtual ~Command();
	
	virtual void execute(std::string cmd, int fd) = 0;
};

#endif