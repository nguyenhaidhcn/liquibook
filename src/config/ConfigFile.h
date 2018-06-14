//
// Created by HaiNt on 6/15/18.
//

#ifndef LIQUIBOOK_CONFIG_H
#define LIQUIBOOK_CONFIG_H
#include <string>
#include <map>


class ConfigFile {
    std::map<std::string,std::string> content_;

public:
    ConfigFile(std::string const& configFile);
    static ConfigFile* configFile;

    static ConfigFile* getInstance();

    std::string const& Value(std::string const& section, std::string const& entry) const;
};


#endif //LIQUIBOOK_CONFIG_H
