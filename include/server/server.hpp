#pragma once

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include "connection.hpp"

using namespace boost::asio;
using namespace boost::system;


class Match
{

private:
    boost::shared_ptr<Connection> player1_;
    boost::shared_ptr<Connection> player2_;

public:
    Match(boost::shared_ptr<Connection> player1, boost::shared_ptr<Connection> player2)
    :   player1_(player1), player2_(player2)
    {
        std::cout << "Match has been created" << std::endl;
    };
    void SetPlayer2(boost::shared_ptr<Connection> player2);
    void sendToPlayer1(const std::string& message);
    void sendToPlayer2(const std::string& message);
    void recieveFromPlayer1();
    void recieveFromPlayer2();
    void end();
};

class Server
{
private:
    io_context context_; // объект io_context для обработки событий ввода-вывода
    ip::tcp::acceptor acceptor_; // объект acceptor для прослушивания входящих соединений
    std::vector<boost::shared_ptr<Connection>> connections_;
    boost::shared_ptr<Match> match_;
    void start_accept();
    void handle_accept(boost::shared_ptr<Connection> connection, const error_code& error );
    void handle_stop();

public:
    Server(const std::string& address, const std::string& port); // конструктор, который создает объект acceptor
    void start();
    void stop(); // метод, который останавливает сервер и закрывает все активные соединения
    
};
