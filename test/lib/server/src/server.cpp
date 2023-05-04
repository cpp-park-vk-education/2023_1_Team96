#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <boost/system/error_code.hpp>

#include "server.hpp"

using namespace boost::asio;
using namespace boost::placeholders;


void print_hello() {
    std::cout << "Hello!" << std::endl;
}

// int main()
// {
//     return 0;
// }