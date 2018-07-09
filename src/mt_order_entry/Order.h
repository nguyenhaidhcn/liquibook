// Copyright (c) 2017 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include "OrderFwd.h"
#include <book/types.h>

#include <string>
#include <vector>
#include "MatchingEngineAPI.h"

#include "json.hpp"
//
//{
//"orderID":"24",
//"loginID":12,
//"cmd":1,
//"quantity":2,
//"symbol":"ETH/BTC",
//"price":24,
//"stopPrice":25,
//"is_cancel":0
//}
using namespace nlohmann;
namespace orderentry
{

class Order
{
public:



public:
    Order(
        int requestType,
        std::string loginId,
        const std::string & id,
        bool buy_side,
        liquibook::book::Quantity quantity,
        std::string symbol,
        liquibook::book::Price price,
        liquibook::book::Price stopPrice,
        bool all_or_none,
        bool is_cancel);

    //////////////////////////
    // Implement the 
    // liquibook::book::order
    // concept.

    //init order
    static OrderPtr InitOrderPtr(std::string input);

    //get json string
    std::string GetJson(json &j);

    /// @brief is this a limit order?
    bool is_limit() const;

    /// @brief is this order a buy?
    bool is_buy() const;

    /// @brief get the price of this order, or 0 if a market order
    liquibook::book::Price price() const;

    /// @brief get the stop price (if any) for this order.
    /// @returns the stop price or zero if not a stop order
    liquibook::book::Price stop_price() const;

    /// @brief get the quantity of this order
    liquibook::book::Quantity order_qty() const;

    /// @brief if no trades should happen until the order
    /// can be filled completely.
    /// Note: one or more trades may be used to fill the order.
    virtual bool all_or_none() const;

    /// @brief After generating as many trades as possible against
    /// orders already on the market, cancel any remaining quantity.
    virtual bool immediate_or_cancel() const;

    std::string symbol() const;

    std::string order_id() const;

    long double quantityFilled() const;

    long double quantityOnMarket() const;

    long double fillCost() const;

    std::string getLoginId_() const;

    Order & verbose(bool verbose = true);
    bool isVerbose()const;
    const History & history() const;
    const StateChange & currentState() const;

    ///////////////////////////
    // Order life cycle events
    void onSubmitted();
    void onAccepted();
    void onRejected(const char * reason);

    void onFilled(
        liquibook::book::Quantity fill_qty, 
        liquibook::book::Cost fill_cost);

    void onCancelRequested();
    void onCancelled();
    void onCancelRejected(const char * reason);

    void onReplaceRequested(
        const int32_t& size_delta, 
        liquibook::book::Price new_price);

    void onReplaced(const int32_t& size_delta, 
        liquibook::book::Price new_price);

    void onReplaceRejected(const char * reaseon);

    //new order, cancel, modify
    int requestType_;

    //info for return
    int msgCode_;
    std::string msgInfo_;

private:

    std::string id_;
    std::string  loginId_;
    bool buy_side_;
    std::string symbol_;
    liquibook::book::Quantity quantity_;
    liquibook::book::Price price_;
    liquibook::book::Price stopPrice_;


    bool all_or_none_;
    bool is_cancel;

    liquibook::book::Quantity quantityFilled_;
    liquibook::book::Quantity totalQuantityFilled_;
    liquibook::book::Quantity quantityOnMarket_;
    liquibook::book::Cost fillCost_;
    liquibook::book::Cost totalFillCost_;
    
    std::vector<StateChange> history_;
    bool verbose_;
};

std::ostream & operator << (std::ostream & out, const Order & order);
std::ostream & operator << (std::ostream & out, const StateChange & event);

}
