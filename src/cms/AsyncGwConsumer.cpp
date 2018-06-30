//
// Created by HaiNt on 6/30/18.
//
//https://stackoverflow.com/questions/12394472/serializing-and-deserializing-json-with-boost

//#include <sstream>
//#include <map>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
//
//using boost::property_tree::ptree;
//using boost::property_tree::read_json;
//using boost::property_tree::write_json;
//
//void example() {
//    // Write json.
//    ptree pt;
//    pt.put ("foo", "bar");
//    std::ostringstream buf;
//    write_json (buf, pt, false);
//    std::string json = buf.str(); // {"foo":"bar"}
//
//    // Read json.
//    ptree pt2;
//    std::istringstream is (json);
//    read_json (is, pt2);
//    std::string foo = pt2.get<std::string> ("foo");
//}
//
//std::string map2json (const std::map<std::string, std::string>& map) {
//    ptree pt;
//    for (auto& entry: map)
//        pt.put (entry.first, entry.second);
//    std::ostringstream buf;
//    write_json (buf, pt, false);
//    return buf.str();
//}
#include "AsyncGwConsumer.h"

#include "global/global.h"

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;


void AsyncGwConsumer::onMessage( const Message* message ){

    static int count = 0;

    try
    {
        count++;
        const TextMessage* textMessage =
            dynamic_cast< const TextMessage* >( message );
        string text = "";

        if( textMessage != NULL ) {
            text = textMessage->getText();
        } else {
            text = "NOT A TEXTMESSAGE!";
        }

        if( clientAck ) {
            message->acknowledge();
        }

        printf( "Message #%d Received: %s\n", count, text.c_str() );

        LOG(INFO)<<"Received:" <<text;
        //std::istringstream request_json(text);
//        this->ProcessOrder(text);
        //resend
//        ExtGwProducer->send(text);


    } catch (CMSException& e) {
        e.printStackTrace();
    }
}


orderentry::Order AsyncGwConsumer::ProcessOrder(std::string input)
{
    LOG(INFO)<<input;

    std::istringstream request_json(input);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(request_json, pt);

//    orderentry::Order order();

    auto loginID = pt.get<int32_t > ("loginID");
    LOG(INFO)<<loginID;

    auto cmd = pt.get<int32_t > ("cmd");
    LOG(INFO)<<cmd;

    auto quantity = pt.get<int32_t > ("quantity");
    LOG(INFO)<<quantity;

    auto symbol = pt.get<std::string > ("symbol");
    LOG(INFO)<<symbol;

    auto orderID = pt.get<std::string > ("orderID");
    LOG(INFO)<<orderID;

    auto price = pt.get<int32_t > ("price");
    LOG(INFO)<<price;

//    return
//     orderentry::Order  order(
//        pt.get<int32_t > ("loginId"),
//        pt.get<std::string > ("orderID"),
//        pt.get<int32_t >("cmd"),
//        pt.get<int32_t >("quantity"),
//        pt.get<std::string >("symbol"),
//        pt.get<int32_t >("price"),
//        0,
//        0,
//        0
//    );

//         orderentry::Order  order(
//        loginID,
//        orderID,
//        cmd,
//        quantity,
//        symbol,
//        price,
//        false,
//        false,
//        false
//    );
//
//     LOG(INFO)<<order;
//    return order;
}