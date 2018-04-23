#ifndef GENERATORIAONEMAX_HPP_INCLUDED
#define GENERATORIAONEMAX_HPP_INCLUDED

#include <cstdlib>
#include "../generatoria.hpp"
#include "../../../individual/vectorbool/vectorbool.hpp"

class GeneratorIAOneMax : public GeneratorIA<VectorBool>
{
	public:

	GeneratorIAOneMax(unsigned int n);
	~GeneratorIAOneMax();

	VectorBool generate() const;
	void crossover(const std::vector<VectorBool>& pop, std::vector<VectorBool>& newgen, unsigned int index) const;
	void mutation(const std::vector<VectorBool>& pop, std::vector<VectorBool>& newgen, unsigned int index) const;

	private:

	unsigned int m_n;
};

#endif