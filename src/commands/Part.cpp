/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 11:02:45 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:28:40 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Part.hpp"

Part::Part(Server *server) : Command(server){}

Part::~Part() {}

Part::Part(const Part &other) : Command(other.server) {}

Part &Part::operator=(const Part &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

void findAndExtractPartCmd(std::string cmd, std::string tofind, std::string &str)
{
    
    size_t pos = cmd.find(tofind);
   
    if (pos != std::string::npos) {
        
        str = cmd.substr(pos + tofind.length());
        
        size_t startPos = str.find_first_not_of(' ');
        if (startPos != std::string::npos)
            str = str.substr(startPos);
		else 
            str.clear();
    } 
	else
	{
        
        str.clear();
    }
}

std::string SplitCmdAndExtract(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, reason;
	int count = 2;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 2) return std::string("");
	findAndExtractPartCmd(cmd, tmp[1], reason);
	return reason;
}


void splitChannels(const std::string &channelsString, std::vector<std::string> &channels) {
    std::string channel;
    for (size_t i = 0; i < channelsString.size(); i++) {
        if (channelsString[i] == ',') {
            if (!channel.empty()) {
                channels.push_back(channel);
                channel.clear();
            }
        } else {
            channel += channelsString[i];
        }
    }
    
    if (!channel.empty()) {
        channels.push_back(channel);
    }
}


void cleanEmptyStrings(std::vector<std::string> &vec) {
    for (size_t i = 0; i < vec.size(); ) {
        if (vec[i].empty()) {
            vec.erase(vec.begin() + i);
        } else {
            ++i;
        }
    }
}


void trimReason(std::string &reason) {
    if (reason[0] == ':') {
        reason.erase(reason.begin());
    } else {
        size_t spacePos = reason.find(' ');
        if (spacePos != std::string::npos) {
            reason = reason.substr(0, spacePos);
        }
    }
}

int Part::SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd) {
    
    reason = SplitCmdAndExtract(cmd, tmp);
    if (tmp.size() < 2) {
        tmp.clear();
        return 0;
    }

    std::string channelsString = tmp[0];
    tmp.erase(tmp.begin());

    
    splitChannels(channelsString, tmp);

   
    cleanEmptyStrings(tmp);

   
    trimReason(reason);

    
    for (size_t i = 0; i < tmp.size(); ) {
        if (!tmp[i].empty() && tmp[i][0] == '#') {
            tmp[i].erase(tmp[i].begin()); 
            ++i;
        } else {
            
            server->senderror(403, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :No such channel\r\n", tmp[i]);
            tmp.erase(tmp.begin() + i);
            
        }
    }
    return 1;
}

void Part::execute(std::string cmd, int fd) {
    std::vector<std::string> tmp;
    std::string reason;

    
    if (!SplitCmdPart(cmd, tmp, reason, fd)) { 
        server->senderror(461, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :Not enough parameters\r\n");
        return;
    }
    std::string nick = server->getClient(fd)->getNick();
    std::string user = server->getClient(fd)->getUser();
    
    for (size_t i = 0; i < tmp.size(); i++) {
        std::string channel = tmp[i];
        bool isChannelFound = false;
        
        for (size_t j = 0; j < server->getChannels().size(); j++) {
            if (server->getChannels()[j].getChannelName() == channel) {
                isChannelFound = true;
                
                if (!server->getChannels()[j].getClientFromChannel(fd) && !server->getChannels()[j].getAdminFromChannel(fd)) {
                    server->senderror(442, nick, server->getClient(fd)->getFd(), " :You're not on that channel\r\n", "#" + channel); // Error si no está en el canal
                    continue;
                }
                
                std::stringstream ss;
                ss << ":" << nick << "!~" << user << "@localhost PART #" << channel;
                if (!reason.empty()) {
                    ss << " :" << reason;
                }
                ss << CRLF;
                
                server->getChannels()[j].allMembersSender(ss.str(), -1);
                
                int clientFd = server->getChannels()[j].getClientOrAdminFromChannel(nick)->getFd();
                if (server->getChannels()[j].getAdminFromChannel(clientFd)) {
                    server->getChannels()[j].channelEraseAdmin(clientFd);
                } else {
                    server->getChannels()[j].channelEraseClient(clientFd);
                }

                
                if (server->getChannels()[j].getTotalNbrClients() == 0) {
                    server->getChannels().erase(server->getChannels().begin() + j);
                }
                break;
            }
        }
        
        if (!isChannelFound) {
            server->senderror(403, nick, server->getClient(fd)->getFd(), " :No such channel\r\n", "#" + channel);
        }
    }
}
