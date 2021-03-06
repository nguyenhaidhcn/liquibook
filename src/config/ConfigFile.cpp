//
// Created by HaiNt on 6/15/18.
//

#include "ConfigFile.h"

#include "sstream"
#include <fstream>
#include <glog/logging.h>

std::string trim(std::string const& source, char const* delims = " \t\r\n") {
    std::string result(source);
    std::string::size_type index = result.find_last_not_of(delims);
    if(index != std::string::npos)
        result.erase(++index);

    index = result.find_first_not_of(delims);
    if(index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();
    return result;
}

ConfigFile::ConfigFile(std::string const& configFile) {
    std::ifstream file(configFile.c_str());

    std::string line;
    std::string name;
    std::string value;
    std::string inSection;
    int posEqual;
    while (std::getline(file,line)) {

        if (! line.length()) continue;

        if (line[0] == '#') continue;
        if (line[0] == ';') continue;

        if (line[0] == '[') {
            inSection=trim(line.substr(1,line.find(']')-1));
            continue;
        }

        posEqual=line.find('=');
        name  = trim(line.substr(0,posEqual));
        value = trim(line.substr(posEqual+1));

        content_[inSection+'/'+name]=(value);
        DLOG(INFO)<<"Section:"<<inSection<<"   "<<name<<"="<<value;
    }
}

ConfigFile* ConfigFile::configFile = 0;

ConfigFile* ConfigFile::getInstance()
{
    if(configFile == 0)
    {
        configFile = new ConfigFile("config.txt");
    }

    return configFile;
}


std::string const& ConfigFile::Value(std::string const& section, std::string const& entry) const {

    std::map<std::string,std::string>::const_iterator ci = content_.find(section + '/' + entry);

    if (ci == content_.end())
    {
        std::stringstream ss;
        ss<<section<<" key:"<<entry<<"does not exist";
        throw ss.str();
    }

    return ci->second;
}
