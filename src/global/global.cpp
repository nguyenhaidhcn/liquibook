//
// Created by HaiNt on 6/15/18.
//

#include "global.h"


CmsConsumer *ExtConsumer;
CmsProducer *ExtProducerOrder;
CmsProducer *ExtProducerData;


orderentry::Market * ExtMarket;
//concurrent_queue<Trade> ExtTradeHistory;
std::map<std::string, ChartInfos> mapCharts;
