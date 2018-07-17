//
// Created by HaiNt on 6/30/18.
//

#ifndef LIQUIBOOK_ASYNCGWPRODUCER_H
#define LIQUIBOOK_ASYNCGWPRODUCER_H

#endif //LIQUIBOOK_ASYNCGWPRODUCER_H
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace activemq;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
class CmsProducer : public Runnable {
private:

    Connection* connection;
    Session* session;
    Destination* destination;
    MessageProducer* producer;
    bool useTopic;
    bool clientAck;
    std::string brokerURI;
    std::string destURI;

private:

    CmsProducer( const CmsProducer& );
    CmsProducer& operator= ( const CmsProducer& );

public:

    CmsProducer( const std::string& brokerURI,
                    const std::string& destURI, bool useTopic = true, bool clientAck = false ) :
        connection(NULL),
        session(NULL),
        destination(NULL),
        producer(NULL),
        useTopic(useTopic),
        clientAck(clientAck),
        \
        brokerURI(brokerURI),
        destURI(destURI) {
    }

    virtual ~CmsProducer(){
        cleanup();
    }

    void close() {
        this->cleanup();
    }

    virtual void run() {
        try {

            // Create a ConnectionFactory
            auto_ptr<ActiveMQConnectionFactory> connectionFactory(
                new ActiveMQConnectionFactory( brokerURI ) );

            // Create a Connection
            try{
                connection = connectionFactory->createConnection();
                connection->start();
            } catch( CMSException& e ) {
                e.printStackTrace();
                throw e;
            }

            // Create a Session
            if( clientAck ) {
                session = connection->createSession( Session::CLIENT_ACKNOWLEDGE );
            } else {
                session = connection->createSession( Session::AUTO_ACKNOWLEDGE );
            }

            // Create the destination (Topic or Queue)
            if( useTopic ) {
                destination = session->createTopic( destURI );
            } else {
                destination = session->createQueue( destURI );
            }

            // Create a MessageProducer from the Session to the Topic or Queue
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

//            // Create the Thread Id String
//            string threadIdStr = Long::toString( Thread::currentThread()->getId() );
//
//            // Create a messages
//            string text = (string)"Hello world! from thread " + threadIdStr;
//
//            for( unsigned int ix=0; ix<numMessages; ++ix ){
//                TextMessage* message = session->createTextMessage( text );
//
//                message->setIntProperty( "Integer", ix );
//
//                // Tell the producer to send the message
//                printf( "Sent message #%d from thread %s\n", ix+1, threadIdStr.c_str() );
//                producer->send( message );
//
//                delete message;
//            }

        }catch ( CMSException& e ) {
            e.printStackTrace();
        }
    }


    void send(std::string msg)
    {
        TextMessage* message = session->createTextMessage( msg );
        producer->send( message );
        delete message;
    }

private:

    void cleanup(){

        // Destroy resources.
        try{
            if( destination != NULL ) delete destination;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        destination = NULL;

        try{
            if( producer != NULL ) delete producer;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        producer = NULL;

        // Close open resources.
        try{
            if( session != NULL ) session->close();
            if( connection != NULL ) connection->close();
        }catch ( CMSException& e ) { e.printStackTrace(); }

        try{
            if( session != NULL ) delete session;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        session = NULL;

        try{
            if( connection != NULL ) delete connection;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        connection = NULL;
    }
};