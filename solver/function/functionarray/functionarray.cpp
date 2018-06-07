#include "functionarray.hpp"

std::default_random_engine FunctionArray::s_gen = std::default_random_engine(rand());
std::uniform_real_distribution<float> FunctionArray::s_dist = std::uniform_real_distribution<float>(0.,1.);

FunctionArray::FunctionArray() :
	Function<VectorBool>(),
	m_maxWidth(0),
	m_maxDepth(0),
	m_maxsize(0),
	m_width(0),
	m_initdepth(0),
	m_op(nullptr),
	m_not(),
	m_scal(nullptr),
	m_isvarin(nullptr),
	m_prev(nullptr),
	m_curr(nullptr),
	m_n(0),
	m_fnset()
{
	
}

FunctionArray::FunctionArray(const FunctionArray& fn) :
	Function<VectorBool>(),
	m_maxWidth(fn.m_maxWidth),
	m_maxDepth(fn.m_maxDepth),
	m_maxsize(fn.m_maxsize),
	m_width(fn.m_width),
	m_initdepth(fn.m_initdepth),
	m_op(nullptr),
	m_not(fn.m_not),
	m_scal(nullptr),
	m_isvarin(nullptr),
	m_prev(nullptr),
	m_curr(nullptr),
	m_n(fn.m_n),
	m_fnset(fn.m_fnset)
{
	m_op = new unsigned int[m_maxWidth*m_maxsize+1];
	for(unsigned int i(0); i < m_maxWidth*m_maxsize+1; ++i)
	{
		m_op[i] = fn.m_op[i];
	}
	m_scal = new float[m_maxWidth];
	for(unsigned int i(0); i < m_width; ++i)
	{
		m_scal[i] = fn.m_scal[i];
	}
	m_isvarin = new bool[m_maxWidth*m_n];
	for(unsigned int i(0); i < m_maxWidth*m_n; ++i)
	{
		m_isvarin[i] = fn.m_isvarin[i];
	}
	m_prev = new float[m_maxWidth];
	m_curr = new float[m_maxWidth];
	for(unsigned int i(0); i < m_maxWidth; ++i)
	{
		m_prev[i] = fn.m_prev[i];
		m_curr[i] = fn.m_curr[i];
	}
}

FunctionArray::FunctionArray(unsigned int maxWidth, unsigned int maxDepth, unsigned int initWidth, unsigned int initDepth, unsigned int termset, const std::vector<unsigned int>& fnset) :
	Function<VectorBool>(),
	m_maxWidth(maxWidth),
	m_maxDepth(maxDepth),
	m_maxsize(1),
	m_width(initWidth),
	m_initdepth(initDepth),
	m_op(nullptr),
	m_not(),
	m_scal(nullptr),
	m_isvarin(nullptr),
	m_prev(nullptr),
	m_curr(nullptr),
	m_n(termset),
	m_fnset(fnset)
{
	unsigned int tmp = 1;
	for(unsigned int i(0); i < maxDepth; ++i)
	{
		tmp *= 2;
		m_maxsize += tmp;
	}

	m_op = new unsigned int[m_maxWidth*m_maxsize+1];
	m_not = std::vector<bool>(m_maxWidth*m_maxsize+1);

	for(unsigned int i(0); i < m_maxWidth*m_maxsize+1; ++i)
	{
		m_not[i] = false;
	}

	m_scal = new float[m_maxWidth];
	for(unsigned int i(0); i < m_width; ++i)
	{
		construct(i*m_maxsize, rand()%2);
		m_scal[i] = s_dist(s_gen);
	}

	m_isvarin = new bool[m_maxWidth*m_n];
	for(unsigned int i(0); i < m_width; ++i)
	{
		updateIsVarIn(i);
	}
	m_prev = new float[m_maxWidth];
	m_curr = new float[m_maxWidth];
	for(unsigned int i(0); i < m_maxWidth; ++i)
	{
		m_prev[i] = 0.;
		m_curr[i] = 0.;
	}
}

