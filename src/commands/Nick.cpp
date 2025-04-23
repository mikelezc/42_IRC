/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:29:53 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:46:55 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Nick.hpp"

Nick::Nick(Server* server) : Command(server) {}

Nick::~Nick() {}

Nick::Nick(const Nick &other) : Command(other.server) {}

Nick &Nick::operator=(const Nick &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}


bool Nick::is_validNickname(std::string& nickname)
{
		
	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for(size_t i = 1; i < nickname.size(); i++)
	{
		if(!std::isalnum(nickname[i]) && nickname[i] != '_')
			return false;
	}
	return true;
}


bool Nick::nickNameInUse(std::string& nickname) {
    // Utiliza el método getClientNick del servidor
    return server->getClientNick(nickname) != NULL;
}


void Nick::set_clientNickname(std::string cmd, int fd) {
	std::string nickname = cmd.substr(4); 
	Client* cli = server->getClient(fd);

	
	size_t pos = nickname.find_first_not_of(" \t\v");
	if (pos != std::string::npos) {
		nickname = nickname.substr(pos);
		if (nickname[0] == ':')
			nickname.erase(0, 1);
	} else {
		server->_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}

	
	if (nickNameInUse(nickname) && cli->getNick() != nickname) {
		server->_sendResponse(ERR_clientNickINUSE(nickname), fd);
		return;
	}

	
	if (!is_validNickname(nickname)) {
		server->_sendResponse(ERR_ERRONEUSNICK(nickname), fd);
		return;
	}

	std::string oldnick = cli->getNick();
	cli->setNick(nickname);

	
	if (!oldnick.empty() && oldnick != nickname) {
		if (oldnick == "*" && !cli->getUser().empty()) {
			cli->setLogin(true);
			server->_sendResponse(RPL_CONNECTED(cli->getNick()), fd);
			server->_sendResponse(RPL_clientNickCHANGE(oldnick, nickname), fd);
		} else {
			server->_sendResponse(RPL_clientNickCHANGE(oldnick, nickname), fd);
		}
		return;
	}

	if (!cli->isClientRegistered()) {
		server->_sendResponse(ERR_NOTREGISTERED(nickname), fd);
		return;
	}


	if (!cli->getUser().empty() && cli->isClientRegistered() && !cli->isLogged()) {
		cli->setLogin(true);
		server->_sendResponse(RPL_CONNECTED(cli->getNick()), fd);
	}
}


void Nick::execute(std::string cmd, int fd) {
    set_clientNickname(cmd, fd);
}