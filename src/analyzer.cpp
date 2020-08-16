#include "analyzer.hpp"

#include "bitmap.hpp"
#include "error.hpp"

Analyzer::Analyzer(Config& cfg)
{
    m_Config = &cfg;
}

bool Analyzer::Load(const char* path)
{
    bool status = true;

    Bitmap bitmap;
    if(!bitmap.Load(path))
    {
        status = error(path, "error reading bitmap file");
    }

    return status;
}
