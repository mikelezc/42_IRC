/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 10:43:18 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:17:48 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Topic.hpp"

Topic::Topic(Server *server) : Command(server){}

Topic::~Topic() {}

Topic::Topic(const Topic &other) : Command(other.server){}

Topic &Topic::operator=(const Topic &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

//Devuelve la representación en cadena del tiempo actual en segundos desde el 1 de enero de 1970 
//(época UNIX, como en el philo)
std::string Topic::formatCurrentTime()
{
	std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}
std::string Topic::getTopic(const std::string &input) {
    size_t pos = input.find(":");
    if (pos == std::string::npos) {
        
        return ""; 
    }  
    std::string topic = input.substr(pos + 1);
    topic.erase(0, topic.find_first_not_of(" \t"));
    return topic;
}

int Topic::getPos(const std::string &cmd)
{
	for (int i = 0; i < (int)cmd.size(); i++)
		if (cmd[i] == ':' && (cmd[i - 1] == ' '))
			return i;
	return -1;
}

void Topic::execute(std::string cmd, int fd)
{
    // Verificar comandos inválidos
    if (cmd == "TOPIC :") {
        server->senderror(461, server->getClient(fd)->getNick(), fd, " :Not enough parameters\r\n");
        return;
    }
    
    std::vector<std::string> scmd = server->split_cmd(cmd);
    if (scmd.size() <= 1) {
        server->senderror(461, server->getClient(fd)->getNick(), fd, " :Not enough parameters\r\n");
        return;
    }
    std::string nmch = scmd[1].substr(1);
    Channel* channel = server->getChannel(nmch);
    
    if (!channel) {
        server->senderror(403, server->getClient(fd)->getNick(), fd, " :No such channel\r\n");
        return;
    }

    if (!channel->getClientFromChannel(fd) && !channel->getAdminFromChannel(fd)) {
        server->senderror(442, server->getClient(fd)->getNick(), fd, " :You're not on that channel\r\n");
        return;
    }

    
    if (scmd.size() == 2) {
        if (channel->getChannelTopic().empty()) {
            server->_sendResponse(": 331 " + server->getClient(fd)->getNick() + " " + "#" + nmch + " :No topic is set\r\n", fd);
            return;
        }

        std::string topic = channel->getChannelTopic();
        server->_sendResponse(": 332 " + server->getClient(fd)->getNick() + " " + "#" + nmch + " " + topic + CRLF, fd); 
        server->_sendResponse(": 333 " + server->getClient(fd)->getNick() + " " + "#" + nmch + " " + server->getClient(fd)->getNick() + " " + channel->getTopicCreatedTime() + CRLF, fd); // RPL_TOPICWHOTIME (333)
        return;
    }

    if (scmd.size() >= 3) {
        int pos = getPos(cmd);
        std::string newTopic = (pos == -1 || scmd[2][0] != ':') ? scmd[2] : cmd.substr(pos);

        if (newTopic.empty() || newTopic == ":") {
            server->senderror(331, "#" + nmch, fd, " :No topic is set\r\n");
            return;
        }

        bool isRestricted = channel->getTopicRestricted();
        if (isRestricted && !channel->getClientFromChannel(fd)) {
            server->senderror(482, "#" + nmch, fd, " :You're Not a channel operator\r\n"); 
            return;
        }

        channel->setChannelTopic(newTopic);
        channel->setTopicCreatedTime(formatCurrentTime());

        std::string rpl = ":" + server->getClient(fd)->getNick() + "!" + server->getClient(fd)->getUser() + "@localhost TOPIC #" + nmch + " " + newTopic + CRLF;
        channel->allMembersSender(rpl, -1);
    }
}