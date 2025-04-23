/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:50:11 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:07:30 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
#define MODE_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>
class Server;

class Mode : public Command
{
    private:
        Server *server;

	public:
        Mode(Server* server);
        ~Mode();

		Mode(const Mode &other);
        Mode &operator=(const Mode &other); 

        void 					mode_command(std::string& cmd, int fd);
		std::string 			inviteOnlyMode(Channel *channel, char opera, std::string chain);
		std::string 			topicRestrictionMode(Channel *channel ,char opera, std::string chain);
		std::string 			setPasswordMode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
		std::string 			setOperatorPrivilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments);
		std::string 			setChannelLimit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
		bool					isValidLimit(std::string& limit);
		std::string 			appendMode(std::string chain, char opera, char mode);
		std::vector<std::string> splitByComma(std::string params);
		void 					getCmdArgs(const std::string& cmd, std::string& name, std::string& modeset, std::string& params);
        
		void					execute(std::string cmd, int fd);
};

#endif