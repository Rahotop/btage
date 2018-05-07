#ifndef FUNCTIONARRAY_HPP_INCLUDED
#define FUNCTIONARRAY_HPP_INCLUDED

#include <random>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "../function.hpp"
#include "../../individual/vectorbool/vectorbool.hpp"

class FunctionArray : public Function<VectorBool>
{
	public:

	FunctionArray();
	FunctionArray(const FunctionArray& fn);
	FunctionArray(unsigned int maxWidth, unsigned int maxDepth, unsigned int initWidth, unsigned int initDepth, unsigned int termset, const std::vector<unsigned int>& fnset);
	~FunctionArray();

	float evaluate(VectorBool& s);
	float evaluateInc(VectorBool& s, unsigned int bitChanged, unsigned int node = 0);
	void makeCurrentPrev(unsigned int bitChanged);

	FunctionArray& operator=(const FunctionArray& f);
	void swap(FunctionArray& f);

	void addRandom();
	void add(const FunctionArray& f, unsigned int index);
	void reconstruct(unsigned int index);
	void erase(unsigned int index);
	void mutate(unsigned int index);
	void simplification();

	unsigned int size() const;
	unsigned int width() const;
	bool isfull() const;

	unsigned int countOP(unsigned int op) const;
	void varLinks(std::ostream& o) const;
	void show(std::ostream& o) const;
	void showClauses(std::ostream& o) const;

	private:

	void construct(unsigned int node, bool full, unsigned int offset = 1, unsigned int depth = 0);
	bool evaluate(VectorBool& s, unsigned int node, unsigned int offset = 1);
	unsigned int size(unsigned int node, unsigned int offset = 1) const;
	unsigned int countOP(unsigned int op, unsigned int node, unsigned int offset = 1) const;
	void show(std::ostream& o, unsigned int node, unsigned int offset = 1, unsigned int depth = 0) const;
	std::vector<unsigned int> varLinks(unsigned int node, unsigned int offset = 1) const;
	unsigned int countVar(unsigned int var, unsigned int node, bool neg, unsigned int offset = 1) const;
	void updateIsVarIn(unsigned int node, unsigned int index, unsigned int offset = 1);
	void updateIsVarIn(unsigned int index);
	int mutate(unsigned int node, int thisone, unsigned int offset = 1);
	bool areSubsEqual(unsigned int index1, unsigned int index2, unsigned int offset1 = 1, unsigned int offset2 = 1) const;
	bool areSubsOpposite(unsigned int index1, unsigned int index2, unsigned int offset1 = 1, unsigned int offset2 = 1) const;

	unsigned int m_maxWidth;
	unsigned int m_maxDepth;
	unsigned int m_maxsize;
	unsigned int m_width;
	unsigned int m_initdepth;
	unsigned int *m_op;
	std::vector<bool> m_not;
	float *m_scal;
	bool *m_isvarin;
	float *m_prev;
	float *m_curr;


	unsigned int m_n;
	std::vector<unsigned int> m_fnset;
	
	static std::default_random_engine s_gen;
	static std::uniform_real_distribution<float> s_dist;
};

#endif