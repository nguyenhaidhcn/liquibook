//
// Created by HaiNt on 6/30/18.
//

#pragma once
#ifndef LIQUIBOOK_MATCHINGENGINEAPI_H
#define LIQUIBOOK_MATCHINGENGINEAPI_H

#endif //LIQUIBOOK_MATCHINGENGINEAPI_H
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
    Unknown
};

static std::string StateConvert(State state)
{
    switch(state)
    {
        case Submitted:
            return "Submitted";
            break;

        case Rejected:
            return "Rejected";
            break;

        case Accepted:
            return "Accepted";
            break;

        case ModifyRequested:
            return "ModifyRequested";
            break;

        case ModifyRejected:
            return "ModifyRejected";
            break;

        case Modified:
            return "Modified";
            break;

        case PartialFilled:
            return "PartialFilled";
            break;

        case Filled:
            return "Filled";
            break;

        case Cancelled:
            return "Cancelled";
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

