/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 22:21:46 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/06 21:14:35 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Join.hpp"
#include <vector>
#include <sstream>
#include <string>

Join::Join(Server *server) : Command(server) {}
Join::~Join(){}

Join::Join(const Join &other) : Command(other.server) {}

Join &Join::operator=(const Join &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

int Join::SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd)
{
    std::vector<std::string> tmp;
    std::string ChStr, PassStr;

    // Extract the commands
    std::istringstream iss(cmd);
    while (iss >> cmd)
        tmp.push_back(cmd);
    
    if (tmp.size() < 2) {
        token.clear();
        return 0;
    }
    tmp.erase(tmp.begin());
    
// Assign the channel chain and password chain
    ChStr = tmp[0];
    tmp.erase(tmp.begin());
    
    if (!tmp.empty()) {
        PassStr = tmp[0];
    }

    ProcessChannels(ChStr, token);
    ProcessPasswords(PassStr, token);

    //delete name empty channels
    for (size_t i = 0; i < token.size(); i++) {
        if (token[i].first.empty())
            token.erase(token.begin() + i--);
    }

    // Validate channels
    for (size_t i = 0; i < token.size(); i++) {
        if (*(token[i].first.begin()) != '#') {
            server->senderror(403, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :No such channel\r\n", token[i].first);
            token.erase(token.begin() + i--);
        } else {
            token[i].first.erase(token[i].first.begin());
        }
    }

    return 1;
}

void Join::ProcessChannels(const std::string& ChStr, std::vector<std::pair<std::string, std::string> >& token) {
    std::string buff;

    for (size_t i = 0; i < ChStr.size(); i++) {
        if (ChStr[i] == ',') {
            token.push_back(std::make_pair(buff, ""));
            buff.clear();
        } else {
            buff += ChStr[i];
        }
    }
    token.push_back(std::make_pair(buff, ""));
}

void Join::ProcessPasswords(const std::string& PassStr, std::vector<std::pair<std::string, std::string> >& token) {
    if (!PassStr.empty()) {
        size_t j = 0;
        std::string buff;

        for (size_t i = 0; i < PassStr.size(); i++) {
            if (PassStr[i] == ',') {
                token[j].second = buff;
                j++;
                buff.clear();
            } else {
                buff += PassStr[i];
            }
        }
        token[j].second = buff;
    }
}

int Join::SearchForClients(std::string nickname) {
    int count = 0;
    for (size_t i = 0; i < server->getChannels().size(); i++) {
        if (server->getChannels()[i].getClientOrAdminFromChannel(nickname)) {
            count++;
        }
    }
    return count;
}

bool IsInvited(Client *cli, std::string channelName, int flag) {
    if (cli->isInvitedChannel(channelName))
    {
        if (flag == 1) {
            cli->eraseChannelInvite(channelName);
        }
        return true;
    }
    return false;
}

void Join::JoinExistingChannel(std::vector<std::pair<std::string, std::string> > &token, int i, int j, int fd) {
    Client *cli = server->getClient(fd);

    if (server->getChannels()[j].getClientOrAdminFromChannel(cli->getNick())) {
        return;
    }

    if (SearchForClients(cli->getNick()) >= 10) {
        server->senderror(405, cli->getNick(), cli->getFd(), " :You have joined too many channels\r\n");
        return;
    }

    if (!server->getChannels()[j].getPassword().empty() && server->getChannels()[j].getPassword() != token[i].second) {
        if (!IsInvited(cli, token[i].first, 0)) {
            server->senderror(475, cli->getNick(), cli->getFd(), " :Cannot join channel (+k) - bad key\r\n", "#" + token[i].first);
            return;
        }
    }

    if (server->getChannels()[j].getInviteOnly()) {
        if (!IsInvited(cli, token[i].first, 1)) {
            server->senderror(473, cli->getNick(), cli->getFd(), " :Cannot join channel (+i)\r\n", "#" + token[i].first);
            return;
        }
    }

    if (server->getChannels()[j].getUsersLimit() && server->getChannels()[j].getTotalNbrClients() >= server->getChannels()[j].getUsersLimit()) {
        server->senderror(471, cli->getNick(), cli->getFd(), " :Cannot join channel (+l)\r\n", "#" + token[i].first);
        return;
    }

    server->getChannels()[j].channelAddClient(*cli);
    // send msg to channel and clients 
    std::string response = RPL_JOINMSG(cli->getHostname(), cli->getIpAddress(), token[i].first) + 
                           RPL_NAMREPLY(cli->getNick(), server->getChannels()[j].getChannelName(), server->getChannels()[j].whoIsInChannel()) + 
                           RPL_ENDOFNAMES(cli->getNick(), server->getChannels()[j].getChannelName());

    if (server->getChannels()[j].getChannelTopic().empty()) {
        server->_sendResponse(response, fd);
    } else {
        response += RPL_TOPICIS(cli->getNick(), server->getChannels()[j].getChannelName(), server->getChannels()[j].getChannelTopic());
        server->_sendResponse(response, fd);
    }

    server->getChannels()[j].allMembersSender(RPL_JOINMSG(cli->getHostname(), cli->getIpAddress(), token[i].first), fd);
}

void Join::CreateChannelIfNotExists(std::vector<std::pair<std::string, std::string> > &token, int i, int fd)
{
    Client *cli = server->getClient(fd);

    if (SearchForClients(cli->getNick()) >= 10) {
        server->senderror(405, cli->getNick(), cli->getFd(), " :You have joined too many channels\r\n");
        return;
    }

    Channel newChannel;
    newChannel.setChannelName(token[i].first);
    newChannel.channelAddAdmin(*cli);
    newChannel.setTimeChannelWasCreated();
    server->getChannels().push_back(newChannel);

    // send client is join
    std::string response = RPL_JOINMSG(cli->getHostname(), cli->getIpAddress(), newChannel.getChannelName()) + 
                           RPL_NAMREPLY(cli->getNick(), newChannel.getChannelName(), newChannel.whoIsInChannel()) + 
                           RPL_ENDOFNAMES(cli->getNick(), newChannel.getChannelName());

    server->_sendResponse(response, fd);
}

void Join::execute(std::string cmd, int fd) {
    std::vector<std::pair<std::string, std::string> > token;

    // Dividir el comando y validar
    if (!SplitJoin(token, cmd, fd)) 
    { 
        server->senderror(461, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :Not enough parameters\r\n");
        return;
    }


    if (token.size() > 10) 
    {
        server->senderror(407, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :Too many channels\r\n");
        return;
    }

    for (size_t i = 0; i < token.size(); i++) {
        bool exists = false;
        for (size_t j = 0; j < server->getChannels().size(); j++) {
            if (server->getChannels()[j].getChannelName() == token[i].first) {
                JoinExistingChannel(token, i, j, fd);
                exists = true;
                break;
            }
        }
        if (!exists) {
            CreateChannelIfNotExists(token, i, fd);
        }
    }
}
