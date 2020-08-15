#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <fstream>
#include <vector>

class Config
{
public:
	enum : uint32_t { MAX_LINE_LENGTH = 64 };

	struct PIN
	{
		uint32_t x;
		uint32_t y;
		char name[32];
	};

private:
	bool Error(const char* file_name, const char* message);
	bool Error(const char* file_name, unsigned int line_number, const char* message);

public:
	std::vector<PIN> Inputs;
	std::vector<PIN> Outputs;

	bool ReadConfig(const char* path);
};

#endif // CONFIG_HPP