#ifndef FUNCTIONTREE_HPP_INCLUDED
#define FUNCTIONTREE_HPP_INCLUDED

#include <iomanip>
#include <unordered_set>
#include <random>
#include <iostream>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "../function.hpp"
#include "../../individual/vectorbool/vectorbool.hpp"

class FunctionTree : public Function<VectorBool>
{
	public:

	FunctionTree();
	FunctionTree(const FunctionTree& f);
	FunctionTree(const FunctionTree& f, unsigned int node);
	FunctionTree(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full = false);
	FunctionTree(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset, unsigned int termset);
	virtual ~FunctionTree();

	float evaluate(VectorBool& s);
	float evaluateInc(VectorBool& s, unsigned int bitChanged, unsigned int node = 0);
	void makeCurrentPrev(unsigned int bitChanged);

	unsigned int size() const;
	unsigned int getNode(unsigned int offset) const;
	bool isLeaf(unsigned int node) const;
	void show(std::ostream& o, unsigned int depth = 0, unsigned int node = 0) const;

	FunctionTree& operator=(const FunctionTree& f);
	unsigned int copy(const FunctionTree& f, unsigned int fnode, unsigned int root = 0);
	void erase(unsigned int node);
	void replace(unsigned int node, const FunctionTree& f);
	void cut(unsigned int node);
	void mutate(unsigned int node);

	unsigned int countOP(unsigned int op) const;
	void varLinks(std::ostream& o) const;

	private:

	unsigned int construct(unsigned int depth, unsigned int termset, bool full, unsigned int root = 0);
	unsigned int constructPb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset, unsigned int termset, unsigned int root = 0);
	unsigned int calculSizePb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset);
	unsigned int leaf(unsigned int root);
	unsigned int newNode();
	float evaluate(VectorBool& s, unsigned int node);
	void updateVarIn(unsigned int node = 0);
	void updateVarInBU(unsigned int node);
	bool isOPinSubTree(unsigned int op, unsigned int node) const;
	std::vector<unsigned int> getVar(unsigned int node) const;
	std::vector<std::vector<unsigned int>> getLinks(unsigned int node = 0) const;

	unsigned int m_maxsize;
	unsigned int *m_op;
	unsigned int *m_root;
	unsigned int *m_child1;
	unsigned int *m_child2;
	bool *m_isvarin;
	float *m_preveval;
	float *m_curreval;
	float *m_scal;
	unsigned int *m_bitindex;
	bool *m_isused;
	unsigned int m_n;

	static std::default_random_engine s_gen;
	static std::uniform_real_distribution<float> s_dist;
};

#endif