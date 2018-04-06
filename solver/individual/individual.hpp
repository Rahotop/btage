#ifndef INDIVIDUAL_HPP_INCLUDED
#define INDIVIDUAL_HPP_INCLUDED

class Individual
{
	public:

	Individual();
	Individual(const Individual& ind);
	virtual ~Individual() = 0;

	float getScore() const;
	void setScore(float score);

	Individual& operator=(const Individual& ind);
	bool operator<(const Individual& ind) const;

	private:

	float m_score;
};

#endif