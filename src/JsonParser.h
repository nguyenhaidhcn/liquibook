//
// Created by HaiNt on 7/4/18.
//
#pragma once

#ifndef LIQUIBOOK_JSONPARSER_H
#define LIQUIBOOK_JSONPARSER_H
#include "json.hpp"
#include "global/global.h"
//
//{
//"orderId":"o-64355541-4026-4ee4-92ee-ddac31a687bb",
//"symbol":"ethbtc",
//"baseCC":"btc",
//"counterCC":"eth",
//"orderType":"MARKET",
//"orderSide":"SELL",
//"orderStatus":"REJECTED",
//"rate":0.0713361,
//"baseAmount":0.5,
//"convertAmount":7.0090739471,
//"remainAmount":null,
//"fee":null,
//"orderTime":1530866673422,
//"userId":"sfs-sfs-fas-fa-sf-sf",
//"errorCode":"INSUFFICIENT_BALANCE"
//}

#define MESSAGE_TYPE            "msgType"
#define ORDER_ID =              "orderId"
#define SYMBOL =                "symbol"
#define BASE_CC =               "baseCC"
#define COUNTER_CC =            "counterCC"
#define ORDER_TYPE =            "orderType"
#define ORDER_SIDE =            "orderSide"
#define ORDER_STATUS =          "orderStatus"
#define RATE =                  "rate"
#define BASE_AMOUNT =           "baseAmount"
#define CONVERT_AMOUNT =        "convertAmount"
#define REMAIN_AMOUNT =         "remainAmount"
#define FEE =                   "fee"
#define ORDER_TIME =            "orderTime"
#define USER_ID =               "userId"
#define ERROR_CODE =            "errorCode"


class JsonParser
{
    static string OrderResponse(orderentry::OrderPtr orderPtr, State state)
    {
        orderPtr->currentState();
    }
};


#endif //LIQUIBOOK_JSONPARSER_H