FunctionArray::~FunctionArray()
{
	if(m_op)
		delete[] m_op;
	if(m_scal)
		delete[] m_scal;
	if(m_isvarin)
		delete[] m_isvarin;
	if(m_prev)
		delete[] m_prev;
	if(m_curr)
		delete[] m_curr;
}

FunctionArray& FunctionArray::operator=(const FunctionArray& f)
{
	if(m_op)
		delete[] m_op;
	if(m_scal)
		delete[] m_scal;
	if(m_isvarin)
		delete[] m_isvarin;
	if(m_prev)
		delete[] m_prev;
	if(m_curr)
		delete[] m_curr;

	m_maxWidth = f.m_maxWidth;
	m_maxDepth = f.m_maxDepth;
	m_maxsize = f.m_maxsize;
	m_width = f.m_width;
	m_initdepth = f.m_initdepth;
	m_n = f.m_n;
	m_op = new unsigned int[m_maxWidth*m_maxsize+1];
	m_scal = new float[m_maxWidth];
	m_isvarin = new bool[m_maxWidth*m_n];
	m_prev = new float[m_maxWidth];
	m_curr = new float[m_maxWidth];
	m_not = f.m_not;
	m_fnset = f.m_fnset;

	for(unsigned int i(0); i < m_maxWidth*m_maxsize+1; ++i)
	{
		m_op[i] = f.m_op[i];
	}
	for(unsigned int i(0); i < m_maxWidth; ++i)
	{
		m_scal[i] = f.m_scal[i];
	}
	for(unsigned int i(0); i < m_maxWidth*m_n; ++i)
	{
		m_isvarin[i] = f.m_isvarin[i];
	}
	for(unsigned int i(0); i < m_maxWidth; ++i)
	{
		m_prev[i] = f.m_prev[i];
		m_curr[i] = f.m_curr[i];
	}

	return *this;
}

void FunctionArray::swap(FunctionArray& f)
{
	unsigned int tmpui = m_maxWidth;
	m_maxWidth = f.m_maxWidth;
	f.m_maxWidth = tmpui;

	tmpui = m_maxDepth;
	m_maxDepth = f.m_maxDepth;
	f.m_maxDepth = tmpui;

	tmpui = m_maxsize;
	m_maxsize = f.m_maxsize;
	f.m_maxsize = tmpui;

	tmpui = m_width;
	m_width = f.m_width;
	f.m_width = tmpui;

	tmpui = m_initdepth;
	m_initdepth = f.m_initdepth;
	f.m_initdepth = tmpui;

	unsigned int *tmpuip = m_op;
	m_op = f.m_op;
	f.m_op = tmpuip;

	float *tmpfp = m_scal;
	m_scal = f.m_scal;
	f.m_scal = tmpfp;

	bool *tmpbp = m_isvarin;
	m_isvarin = f.m_isvarin;
	f.m_isvarin = tmpbp;

	tmpfp =  m_prev;
	m_prev = f.m_prev;
	f.m_prev = tmpfp;

	tmpfp = m_curr;
	m_curr = f.m_curr;
	f.m_curr = tmpfp;

	m_not.swap(f.m_not);

	tmpui = m_n;
	m_n = f.m_n;
	f.m_n = tmpui;
}

void FunctionArray::construct(unsigned int node, bool full, unsigned int offset, unsigned int depth)
{
	if(full)
	{
		if(depth < m_initdepth)
			m_op[node+offset] = m_fnset[rand()%m_fnset.size()];
		else
			m_op[node+offset] = 16 + rand()%m_n;
	}
	else
	{
		if(depth < m_initdepth)
		{
			if(rand()%(m_fnset.size()+m_n) < m_fnset.size())
				m_op[node+offset] = m_fnset[rand()%m_fnset.size()];
			else
				m_op[node+offset] = 16 + rand()%m_n;
		}
		else
		{
			m_op[node+offset] = 16 + rand()%m_n;
		}
	}
	m_not[node+offset] = rand()%2;

	if(m_op[node+offset] < 16)
	{
		construct(node,full,offset*2,depth+1);
		construct(node,full,offset*2+1,depth+1);
	}
}

