/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:32:12 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/03 13:10:45 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Join.hpp"
#include "../inc/commands/Privmsg.hpp"
#include "../inc/commands/Mode.hpp"
#include "../inc/commands/Invite.hpp"
#include "../inc/commands/Topic.hpp"
#include "../inc/commands/Part.hpp"
#include "../inc/commands/Kick.hpp"
#include "../inc/commands/Quit.hpp"
#include "../inc/commands/Nick.hpp"
#include "../inc/commands/User.hpp"
#include "../inc/commands/Pass.hpp"

std::vector<std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string> tokens;
    std::istringstream commandStream(cmd);
    std::string token;
    while (commandStream >> token)
    {
        tokens.push_back(token);
        //token.clear(); no es necesario, ya que por cada vuelta se sobrescribe
    }
    return tokens;
}

bool Server::isClientRegistered(int fd)
{
    Client* client = getClient(fd); // Llamamos una vez a getClient
    
    if (!client || client->getNick().empty() || client->getUser().empty() ||
     client->getNick() == "*" || !client->isLogged())
        return false;
    return true;
}

void Server::parse_exec_cmd(std::string &cmd, int fd)
{
    if (cmd.empty())
        return;
    
    //Eliminamos espacios en blanco iniciales
    size_t found = cmd.find_first_not_of(" \t\v");
    if (found != std::string::npos)
        cmd = cmd.substr(found);
    
    //Dividimos el comando
    std::vector<std::string> splited_cmd = split_cmd(cmd);
    
    //Convertidor del comando a minúsculas para compararlo
    std::string command = splited_cmd[0];
    for (size_t i = 0; i < command.size(); i++)
    {
        command[i] = std::tolower(command[i]);
    }

    //Comparamos y lanzamos 
    // Comando ignorado: BONG
    if (command == "bong")
        return;

    // Procesar comandos antes del registro
    if (command == "pass")
    {
        Pass pass(this);
        pass.execute(cmd, fd);
        return;
    }
    if (command == "nick")
    {
        Nick nick(this);
        nick.execute(cmd, fd);
        return;
    }
    if (command == "user")
    {
        User user(this);
        user.execute(cmd, fd);
        return;
    }
    if (command == "quit")
    {
        Quit quit(this);
        quit.execute(cmd, fd);
        return;
    }
    // Verificar si el cliente está registrado antes de ejecutar otros comandos
    if (!isClientRegistered(fd))
    {
        _sendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
        return;
    }
    if (command == "join")
    {
        Join join(this);
        join.execute(cmd, fd);
    }
    else if (command == "privmsg")
    {
        PrivMsg privmsg(this);
        privmsg.execute(cmd, fd);
    }
    else if (command == "mode")
    {
        Mode mode(this);
        mode.execute(cmd, fd);
    }
    else if (command == "invite")
    {
        Invite invite(this);
        invite.execute(cmd, fd);
    }
    else if (command == "topic")
    {
        Topic topic(this);
        topic.execute(cmd, fd);
    }
    else if (command == "part")
    {
        Part part(this);
        part.execute(cmd, fd);
    }
    else if (command == "kick")
    {
        Kick kick(this);
        kick.execute(cmd, fd);
    }
    else
    {
        // Si no coincide con ningún comando
        _sendResponse(ERR_CMDNOTFOUND(getClient(fd)->getNick(), splited_cmd[0]), fd);
    }
}

void Server::senderror(int code, const std::string& clientname, int fd, const std::string& msg, const std::string& channelname)
{
    std::stringstream ss;
    ss << ":localhost " << code << " " << clientname;
    
    // Agrega el nombre del canal solo si se proporciona
    if (!channelname.empty()) {
        ss << " " << channelname;
    }

    ss << msg; // Agrega el mensaje al final
    std::string resp = ss.str();
    
    // Envía la respuesta al cliente
    if (send(fd, resp.c_str(), resp.size(), 0) == -1) {
        std::cerr << "send() failed" << std::endl;
    }
}

void Server::_sendResponse(std::string response, int fd)
{
    if (send(fd, response.c_str(), response.size(), 0) == -1)
        std::cerr << "Response send() faild" << std::endl;
}
