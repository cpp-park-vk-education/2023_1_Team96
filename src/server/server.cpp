#include "server/server.hpp"

void Match::sendToPlayer1(const std::string& message)
{
    if (player1_)
        player1_->write(message);
}

void Match::sendToPlayer2(const std::string& message)
{
    if (player2_)
        player2_->write(message);
}

void Match::end()
{

    if (player1_)
        player1_->close();

    if (player2_)
        player2_->close();

}

std::string Match::recieveFromPlayer1()
{
    if (player1_)
        return player1_->get_last_accepted_str();
    
    return "Error";
}

std::string Match::recieveFromPlayer2()
{
    if (player2_)
        return player1_->get_last_accepted_str();
    
    return "Error";
}