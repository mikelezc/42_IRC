/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:12:38 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/06 21:32:41 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class User : public Command
{
    private:
        Server *server;
	public:
        User(Server* server);
        ~User();

        User(const User &other);
        User &operator=(const User &other);
        
		void	execute(std::string cmd, int fd);
        void    set_clientUsername(std::string& username, int fd);
};

#endif