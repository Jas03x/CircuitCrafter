#include "analyzer.hpp"

Analyzer::Analyzer(Config& cfg)
{
    m_Config = &cfg;
}

bool Analyzer::Load(const char* path)
{
    bool status = true;

    if(!m_Bitmap.Load(path))
    {
        status = false;
        printf("error reading bitmap file\n");
    }

    for(unsigned int i = 0; status && (i < m_Config->Inputs.size()); i++)
    {
        Config::PIN* p = &m_Config->Inputs[i];
        status = ProcessInput(p->x, p->y);
    }

    return status;
}

bool Analyzer::ProcessInput(unsigned int x, unsigned int y)
{
    bool status = true;

    if(m_Bitmap.GetPixel(x, y) != 0xFF7F27)
    {
        status = false;
        printf("error: input (%u, %u) is not a pin\n", x, y);
    }
    

    return status;
}
