#ifndef VECTORBOOL_HPP_INCLUDED
#define VECTORBOOL_HPP_INCLUDED

#include <vector>
#include "../individual.hpp"

class VectorBool : public Individual
{
	public:

	VectorBool();
	VectorBool(unsigned int size);
	~VectorBool();

	std::vector<bool>::reference operator[](unsigned int index);
	bool operator[](unsigned int index) const;
	unsigned int size() const;

	VectorBool& operator=(const VectorBool& v);
	unsigned int dist(const VectorBool& v) const;

	private:

	std::vector<bool> m_ind;
};

#endif