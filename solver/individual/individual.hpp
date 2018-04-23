#ifndef INDIVIDUAL_HPP_INCLUDED
#define INDIVIDUAL_HPP_INCLUDED

#include <iostream>

class Individual
{
	public:

	Individual();
	Individual(const Individual& ind);
	virtual ~Individual() = 0;

	virtual float getScore() const;
	virtual void setScore(float score);

	Individual& operator=(const Individual& ind);
	virtual bool operator<(const Individual& ind) const;
	void swap(Individual& ind);

	private:

	float m_score;
};

#endif