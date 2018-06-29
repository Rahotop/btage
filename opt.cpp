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
		else if(args[i] == "-onemax")
		{
			tmp.back().onemax = true;
			tmp.back().n = std::stoi(args[i+1]);
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
		else if(args[i] == "-diversity")
		{
			tmp.back().diversity = true;
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
			tmp.back().diversity = true;

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
			std::string sep(70, '=');
			std::string skip(30, ' ');
			sep += "\n";
			std::cout << sep << "PROBLEMS" << std::endl << sep;

			std::cout << std::setw(20) << "-pbtree" << std::setw(6) << "A B C" << "    The problem to solve is 'pbtree'. A,B" << std::endl;
			std::cout << skip << "and C are ints. A is the depth of '+'. B" << std::endl;
			std::cout << skip << "is the depth of 'min'. C is the number" << std::endl;
			std::cout << skip << "of bits." << std::endl << std::endl;

			std::cout << std::setw(20) << "-nk" << std::setw(6) << "A" << "    The problem to solve is a NK instance. A" << std::endl;
			std::cout << skip << " is a string that contatins the path of" << std::endl;
			std::cout << skip << "the instance to solve." << std::endl << std::endl;

			std::cout << std::setw(20) << "-maxsat" << std::setw(6) << "A" << "    The problem to solve is a MaxSat" << std::endl;
			std::cout << skip << "instance. A is a string that contatins" << std::endl;
			std::cout << skip << "the path of the instance to solve." << std::endl;

			std::cout << sep << "SOLUTIONS" << std::endl << sep;

			std::cout << std::setw(20) << "-fnarray" << std::setw(6) << "A B" << "    The solutions will be represented by an" << std::endl;
			std::cout << skip << "array of boolean functions. A and B are" << std::endl;
			std::cout << skip << "ints. A is the maximum depth for a" << std::endl;
			std::cout << skip << "boolean function. B is the number of" << std::endl;
			std::cout << skip << "functions the solutions will start with" << std::endl;
			std::cout << skip << "If -fnarray isn't present, the" << std::endl;
			std::cout << skip << "solutions will be trees. Nodes can be" << std::endl;
			std::cout << skip << "+,min,max,scal,equal. And leafs bits." << std::endl << std::endl;

			std::cout << std::setw(20) << "-treesize" << std::setw(6) << "A" << "    The size of the solutions. A is an int." << std::endl;
			std::cout << skip << "A is the limit that nodes+leafs can't" << std::endl;
			std::cout << skip << "go further. For -fnarray A is the" << std::endl;
			std::cout << skip << "maximum number of boolean functions." << std::endl << std::endl;

			std::cout << std::setw(20) << "-initdepth" << std::setw(6) << "INT" << "    The depth maximum solutions will start" << std::endl;
			std::cout << skip << "with." << std::endl;

			std::cout << sep << "ALGORITHM OPTIONS" << std::endl << sep;

			std::cout << std::setw(20) << "-nbind" << std::setw(6) << "INT" << "    The number of solutions at each" << std::endl;
			std::cout << skip << "generation." << std::endl << std::endl;

			std::cout << std::setw(20) << "-i" << std::setw(6) << "INT" << "    The number of generations." << std::endl;

			std::cout << std::setw(20) << "-cross" << std::setw(6) << "FLOAT" << "    The proportion of crossovers to generate" << std::endl;
			std::cout << skip << "the next generation." << std::endl << std::endl;

			std::cout << std::setw(20) << "-mutation" << std::setw(6) << "FLOAT" << "    The proportion of mutations to generate" << std::endl;
			std::cout << skip << "the next generation." << std::endl << std::endl;

			std::cout << std::setw(20) << "-copy" << std::setw(6) << "FLOAT" << "    The proportion of copies to generate" << std::endl;
			std::cout << skip << "the next generation." << std::endl << std::endl;

			std::cout << std::setw(20) << "-new" << std::setw(6) << "FLOAT" << "    The proportion of new to generate" << std::endl;
			std::cout << skip << "the next generation." << std::endl << std::endl;

			std::cout << std::setw(20) << "-ccmn" << std::setw(6) << "A" << "    A is an int with 4 digits. The first is" << std::endl;
			std::cout << skip << "the proportion of crossovers in %x10." << std::endl;
			std::cout << skip << "Same as the 2nd for copy 3rd for" << std::endl;
			std::cout << skip << "mutation and 4th for new" << std::endl;

			std::cout << sep << "OUTPUT OPTIONS" << std::endl << sep;

			std::cout << std::setw(20) << "-graphalgogen" << std::setw(6) << " " << "    Will generate the files to track what" << std::endl;
			std::cout << skip << "happened during the search." << std::endl << std::endl;

			std::cout << std::setw(20) << "-graphmean" << std::setw(6) << "A" << "    Will generate a graph of an average" << std::endl;
			std::cout << skip << "local search. Average out of A." << std::endl << std::endl;

			std::cout << std::setw(20) << "-graphagregate" << std::setw(6) << "A" << "     Will generate a graph of A local" << std::endl;
			std::cout << skip << "searches and their mean." << std::endl << std::endl;

			std::cout << std::setw(20) << "-links" << std::setw(6) << " " << "    Will generate a file that shows the" << std::endl;
			std::cout << skip << "links between the bits in the solution," << std::endl;
			std::cout << skip << "in problem. And then shows the solution." << std::endl << std::endl;

			std::cout << std::setw(20) << "-correlation" << std::setw(6) << " " << "    Will generate a graph that represent" << std::endl;
			std::cout << skip << "the correlation of the solution and the" << std::endl;
			std::cout << skip << "problem." << std::endl << std::endl;

			std::cout << std::setw(20) << "-freq" << std::setw(6) << " " << "    Will generate a file that shows the" << std::endl;
			std::cout << skip << "solutions of 100 local searches their" << std::endl;
			std::cout << skip << "scores and frequency." << std::endl << std::endl;

			std::cout << std::setw(20) << "-diversity" << std::setw(6) << " " << "    Will generate a graph that represent the" << std::endl;
			std::cout << skip << "diversity in population in function of" << std::endl;
			std::cout << skip << "generation." << std::endl << std::endl;

			std::cout << std::setw(20) << "-all" << std::setw(6) << "A" << "    Generate all graphs." << std::endl << std::endl;

			std::cout << std::setw(20) << "-out" << std::setw(6) << "A" << "    Set the output at A." << std::endl;

			std::cout << sep << "OTHER" << std::endl << sep;

			std::cout << std::setw(20) << "-optfile" << std::setw(6) << "A" << "    Use the A file as options" << std::endl << std::endl;

			std::cout << std::setw(20) << "-exe" << std::setw(6) << "A" << "    Execute only the A'th instance." << std::endl << std::endl;

			std::cout << std::setw(20) << "-seed" << std::setw(6) << "A" << "    Set the seed to A." << std::endl << std::endl;

			std::cout << std::setw(20) << "-help" << std::setw(6) << " " << "    Shows this." << std::endl << std::endl;

			std::cout << std::setw(20) << "--end" << std::setw(6) << " " << "    End an instance" << std::endl << std::endl;

			std::cout << skip << "" << std::endl;
			++i;
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




