/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:37 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/16 19:26:37 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PART_HPP
#define PART_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"

#include <sstream>
#include <vector>
#include <string>

class Server;

class Part : public Command
{
	private:
	public:
	Part	(Server *server);
	~Part();

	Part(const Part &other);
    Part &operator=(const Part &other);

	int		SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);
	void	execute(std::string cmd, int fd);
};
#endif