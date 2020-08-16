#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include "config.hpp"

class Analyzer
{
private:
    Config* m_Config;
    
public:
    Analyzer(Config& cfg);

    bool Load(const char* path);
};

#endif // ANALYZER_HPP