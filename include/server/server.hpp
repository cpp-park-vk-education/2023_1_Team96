#pragma once

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>


using namespace boost::asio;
using namespace boost::system;

class Connection: public boost::enable_shared_from_this<Connection>, boost::noncopyable 
{
    
private:

    ip::tcp::socket socket_;
    std::string read_buffer_;
    std::string write_buffer_;

public:
    Connection(io_service service);
    void start();
    void write(std::string message);
    void close();
    void handle_read(error_code& error, size_t bytes_transferred);
    void handle_write(error_code& error, size_t bytes_transferred);
    ip::tcp::socket get_socket();
};

class Client
{

private:

    ip::tcp::endpoint ep_;
    io_service service_;
    Connection connection_;
    
public:

    Client(const std::string& address, const std::string& port);
    void connect();
    void start();
    void write(const std::string& message);
    void close();

};


class Server
{
private:
    io_service service_; // объект io_context для обработки событий ввода-вывода
    ip::tcp::acceptor acceptor_; // объект acceptor для прослушивания входящих соединений
   

public:
    Server(const std::string& address, const std::string& port); // конструктор, который создает объект acceptor
    void start_accept(); // метод, который начинает прослушивание входящих соединений
    void handle_accept(boost::shared_ptr<Connection> connection, const error_code& error); // метод, который вызывается при установке нового соединения
    void stop(); // метод, который останавливает сервер и закрывает все активные соединения
};

class GameServer : public boost::enable_shared_from_this<GameServer>, boost::noncopyable {
public:
    GameServer(io_service& service, const std::string& address, const std::string& port);
    void start_accept();
    void serialize(std::string message);
    
private:
    void handle_accept(const error_code& error);

    Server server_;    
    io_service& service_;
    std::vector<boost::shared_ptr<Connection>> connections_;
};


class Match
{

private:
    boost::shared_ptr<Connection> player1;
    boost::shared_ptr<Connection> player2;
    boost::shared_ptr<GameServer> game_server;
};

class Matchmaking
{

public:

    Matchmaking(io_service& service, const std::string& address, const std::string& port);

    void start();
    void stop();

private:

    void handle_player_connection(boost::shared_ptr<Connection> connection, const error_code& error);
    void handle_match_creation();

    io_service& service_;
    Server& server_;
 //   GameServer game_server_;

    std::vector<boost::shared_ptr<Connection>> players_ ; // список подключенных игроков
    std::vector<boost::shared_ptr<Match>> matches_; // список созданных матчей
    
};

void print_hello(); 