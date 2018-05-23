#ifndef GENERATORIAINDARRAY_HPP_INCLUDED
#define GENERATORIAINDARRAY_HPP_INCLUDED

#include <random>
#include <sstream>
#include <string>
#include "../generatoria.hpp"
#include "../../../individual/indarray/indarray.hpp"

class GeneratorIAIndArray : public GeneratorIA<IndArray>
{
	public:

	GeneratorIAIndArray(unsigned int maxWidth, unsigned int maxDepth, unsigned int initWidth, unsigned int initDepth, unsigned int termset);
	~GeneratorIAIndArray();

	IndArray generate() const;
	void crossover(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const;
	void mutation(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const;

	void crossuni(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const;
	void crossrand(const std::vector<IndArray>& pop, std::vector<IndArray>& newgen, unsigned int index) const;

	private:

	unsigned int m_maxWidth;
	unsigned int m_maxDepth;
	unsigned int m_initWidth;
	unsigned int m_initDepth;
	unsigned int m_n;
};

#endif