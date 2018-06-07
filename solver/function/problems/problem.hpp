#ifndef PROBLEM_HPP_INCLUDED
#define PROBLEM_HPP_INCLUDED

#include "../function.hpp"

template<class Indiv>
class Problem : public Function<Indiv>
{
	public:

	Problem() : Function<Indiv>()
	{

	}

	virtual ~Problem()
	{

	}

	private:

};

#endif