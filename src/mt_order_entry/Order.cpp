// Copyright (c) 2017 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "Order.h"
#include <sstream>
#include <strstream>
#include <string>

#include "global/global.h"
#include "json.hpp"

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

using namespace nlohmann;

#define REQUEST_TYPE_KEY            "requestType"
#define LOGIN_ID_KEY                "loginID"
#define CMD_KEY                     "cmd"
#define QUANTITY_KEY                "quantity"
#define SYMBOL_KEY                  "symbol"
#define ORDER_ID_KEY                "orderID"
#define PRICE_KEY                   "price"
#define MSG_CODE_KEY                   "msgCode"
#define MESSAGE_KEY                   "message"



namespace orderentry
{

Order::Order(
    const int requestType,
    const int32_t loginId,
    const std::string & id,
    bool buy_side,
    liquibook::book::Quantity quantity,
    std::string symbol,
    liquibook::book::Price price,
    liquibook::book::Price stopPrice,
    bool all_or_none,
    bool is_cancel)
    :requestType_(requestType),
    loginId_(loginId)
    ,id_(id)
    , buy_side_(buy_side)
    , symbol_(symbol)
    , quantity_(quantity)
    , price_(price)
    , stopPrice_(stopPrice)
    , is_cancel(is_cancel)
    , all_or_none_(all_or_none)
    , quantityFilled_(0)
    , quantityOnMarket_(0)
    , fillCost_(0)
    , verbose_(false)
    ,msgCode_(0)

{
}

std::string Order::GetJson()
{
    json j = json{
        {LOGIN_ID_KEY, this->loginId_},
        {ORDER_ID_KEY, this->order_id()},
        {QUANTITY_KEY, this->quantity_},
        {CMD_KEY, this->is_buy()},
        {SYMBOL_KEY, this->symbol_.c_str()},
        {PRICE_KEY, this->price_},
        {MSG_CODE_KEY, this->msgCode_},
        {MESSAGE_KEY, this->msgInfo_}
    };

    LOG(INFO)<<j;
    return j;


}

OrderPtr Order::InitOrderPtr(std::string input)
{
    LOG(INFO)<<input;

    std::istringstream request_json(input);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(request_json, pt);

//    orderentry::Order order();


    auto requestType = pt.get<int32_t > (REQUEST_TYPE_KEY,-1);
//    LOG(INFO)<<requestType;

    auto loginID = pt.get<int32_t > (LOGIN_ID_KEY,-1);
//    LOG(INFO)<<loginID;

    auto cmd = pt.get<int32_t > (CMD_KEY,-1);
//    LOG(INFO)<<cmd;

    auto quantity = pt.get<long double > (QUANTITY_KEY,-1);
//    LOG(INFO)<<quantity;

    auto symbol = pt.get<std::string > (SYMBOL_KEY,"");
//    LOG(INFO)<<symbol;

    auto orderID = pt.get<std::string > (ORDER_ID_KEY,"");
//    LOG(INFO)<<orderID;

    auto price = pt.get<long double > (PRICE_KEY,-1);
//    LOG(INFO)<<price;

//    orderentry::OrderPtr order = std::make_shared<orderentry::Order>(requestType,loginID, orderID, cmd, quantity, symbol, price, 0, 0,0);
    orderentry::OrderPtr order = std::make_shared<orderentry::Order>(requestType,loginID, orderID, cmd, quantity, symbol, price, 0, 0,0);




    return order;
}

std::string 
Order::order_id() const
{
    return id_;
}

bool 
Order::is_limit() const
{
    return price() != 0;
}

bool 
Order::is_buy() const
{
    return buy_side_;
}

bool 
Order::all_or_none() const
{
    return all_or_none_;
}

bool 
Order::immediate_or_cancel() const
{
    return is_cancel;
}

std::string 
Order::symbol() const
{
   return symbol_;
}

liquibook::book::Price 
Order::price() const
{
    return price_;
}

liquibook::book::Quantity 
Order::order_qty() const
{
    return quantity_;
}


liquibook::book::Price 
Order::stop_price() const
{
    return stopPrice_;
}

uint32_t 
Order::quantityOnMarket() const
{
    return quantityOnMarket_;
}

uint32_t 
Order::quantityFilled() const
{
    return quantityFilled_;
}

uint32_t 
Order::fillCost() const
{
    return fillCost_;
}


const History &
Order::history() const
{
    return history_;
}

const StateChange &
Order::currentState() const
{
    return history_.back();
}


Order & 
Order::verbose(bool verbose)
{
    verbose_ = verbose;
    return *this;
}

bool
Order::isVerbose() const
{
    return verbose_;
}

void 
Order::onSubmitted()
{
    std::stringstream msg;
    msg << (is_buy() ? "BUY " : "SELL ") << quantity_ << ' ' << symbol_ << " @";
    if( price_ == 0)
    {
        msg << "MKT";
    }
    else
    {
        msg << price_;
    }
    history_.emplace_back(StateChange(Submitted, msg.str()));
}

void 
Order::onAccepted()
{
    quantityOnMarket_ = quantity_;
    history_.emplace_back(Accepted);
}

void 
Order::onRejected(const char * reason)
{
    history_.emplace_back(StateChange(Rejected, reason));
}

void 
Order::onFilled(
    liquibook::book::Quantity fill_qty, 
    liquibook::book::Cost fill_cost)
{
    quantityOnMarket_ -= fill_qty;
    fillCost_ += fill_cost;

    std::stringstream msg;
    msg << fill_qty << " for " << fill_cost;
    history_.emplace_back(StateChange(Filled, msg.str()));
}

void 
Order::onCancelRequested()
{
    history_.emplace_back(CancelRequested);
}

void 
Order::onCancelled()
{
    quantityOnMarket_ = 0;
    history_.emplace_back(Cancelled);
}

void 
Order::onCancelRejected(const char * reason)
{
    history_.emplace_back(StateChange(CancelRejected, reason));
}

void 
Order::onReplaceRequested(
    const int32_t& size_delta, 
    liquibook::book::Price new_price)
{
    std::stringstream msg;
    if(size_delta != liquibook::book::SIZE_UNCHANGED)
    {
        msg << "Quantity change: " << size_delta << ' ';
    }
    if(new_price != liquibook::book::PRICE_UNCHANGED)
    {
        msg << "New Price " << new_price;
    }
    history_.emplace_back(StateChange(ModifyRequested, msg.str()));
}

void 
Order::onReplaced(const int32_t& size_delta, 
    liquibook::book::Price new_price)
{
    std::stringstream msg;
    if(size_delta != liquibook::book::SIZE_UNCHANGED)
    {
        quantity_ += size_delta;
        quantityOnMarket_ += size_delta;
        msg << "Quantity change: " << size_delta << ' ';
    }
    if(new_price != liquibook::book::PRICE_UNCHANGED)
    {
        price_ = new_price;
        msg << "New Price " << new_price;
    }
    history_.emplace_back(StateChange(Modified, msg.str()));
}

void 
Order::onReplaceRejected(const char * reason)
{
    history_.emplace_back(StateChange(ModifyRejected, reason));
}
int32_t
Order::getLoginId_() const
{
    return loginId_;
}

std::ostream & operator << (std::ostream & out, const StateChange & event)
{
    DLOG(INFO) << "{";
    switch(event.state_)
    {
    case Submitted:
        DLOG(INFO) << "Submitted ";
        break;
    case Rejected:
        DLOG(INFO) << "Rejected ";
        break;
    case Accepted:
        DLOG(INFO) << "Accepted ";
        break;
    case ModifyRequested:
        DLOG(INFO) << "ModifyRequested ";
        break;
    case ModifyRejected:
        DLOG(INFO) << "ModifyRejected ";
        break;
    case Modified:
        DLOG(INFO) << "Modified ";
        break;
    case PartialFilled:
        DLOG(INFO) << "PartialFilled ";
        break;
    case Filled:
        DLOG(INFO) << "Filled ";
        break;
    case CancelRequested:
        DLOG(INFO) << "CancelRequested ";
        break;
    case CancelRejected:
        DLOG(INFO) << "CancelRejected ";
        break;
    case Cancelled:
        DLOG(INFO) << "Cancelled ";
        break;
    case Unknown:
        DLOG(INFO) << "Unknown ";
        break;
    }
    DLOG(INFO) << event.description_;
    DLOG(INFO) << "}";
    return out;
}

std::ostream & operator << (std::ostream & out, const Order & order)
{
    std::strstream ss;
    ss << "[#" << order.order_id();
    ss << ' ' << (order.is_buy() ? "BUY" : "SELL");
    ss << ' ' << order.order_qty();
    ss << ' ' << order.symbol();
    if(order.price() == 0)
    {
        ss << " MKT";
    }
    else
    {
        ss << " $" << order.price();
    }

    if(order.stop_price() != 0)
    {
        ss << " STOP " << order.stop_price();
    }

    ss  << (order.all_or_none() ? " AON" : "")
        << (order.immediate_or_cancel() ? " IOC" : "");

    auto onMarket = order.quantityOnMarket();
    if(onMarket != 0)
    {
        ss << " Open: " << onMarket;
    }

    auto filled = order.quantityFilled();
    if(filled != 0)
    {
        ss << " Filled: " << filled;
    }

    auto cost = order.fillCost();
    if(cost != 0)
    {
        ss << " Cost: " << cost;
    }

    if(order.isVerbose())
    {
        const History & history = order.history();
        for(auto event = history.begin(); event != history.end(); ++event)
        {
            ss << "\n\t" << *event;
        } 
    }
    else
    {
        ss << " Last Event:" << order.currentState().description_;
    }

    ss << ']';
    LOG(INFO)<<ss.str();
   
   return out;
}


}
