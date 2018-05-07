#include "indarray.hpp"

IndArray::IndArray() : Individual(), FunctionArray(), m_scores()
{

}

IndArray::IndArray(const IndArray& ind) : Individual(ind), FunctionArray(ind), m_scores(ind.m_scores)
{

}

IndArray::IndArray(const FunctionArray& fn) : Individual(), FunctionArray(fn), m_scores()
{

}

IndArray::IndArray(unsigned int maxWidth, unsigned int maxDepth, unsigned int initWidth, unsigned int initDepth, unsigned int termset, const std::vector<unsigned int>& fnset) : Individual(), FunctionArray(maxWidth, maxDepth, initWidth, initDepth, termset, fnset), m_scores()
{

}

IndArray::~IndArray()
{
	
}
/*
float IndArray::getScore() const
{
	float tmp = 0;
	for(unsigned int i(0); i < m_scores.size(); ++i)
	{
		tmp += m_scores[i];
	}
	tmp /= m_scores.size();
	return tmp;
}

void IndArray::setScore(float score)
{
	m_scores.push_back(score);
}*/

void IndArray::resetScore()
{
	m_scores.clear();
}
/*
bool IndArray::operator<(const Individual& ind) const
{
	return getScore() < ind.getScore();
}*/

FunctionArray& IndArray::getFunction()
{
	return *this;
}

const FunctionArray& IndArray::getFunction() const
{
	return *this;
}

IndArray& IndArray::operator=(const IndArray& ind)
{
	Individual::operator=(ind);
	FunctionArray::operator=(ind);
	m_scores = ind.m_scores;

	return *this;
}

void IndArray::swap(IndArray& ind)
{
	Individual::swap(ind);

	FunctionArray::swap(ind);
	m_scores.swap(ind.m_scores);
}

