/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:26:32 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:21:35 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class PrivMsg : public Command
{
    private:
        Server *server;
	public:
        PrivMsg(Server* server);
        ~PrivMsg();

        PrivMsg(const PrivMsg &other);
        PrivMsg &operator=(const PrivMsg &other);

		void	CheckForChannels_Clients(std::vector<std::string> &tmp, int fd);
		void	execute(std::string cmd, int fd); //este no sirve, pero debe ser declarado porque command lo tiene como abstracto;
};

#endif