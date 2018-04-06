#ifndef VECTORBOOLCOMPLETE_HPP_INCLUDED
#define VECTORBOOLCOMPLETE_HPP_INCLUDED

#include "../../solver.hpp"
#include "../../individual/vectorbool/vectorbool.hpp"

class VectorBoolComplete : public Solver<VectorBool>
{
	public:

	VectorBoolComplete(Function<VectorBool>& fn, unsigned int n);
	~VectorBoolComplete();

	VectorBool solve();

	private:

	unsigned int m_n;
};

#endif