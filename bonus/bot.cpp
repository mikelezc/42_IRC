#include "../inc/bonus/Bot.hpp"

Bot::Bot()
{
	this->Loggedin = false;
}

Bot::Bot(const Bot &other)
{
	*this = other;
}

Bot &Bot::operator=(const Bot &other)
{
	if (this != &other)
	{
		this->Loggedin = other.Loggedin;
		this->botnick = other.botnick;
	}
	return *this;
}

Bot::~Bot()
{
}

// ///////////////////////////////////////////////////////////////////////////////
void Bot::setNick(std::string nick)
{
	botnick = nick;
}

std::string Bot::getNick(void)
{
	return this->botnick;
}

// //---------------------------------------------------send methods
void Bot::_sendMessage(std::string message, int fd)
{
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		std::cerr << "Send failed" << std::endl;
}

void Bot::send_privmsg(std::string message, std::string UserNick, int ircsock)
{
	std::string msg = "PRIVMSG " + UserNick + " :" + message + CRLF;
	if (send(ircsock, msg.c_str(), msg.size(), 0) == -1)
		std::cerr << "Send failed" << std::endl;
}
//-------------------------------------------------parse methods
void Bot::getCommand(std::string recived, std::string &nick, std::string &command)
{
	if (recived[0] == ':')
		recived.erase(recived.begin());
	size_t pos = recived.find('!');
	nick = recived.substr(0, pos);
	pos = std::string::npos;
	pos = recived.find(':');
	command = recived.substr(pos);
	if (!command.empty() && command[0] == ':')
		command.erase(command.begin());
}
std::string Bot::SplitBuff(std::string buff, std::string &date)
{
	std::istringstream stm(buff);
	std::string token;
	stm >> token;
	stm >> date;
	return token;
}

int Bot::getRandomData(std::string filename)
{
	std::vector <std::string> values;
	std::fstream file(filename.c_str());
	std::string line;
	if (!file.is_open())
	{
		std::cerr << "Failed to open"+  filename+"file" << std::endl;
		return (0);
	}
	while (std::getline(file, line))
	{	
		this->random.push_back(line);		
	}
	// for (size_t i = 0; i < this->random.size(); i++)
	// {
	// 	std::cout << this->random[i] << "\n";
	// }
	file.close();
	return (1);
}
void	Bot::generateRandom(const std::string &nickname, int ircsock)
{
	srand(static_cast<unsigned int>(time(0)));
	int choice = rand() % this->random.size();
	send_privmsg(this->random[choice], nickname, ircsock);
}

/////////////////////////////RANDOM HERO!
// Método para inicializar apodos y superpoderes
void Bot::initializeNicknamesAndPowers() {
    if (nicknames.empty() && superpowers.empty())
    {
        nicknames.push_back("El Destructor de Almohadas 🛏️");
        nicknames.push_back("Súper Tupperware 🥡");
        nicknames.push_back("El Capitán Noodle 🍜");
        nicknames.push_back("Señor Marmota 🦫");
        nicknames.push_back("El Flautista de Bugs 🐛");

        superpowers.push_back("controlar el Wi-Fi solo cuando está débil 📶");
        superpowers.push_back("la habilidad de escribir código sin un solo bug a la primera 💻");
        superpowers.push_back("el poder de recordar cada comando de la terminal 🖥️");
        superpowers.push_back("puede quedarse despierto hasta tarde sin consecuencias 🦸‍♂️");
        superpowers.push_back("invocar la siesta perfecta en cualquier lugar 😴");
    }
}

// Función para generar apodo y superpoder
void Bot::generateNicknameAndPower(const std::string &name, const std::string &nickname, int ircsock)
{
    initializeNicknamesAndPowers();

    srand(static_cast<unsigned int>(time(0)));
    int randomIndexNickname = rand() % nicknames.size();
    int randomIndexSuperpower = rand() % superpowers.size();

    std::string randomNickname = nicknames[randomIndexNickname];
    std::string randomSuperpower = superpowers[randomIndexSuperpower];

    std::string message = "Hola " + name + ", tu apodo es: " + randomNickname + " y tu superpoder asqueroso es: " + randomSuperpower + "!";
    send_privmsg(message, nickname, ircsock);
}

void Bot::rock_paper(const std::string &nickname, int ircsock, const std::string &userChoice)
{
	std::vector<std::string> options;
    options.push_back("PIEDRA");
    options.push_back("PAPEL");
    options.push_back("TIJERA");
	bool validChoice = false;

	// Verificar si la elección del usuario es válida
	for (size_t i = 0; i < options.size(); ++i)
	{
		if (userChoice == options[i])
		{
			validChoice = true;
			break;
		}
	}
	if (!validChoice)
	{
		send_privmsg("Opción inválida. Por favor elige entre PIEDRA, PAPEL o TIJERA.", nickname, ircsock);
		return;
	}
	srand(static_cast<unsigned int>(time(0)));
	int computerChoiceIndex = rand() % options.size();
	std::string computerChoice = options[computerChoiceIndex];
	
	send_privmsg("Tu opción: " + userChoice, nickname, ircsock);
	send_privmsg("Opción de la computadora: " + computerChoice, nickname, ircsock);
	if (userChoice == computerChoice)
	{
		send_privmsg("¡Es un empate! 🤝", nickname, ircsock);
	}
	else if ((userChoice == "PIEDRA" && computerChoice == "TIJERA") ||
			 (userChoice == "PAPEL" && computerChoice == "PIEDRA") ||
			 (userChoice == "TIJERA" && computerChoice == "PAPEL"))
		send_privmsg("¡Ganaste! 🥳", nickname, ircsock);
	else
		send_privmsg("¡Perdiste! 😞", nickname, ircsock);
}

Player *Bot::GetPlayer(std::string nickname)
{
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].get_nick() == nickname)
			return &this->players[i];
	}
	return NULL;
}

void Bot::init(int ircsock)
{
	std::string recived, nickname, command;
	ssize_t recivedBytes;

	char buff[1024];
	while (true)
	{
		memset(buff, 0, sizeof(buff));
		recivedBytes = recv(ircsock, buff, (sizeof(buff) - 1), 0);
		if (recivedBytes <= 0)
			break;
		recived = buff;
		size_t pos = recived.find_first_of("\n\r");
		if (pos != std::string::npos)
			recived = recived.substr(0, pos);
		if (recived == ": 001 " + botnick + " : Welcome to the IRC server!" && !Loggedin)
		{
			Loggedin = true;
			std::cout << recived << std::endl;
		}
		else if (!Loggedin)
		{
			std::cout << recived << std::endl;
			return;
		}
		else if (strcmp(recived.c_str(), "PRIVMSG") && Loggedin)
		{
			getCommand(recived, nickname, command);
			// Aquí manejamos el comando para generar apodo y superpoder
            if (command == "hero") {
                generateNicknameAndPower(nickname, nickname, ircsock);
                continue;
            }
			else if (command.find("game") != std::string::npos)
			{
				std::string userChoice;

				try
				{
					userChoice = recived.substr(recived.find("game") + 5);
					userChoice.erase(0, userChoice.find_first_not_of(" \t"));
					userChoice.erase(userChoice.find_last_not_of(" \t") + 1);
					rock_paper(nickname, ircsock, userChoice);
				}
				catch (const std::exception &e)
				{
					send_privmsg("Uso: PRIVMSG bot :game <opción>. Ejemplo: PRIVMSG bot :game PAPEL", nickname, ircsock);					
				}
				continue;
			}
			else if (command == "random")
			{
				generateRandom(nickname, ircsock);
				continue;
			}			
		}
	}
}
