
#include "CmsConsumer.h"

#include "global/global.h"
#include "Market.h"

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;


void CmsConsumer::onMessage( const Message* message ){

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

        LOG(INFO)<<"Received:" <<text;
        this->ProcessRequest(text);

    } catch (CMSException& e) {
        e.printStackTrace();
    }
}


void CmsConsumer::ProcessRequest(std::string input)
{
    LOG(INFO)<<input;
//
    std::istringstream request_json(input);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(request_json, pt);
//
    enum MSG_Type msg_type;

//
//
    std::string msgTypeStr = pt.get<std::string > ("msgType", "");

    msg_type = MSG_Type ::MsgDefault;

    if(msgTypeStr.compare("REQUESTNEW") == 0)
    {
        msg_type = MSG_Type ::RequestNew;
    }

    if(msgTypeStr.compare("REQUESTCANCEL") == 0)
    {
        msg_type = MSG_Type ::RequestCancel;
    }

//    .compare("REQUESTNEW") == 0? MSG_Type ::RequestNew: MSG_Type ::MsgDefault;
//    msg_type = pt.get<std::string > ("msgType", "").compare("REQUESTCANCEL") == 0? MSG_Type ::RequestCancel: MSG_Type ::MsgDefault;


    LOG(INFO)<<"msg_type:"<<msg_type;

    auto orderptr =  orderentry::Order::InitOrderPtr(input);
    if (orderptr == nullptr)
        return;
    orderptr->requestType_ = msg_type;
    ExtMarket->Process(orderptr);
}