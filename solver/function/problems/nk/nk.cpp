#include "nk.hpp"

Nk::Nk(const std::string& path) : m_n(0), m_k1(0), m_2k1(0), m_mat(nullptr), m_var(nullptr)
{
	std::ifstream in(path.c_str());

	in >> m_n >> m_k1;
	++m_k1;
	m_2k1 = pow(2,m_k1);

	m_mat = new float[m_n*m_2k1];
	m_var = new unsigned int[m_n*m_k1];

	for(unsigned int i(0); i < m_n; ++i)
	{
		for(unsigned int j(0); j < m_k1; ++j)
		{
			in >> m_var[m_k1*i+j];
		}
	}

	for(unsigned int i(0); i < m_n; ++i)
	{
		for(unsigned int j(0); j < m_2k1; ++j)
		{
			in >> m_mat[m_2k1*i+j];
		}
	}
}

Nk::~Nk()
{
	delete[] m_mat;
	delete[] m_var;
}

float Nk::evaluate(VectorBool& s)
{
	float sum = 0.;
	for(unsigned int i(0); i < m_n; ++i)
	{
		unsigned int tmp = 0;
		for(unsigned int j(0); j < m_k1; ++j)
		{
			tmp = tmp << 1;
			tmp += s[m_var[m_k1*i+j]];
		}

		sum += m_mat[m_2k1*i+tmp];
	}
	return sum/(float)m_n;
}

float Nk::evaluate(IndFunction& s)
{
	GeneratorROOneMax gen(m_n);
	FunctionTree fn(s.getFunction());
	FixedSizeDescentInc<VectorBool> d(fn, gen);

	float tmp = 0.;
	for(unsigned int i(0); i < 30; ++i)
	{
		VectorBool v = d.solve(); 
		tmp += evaluate(v);
	}
	tmp /= 30.;
	return tmp;
}

unsigned int Nk::getN() const
{
	return m_n;
}
