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

	virtual void crossover(const std::vector<Indiv>& pop, std::vector<Indiv>& newgen, unsigned int index) const = 0;
	virtual void mutation(const std::vector<Indiv>& pop, std::vector<Indiv>& newgen, unsigned int index) const = 0;

	private:


};

#endif