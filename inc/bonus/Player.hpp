/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:28:22 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/16 19:28:22 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <cstdlib> 

class Player
{
	private:		
		std::string nickname;
		std::string cmd;		
		std::string superpower;

	public:
		std::vector<char> board;

		Player();
		Player(const Player& other);
		Player& operator=(const Player& other);
		~Player();

		//random de superhéroes
		void setSuperpower(const std::string& power);
		std::string get_nick() const;
		std::string getSuperpower() const;
};

