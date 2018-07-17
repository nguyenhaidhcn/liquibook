//
// Created by HaiNt on 6/30/18.
//

#pragma once
#ifndef LIQUIBOOK_MATCHINGENGINEAPI_H
#define LIQUIBOOK_MATCHINGENGINEAPI_H

#endif //LIQUIBOOK_MATCHINGENGINEAPI_H


#define LAST_UPDATE_ID "lastUpdateId"
#define BIDS "bids"
#define ASKS "asks"
#define MSG_TYPE "msgType"
#define MSG "msg"

//msg type send market data
#define DEPTH "DEPTH"
#define BBO "BBO"
#define DEPTH "DEPTH"
#define TRADE "TRADE"
#define CHART "CHART"
#define IS_BUYER_MAKER "isBuyerMaker"

#define REQUEST_TYPE_KEY            "requestType"
#define LOGIN_ID_KEY                "userId"
#define ORDER_SIDE                  "orderSide"
#define QUANTITY_KEY                "quantity"
#define SYMBOL_KEY                  "symbol"
#define SNAPSHOT                  "isSnapshot"
#define ORDER_ID_KEY                "orderId"
#define PRICE_KEY                   "rate"
#define MSG_CODE_KEY                   "msgCode"
#define MESSAGE_KEY                   "message"
#define QUANTITY_FILLED                "filledQuantity"
#define TOTAL_QUANTITY_FILLED        "totalFilledQuantity"
#define QUANTITY_MARKET               "remainQuantity"
#define FILL_COST                   "fillCost"
#define TOTAL_FILL_COST              "totalFillCost"

enum State{
    Submitted,
    Rejected, // Terminal state
    Accepted,
    ModifyRequested,
    ModifyRejected,
    Modified,
    PartialFilled,
    Filled, // Terminal State
    CancelRequested,
    CancelRejected,
    Cancelled, // Terminal state

    //Marketdata
    BboChanged,
    OrderBookChanged,
    Unknown
};

static std::string StateConvert(State state)
{
    switch(state)
    {
        case Submitted:
            return "SUBMITTED";
            break;

        case Rejected:
            return "Rejected";
            break;

        case Accepted:
            return "ACCEPTED";
            break;

        case ModifyRequested:
            return "ModifyRequested";
            break;

        case ModifyRejected:
            return "ModifyRejected";
            break;

        case Modified:
            return "MODIFIED";
            break;

        case PartialFilled:
            return "PARTIAL_FILLED";
            break;

        case Filled:
            return "FILLED";
            break;

        case Cancelled:
            return "CANCELLED";
            break;

        case BboChanged:
            return "BBOCHANGED";
            break;

        case OrderBookChanged:
            return "ORDER_BOOK_CHANGED";
            break;

        default:
            return "Unknow";
            break;
    }
}

//use for filler request
enum MSG_Type
{
    //order request
    RequestCancel,
    RequestNew,
    RequestMofiy,

    // event notify
    EventDepMarketChanged,
    EventBboChanged,
    EventPartialFilled,
    EventFilled,
    EventCanceled,


    MsgDefault

};

enum OrderType
{
    PENDING,
    MARKET
};

//
enum OrderSide
{
    OP_SELL,
    OP_BUY
};

struct StateChange
{
    State state_;
    std::string description_;
    StateChange()
        : state_(Unknown)
    {}

    StateChange(State state, const std::string & description = "")
        : state_(state)
        , description_(description)
    {}
};
typedef std::vector<StateChange> History;

