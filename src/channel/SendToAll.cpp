/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendToAll.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlezcano <mlezcano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:00:05 by mlezcano          #+#    #+#             */
/*   Updated: 2024/10/03 11:02:07 by mlezcano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

void Channel::allMembersSender(std::string message, int excludeFd = -1)		// Optionally excluding one (-1 by default to send to all)
{
    for (size_t i = 0; i < _channelAdminsBook.size(); i++)
	{
        int memberFd = _channelAdminsBook[i].getFd();
        if (memberFd != excludeFd)											// Excluding the sender if its fd matches
            if (send(memberFd, message.c_str(), message.size(), 0) == -1)
                std::cerr << "send() failed" << std::endl;
    }
	
    for (size_t i = 0; i < _channelClientsBook.size(); i++)
	{
        int memberFd = _channelClientsBook[i].getFd();
        if (memberFd != excludeFd)											// Excluding the sender if its fd matches
            if (send(memberFd, message.c_str(), message.size(), 0) == -1)
                std::cerr << "send() failed" << std::endl;
    }
}
