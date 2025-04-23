/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 19:22:03 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:03:31 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Invite.hpp"

Invite::Invite(Server* server) : Command(server), server(server) {}

Invite::~Invite() {}

Invite::Invite(const Invite &other) : Command(other.server), server(other.server) {}

Invite &Invite::operator=(const Invite &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}


void Invite::processInvite(std::string &cmd, int &fd)
{
    std::vector<std::string> scmd = server->split_cmd(cmd);

    if (scmd.size() < 3)
    {
        server->senderror(461, server->getClient(fd)->getNick(), fd, ":Not enough parameters");
        return;
    }

    std::string channelName = scmd[2].substr(1);
    Channel* channel = server->getChannel(channelName);

    if (scmd[2][0] != '#' || !channel)
    {
        server->senderror(403, channelName, fd, ":No such channel");
        return;
    }

    if (!channel->getClientFromChannel(fd) && !channel->getAdminFromChannel(fd))
    {
        server->senderror(442, channelName, fd, ":You're not on that channel");
        return;
    }

    if (channel->getClientOrAdminFromChannel(scmd[1]))
    {
        server->senderror(443, server->getClient(fd)->getNick(), fd, " :is already on channel");
        return;
    }

    Client* clt = server->getClientNick(scmd[1]);

    if (!clt)
    {
        server->senderror(401, scmd[1], fd, ":No such nick");
        return;
    }

    if (channel->getInviteOnly() && !channel->getAdminFromChannel(fd))
    {
        server->senderror(482, server->getClient(fd)->getNick(), fd, ":You're not channel operator");
        return;
    }

    if (channel->getUsersLimit() && channel->getTotalNbrClients() >= channel->getUsersLimit())
    {
        server->senderror(471, server->getClient(fd)->getNick(), fd, ":Cannot invite to channel (+i)");
        return;
    }
    clt->addChannelInvite(channelName);
    
    std::string rep1 = ": 341 " + server->getClient(fd)->getNick() + " " + clt->getNick() + " " + scmd[2] + CRLF;
    server->_sendResponse(rep1, fd);

    std::string rep2 = ":" + clt->getHostname() + " INVITE " + clt->getNick() + " " + scmd[2] + CRLF;
    server->_sendResponse(rep2, clt->getFd());
}

void Invite::execute(std::string cmd, int fd)
{
    processInvite(cmd, fd);
}