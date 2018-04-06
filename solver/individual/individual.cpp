#include "individual.hpp"

Individual::Individual() : m_score(0.)
{
	
}

Individual::Individual(const Individual& ind) : m_score(ind.m_score)
{
	
}

Individual::~Individual()
{
	
}

float Individual::getScore() const
{
	return m_score;
}

void Individual::setScore(float score)
{
	m_score = score;
}

Individual& Individual::operator=(const Individual& ind)
{
	m_score = ind.m_score;
	return *this;
}

bool Individual::operator<(const Individual& ind) const
{
	return m_score < ind.m_score;
}




