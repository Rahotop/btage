#ifndef GENERATORRO_HPP_INCLUDED
#define GENERATORRO_HPP_INCLUDED

#include "../generator.hpp"

template<class Indiv>
class GeneratorRO : public Generator<Indiv>
{
	public:

	GeneratorRO() : Generator<Indiv>()
	{

	}

	~GeneratorRO()
	{

	}

	virtual Indiv near(const Indiv& ind, unsigned int i) const = 0;
	virtual unsigned int nearSize(const Indiv& ind) const = 0;

	private:


};

#endif