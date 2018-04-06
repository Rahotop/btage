#ifndef GENERATORIA_HPP_INCLUDED
#define GENERATORIA_HPP_INCLUDED

#include <vector>
#include "../generator.hpp"

template<class Indiv>
class GeneratorIA : public Generator<Indiv>
{
	public:

	GeneratorIA() : Generator<Indiv>()
	{

	}

	virtual ~GeneratorIA()
	{

	}

	virtual Indiv crossover(const std::vector<Indiv>& pop) const = 0;
	virtual Indiv mutation(const std::vector<Indiv>& pop) const = 0;

	private:


};

#endif