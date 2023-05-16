#include "server/server.hpp"

Server::Server(const std::string& address, const std::string& port)
    : context_(), acceptor_(context_)
{
    std::cout << "Hello" << std::endl;
    ip::tcp::endpoint endpoint(ip::address::from_string(address), std::stoi(port));
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
 
    acceptor_.listen();
    std::cout << "Listening" << std::endl;
}
 
void Server::handle_accept(boost::shared_ptr<Connection> connection, const error_code& error)
{
    std::cout << "New connection has been accepted" << std::endl;
    if (!error)
    {
        if (match_)
            match_->SetPlayer2(connection);
        else
            match_ = boost::make_shared<Match>(connection, nullptr);
 
        connections_.push_back(connection);
        std::cout << "Connections:" << connections_.size() << std::endl;
        connection->start();
        //stop();
        start_accept();
       // start();
    }
    else
        std::cout << "Server: Error with accepting" << std::endl;
}
 
void Server::start_accept()
{
    boost::shared_ptr<Connection> newConnection = boost::make_shared<Connection>(context_);
    std::cout << "NewConnection has been created" << std::endl;
    acceptor_.async_accept(newConnection->get_socket(), boost::bind(&Server::handle_accept, this, newConnection, boost::asio::placeholders::error));
    std::cout << "NewConnection is wating" << std::endl;
}
 
void Server::stop()
{
    acceptor_.close();
 
    handle_stop();
 
}
 
void Server::start()
{
    std::cout << "Start" << std::endl;
    start_accept();
 
   // std::cout << "Run" << std::endl;
    context_.run();
 
}
 
void Server::handle_stop()
{
    for (auto& connection: connections_)
            connection->close();
 
    connections_.clear();
 
    context_.stop();
 
    std::cout << "Server stopped" << std::endl;
}
 
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
        return player2_->get_last_accepted_str();
 
    return "Error";
}
 
void Match::SetPlayer2(boost::shared_ptr<Connection> player2)
{
    player2_ = player2;
}