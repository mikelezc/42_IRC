/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:55:23 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/03 12:59:51 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void	Server::signalReceived(int sigNbr)
{
	std::cout << std::endl
			  << "Received signal number: " << sigNbr << std::endl
			  << std::endl;
			  
	Server::_signal = true;
}
