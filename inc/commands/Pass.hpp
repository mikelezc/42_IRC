/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 13:23:34 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:20:12 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
#define PASS_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class Pass : public Command
{
    private:
        Server* server;
	public:
        Pass(Server* server);
        ~Pass();

       Pass(const Pass& other);
       Pass& operator=(const Pass& other);
       
       void	execute(std::string cmd, int fd);
};

#endif