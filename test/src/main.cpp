#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <boost/system/error_code.hpp>

#include "server.hpp"


using namespace boost::asio;
using namespace boost::placeholders;

int main()
{
  print_hello();
  return 0;
}