float FunctionArray::evaluate(VectorBool& s)
{
	float sum = 0;
	for(unsigned int i(0); i < m_width; ++i)
	{
		m_prev[i] = m_scal[i] * evaluate(s, i*m_maxsize);
		m_curr[i] = m_prev[i];
		sum += m_prev[i];
	}
	return sum;
}

bool FunctionArray::evaluate(VectorBool& s, unsigned int node, unsigned int offset)
{
	unsigned int tmp = m_op[node+offset];

	if(tmp > 15)
	{
		return s[tmp-16] != m_not[node+offset];
	}

	return (bool)((tmp << (evaluate(s, node, offset*2)*2+evaluate(s, node, offset*2+1))) >> 3) != m_not[node+offset];
}

float FunctionArray::evaluateInc(VectorBool& s, unsigned int bitChanged, unsigned int node)
{
	node = node;
	float sum = 0;
	for(unsigned int i(0); i < m_width; ++i)
	{
		m_curr[i] = (m_isvarin[bitChanged*m_maxWidth+i]) ? evaluate(s, i*m_maxsize) : m_prev[i];
		sum += m_curr[i];
	}
	return sum;
}

void FunctionArray::makeCurrentPrev(unsigned int bitChanged)
{
	for(unsigned int i(0); i < m_width; ++i)
	{
		m_prev[i] = (m_isvarin[bitChanged*m_maxWidth+i]) ? m_curr[i] : m_prev[i];
	}
}

void FunctionArray::addRandom()
{
	if(!isfull())
	{
		construct(m_width*m_maxsize, rand()%2);
		m_scal[m_width] = s_dist(s_gen);
		updateIsVarIn(m_width);
		++m_width;
	}
	else
	{
		simplification();
	}
}

void FunctionArray::add(const FunctionArray& f, unsigned int index)
{
	if(!isfull())
	{
		for(unsigned int i(0); i < m_maxsize; ++i)
		{
			m_op[m_width*m_maxsize+i+1] = f.m_op[index*f.m_maxsize+i+1];
			m_not[m_width*m_maxsize+i+1] = f.m_not[index*f.m_maxsize+i+1];
		}
		m_scal[m_width] = f.m_scal[index];
		updateIsVarIn(m_width);
		++m_width;
	}
}

void FunctionArray::add(unsigned int var, bool varnot, float scal)
{
	if(!isfull())
	{
		m_op[m_width*m_maxsize+1] = var + 16;
		m_not[m_width*m_maxsize+1] = varnot;
		m_scal[m_width] = scal;
		updateIsVarIn(m_width);
		++m_width;
	}
}

void FunctionArray::add(float scal, unsigned int op, bool opnot, unsigned int var1, bool var1not, unsigned int var2, bool var2not)
{
	if(!isfull())
	{
		m_op[m_width*m_maxsize+1] = op;
		m_not[m_width*m_maxsize+1] = opnot;
		m_scal[m_width] = scal;

		m_op[m_width*m_maxsize+2] = var1 + 16;
		m_not[m_width*m_maxsize+2] = var1not;

		m_op[m_width*m_maxsize+3] = var2 + 16;
		m_not[m_width*m_maxsize+3] = var2not;

		updateIsVarIn(m_width);
		++m_width;
	}
}

void FunctionArray::reconstruct(unsigned int index)
{
	construct(index*m_maxsize, rand()%2);
	m_scal[index] = s_dist(s_gen);
	updateIsVarIn(index);
}

