#include "indfunction.hpp"

IndFunction::IndFunction() : Individual(), m_tree(), m_scores()
{

}

IndFunction::IndFunction(const IndFunction& ind) : Individual(ind), m_tree(ind.m_tree), m_scores(ind.m_scores)
{

}

IndFunction::IndFunction(const FunctionTree& fn) : Individual(), m_tree(fn), m_scores()
{

}

IndFunction::IndFunction(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full) : Individual(), m_tree(maxsize,depth,termset,full), m_scores()
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
	return m_tree;
}

IndFunction& IndFunction::operator=(const IndFunction& ind)
{
	Individual::operator=(ind);
	m_tree = ind.m_tree;
	m_scores = ind.m_scores;

	return *this;
}

void IndFunction::swap(IndFunction& ind)
{
	Individual::swap(ind);

	m_tree.swap(ind.m_tree);
	m_scores.swap(ind.m_scores);
}




