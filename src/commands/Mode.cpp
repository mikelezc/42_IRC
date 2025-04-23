/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:52:27 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:07:50 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Mode.hpp"

Mode::Mode(Server* server) : Command(server), server(server) {}
Mode::~Mode() {}

Mode::Mode(const Mode &other) : Command(other.server), server(other.server) {}

Mode &Mode::operator=(const Mode &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

std::string Mode::appendMode(std::string chain, char opera, char mode)
{
    char last = '\0';
    // Iterar sobre cada carácter de la cadena para encontrar el último operador
    for (size_t i = 0; i < chain.size(); ++i)
	{
        if (chain[i] == '+' || chain[i] == '-') {
            last = chain[i];
        }
    }
    if (last != opera) {
        return std::string(1, opera) + mode;
    } else {
        return std::string(1, mode);
    }
}

void Mode::getCmdArgs(const std::string& cmd, std::string& name, std::string& modeset, std::string& params)
{
    size_t pos = 0;
    
    while (pos < cmd.size() && cmd[pos] != ' ' && cmd[pos] != '\t') {
        name += cmd[pos];
        pos++;
    }
    
    while (pos < cmd.size() && (cmd[pos] == ' ' || cmd[pos] == '\t')) {
        pos++;
    }
    
    while (pos < cmd.size() && cmd[pos] != ' ' && cmd[pos] != '\t') {
        modeset += cmd[pos];  // Agregar cada carácter hasta un espacio
        pos++;
    }
    
    while (pos < cmd.size() && (cmd[pos] == ' ' || cmd[pos] == '\t')) {
        pos++;
    }
    
    if (pos < cmd.size()) {
        params = cmd.substr(pos);
    } else {
        params.clear();
    }
}

std::vector<std::string> Mode::splitByComma(std::string params) {
    
    if (!params.empty() && params[0] == ':') {
        params.erase(params.begin());
    }
    
    std::vector<std::string> tokens;
    std::string param;
    size_t start = 0;
    
    while (start < params.size()) {
       
        size_t end = params.find(',', start);
        
        if (end == std::string::npos) {
            tokens.push_back(params.substr(start)); 
            break;
        }
        
        tokens.push_back(params.substr(start, end - start));
        start = end + 1;
    }
    return tokens;
}

void Mode::mode_command(std::string& cmd, int fd)
{
    Client *cli = server->getClient(fd);
    std::string channelName, params, modeset, arguments = ":";
    Channel *channel;
    std::stringstream mode_chain;
    char opera = '\0';
    size_t pos = 0;
    
    size_t found = cmd.find_first_not_of("MODEmode \t\v");
    if (found != std::string::npos)
        cmd = cmd.substr(found);
    else
    {
        server->_sendResponse(ERR_NOTENOUGHPARAM(cli->getNick()), fd);
        return;
    }
    
    getCmdArgs(cmd, channelName, modeset, params);
    std::vector<std::string> tokens = splitByComma(params);
    
    if (channelName.empty() || channelName[0] != '#' || !(channel = server->getChannel(channelName.substr(1))))
    {
        server->_sendResponse(ERR_CHANNELNOTFOUND(cli->getUser(), channelName), fd);
        return;
    }
    
    if (!channel->getClientFromChannel(fd) && !channel->getAdminFromChannel(fd))
    {
        server->senderror(442, cli->getNick(), cli->getFd(), " :You're not on that channel\r\n", channelName);
        return;
    }
    
    if (modeset.empty())
    {
        server->_sendResponse(RPL_CHANNELMODES(cli->getNick(), channel->getChannelName(), channel->getConfiguredModeFromChannel()) +
                              RPL_CREATIONTIME(cli->getNick(), channel->getChannelName(), channel->getTimeChannelWasCreated()), fd);
        return;
    }
    
    if (!channel->getAdminFromChannel(fd))
    {
        server->_sendResponse(ERR_NOTOPERATOR(channel->getChannelName()), fd);
        return;
    }
    
    for (size_t i = 0; i < modeset.size(); i++)
    {
        if (modeset[i] == '+' || modeset[i] == '-')
            opera = modeset[i];
        else
        {
            switch (modeset[i])
            {
                case 'i':
                    mode_chain << inviteOnlyMode(channel, opera, mode_chain.str());
                    break;
                case 't':
                    mode_chain << topicRestrictionMode(channel, opera, mode_chain.str());
                    break;
                case 'k':
                    mode_chain << setPasswordMode(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
                    break;
                case 'o':
                    mode_chain << setOperatorPrivilege(tokens, channel, pos, fd, opera, mode_chain.str(), arguments);
                    break;
                case 'l':
                    mode_chain << setChannelLimit(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
                    break;
                default:
                    server->_sendResponse(ERR_UNKNOWNMODE(cli->getNick(), channel->getChannelName(), modeset[i]), fd);
                    return;
            }
        }
    }
    
    std::string chain = mode_chain.str();
    if (!chain.empty())
        channel->allMembersSender(RPL_CHANGEMODE(cli->getHostname(), channel->getChannelName(), chain, arguments), -1);
}


std::string Mode::inviteOnlyMode(Channel *channel, char opera, std::string chain)
{
    if (opera == '+' && !channel->getChannelMode(0))
    {
        channel->setChannelMode(0, true);
        channel->setInviteOnly(true);
        return appendMode(chain, opera, 'i');
    }
    else if (opera == '-' && channel->getChannelMode(0))
    {
        channel->setChannelMode(0, false);
        channel->setInviteOnly(false);
        return appendMode(chain, opera, 'i');
    }
    return "";
}


std::string Mode::topicRestrictionMode(Channel *channel, char opera, std::string chain)
{
    
    if (opera == '+' && !channel->getChannelMode(1))
    {
        channel->setChannelMode(1, true);
        channel->setTopicRestricted(true);
        return appendMode(chain, opera, 't');
    }
    
    else if (opera == '-' && channel->getChannelMode(1))
    {
        channel->setChannelMode(1, false);
        channel->setTopicRestricted(false);
        return appendMode(chain, opera, 't');
    }

    return "";
}

bool isValidPassword(std::string password)
{
	if(password.empty())
		return false;
	for(size_t i = 0; i < password.size(); i++)
	{
		if(!std::isalnum(password[i]) && password[i] != '_')
			return false;
	}
	return true;
}
std::string Mode::setPasswordMode(std::vector<std::string> tokens, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments)
{
	std::string param;
	std::string pass;

	param.clear();
	pass.clear();
	if(tokens.size() > pos)
		pass = tokens[pos++];
	else
	{
		server->_sendResponse(ERR_NEEDMODEPARM(channel->getChannelName(),std::string("(k)")),fd);
		return param;
	}
	if(!isValidPassword(pass))
	{
		server->_sendResponse(ERR_INVALIDMODEPARM(channel->getChannelName(),std::string("(k)")),fd);
		return param;
	}
	if(opera == '+')
	{
		channel->setChannelMode(2, true);
		channel->setChannelKey(pass);
		if(!arguments.empty())
			arguments += " ";
		arguments += pass;
		param = appendMode(chain,opera, 'k');
	}
	else if (opera == '-' && channel->getChannelMode(2))
	{
		if(pass == channel->getPassword())
		{		
			channel->setChannelMode(2, false);
			channel->setChannelKey("");
			param = appendMode(chain,opera, 'k');
		}
		else
			server->_sendResponse(ERR_KEYSET(channel->getChannelName()),fd);
	}
	return param;
}

std::string Mode::setOperatorPrivilege(std::vector<std::string> tokens, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments)
{
	std::string user;
	std::string param;

	param.clear();
	user.clear();
	if(tokens.size() > pos)
		user = tokens[pos++];
	else
	{
		server->_sendResponse(ERR_NEEDMODEPARM(channel->getChannelName(),"(o)"),fd);
		return param;
	}
	if(!channel->isClientInChannel(user))
	{
		server->_sendResponse(ERR_NOSUCHNICK(channel->getChannelName(), user),fd);
		return param;
	}
	if(opera == '+')
	{
		channel->setChannelMode(3,true);
		if(channel->clientToAdminUpgrd(user))
		{
			param = appendMode(chain, opera, 'o');
			if(!arguments.empty())
				arguments += " ";
			arguments += user;
		}
	}
	else if (opera == '-')
	{
		channel->setChannelMode(3,false);
		if(channel->adminToClientDwngrd(user))
		{
			param = appendMode(chain, opera, 'o');
				if(!arguments.empty())
					arguments += " ";
			arguments += user;

		}
	}
	return param;
}

bool Mode::isValidLimit(std::string& limit)
{
	return (!(limit.find_first_not_of("0123456789")!= std::string::npos) && std::atoi(limit.c_str()) > 0);
}

std::string Mode::setChannelLimit(std::vector<std::string> tokens,  Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments)
{
	std::string limit;
	std::string param;

	param.clear();
	limit.clear();
	if(opera == '+')
	{
		if(tokens.size() > pos )
		{
			limit = tokens[pos++];
			if(!isValidLimit(limit))
			{
				server->_sendResponse(ERR_INVALIDMODEPARM(channel->getChannelName(),"(l)"), fd);
			}
			else
			{
				channel->setChannelMode(4, true);
				channel->setUsersLimit(std::atoi(limit.c_str()));
				if(!arguments.empty())
					arguments += " ";
				arguments += limit;
				param =  appendMode(chain, opera, 'l');
			}
		}
		else
			server->_sendResponse(ERR_NEEDMODEPARM(channel->getChannelName(),"(l)"),fd);
	}
	else if (opera == '-' && channel->getChannelMode(4))
	{
		channel->setChannelMode(4, false);
		channel->setUsersLimit(0);
		param  = appendMode(chain, opera, 'l');
	}
	return param;
}

void	Mode::execute(std::string cmd, int fd)
{
	mode_command(cmd, fd);
}