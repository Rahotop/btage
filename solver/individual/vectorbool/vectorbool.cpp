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

unsigned int VectorBool::dist(const VectorBool& v) const
{
	unsigned int tmp = 0;
	for(unsigned int i(0); i < m_ind.size(); ++i)
	{
		tmp += m_ind[i] != v.m_ind[i];
	}
	return tmp;
}

bool VectorBool::operator==(const VectorBool& v)
{
	for(unsigned int i(0); i < m_ind.size(); ++i)
	{
		if(m_ind[i] != v.m_ind[i])
			return false;
	}
	return true;
}





