#ifndef INDFUNCTION_HPP_INCLUDED
#define INDFUNCTION_HPP_INCLUDED

#include "../individual.hpp"
#include "../../function/functiontree/functiontree.hpp"

class IndFunction : public Individual
{
	public:

	IndFunction();
	IndFunction(const IndFunction& ind);
	IndFunction(const FunctionTree& fn);
	IndFunction(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full = false);
	~IndFunction();

	FunctionTree& getFunction();
	IndFunction& operator=(const IndFunction& ind);

	private:

	FunctionTree m_tree;
};

#endif