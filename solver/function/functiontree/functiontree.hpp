#ifndef FUNCTIONTREE_HPP_INCLUDED
#define FUNCTIONTREE_HPP_INCLUDED

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

	void oui() const;

	private:

	unsigned int construct(unsigned int depth, unsigned int termset, bool full, unsigned int root = 0);
	unsigned int constructPb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset, unsigned int termset, unsigned int root = 0);
	unsigned int calculSizePb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset);
	float evaluate(VectorBool& s, unsigned int node);
	unsigned int leaf(unsigned int root);
	unsigned int newNode();

	unsigned int m_maxsize;
	unsigned int *m_op;
	unsigned int *m_root;
	unsigned int *m_child1;
	unsigned int *m_child2;
	float *m_scal;
	unsigned int *m_bitindex;
	bool *m_isused;
	unsigned int m_n;

	static std::default_random_engine s_gen;
	static std::uniform_real_distribution<float> s_dist;
};

#endif