void FunctionArray::erase(unsigned int index)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_op[index*m_maxsize+i+1] = m_op[(m_width-1)*m_maxsize+i+1];
		m_not[index*m_maxsize+i+1] = m_not[(m_width-1)*m_maxsize+i+1];
	}
	m_scal[index] = m_scal[m_width-1];
	updateIsVarIn(index);
	--m_width;
}

void FunctionArray::mutate(unsigned int index)
{
	unsigned int tmp = size(index*m_maxsize);
	unsigned int mut = rand()%(tmp+1);

	if(mut == tmp)
	{
		m_scal[index] += (s_dist(s_gen)/10) - 0.05;
		if(m_scal[index] < 0)
		{
			m_scal[index] *= -1;
			m_not[index*m_maxsize+1] = !m_not[index*m_maxsize+1];
		}
		m_scal[index] = (m_scal[index] > 1.) ? 1. : m_scal[index];
	}
	else
	{
		mutate(index*m_maxsize,mut);
		updateIsVarIn(index);
	}
}

int FunctionArray::mutate(unsigned int node, int thisone, unsigned int offset)
{
	if(!thisone)
	{
		if(m_op[node+offset] < 16)
		{
			if(rand()%10)
			{
				m_op[node+offset] = m_fnset[rand()%m_fnset.size()];
				m_not[node+offset] = rand()%2;
			}
			else
			{
				m_op[node+offset] = 16 + rand()%m_n;
				m_not[node+offset] = rand()%2;
			}
		}
		else
		{
			if(rand()%2)
			{
				m_op[node+offset] = 16 + rand()%m_n;
				m_not[node+offset] = rand()%2;
			}
			else
			{
				if(offset*2+1 < m_maxsize)
				{
					if(rand()%2)
					{
						m_op[node+offset*2] = m_op[node+offset];
						m_not[node+offset*2] = m_not[node+offset];

						m_op[node+offset] = m_fnset[rand()%m_fnset.size()];
						m_not[node+offset] = rand()%2;

						m_op[node+offset*2+1] = 16 + rand()%m_n;
						m_not[node+offset*2+1] = rand()%2;
					}
					else
					{
						m_op[node+offset*2+1] = m_op[node+offset];
						m_not[node+offset*2+1] = m_not[node+offset];

						m_op[node+offset] = m_fnset[rand()%m_fnset.size()];
						m_not[node+offset] = rand()%2;

						m_op[node+offset*2] = 16 + rand()%m_n;
						m_not[node+offset*2] = rand()%2;
					}
				}
			}
		}
	}

	if(m_op[node+offset] < 16)
	{
		thisone = mutate(node, thisone-1, offset*2);
		mutate(node, thisone-1, offset*2+1);
	}
	return thisone;
}

unsigned int FunctionArray::size() const
{
	unsigned int sum = 0;
	for(unsigned int i(0); i < m_width; ++i)
	{
		sum += size(i*m_maxsize);
	}
	return sum;
}

unsigned int FunctionArray::size(unsigned int node, unsigned int offset) const
{
	if(m_op[node+offset] > 15)
	{
		return 1;
	}
	else
	{
		return 1 + size(node, offset*2) + size(node, offset*2+1);
	}
}

unsigned int FunctionArray::width() const
{
	return m_width;
}

bool FunctionArray::isfull() const
{
	return m_width == m_maxWidth;
}

float FunctionArray::operator==(const FunctionArray& f)
{
	unsigned int tmp = 0;
	for(unsigned int i(0); i < m_width; ++i)
	{
		for(unsigned int j(0); j < f.m_width; ++j)
		{
			if(areSubsEqual(f,i*m_maxsize,j*f.m_maxsize))
			{
				++tmp;
				break;
			}
		}
	}
	return (float)tmp / (float)m_width;
}

unsigned int FunctionArray::countOP(unsigned int op) const
{
	unsigned int sum = 0;
	for(unsigned int i(0); i < m_width; ++i)
	{
		sum += countOP(op, i*m_maxsize);
	}
	return sum;
}

