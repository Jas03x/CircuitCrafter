#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <cstdint>

class Bitmap
{
private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint8_t* m_Pixels;

public:
    Bitmap();
    ~Bitmap();

    bool Load(const char* path);

    uint32_t GetWidth();
    uint32_t GetHeight();

    uint32_t GetPixel(unsigned int x, unsigned int y);
    uint8_t* GetPixels();
};

#endif // BITMAP_HPP