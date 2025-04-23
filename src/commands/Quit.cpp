/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampjimen <ampjimen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:23:39 by ampjimen          #+#    #+#             */
/*   Updated: 2024/10/07 17:22:26 by ampjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/commands/Quit.hpp"

Quit::Quit(Server* server) : Command(server), server(server) {}

Quit::~Quit() {}

Quit::Quit(const Quit &other) : Command(other.server), server(other.server) {}

Quit &Quit::operator=(const Quit &other) {
    if (this != &other) {
        this->server = other.server;
    }
    return *this;
}

// Function to find the 'QUIT' command and extract the reason
void FindQ(const std::string& cmd, const std::string& tofind, std::string& str) {
    size_t pos = cmd.find(tofind);
    if (pos != std::string::npos) {
        str = cmd.substr(pos + tofind.size());
        str.erase(0, str.find_first_not_of(' ')); // Remove leading spaces
    }
}

// Function to split the command and extract the quit reason
std::string SplitQuit(const std::string& cmd) {
    std::istringstream stm(cmd);
    std::string command, reason;

    stm >> command; // Read the first part of the command
    FindQ(cmd, command, reason); // Find the reason for quitting

    // Return default reason if none provided
    if (reason.empty())
        return "Quit";

    // Add ':' prefix if not present
    if (reason[0] != ':') {
        size_t spacePos = reason.find(' ');
        if (spacePos != std::string::npos) {
            reason.erase(spacePos); // Remove everything after the first space
        }
        reason.insert(reason.begin(), ':'); // Add ':' at the start
    }
    return reason;
}

// Execute the QUIT command
void Quit::execute(std::string cmd, int fd) {
    std::string reason = SplitQuit(cmd); // Extract quit reason
    std::vector<Channel>& channels = server->getChannels(); // Use vector type explicitly

    // Iterate through the channels to remove the client
    for (size_t i = 0; i < channels.size(); i++) {
        Channel& channel = channels[i];

        // Check if the client is in the channel
        if (channel.getClientFromChannel(fd)) {
            channel.channelEraseClient(fd); // Remove client

            // Check if no clients left in the channel
            if (channel.getTotalNbrClients() == 0) {
                channels.erase(channels.begin() + i); // Erase channel
                --i; // Adjust index due to removal
            } else {
                // Construct and send the quit message to all members
                std::string rpl = ":" + server->getClient(fd)->getNick() + "!~" +
                                  server->getClient(fd)->getUser() + "@localhost QUIT " + reason + CRLF;
                channel.allMembersSender(rpl, -1); // Send to all members
            }
        } else if (channel.getAdminFromChannel(fd)) {
            channel.channelEraseAdmin(fd); // Remove admin

            // Check if no clients left in the channel
            if (channel.getTotalNbrClients() == 0) {
                channels.erase(channels.begin() + i); // Erase channel
                --i; // Adjust index due to removal
            } else {
                // Construct and send the quit message to all members
                std::string rpl = ":" + server->getClient(fd)->getNick() + "!~" +
                                  server->getClient(fd)->getUser() + "@localhost QUIT " + reason + CRLF;
                channel.allMembersSender(rpl, -1); // Send to all members
            }
        }
    }

    // Log disconnection
    std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
    server->eraseAll(fd); // Erase client from server
}
