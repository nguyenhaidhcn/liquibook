
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
    msg_type = pt.get<std::string > ("msgType", "").compare("RequestNew") == 0? MSG_Type ::RequestNew: MSG_Type ::MsgDefault;

    LOG(INFO)<<"msg_type:"<<msg_type;
    switch (msg_type)
    {
        case  MSG_Type::RequestNew:
        case  MSG_Type::RequestMofiy:
        case  MSG_Type::RequestCancel:
        {
            auto orderptr =  orderentry::Order::InitOrderPtr(input);
            orderptr->requestType_ = msg_type;
            ExtMarket->Process(orderptr);
            break;
        }
        default:
        {
            LOG(INFO)<<"Not support msgType:"<<msg_type;
            LOG(INFO)<<input;
            break;
        }
    }

}