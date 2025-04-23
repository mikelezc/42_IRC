/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:28:55 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:46:09 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
#define NICK_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class Nick : public Command
{
	public:
		Nick	(Server* server);
        ~Nick();

        Nick(const Nick &other);
    	Nick &operator=(const Nick &other);

        bool    is_validNickname(std::string& nickname);
        bool    nickNameInUse(std::string& nickname);
        void    set_clientNickname(std::string cmd, int fd);

        void	execute(std::string cmd, int fd);
};

#endif