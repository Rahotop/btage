#ifndef INDARRAY_HPP_INCLUDED
#define INDARRAY_HPP_INCLUDED

#include "../individual.hpp"
#include "../../function/functionarray/functionarray.hpp"

class IndArray : public Individual, public FunctionArray
{
	public:

	IndArray();
	IndArray(const IndArray& ind);
	IndArray(const FunctionArray& fn);
	IndArray(unsigned int maxWidth, unsigned int maxDepth, unsigned int initWidth, unsigned int initDepth, unsigned int termset, const std::vector<unsigned int>& fnset);
	~IndArray();

	//float getScore() const;
	//void setScore(float score);
	void resetScore();
	//bool operator<(const Individual& ind) const;

	FunctionArray& getFunction();
	const FunctionArray& getFunction() const;
	IndArray& operator=(const IndArray& ind);
	void swap(IndArray& ind);

	private:

	std::vector<float> m_scores;
};

#endif