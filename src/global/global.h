//
// Created by HaiNt on 6/15/18.
//

#ifndef LIQUIBOOK_GLOBAL_H
#define LIQUIBOOK_GLOBAL_H
#include "cms/CmsConsumer.h"
#include "cms/CmsProducer.h"
#include "Market.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

extern CmsConsumer *ExtConsumer;
extern AsynGwProducer *ExtGwProducer;

extern orderentry::Market * ExtMarket;

#endif //LIQUIBOOK_GLOBAL_H
