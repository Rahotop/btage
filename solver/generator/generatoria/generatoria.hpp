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

	virtual Indiv generatenew() const = 0;
	virtual void crossover(const std::vector<Indiv>& pop, std::vector<Indiv>& newgen, unsigned int index) const = 0;
	virtual void mutation(const std::vector<Indiv>& pop, std::vector<Indiv>& newgen, unsigned int index) const = 0;

	//virtual void generate(const std::vector<Indiv>& pop, std::vector<Indiv>& newgen) const = 0;

	private:


};

#endif