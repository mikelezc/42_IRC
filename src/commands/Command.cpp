/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:28:48 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/16 19:28:48 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Command.hpp"

Command::Command( Server *server) : server(server){}

Command &Command::operator=(Command const &cmd)
{
	if (this != &cmd)
	{
		this->server = cmd.server;
	}
	return *this;
}

Command::Command(Command const &cmd)
{
	*this = cmd;
}

Command::~Command( void ){}