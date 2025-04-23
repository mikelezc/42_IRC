#include "../inc/bonus/Bot.hpp"

int ircsock;

std::string trim(const std::string &str)
{
	std::string result;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isspace(str[i]))
		{
			result += str[i];
		}
	}
	return result; // eliminamos todos los espacios mimimi
}

int parse(std::vector<std::string> &values)
{
	std::vector<std::pair<std::string, std::string> > envs;
	std::fstream file("./bonus/.env");
	if (!file.is_open())
	{
		std::cerr << "Failed to open .env file" << std::endl;
		return 1;
	}

	// Definir los nombres esperados de las variables
	std::string expectedKeys[5];
	expectedKeys[0] = "ADDRESS";
	expectedKeys[1] = "PORT";
	expectedKeys[2] = "PASSWORD";
	expectedKeys[3] = "NICKNAME";
	expectedKeys[4] = "FILE";

	std::string line, key, value;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		if (std::getline(ss, key, '=') && std::getline(ss, value))
		{
			key = trim(key);	 // Elimina espacios en key
			value = trim(value); // Elimina espacios en value
			envs.push_back(std::make_pair(key, value));

			// Pilla bien las variables de entorno
			//  std::cout << "Read key: " << key << ", value: " << value << std::endl;
		}
	}
	file.close();

	// Verificar si se leyeron 5 variables
	if (envs.size() != 5)
	{
		std::cerr << "Invalid number of ENV variables" << std::endl;
		return 1;
	}

	// Validar las claves y los valores
	for (size_t i = 0; i < envs.size(); ++i)
	{
		if (envs[i].first != expectedKeys[i])
		{
			std::cerr << "Invalid ENV variable" << std::endl;
			return 1;
		}
	}
	// Rellenar el vector 'values' con los valores
	for (size_t i = 0; i < envs.size(); ++i)
	{
		values.push_back(envs[i].second);
	}

	return 0;
}

bool isPasswordValid(const std::string &password)
{
	return !password.empty() && password.size() <= 20;
}
bool isPortValid(const std::string &port)
{

	if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		return true;
	}

	int portNumber = std::atoi(port.c_str());

	if (portNumber < 1024 || portNumber > 65535)
	{
		return true;
	}

	return false;
}

int check_values(std::vector<std::string> values)
{
	if (isPortValid(values[1]))
	{
		std::cerr << RED << "Invalid port! Please ensure it is a number between 1024 and 65535." << WHITE << std::endl;
		return 1;
	}
	if (!isPasswordValid(values[2]))
	{
		std::cerr << RED << "Invalid password! It cannot be empty and must not exceed 20 characters." << WHITE << std::endl;
		return 1;
	}
	return 0;
}

void _sendMessage(std::string message, int fd)
{	
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		std::cerr << "Send failed" << std::endl;
}

void signalReceived(int signum)
{
	(void)signum;
	std::cout << std::endl
			  << "Signal Received!" << std::endl;
	_sendMessage("QUIT\r\n", ircsock);
}

int main()
{
	Bot bot;
	std::vector<std::string> values;

	std::cout << "The bot retrieves its arguments from the .env file. Feel free to adjust the values as needed." << std::endl;
	if (parse(values))
		return 1;
	if (check_values(values))
		return 1;
	signal(SIGINT, signalReceived);
	signal(SIGQUIT, signalReceived);
	std::string address = values[0];	
	if (strcmp(address.c_str(), "localhost") || strcmp(address.c_str(), "LOCALHOST"))
		address = "127.0.0.1";
	std::string filename = values[4];
	struct sockaddr_in ircHints;
	ircsock = socket(AF_INET, SOCK_STREAM, 0);
	if (ircsock == -1)
	{
		std::cerr << "failed to create socket (ircsock)" << std::endl;
		return 1;
	}
	ircHints.sin_family = AF_INET;
	ircHints.sin_port = htons(std::atoi(values[1].c_str()));
	ircHints.sin_addr.s_addr = inet_addr(address.c_str());

	if (connect(ircsock, (struct sockaddr *)&ircHints, sizeof(ircHints)) == -1)
	{
		std::cerr << RED << "connect() failed: " << strerror(errno) << WHITE << std::endl;
		return 1;
	}

	std::string userMessage = "USER ";
	userMessage.append(values[3]);
	userMessage.append(" 0 * :");
	userMessage.append(values[3]);
	userMessage.append(CRLF);
	if (!bot.getRandomData(values[4]))
	{
		std::cerr << RED << "Cannot read: " << values[4] << WHITE << std::endl;
		return 1;
	}
	_sendMessage("PASS " + std::string(values[2]) + CRLF, ircsock);
	_sendMessage("NICK " + std::string(values[3]) + CRLF, ircsock);
	_sendMessage(userMessage, ircsock);
	bot.setNick(std::string(values[3]));
	std::cout << GREEN << "BOT Is Connected!" << WHITE << std::endl;
	bot.init(ircsock);
	std::cout << RED << "BOT Is Disconnected!" << WHITE << std::endl;
	return 0;
}
