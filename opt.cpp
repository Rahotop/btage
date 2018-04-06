#include "opt.hpp"

std::vector<opt> getopt(int argc, char **argv)
{
	std::vector<std::string> tmp;
	for(int i(1); i < argc; ++i)
	{
		tmp.push_back(std::string(argv[i]));
	}

	return getopt(tmp);
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
		else
		{
			std::cout << "error : \"" << args[i] << "\" unknown command." << std::endl;
			++i;
		}
	}

	return tmp;
}



