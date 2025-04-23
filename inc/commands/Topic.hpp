/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:58 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/16 19:26:58 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class Topic : public Command
{
	private:

	public:
		Topic	(Server *server);
		~Topic(); 

		
		Topic(const Topic &other);
        Topic &operator=(const Topic &other);

		void		execute(std::string cmd, int fd);
		std::string	formatCurrentTime(void);
		int			getPos(const std::string &cmd);
		std::string getTopic(const std::string& input);
};

#endif