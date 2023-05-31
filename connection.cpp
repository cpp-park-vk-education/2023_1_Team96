    #include <iostream>
    #include <boost/asio.hpp>
    #include <boost/thread.hpp>
    #include <memory>
    #include <boost/system/error_code.hpp>
    #include <functional>

    using namespace boost::asio;
    using namespace boost::placeholders;
    using namespace boost::system;


    class Connection: public boost::enable_shared_from_this<Connection>
    {
        
    private:

        using callback_t = std::function<void()>;
        boost::asio::streambuf b;
        ip::tcp::socket socket_;
        std::string read_buffer_;
        std::string write_buffer_;
        std::string last_accepted_str_;

    public:
        Connection(io_context& context)
        : socket_(context)
        {
            std::cout << "Connection: has been created" << std::endl;
        };
        void start();
        void start(callback_t handler);
        void write(const std::string& message);
        void close();
        void handle_read(error_code error, size_t bytes_transferred);
        void handle_write(error_code& error, size_t bytes_transferred);
        std::string get_last_accepted_str();
        ip::tcp::socket& get_socket();
    };

    void Connection::close()
    {
        error_code ec;
        socket_.close(ec);
    
        if (ec)
            std::cout << "Error with tryng to close connection" << std::endl;
    }
    
    void Connection::start(callback_t handler)
    {
    //std::cout << "Connection has started reading" << std::endl;
        auto self = shared_from_this();
        //std::cout << "last " << last_accepted_str_ << std::endl;
        async_read_until(socket_, b, "\r\n",
        [handler, self](error_code error, size_t bytes_transferred)
        {
            self->handle_read(error, bytes_transferred);
            handler();
        });
        // error_code error;
        // read_until(socket_, b, "\r\n", error);
        // std::istream is(&b);
        // std::getline(is, read_buffer_);
        // handle_read(error, read_buffer_.size());
        // socket_.async_read_some(buffer(read_buffer_, 26), [this](error_code error, size_t bytes_transferred)
        // {
        //     handle_read(error, bytes_transferred);
        // }); 

    //    std::cout << "Connection has started reading" << std::endl;
    //     size_t bytes_transferred = socket_.read_some(buffer(read_buffer_));
    //     if (bytes_transferred != 0)
    //     {    
    //         std::cout << "URA" << std::endl;
    //     }
    //     error_code error;
    //     handle_read(error, bytes_transferred);   
    }

    void Connection::start() {
        this->start([](){});
    }
    
    void Connection::handle_read(error_code error, size_t bytes_transferred)
    {
        if (!error)
        {
        std::istream is(&b);
        std::getline(is, read_buffer_);
        b.consume(bytes_transferred);
        std::cout << "Вызван" << std::endl;

        //std::cout << read_buffer_.size() << std::endl;
        if (bytes_transferred != 0)
            std::cout <<"Data has been read\n" << read_buffer_ << std::endl;
        
        
            std::string recived_data(read_buffer_);
            last_accepted_str_ = recived_data;
            
        //  write("Accepted");
           // start();
            //Обработка данных
            //std::cout << "Data has been read" << bytes_transferred << std::endl;
            //start();
        }
        else
        {
            std::cout << "Connection: smth wrong with connection" << std::endl;
            close();
        }
    }
    
    
    
    void Connection::write(const std::string& message)
    {
        std::string send_msg(message);
        send_msg += "\r\n";
        std::cout << "Sent msg: " << send_msg << std::endl;
        async_write(socket_, buffer(send_msg), [this](error_code error, size_t bytes_transferred)
        {   
            handle_write(error, bytes_transferred);
        });
    
    }
    
    void Connection::handle_write(error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            std::cout << "Data has been sent successfully " << bytes_transferred << std::endl;
        }
        else{
            std::cout << "Connection: Error with write in socket" << std::endl;
        }
    }
    
    std::string Connection::get_last_accepted_str()
    {
        return last_accepted_str_;
    }
    
    ip::tcp::socket& Connection::get_socket()
    {
        return socket_;
    }