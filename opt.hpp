#ifndef OPT_HPP_INCLUDED
#define OPT_HPP_INCLUDED

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>

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
		Rgraphmean(false),
		links(false),
		correlation(false),
		freq(false),
		diversity(false),
		meansize(10),
		agregatesize(10),
		Rmeansize(10),
		out("out"),
		seed(rand()),
		execute(false),
		functionarray(false),
		maxdepth(5),
		initwidth(20) {}

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
	bool Rgraphmean;
	bool links;
	bool correlation;
	bool freq;
	bool diversity;
	unsigned int meansize;
	unsigned int agregatesize;
	unsigned int Rmeansize;
	std::string out;
	unsigned int seed;
	bool execute;
	bool functionarray;
	unsigned int maxdepth;
	unsigned int initwidth;
};

std::vector<opt> getopt(int argc, char **argv);
std::vector<opt> getopt(std::vector<std::string> args);

#endif