/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:17:26 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/06 21:33:02 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/User.hpp"


User::User(Server* server) : Command(server), server(server) {}

User::~User() {}

User::User(const User &other) : Command(other.server), server(other.server) {}

User &User::operator=(const User &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

void	User::set_clientUsername(std::string& cmd, int fd)
{
	std::vector<std::string> splited_cmd = server->split_cmd(cmd);

	Client *cli = server->getClient(fd); 
	if((cli && splited_cmd.size() < 5))
		{
            server->_sendResponse(ERR_NOTENOUGHPARAM(cli->getNick()), fd); return; }
	if(!cli  || !cli->isClientRegistered())
		server->_sendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	else if (cli && !cli->getUser().empty())
		{
            server->_sendResponse(ERR_ALREADYREGISTERED(cli->getNick()), fd); return;}
	else
		cli->setUser(splited_cmd[1]);
	if(cli && cli->isClientRegistered() && !cli->getUser().empty() && !cli->getNick().empty() && cli->getNick() != "*"  && !cli->isLogged())
	{
		cli->setLogin(true);
		server->_sendResponse(RPL_CONNECTED(cli->getNick()), fd);
	}
}

void User::execute(std::string cmd, int fd) {
    set_clientUsername(cmd, fd);
}
