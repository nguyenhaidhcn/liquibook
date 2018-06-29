//
// Created by HaiNt on 6/30/18.
//

#include "AsyncGwConsumer.h"

#include "global/global.h"

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

        //resend
        ExtGwProducer->send(text);


    } catch (CMSException& e) {
        e.printStackTrace();
    }
}