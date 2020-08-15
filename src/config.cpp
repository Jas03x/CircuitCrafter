#include "config.hpp"

#include <cstdio>
#include <regex>

bool Config::Error(const char* file_name, const char* message)
{
    printf("%s: %s\n", file_name, message);
    return false;
}

bool Config::Error(const char* file_name, unsigned int line_number, const char* message)
{
    printf("%s:%u: %s\n", file_name, line_number, message);
    return false;
}

bool Config::ReadConfig(const char* path)
{
    bool status = true;

    std::ifstream config(path);
    if(!config.good())
    {
        status = false;
        printf("error: could not read config file \"%s\"\n", path);
    }

    std::cmatch cm;
    std::regex pin("PIN (\\S+) (\\S{1,31}) = \\((\\d+),\\s*(\\d+)\\)");
    std::regex empty("^\\s*$");

    unsigned int line = 0;
    char buffer[MAX_LINE_LENGTH];

    while(status)
    {
        if(config.eof()) {
            break;
        } else {
            line ++;
            *buffer = 0;
            config.getline(buffer, MAX_LINE_LENGTH);
        }

        if(std::regex_match(buffer, cm, pin))
        {
            PIN p = {
                static_cast<uint32_t>(std::strtoul(cm[3].str().c_str(), nullptr, 0)),
                static_cast<uint32_t>(std::strtoul(cm[4].str().c_str(), nullptr, 0)),
                { }
            };
            strncat(p.name, cm[2].str().c_str(), 32);

            if(cm[1].str() == "IN") {
                Inputs.push_back(p);
            } else if(cm[1].str() == "OUT") {
                Outputs.push_back(p);
            } else {
                status = Error(path, line, "unknown pin");
            }
        }
        else if((*buffer != 0) && !std::regex_match(buffer, empty))
        {
            status = Error(path, line, "unknown line:");
            printf("%s\n", buffer);
        }
    }

    return status;
}
