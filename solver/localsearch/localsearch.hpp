#ifndef LOCALSEARCH_HPP_INCLUDED
#define LOCALSEARCH_HPP_INCLUDED

#include <algorithm>
#include "../solver.hpp"
#include "../generator/generatorro/generatorro.hpp"

template<class Indiv>
class LocalSearch : public Solver<Indiv>
{
	public:

	LocalSearch(Function<Indiv>& fn, GeneratorRO<Indiv>& gen) : Solver<Indiv>(fn), m_gen(gen)
	{

	}

	virtual ~LocalSearch()
	{

	}

	protected:

	GeneratorRO<Indiv>& m_gen;
};

#endif