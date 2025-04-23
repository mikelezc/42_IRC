/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:22:35 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:22:04 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
#define QUIT_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class Quit : public Command
{
    private:
        Server *server;
	public:
		Quit	(Server* server);
        ~Quit();

        Quit(const Quit &other);
        Quit &operator=(const Quit &other); 
        
		void	execute(std::string cmd, int fd);
};

#endif