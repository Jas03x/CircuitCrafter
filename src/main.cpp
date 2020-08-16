
#include <cstdio>

#include "analyzer.hpp"
#include "config.hpp"

int main(int argc, char* argv[])
{
	bool status = true;

	if(argc != 3)
	{
		printf("error: invalid number of parameters\n");
	}

	Config config;
	if(!config.ReadConfig(argv[1]))
	{
		status = false;
		printf("config read failed\n");
	}

	if(status)
	{
		for(unsigned int i = 0; i < config.Inputs.size(); i++)
		{
			Config::PIN& p = config.Inputs[i];
			printf("Input %s: (%u, %u)\n", p.name, p.x, p.y);
		}

		for(unsigned int i = 0; i < config.Outputs.size(); i++)
		{
			Config::PIN& p = config.Outputs[i];
			printf("Output %s: (%u, %u)\n", p.name, p.x, p.y);
		}
	}

	Analyzer analyzer(config);
	if(status)
	{
		analyzer.Load(argv[2]);
	}

	return status ? 0 : -1;
}
