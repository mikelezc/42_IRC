/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:27:18 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/16 19:27:18 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <netdb.h>
#include <sstream>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <cstring>
#include "Player.hpp"
#include "../Macros.hpp"
#include <cctype>

class Bot
{
	private:

		int age;
		std::string botnick;
		bool Loggedin;
		std::vector<Player> players;
		std::vector<std::string> random;

		//superhéroes
		std::vector<std::string> nicknames;
    	std::vector<std::string> superpowers;

		void initializeNicknamesAndPowers();
	public:
		Bot();
		Bot(const Bot& other);
		Bot& operator=(const Bot& other);
		~Bot();
	
		//init methods
		void	init(int ircsock);
		Player	*GetPlayer(std::string nickname);
		//send methods
		void	_sendMessage(std::string message, int fd);
		void	send_privmsg(std::string message, std::string UserNick, int ircsock);			
		//parse methods
		void	getCommand(std::string recived, std::string& nick , std::string &command);
		std::string SplitBuff(std::string buff, std::string &date);
		//rock, paper, scissors methods
		void rock_paper(const std::string &nickname, int ircsock, const std::string &userChoice);
		//Nick methods
		void	setNick(std::string nick);
		std::string	getNick(void);	
		//random superhéroes
		void generateNicknameAndPower(const std::string &name, const std::string &nickname, int ircsock);
		//random data
		int		getRandomData(std::string filename);
		void	generateRandom(const std::string &nickname, int ircsock);
    
};
