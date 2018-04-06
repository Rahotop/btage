#include "vectorbool.hpp"

VectorBool::VectorBool() : m_ind()
{

}

VectorBool::VectorBool(unsigned int size) : m_ind(size)
{

}

VectorBool::~VectorBool()
{

}

std::vector<bool>::reference VectorBool::operator[](unsigned int index)
{
	return m_ind[index];
}

bool VectorBool::operator[](unsigned int index) const
{
	return m_ind[index];
}

unsigned int VectorBool::size() const
{
	return m_ind.size();
}

VectorBool& VectorBool::operator=(const VectorBool& v)
{
	Individual::operator=(v);
	m_ind = v.m_ind;

	return *this;
}

