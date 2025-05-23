/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:28:42 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:21:06 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Privmsg.hpp"

PrivMsg::PrivMsg(Server* server) : Command(server), server(server) {}

PrivMsg::~PrivMsg() {}

PrivMsg::PrivMsg(const PrivMsg &other) : Command(other.server), server(other.server) {}

PrivMsg &PrivMsg::operator=(const PrivMsg &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

void FindPM(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind) break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string SplitCmdPM(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, msg;
	int count = 2;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 2) return std::string("");
	FindPM(cmd, tmp[1], msg);
	return msg;
}

std::string SplitCmdPrivmsg(std::string cmd, std::vector<std::string> &tmp)
{
	std::string str = SplitCmdPM(cmd, tmp);
	if (tmp.size() != 2) {tmp.clear(); return std::string("");}
	tmp.erase(tmp.begin());
	std::string str1 = tmp[0]; std::string str2; tmp.clear();
	for (size_t i = 0; i < str1.size(); i++){//split the first string by ',' to get the channels names
		if (str1[i] == ',')
			{tmp.push_back(str2); str2.clear();}
		else str2 += str1[i];
	}
	tmp.push_back(str2);
	for (size_t i = 0; i < tmp.size(); i++)//erase the empty strings
		{if (tmp[i].empty())tmp.erase(tmp.begin() + i--);}
	if (str[0] == ':') str.erase(str.begin());
	else //shrink to the first space
		{for (size_t i = 0; i < str.size(); i++){if (str[i] == ' '){str = str.substr(0, i);break;}}}
	return  str;
	
}

void	PrivMsg::CheckForChannels_Clients(std::vector<std::string> &tmp, int fd)
{
	for(size_t i = 0; i < tmp.size(); i++){
		if (tmp[i][0] == '#'){
			tmp[i].erase(tmp[i].begin());
			if(!server->getChannel(tmp[i]))
			{
				server->senderror(401, "#" + tmp[i], server->getClient(fd)->getFd(), " :No such nick/channel\r\n");
				tmp.erase(tmp.begin() + i);
				i--;
			}
			else if (!server->getChannel(tmp[i])->getClientOrAdminFromChannel(server->getClient(fd)->getNick()))
			{
				server->senderror(404, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :Cannot send to channel\r\n", "#" + tmp[i]);
				tmp.erase(tmp.begin() + i);
				i--;
			}
			else tmp[i] = "#" + tmp[i];
		}
		else{
			if (!server->getClientNick(tmp[i]))
			{
				server->senderror(401, tmp[i], server->getClient(fd)->getFd(), " :No such nick/channel\r\n");
				tmp.erase(tmp.begin() + i);
				i--;
			}
		}
	}
}

void	PrivMsg::execute(std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string message = SplitCmdPrivmsg(cmd, tmp);
	if (!tmp.size())
	{
		server->senderror(411, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :No recipient given (PRIVMSG)\r\n");
		return;
	}
	if (message.empty())
	{
		server->senderror(412, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :No text to send\r\n");
		return;
	}
	if (tmp.size() > 10)
	{
		server->senderror(407, server->getClient(fd)->getNick(), server->getClient(fd)->getFd(), " :Too many recipients\r\n");
		return;
	}
	CheckForChannels_Clients(tmp, fd); // check if the channels and clients exist
	for (size_t i = 0; i < tmp.size(); i++){// send the message to the clients and channels
		if (tmp[i][0] == '#'){
			tmp[i].erase(tmp[i].begin());
			std::string resp = ":" + server->getClient(fd)->getNick() + "!~" + server->getClient(fd)->getUser() + "@localhost PRIVMSG #" + tmp[i] + " :" + message + CRLF;
			server->getChannel(tmp[i])->allMembersSender(resp, fd);
		}
		else{
			std::string resp = ":" + server->getClient(fd)->getNick() + "!~" + server->getClient(fd)->getUser() + "@localhost PRIVMSG " + tmp[i] + " :" + message + CRLF;
			server->_sendResponse(resp, server->getClientNick(tmp[i])->getFd());
		}
	}
}
