#ifndef OPT_HPP_INCLUDED
#define OPT_HPP_INCLUDED

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct opt
{
	opt() :
		pbtree(false),
		nk(false),
		maxsat(false),
		depthplus(1),
		depthmin(1),
		n(16),
		pbfile(""),
		maxsizetree(100),
		initdepth(2),
		nbind(100),
		nbit(500),
		cross(0.8),
		copy(0.1),
		mutation(0.1),
		newop(0.),
		graphalgogen(false),
		graphmean(false),
		graphagregate(false),
		meansize(10),
		agregatesize(10),
		out("out") {}

	bool pbtree;
	bool nk;
	bool maxsat;
	unsigned int depthplus;
	unsigned int depthmin;
	unsigned int n;
	std::string pbfile;
	unsigned int maxsizetree;
	unsigned int initdepth;
	unsigned int nbind;
	unsigned int nbit;
	float cross;
	float copy;
	float mutation;
	float newop;
	bool graphalgogen;
	bool graphmean;
	bool graphagregate;
	unsigned int meansize;
	unsigned int agregatesize;
	std::string out;
};

std::vector<opt> getopt(int argc, char **argv);
std::vector<opt> getopt(std::vector<std::string> args);

#endif