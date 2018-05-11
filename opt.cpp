#include "opt.hpp"

std::vector<opt> getopt(int argc, char **argv)
{
	std::vector<std::string> tmp;
	for(int i(1); i < argc; ++i)
	{
		tmp.push_back(std::string(argv[i]));
	}


	std::vector<opt> run = getopt(tmp);

	bool exeall(true);
	for(unsigned int i(0); i < run.size(); ++i)
	{
		if(run[i].execute)
		{
			exeall = false;
			break;
		}
	}

	if(exeall)
	{
		for(unsigned int i(0); i < run.size(); ++i)
		{
			run[i].execute = true;
		}
	}

	return run;
}

std::vector<opt> getopt(std::vector<std::string> args)
{
	std::vector<opt> tmp;
	if(args.size())
		tmp.push_back(opt());
	for(unsigned int i(0); i < args.size();)
	{
		if(args[i] == "-pbtree")
		{
			tmp.back().pbtree = true;
			tmp.back().depthplus = std::stoi(args[i+1]);
			tmp.back().depthmin = std::stoi(args[i+2]);
			tmp.back().n = std::stoi(args[i+3]);
			i += 4;
		}
		else if(args[i] == "-nk")
		{
			tmp.back().nk = true;
			tmp.back().pbfile = args[i+1];
			i += 2;
		}
		else if(args[i] == "-maxsat")
		{
			tmp.back().maxsat = true;
			tmp.back().pbfile = args[i+1];
			i += 2;
		}
		else if(args[i] == "-treesize")
		{
			tmp.back().maxsizetree = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-initdepth")
		{
			tmp.back().initdepth = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-nbind")
		{
			tmp.back().nbind = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-i")
		{
			tmp.back().nbit = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-cross")
		{
			tmp.back().cross = std::stof(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-copy")
		{
			tmp.back().copy = std::stof(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-mutation")
		{
			tmp.back().mutation = std::stof(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-new")
		{
			tmp.back().newop = std::stof(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-ccmn")
		{
			unsigned int ratio = std::stoi(args[i+1]);

			tmp.back().newop = (float)(ratio % 10)/10.;
			ratio /= 10;
			tmp.back().mutation = (float)(ratio % 10)/10.;
			ratio /= 10;
			tmp.back().copy = (float)(ratio % 10)/10.;
			ratio /= 10;
			tmp.back().cross = (float)(ratio % 10)/10.;

			i += 2;
		}
		else if(args[i] == "-graphalgogen")
		{
			tmp.back().graphalgogen = true;
			++i;
		}
		else if(args[i] == "-graphmean")
		{
			tmp.back().graphmean = true;
			tmp.back().meansize = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-graphagregate")
		{
			tmp.back().graphagregate = true;
			tmp.back().agregatesize = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-Rgraphmean")
		{
			tmp.back().Rgraphmean = true;
			tmp.back().Rmeansize = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-links")
		{
			tmp.back().links = true;
			++i;
		}
		else if(args[i] == "-correlation")
		{
			tmp.back().correlation = true;
			++i;
		}
		else if(args[i] == "-freq")
		{
			tmp.back().freq = true;
			++i;
		}
		else if(args[i] == "-all")
		{
			tmp.back().graphalgogen = true;
			tmp.back().graphmean = true;
			tmp.back().graphagregate = true;
			tmp.back().Rgraphmean = true;
			tmp.back().links = true;
			tmp.back().correlation = true;
			tmp.back().freq = true;

			tmp.back().meansize = std::stoi(args[i+1]);
			tmp.back().agregatesize = std::stoi(args[i+1]);
			tmp.back().Rmeansize = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-out")
		{
			tmp.back().out = args[i+1];
			i += 2;
		}
		else if(args[i] == "-optfile")
		{
			std::ifstream in(args[i+1].c_str());
			std::vector<std::string> optstr;
			while(!in.eof())
			{
				std::string buffer;
				in >> buffer;
				optstr.push_back(buffer);
			}
			tmp.pop_back();
			std::vector<opt> optfile = getopt(optstr);
			for(unsigned int i(0); i < optfile.size(); ++i)
			{
				tmp.push_back(optfile[i]);
			}
			i += 2;
		}
		else if(args[i] == "--help")
		{
			++i;
		}
		else if(args[i] == "-help")
		{
			i += 2;
		}
		else if(args[i] == "--end")
		{
			if(i+1 < args.size())
			{
				tmp.push_back(opt());
			}
			++i;
		}
		else if(args[i] == "-seed")
		{
			tmp.back().seed = std::stoi(args[i+1]);
			i += 2;
		}
		else if(args[i] == "-exe")
		{
			unsigned int id = std::stoi(args[i+1]) - 1;
			tmp[id].execute = true;
			i += 2;
		}
		else if(args[i] == "-fnarray")
		{
			tmp.back().functionarray = true;
			tmp.back().maxdepth = std::stoi(args[i+1]);
			tmp.back().initwidth = std::stoi(args[i+2]);
			i += 3;
		}
		else if(args[i].size())
		{
			std::cout << "error : \"" << args[i] << "\" unknown command." << std::endl;
			++i;
		}
		else
		{
			++i;
		}
	}

	return tmp;
}




