#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include "bitmap.hpp"
#include "config.hpp"

class Analyzer
{
private:
    Bitmap  m_Bitmap;
    Config* m_Config;

    bool ProcessInput(unsigned int x, unsigned int y);
    
public:
    Analyzer(Config& cfg);

    bool Load(const char* path);
};

#endif // ANALYZER_HPP