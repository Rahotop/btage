#include "indfunction.hpp"

IndFunction::IndFunction() : Individual(), m_tree()
{

}

IndFunction::IndFunction(const IndFunction& ind) : Individual(ind), m_tree(ind.m_tree)
{

}

IndFunction::IndFunction(const FunctionTree& fn) : Individual(), m_tree(fn)
{

}

IndFunction::IndFunction(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full) : Individual(), m_tree(maxsize,depth,termset,full)
{

}

IndFunction::~IndFunction()
{
	
}

FunctionTree& IndFunction::getFunction()
{
	return m_tree;
}

IndFunction& IndFunction::operator=(const IndFunction& ind)
{
	Individual::operator=(ind);
	m_tree = ind.m_tree;

	return *this;
}

