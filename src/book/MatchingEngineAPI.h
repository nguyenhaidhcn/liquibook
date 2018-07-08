//
// Created by HaiNt on 6/30/18.
//

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

