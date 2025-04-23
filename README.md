# 42 Madrid IRC Project

![Logo IRC](./irc.png)

## Introduction
This project involves implementing an IRC (Internet Relay Chat) server using C++98. The server is designed to handle multiple client connections simultaneously, ensuring smooth and efficient communication through non-blocking input/output operations.

## Starting the Server
Compile the project with `make`.

Once compilation is complete, you can run the IRC server with the following command:
```./ircserv <port> <password>```

**The server uses ports in the range 1024 to 65535 because these are the ports that regular users and non-privileged services can use without interfering with system-reserved or essential service ports.*

## Starting a Client via Netcat
Netcat (or nc) is a networking tool that allows you to create TCP or UDP connections, making it a useful resource for interacting with IRC servers. It can be used to test server connections, send messages, or simulate a client, thus facilitating debugging and protocol development for our IRC project.  
To start with nc, type:  
```nc localhost <port>```  
```pass <password>```  
```nick <nickname>```  
```user <name> 0 * <name>``` 

## Starting a Client via Telnet  
Telnet is a network protocol that enables communication between a client and a server through a TCP connection. It is primarily used to access network devices and systems remotely via a command-line interface. Despite its simplicity, Telnet does not encrypt information, making it less secure compared to alternatives like SSH (Secure Shell).  

```telnet localhost <port>```  
```pass <password>```  
```nick <nickname>```  
```user <name> 0 * <name>```  

**Both `<port>` and `<password>` fields must match the values you used in the server.*  

## IRC Commands

| Command                             | Description                                                                            |
|-------------------------------------|----------------------------------------------------------------------------------------|
| `nick <new_nickname>`          | Allows you to change your nickname                                        |
| `join #<channel>`          | Creates a channel or adds you to it if it already exists                                        |
| `mode #<channel> <mode>`             | Changes the channel mode (only if you are an operator):                                            |
|                                     | - `i`: Sets/unsets the channel invite-only mode.                        |
|                                     | - `t`: Sets/unsets topic restrictions to channel operators.|
|                                     | - `k <key>`: Sets/unsets the channel key (password).                     |
|                                     | - `o <user>`: Grants/removes channel operator privileges to a user.        |
| `kick #<channel> <nickname>`          | Kicks a client from the specified channel (only if you are an operator).                                         |
| `invite #<nickname> <channel>`        | Invites a client to a specific channel.                                          |
| `topic #<channel> [new_topic]`     | Changes or displays the channel topic.                                                  |
| `part #<channel>`     | Leaves the specified channel                                                  |
| `privmsg  <nickname> #<channel> : [message]`     | Sends a private message to a user                                                 |
| `privmsg #<channel> : [message]`     | Sends a message to a channel                                                 |
| `quit`     | Disconnects from the server                                                |  

## Server Signals  

| Signal    | Description                                             | Server Action                              |
|----------|---------------------------------------------------------|----------------------------------------------------|
| `SIGINT` | Interrupt signal sent with `Ctrl + C`.                     | Gracefully shuts down the server.             |
| `SIGQUIT`| Quit signal sent with `Ctrl + \`.                  | Shuts down the server and generates a memory dump. |
| `SIGTERM`| Signal to request program termination.     | Properly shuts down the server.                  |
| `SIGPIPE`| Occurs when attempting to write to a closed socket.  | Ignored to prevent server shutdown.    |

## Sockets  
A socket is an endpoint of a network connection. It is used to send and receive data between two machines over a network, such as the Internet.  
In an IRC server:  

Server: Creates a socket to "listen" on a port.  
Clients: Connect to this port by creating their own socket.  
Communication: Once the connection is established, messages are sent and received through sockets.  

## poll()
What is poll()?  
poll() is a system function used to monitor multiple file descriptors (FDs) simultaneously and detect when they are ready for operations such as:

- Listening for incoming client connections on a specific port.  
- Reading messages from connected clients.  
- Writing responses or messages to those clients.  
- Keeping the server available to handle new connections and messages without blocking while waiting.  

## fcntl()  
We use fcntl() to set sockets to non-blocking mode. This means the server will not "wait" (block) when a client does not send data immediately. This allows the server to continue functioning and manage other connections while waiting for a client to respond.  
Why is this important?  
Without non-blocking mode, your server could get "stuck" waiting for data from a client. With fcntl() in non-blocking mode, the server can handle multiple clients simultaneously without stopping.


## Authors

- mlezcano - [GitHub Profile](https://github.com/mikelezc)
- ampjimen - [GitHub Profile](https://github.com/Amparojd)
