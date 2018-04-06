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
	VectorBool crossover(const std::vector<VectorBool>& pop) const;
	VectorBool mutation(const std::vector<VectorBool>& pop) const;

	private:

	unsigned int m_n;
};

#endif