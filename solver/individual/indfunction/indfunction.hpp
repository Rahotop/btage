#ifndef INDFUNCTION_HPP_INCLUDED
#define INDFUNCTION_HPP_INCLUDED

#include <vector>
#include "../individual.hpp"
#include "../../function/functiontree/functiontree.hpp"

class IndFunction : public Individual, public FunctionTree
{
	public:

	IndFunction();
	IndFunction(const IndFunction& ind);
	IndFunction(const FunctionTree& fn);
	IndFunction(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full = false);
	~IndFunction();

	float getScore() const;
	void setScore(float score);
	void resetScore();
	bool operator<(const Individual& ind) const;

	FunctionTree& getFunction();
	IndFunction& operator=(const IndFunction& ind);
	void swap(IndFunction& ind);

	private:

	std::vector<float> m_scores;
};

#endif