unsigned int FunctionArray::countOP(unsigned int op, unsigned int node, unsigned int offset) const
{
	if(op == 16 && m_op[node+offset] > 15)
		return 1;
	else if(m_op[node+offset] > 15)
		return 0;
	else
		return (op == m_op[node+offset]) + countOP(op, node, offset*2) + countOP(op, node, offset*2+1);
}

void FunctionArray::varLinks(std::ostream& o) const
{
	unsigned int *tab = new unsigned int[m_n*m_n];
	for(unsigned int i(0); i < m_n*m_n; ++i) tab[i] = 0;

	for(unsigned int i(0); i < m_width; ++i)
	{
		auto links = varLinks(i*m_maxsize);
		for(unsigned int j(0); j < links.size(); ++j)
		{
			for(unsigned int k(j+1); k < links.size(); ++k)
			{
				++tab[links[j]*m_n+links[k]];
				++tab[links[k]*m_n+links[j]];
			}
		}
	}

	for(unsigned int i(0); i < m_n; ++i)
	{
		for(unsigned int j(0); j < m_n; ++j)
		{
			if(tab[i*m_n+j])
				o << std::setw(3) << tab[i*m_n+j];
			else if(i == j)
				o << "  |";
			else
				o << "  .";
		}
		o << std::endl;
	}
	for(unsigned int i(0); i < m_n; ++i)
	{
		unsigned int tmp = 0;
		for(unsigned int j(0); j < m_width; ++j)
		{
			tmp += countVar(i, j*m_maxsize, false);
		}
		o << std::setw(3) << tmp;
	}
	o << std::endl;
	for(unsigned int i(0); i < m_n; ++i)
	{
		unsigned int tmp = 0;
		for(unsigned int j(0); j < m_width; ++j)
		{
			tmp += countVar(i, j*m_maxsize, true);
		}
		o << std::setw(3) << tmp;
	}
	o << std::endl;
	delete[] tab;
}

unsigned int FunctionArray::countVar(unsigned int var, unsigned int node, bool neg, unsigned int offset) const
{
	if(m_op[node+offset] < 16)
	{
		return countVar(var,node,neg,offset*2)+countVar(var,node,neg,offset*2+1);
	}
	else
	{
		return var == (m_op[node+offset]-16) && m_not[node+offset] == neg;
	}
}

std::vector<unsigned int> FunctionArray::varLinks(unsigned int node, unsigned int offset) const
{
	if(m_op[node+offset] > 15)
	{
		return std::vector<unsigned int>({m_op[node+offset]-16});
	}
	else
	{
		auto c1 = varLinks(node, offset*2);
		auto c2 = varLinks(node, offset*2+1);

		for(unsigned int i(0); i < c2.size(); ++i)
		{
			c1.push_back(c2[i]);
		}

		return c1;
	}
}

void FunctionArray::show(std::ostream& o) const
{
	for(unsigned int i(0); i < m_width; ++i)
	{
		o << i << "; " << m_scal[i] << std::endl;
		show(o, i*m_maxsize);
		o << std::endl;
	}
}

void FunctionArray::show(std::ostream& o, unsigned int node, unsigned int offset, unsigned int depth) const
{
	for(unsigned int i(0); i < depth; ++i)
		o << "| ";
	o << "-";

	if(m_op[node+offset] > 15)
	{
		if(m_not[node+offset])
			o << "!";
		o << "X" << m_op[node+offset]-16 << std::endl;
	}
	else
	{
		if(m_not[node+offset])
			o << "!( ";

		switch(m_op[node+offset])
		{
			case 0: o << "BOTTOM";
				break;
			case 1: o << "and";
				break;
			case 2: o << "!->";
				break;
			case 3: o << "RIGHT";
				break;
			case 4: o << "!<-";
				break;
			case 5: o << "LEFT";
				break;
			case 6: o << "!<->";
				break;
			case 7: o << "or";
				break;
			case 8: o << "!or";
				break;
			case 9: o << "<->";
				break;
			case 10: o << "!LEFT";
				break;
			case 11: o << "<-";
				break;
			case 12: o << "!RIGHT";
				break;
			case 13: o << "->";
				break;
			case 14: o << "!and";
				break;
			case 15: o << "TOP";
				break;
		}

		if(m_not[node+offset])
			o << " )";

		o << std::endl;
		show(o, node, offset*2, depth+1);
		show(o, node, offset*2+1, depth+1);
	}
}

