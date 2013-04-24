#ifndef example_exchange_h
#define example_exchange_h

#include "order.h"
#include "example_order_book.h"

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

namespace liquibook { namespace examples {

class Exchange {
public:
  Exchange(ExampleOrderBook::TypedDepthListener* listener);
  void add_order_book(const std::string& symbol);
  void add_order(const std::string& symbol, OrderPtr& order);
private:
  typedef std::map<std::string, ExampleOrderBook> OrderBookMap;
  OrderBookMap order_books_;
  ExampleOrderBook::TypedDepthListener* listener_;
};

} }

#endif