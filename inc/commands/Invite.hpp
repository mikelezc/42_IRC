/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 19:22:17 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:03:56 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef INVITE_HPP
#define INVITE_HPP

#include "Command.hpp"
#include "../Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class Invite : public Command
{
    private:
        Server *server;
	public:
        Invite(Server* server);
        ~Invite();

        Invite(const Invite &other);
        Invite &operator=(const Invite &other);

        void    processInvite(std::string &cmd, int &fd);
		void	execute(std::string cmd, int fd);

};

#endif