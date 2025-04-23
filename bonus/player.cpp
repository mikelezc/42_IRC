#include "../inc/bonus/Player.hpp"

Player::Player()
{
    nickname = "";
}

Player::Player(const Player& other)
{
	*this = other;
}

Player& Player::operator=(const Player& other)
{
	if (this != &other){
    	nickname = other.nickname;
    	board = other.board;
    	cmd = other.cmd;
    	board = other.board;
	}
    return *this;
}

Player::~Player(){}

///////////////////////////////////////////////////////////////////////////////

std::string Player::get_nick() const
{
	return nickname;
}

//random Hero
void Player::setSuperpower(const std::string& power) {
    superpower = power;
}

std::string Player::getSuperpower() const {
    return superpower;
}