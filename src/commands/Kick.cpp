/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:39:11 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 18:11:13 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Kick.hpp"


Kick::Kick(Server *server) : Command(server){}

Kick::~Kick(){};

Kick::Kick(const Kick &other) : Command(other.server){}

Kick &Kick::operator=(const Kick &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}


void FindCmdKick(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind)
				break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string SplitCmdKick(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, reason;
	int count = 3;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 3) return std::string("");
	FindCmdKick(cmd, tmp[2], reason);
	return reason;
}

std::string Kick::parseKickCommand(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd) {
	std::string reason = SplitCmdKick(cmd, tmp);

	if (tmp.size() < 3)
		return std::string("");

	tmp.erase(tmp.begin());
	std::string channelsStr = tmp[0];
	user = tmp[1];
	tmp.clear();

	std::stringstream ss(channelsStr);
	std::string channel;
	while (std::getline(ss, channel, ',')) {
		if (!channel.empty()) tmp.push_back(channel);
	}

	if (reason[0] == ':') reason.erase(reason.begin());
	else reason = reason.substr(0, reason.find(' '));

	for (size_t i = 0; i < tmp.size(); i++) {
		if (tmp[i][0] != '#') {
			server->senderror(403, server->getClient(fd)->getNick(), fd, " :No such channel\r\n", tmp[i]);
			tmp.erase(tmp.begin() + i--);
		} else {
			tmp[i].erase(0, 1);
		}
	}
	return reason;
}

void Kick::execute(std::string cmd, int fd) {
	std::vector<std::string> tmp;
	std::string reason, user;
	reason = parseKickCommand(cmd, tmp, user, fd);

	if (user.empty()) {
		server->senderror(461, server->getClient(fd)->getNick(), fd, " :Not enough parameters\r\n");
		return;
	}

	for (size_t i = 0; i < tmp.size(); i++) {
		Channel* ch = server->getChannel(tmp[i]);

		if (!ch) {
			server->senderror(403, server->getClient(fd)->getNick(), fd, " :No such channel\r\n", "#" + tmp[i]);
			continue;
		}

		if (!ch->getClientFromChannel(fd) && !ch->getAdminFromChannel(fd)) {
			server->senderror(442, server->getClient(fd)->getNick(), fd, " :You're not on that channel\r\n", "#" + tmp[i]);
			continue;
		}

		if (!ch->getAdminFromChannel(fd)) {
			server->senderror(482, server->getClient(fd)->getNick(), fd, " :You're not channel operator\r\n", "#" + tmp[i]);
			continue;
		}

		Client* clientToKick = ch->getClientOrAdminFromChannel(user);
		if (!clientToKick) {
			server->senderror(441, server->getClient(fd)->getNick(), fd, " :They aren't on that channel\r\n", "#" + tmp[i]);
			continue;
		}

		std::stringstream ss;
		ss << ":" << server->getClient(fd)->getNick() << "!~" << server->getClient(fd)->getUser() << "@localhost"
		   << " KICK #" << tmp[i] << " " << user << (reason.empty() ? "" : " :" + reason) << CRLF;
		ch->allMembersSender(ss.str(), -1);

		if (ch->getAdminFromChannel(clientToKick->getFd())) {
			ch->channelEraseAdmin(clientToKick->getFd());
		} else {
			ch->channelEraseClient(clientToKick->getFd());
		}

		if (ch->getTotalNbrClients() == 0) {
			server->getChannels().erase(server->getChannels().begin() + i);
		}
	}
}