void FunctionArray::showClauses(std::ostream& o) const
{
	o << "";
}

void FunctionArray::updateIsVarIn(unsigned int node, unsigned int index, unsigned int offset)
{
	if(m_op[node+offset] < 16)
	{
		updateIsVarIn(node,index,offset*2);
		updateIsVarIn(node,index,offset*2+1);
	}
	else
	{
		m_isvarin[(m_op[node+offset]-16)*m_maxWidth+index] = true;
	}
}

void FunctionArray::updateIsVarIn(unsigned int index)
{
	for(unsigned int i(0); i < m_n; ++i)
	{
		m_isvarin[i*m_maxWidth+index] = false;
	}
	updateIsVarIn(index*m_maxsize,index);
}

void FunctionArray::simplification()
{
	for(unsigned int i(0); i < m_width-1; ++i)
	{
		for(unsigned int j(i+1); j < m_width; ++j)
		{
			if(areSubsEqual(i*m_maxsize,j*m_maxsize))
			{
				m_scal[i] += m_scal[j];
				m_scal[i] /= 2.;
				//m_scal[i] = (m_scal[i] > 1.) ? 1. : m_scal[i];
				erase(j);
			}
			else if(areSubsOpposite(i*m_maxsize,j*m_maxsize))
			{
				m_scal[i] -= m_scal[j];
				if(m_scal[i] < 0)
				{
					m_scal[i] *= -1;
					m_not[i*m_maxsize+1] = !m_not[i*m_maxsize+1];
				}
				m_scal[i] /= 2.;
				//m_scal[i] = (m_scal[i] > 1.) ? 1. : m_scal[i];
				erase(j);
			}
		}
	}
	for(unsigned int i(0) ; i < m_width;)
	{
		if(!m_scal[i])
		{
			erase(i);
		}
		else
		{
			++i;
		}
	}
}

bool FunctionArray::areSubsEqual(unsigned int index1, unsigned int index2, unsigned int offset1, unsigned int offset2) const
{
	if((m_op[index1+offset1] != m_op[index2+offset2]) || (m_not[index1+offset1] != m_not[index2+offset2]))
	{
		return false;
	}
	if(m_op[index1+offset1] > 15)
	{
		return true;
	}

	return areSubsEqual(index1,index2,offset1*2,offset2*2) && areSubsEqual(index1,index2,offset1*2+1,offset2*2+1);
}

bool FunctionArray::areSubsEqual(const FunctionArray& f, unsigned int index1, unsigned int index2, unsigned int offset1, unsigned int offset2) const
{
	if((m_op[index1+offset1] != f.m_op[index2+offset2]) || (m_not[index1+offset1] != f.m_not[index2+offset2]))
	{
		return false;
	}
	if(m_op[index1+offset1] > 15)
	{
		return true;
	}

	return areSubsEqual(f,index1,index2,offset1*2,offset2*2) && areSubsEqual(f,index1,index2,offset1*2+1,offset2*2+1);
}

bool FunctionArray::areSubsOpposite(unsigned int index1, unsigned int index2, unsigned int offset1, unsigned int offset2) const
{
	if((m_op[index1+offset1] != m_op[index2+offset2]) || (m_not[index1+offset1] == m_not[index2+offset2]))
	{
		return false;
	}
	if(m_op[index1+offset1] > 15)
	{
		return true;
	}

	return areSubsEqual(index1,index2,offset1*2,offset2*2) && areSubsEqual(index1,index2,offset1*2+1,offset2*2+1);
}










