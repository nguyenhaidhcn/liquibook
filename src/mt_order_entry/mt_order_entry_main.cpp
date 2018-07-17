// Copyright (c) 2017 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "Util.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <locale>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iterator>
#include <Market.h>
#include "config/ConfigFile.h"
#include "cms/CmsConsumer.h"
#include "global/global.h"

//using namespace orderentry;

void start_cms()
{


    //init market
    std::ostream * log = &std::cout;
    ExtMarket =  new orderentry::Market(log);

    //init activemq

    activemq::library::ActiveMQCPP::initializeLibrary();

    DLOG(INFO) << "=====================================================\n";
    DLOG(INFO) << "Starting the Matching engine:" << std::endl;
    DLOG(INFO) << "-----------------------------------------------------\n";

    std::string brokerURI = ConfigFile::getInstance()->Value("Activemq","brokerURI");


    std::string destURI = ConfigFile::getInstance()->Value("Activemq","queue.orderRequest");
    //?consumer.prefetchSize=1";

    // Create the consumer
    ExtConsumer = new CmsConsumer( brokerURI, destURI, false, false );

    std::string destURI2 = ConfigFile::getInstance()->Value("Activemq","topic.orderResponse");

    std::string destURI3 = ConfigFile::getInstance()->Value("Activemq","topic.marketData");


    // Create the producer and run it.
    ExtProducerOrder  =new CmsProducer( brokerURI, destURI2, true , false);
    ExtProducerOrder->run();


    // Create the producer and run it.
    ExtProducerData  =new CmsProducer( brokerURI, destURI3, true , false);
    ExtProducerData->run();


//
//    // Start it up and it will listen forever.
    ExtConsumer->runConsumer();


    DLOG(INFO) << "=====================================================\n";
    DLOG(INFO) << "Started the Matching engine:" << std::endl;
    DLOG(INFO) << "-----------------------------------------------------\n";
    time_t t;
    time(&t);
    sleep(t);


};

int main(int argc, const char * argv[])
{
//    google::InitGoogleLogging(argv[0]);

    auto config = ConfigFile::getInstance();
    DLOG(INFO)<<config;

    start_cms();

    getchar();
    
//    bool done = false;
//    bool prompt = true;
//    bool interactive = true;
//    bool fileActive = false;
//    std::ostream * log = &std::cout;
//    std::ifstream commandFile;
//    std::ofstream logFile;
//
//    DLOG(INFO) << "Test market ";
//
//
//
//    if(argc > 1)
//    {
//        std::string filename = argv[1];
//        if(filename != "-")
//        {
//            commandFile.open(filename);
//            if(!commandFile.good())
//            {
//                std::cerr << "Can't open command file " << filename << ". Exiting." << std::endl;
//                return -1;
//            }
//            interactive = false;
//            fileActive = true;
//        }
//    }
//    if(argc > 2)
//    {
//        const char * filename = argv[2];
//        logFile.open(filename);
//        if(!logFile.good())
//        {
//            std::cerr << "Can't open log file " << filename << ". Exiting." << std::endl;
//            return -1;
//        }
//        log = & logFile;
//    }
//
//
//
//    orderentry::Market * market;
//    market = new orderentry::Market(log);
//
//
//    while( !done)
//    {
//        std::string input;
//        if(fileActive)
//        {
//            std::getline(commandFile, input);
//            if(!commandFile.good())
//            {
//                if(interactive)
//                {
//                    input = "# Switching to console input.";
//                    fileActive = false;
//                }
//                else
//                {
//                    input = "# end of command file.";
//                    done = true;
//                }
//            }
//            // if it came from a file, echo it to the log
//            if(input.substr(0,2) != "##") // don't log ## comments.
//            {
//                *log << input << std::endl;
//            }
//        }
//        else
//        {
//            if(prompt)
//            {
//                DLOG(INFO) << "Action[" << orderentry::Market::prompt()
//                          << "\t(?)    help for more options and detail.\n"
//                          << "\t(Quit) ]\n";
//                prompt = false;
//            }
//            DLOG(INFO) << "> " << std::flush;
//            std::getline(std::cin, input);
//        }
////        std::transform(input.begin(), input.end(), input.begin(), toupper);
//
//        std::locale loc;
////        std::string str="Test String.\n";
//        for (std::string::size_type i=0; i<input.length(); ++i)
//            input[i] = std::toupper(input[i],loc);
//
//        if(log != &std::cout && !fileActive)
//        {
//            if(input.substr(0,2) != "##") // don't log ## comments.
//            {
//                *log << input << std::endl;
//            }
//        }
//
//        // if input ends in a ';' be sure there's a space before it to simplify parsing.
//        if(input.length() > 1)
//        {
//            if(input.back() == ';')
//            {
//                input.pop_back();
//                if(input.back() == ' ')
//                {
//                    input.pop_back();
//                }
//                input.append(" ;");
//            }
//        }
//
//        std::vector< std::string> words;
//        orderentry::split(input," \t\v\n\r", words);
//        if(!words.empty())
//        {
//            const std::string command = words[0];
//            if(command == "QUIT")
//            {
//                done = true;
//            }
//            else if(command[0] == '#')
//            {
//                // nothing
//            }
//            else if(command == "F" || command == "FILE")
//            {
//                if(fileActive)
//                {
//                    DLOG(INFO) << "Only one input file at a time can be open." << std::endl;
//                }
//                else
//                {
//                    DLOG(INFO) << "Command file name: " << std::flush;
//                    std::string filename;
//                    std::getline(std::cin, filename);
//                    commandFile.open(filename);
//                    if(commandFile.good())
//                    {
//                        fileActive = true;
//                    }
//                    else
//                    {
//                        DLOG(INFO) << "Cannot open " << filename << std::endl;
//                    }
//                }
//            }
//            else if(command == "?" || command == "HELP")
//            {
//                market->help();
//                *log << "(F)ile  Open or Close a command input file\n"
//                     << "\tArguments\n"
//                     << "\t\t<FileName>  Required if no file is open. Must not appear if file is open.\n";
//                *log << "QUIT  Exit from this program.\n";
//                bool prompt = true;
//            }
//            else if(!market->apply(words))
//            {
//                std::cerr << "Cannot process command";
//                for(auto word = words.begin(); word != words.end(); ++ word)
//                {
//                    std::cerr << ' ' << *word;
//                }
//                std::cerr << std::endl;
//                bool prompt = true;
//            }
//        }
//    }
    return 0;
}
