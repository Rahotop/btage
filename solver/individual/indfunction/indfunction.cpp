#include "indfunction.hpp"

IndFunction::IndFunction() : Individual(), FunctionTree(), m_scores()
{

}

IndFunction::IndFunction(const IndFunction& ind) : Individual(ind), FunctionTree(ind), m_scores(ind.m_scores)
{

}

IndFunction::IndFunction(const FunctionTree& fn) : Individual(), FunctionTree(fn), m_scores()
{

}

IndFunction::IndFunction(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full) : Individual(), FunctionTree(maxsize,depth,termset,full), m_scores()
{

}

IndFunction::~IndFunction()
{
	
}

float IndFunction::getScore() const
{
	float tmp = 0;
	for(unsigned int i(0); i < m_scores.size(); ++i)
	{
		tmp += m_scores[i];
	}
	tmp /= m_scores.size();
	return tmp;
}

void IndFunction::setScore(float score)
{
	m_scores.push_back(score);
}

void IndFunction::resetScore()
{
	m_scores.clear();
}

bool IndFunction::operator<(const Individual& ind) const
{
	return getScore() < ind.getScore();
}

FunctionTree& IndFunction::getFunction()
{
	return *this;
}

IndFunction& IndFunction::operator=(const IndFunction& ind)
{
	Individual::operator=(ind);
	FunctionTree::operator=(ind);
	m_scores = ind.m_scores;

	return *this;
}

void IndFunction::swap(IndFunction& ind)
{
	Individual::swap(ind);

	FunctionTree::swap(ind);
	m_scores.swap(ind.m_scores);
}




