#ifndef INDIVIDUAL_HPP_INCLUDED
#define INDIVIDUAL_HPP_INCLUDED

class Individual
{
	public:

	Individual();
	Individual(const Individual& ind);
	virtual ~Individual() = 0;

	virtual float getScore() const;
	virtual void setScore(float score);

	Individual& operator=(const Individual& ind);
	bool operator<(const Individual& ind) const;

	private:

	float m_score;
};

#endif