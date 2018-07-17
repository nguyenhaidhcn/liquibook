//
// Created by HaiNt on 6/15/18.
//
#pragma once
#ifndef LIQUIBOOK_GLOBAL_H
#define LIQUIBOOK_GLOBAL_H
#include "cms/CmsConsumer.h"
#include "cms/CmsProducer.h"
#include "Market.h"
#include "chart/ChartThread.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "concurrent_queue.h"
#include "map"
struct Trade
{
    std::string symbol;
    liquibook::book::Price price;
    liquibook::book::Quantity quantity;
    long long time;
};

extern CmsConsumer *ExtConsumer;
extern CmsProducer *ExtProducerOrder;
extern CmsProducer *ExtProducerData;
extern concurrent_queue<Trade> ExtTradeHistory;

extern std::map<std::string, ChartInfos> mapChartsM1;
extern std::map<std::string, ChartInfos> mapChartsM5;
extern std::map<std::string, ChartInfos> mapChartsM10;
extern std::map<std::string, ChartInfos> mapChartsM15;
extern std::map<std::string, ChartInfos> mapChartsM30;
extern std::map<std::string, ChartInfos> mapChartsM60;
extern std::map<std::string, ChartInfos> mapChartsH1;
extern std::map<std::string, ChartInfos> mapChartsH4;
extern std::map<std::string, ChartInfos> mapChartsD1;

extern orderentry::Market * ExtMarket;

#endif //LIQUIBOOK_GLOBAL_H
