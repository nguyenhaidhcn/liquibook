// Copyright (c) 2017 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "Order.h"
#include <sstream>

namespace orderentry
{

Order::Order(const std::string & id,
    bool buy_side,
    liquibook::book::Quantity quantity,
    std::string symbol,
    liquibook::book::Price price,
    liquibook::book::Price stopPrice,
    bool aon,
    bool ioc)
    : id_(id)
    , buy_side_(buy_side)
    , symbol_(symbol)
    , quantity_(quantity)
    , price_(price)
    , stopPrice_(stopPrice)
    , ioc_(ioc)
    , aon_(aon)
    , quantityFilled_(0)
    , quantityOnMarket_(0)
    , fillCost_(0)
    , verbose_(false)

{
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
    return aon_;
}

bool 
Order::immediate_or_cancel() const
{
    return ioc_;
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


const Order::History & 
Order::history() const
{
    return history_;
}

const Order::StateChange & 
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

std::ostream & operator << (std::ostream & out, const Order::StateChange & event)
{
    DLOG(INFO) << "{";
    switch(event.state_)
    {
    case Order::Submitted:
        DLOG(INFO) << "Submitted ";
        break;
    case Order::Rejected:
        DLOG(INFO) << "Rejected ";
        break;
    case Order::Accepted:
        DLOG(INFO) << "Accepted ";
        break;
    case Order::ModifyRequested:
        DLOG(INFO) << "ModifyRequested ";
        break;
    case Order::ModifyRejected:
        DLOG(INFO) << "ModifyRejected ";
        break;
    case Order::Modified:
        DLOG(INFO) << "Modified ";
        break;
    case Order::PartialFilled:
        DLOG(INFO) << "PartialFilled ";
        break;
    case Order::Filled:
        DLOG(INFO) << "Filled ";
        break;
    case Order::CancelRequested:
        DLOG(INFO) << "CancelRequested ";
        break;
    case Order::CancelRejected:
        DLOG(INFO) << "CancelRejected ";
        break;
    case Order::Cancelled:
        DLOG(INFO) << "Cancelled ";
        break;
    case Order::Unknown:
        DLOG(INFO) << "Unknown ";
        break;
    }
    DLOG(INFO) << event.description_;
    DLOG(INFO) << "}";
    return out;
}

std::ostream & operator << (std::ostream & out, const Order & order)
{
    DLOG(INFO) << "[#" << order.order_id();
    DLOG(INFO) << ' ' << (order.is_buy() ? "BUY" : "SELL");
    DLOG(INFO) << ' ' << order.order_qty();
    DLOG(INFO) << ' ' << order.symbol();
    if(order.price() == 0)
    {
        DLOG(INFO) << " MKT";
    }
    else
    {
        DLOG(INFO) << " $" << order.price();
    }

    if(order.stop_price() != 0)
    {
        DLOG(INFO) << " STOP " << order.stop_price();
    }

    DLOG(INFO)  << (order.all_or_none() ? " AON" : "")
        << (order.immediate_or_cancel() ? " IOC" : "");

    auto onMarket = order.quantityOnMarket();
    if(onMarket != 0)
    {
        DLOG(INFO) << " Open: " << onMarket;
    }

    auto filled = order.quantityFilled();
    if(filled != 0)
    {
        DLOG(INFO) << " Filled: " << filled;
    }

    auto cost = order.fillCost();
    if(cost != 0)
    {
        DLOG(INFO) << " Cost: " << cost;
    }

    if(order.isVerbose())
    {
        const Order::History & history = order.history();
        for(auto event = history.begin(); event != history.end(); ++event)
        {
            DLOG(INFO) << "\n\t" << *event;
        } 
    }
    else
    {
        DLOG(INFO) << " Last Event:" << order.currentState();
    }

    DLOG(INFO) << ']';
   
   return out;
}


}
