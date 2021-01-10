#include "bitmap.hpp"

#include <fstream>

// bmp file format: https://en.wikipedia.org/wiki/BMP_file_format

enum : uint16_t { BMP_SIGNATURE = 0x4D42 };

enum : uint32_t
{
    BMP_RGB = 0x0,
    BMP_BITFIELD = 0x3
};

enum : uint32_t
{
    BMP_V4_HDR = 0x6C,
    BMP_V5_HDR = 0x7C
};

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
#ifdef _WIN32 
    #pragma pack(push, 1)
#endif
struct BMP_HDR
{
    uint16_t type; // 'BM'
    uint32_t size; // size of file
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset; // offset to pixel data
}
#ifdef _WIN32 
    ;
    #pragma pack(pop)
#else
    __attribute__((packed));
#endif

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv4header
struct BMP_DESC_V4
{
    uint32_t width; // width of this image
    uint32_t height; // height of this image
    uint16_t planes; // number of planes = 1
    uint16_t bpp; // bits per pixel
    uint32_t compression;
    uint32_t size;
    uint32_t x_resolution; // horizontal resolution - pixels per meter
    uint32_t y_resolution; // vertical resolution - pixels per meter
    uint32_t clr_used;
    uint32_t clr_important;
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;
    uint32_t alpha_mask;
    uint32_t color_space_type;
    struct {
        struct { uint32_t x, y, z; } red;
        struct { uint32_t x, y, z; } green;
        struct { uint32_t x, y, z; } blue;
    } endpoints;
    uint32_t gamma_red;
    uint32_t gamma_green;
    uint32_t gamma_blue;
};

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header
struct BMP_DESC_V5
{
    BMP_DESC_V4 v4;

    uint32_t intent;
    uint32_t profile_data;
    uint32_t profile_size;
    uint32_t reserved;
};

Bitmap::Bitmap()
{
    m_Width = 0;
    m_Height = 0;
    m_Pixels = nullptr;
}

Bitmap::~Bitmap()
{
    if(m_Pixels != nullptr)
    {
        delete[] m_Pixels;
    }
}

bool Bitmap::Load(const char* path)
{
    bool status = true;

    std::ifstream file(path, std::ifstream::binary);
    if(!file.good())
    {
        status = false;
        printf("error: could not open bitmap for reading\n");
    }

    BMP_HDR hdr = {};
    if(status)
    {
        status = file.read(reinterpret_cast<char*>(&hdr), sizeof(BMP_HDR)).good();
    }

    if(status && (hdr.type != BMP_SIGNATURE))
    {
        status = false;
        printf("error: invalid bitmap signature\n");
    }

    uint32_t hdr_ver = 0;
    if(status)
    {
        status = file.read(reinterpret_cast<char*>(&hdr_ver), sizeof(uint32_t)).good();
    }

    BMP_DESC_V5 desc = {};
    if(status)
    {
        if(hdr_ver == BMP_V4_HDR)
        {
            status = file.read(reinterpret_cast<char*>(&desc.v4), sizeof(BMP_DESC_V4)).good();
        }
        else if(hdr_ver == BMP_V5_HDR)
        {
            status = file.read(reinterpret_cast<char*>(&desc), sizeof(BMP_DESC_V5)).good();
        }
        else
        {
            status = false;
            printf("error:unsupported bitmap file\n");
        }
    }

    if(status)
    {
        if((desc.v4.compression != BMP_RGB) && (desc.v4.compression != BMP_BITFIELD))
        {
            status = false;
            printf("error: invalid compression\n");
        }
        else if(desc.v4.bpp != 24)
        {
            status = false;
            printf("error: bitmap has alpha channel\n");
        }
    }

    if(status)
    {
        m_Width = desc.v4.width;
        m_Height = desc.v4.height;

        uint32_t num_pixels = m_Width * m_Height;
        m_Pixels = new uint8_t[num_pixels * 3];

        uint8_t buffer[3];
        for(unsigned int i = 0; status && (i < num_pixels); i++)
        {
            if(!file.read(reinterpret_cast<char*>(buffer), 3).good())
            {
                status = false;
                printf("error: IO failure\n");
            }

            m_Pixels[i * 3 + 0] = buffer[0];
            m_Pixels[i * 3 + 1] = buffer[1];
            m_Pixels[i * 3 + 2] = buffer[2];
        }
    }

    return status;
}

uint32_t Bitmap::GetWidth()
{
    return m_Width;
}

uint32_t Bitmap::GetHeight()
{
    return m_Height;
}

uint8_t* Bitmap::GetPixels()
{
    return m_Pixels;
}

uint32_t Bitmap::GetPixel(unsigned int x, unsigned int y)
{
    unsigned int rgb = 0;
    uint8_t* pixel = &m_Pixels[(x + (m_Height - y - 1) * m_Width) * 3];

    uint8_t* p = reinterpret_cast<uint8_t*>(&rgb);
    p[0] = pixel[0];
    p[1] = pixel[1];
    p[2] = pixel[2];

    return rgb;
}
