/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:01:42 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:20:32 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Pass.hpp"

Pass::Pass(Server* server) : Command(server), server(server) {}

Pass::~Pass() {}

Pass::Pass(const Pass &other) : Command(other.server), server(other.server) {}

Pass &Pass::operator=(const Pass &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

void Pass::execute(std::string cmd, int fd) {
    Client *cli = server->getClient(fd);

    cmd = cmd.substr(4);
    size_t pos = cmd.find_first_not_of("\t\v ");
        if (pos != std::string::npos) {
        cmd = cmd.substr(pos);
        if (cmd[0] == ':') {
            cmd.erase(cmd.begin());
        }
    }
    if (cmd.empty()) {
        server->_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
        return;
    }
    if (!cli->isClientRegistered()) {
        std::string pass = cmd;
        if (pass == server->getPassword()) {
            cli->setRegistered(true);
        } else {
            server->_sendResponse(ERR_INCORPASS(std::string("*")), fd);
        }
    } else {
        server->_sendResponse(ERR_ALREADYREGISTERED(cli->getNick()), fd);
    }
}
