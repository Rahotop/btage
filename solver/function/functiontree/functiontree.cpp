#include "functiontree.hpp"

std::default_random_engine FunctionTree::s_gen = std::default_random_engine(rand());
std::uniform_real_distribution<float> FunctionTree::s_dist = std::uniform_real_distribution<float>(0.,1.);

FunctionTree::FunctionTree() :
	Function<VectorBool>(),
	m_maxsize(0),
	m_op(nullptr),
	m_root(nullptr),
	m_child1(nullptr),
	m_child2(nullptr),
	m_scal(nullptr),
	m_bitindex(nullptr),
	m_isused(nullptr),
	m_n(0)
{
	
}

FunctionTree::FunctionTree(const FunctionTree& fn) :
	Function<VectorBool>(),
	m_maxsize(fn.m_maxsize),
	m_op(new unsigned int[fn.m_maxsize]),
	m_root(new unsigned int[fn.m_maxsize]),
	m_child1(new unsigned int[fn.m_maxsize]),
	m_child2(new unsigned int[fn.m_maxsize]),
	m_scal(new float[fn.m_maxsize]),
	m_bitindex(new unsigned int[fn.m_maxsize]),
	m_isused(new bool[fn.m_maxsize]),
	m_n(fn.m_n)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_op[i] = fn.m_op[i];
		m_root[i] = fn.m_root[i];
		m_child1[i] = fn.m_child1[i];
		m_child2[i] = fn.m_child2[i];
		m_scal[i] = fn.m_scal[i];
		m_bitindex[i] = fn.m_bitindex[i];
		m_isused[i] = fn.m_isused[i];
	}
}

FunctionTree::FunctionTree(const FunctionTree& fn, unsigned int node) :
	Function<VectorBool>(),
	m_maxsize(fn.m_maxsize),
	m_op(new unsigned int[fn.m_maxsize]),
	m_root(new unsigned int[fn.m_maxsize]),
	m_child1(new unsigned int[fn.m_maxsize]),
	m_child2(new unsigned int[fn.m_maxsize]),
	m_scal(new float[fn.m_maxsize]),
	m_bitindex(new unsigned int[fn.m_maxsize]),
	m_isused(new bool[fn.m_maxsize]),
	m_n(fn.m_n)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}

	copy(fn,node);
}

FunctionTree::FunctionTree(unsigned int maxsize, unsigned int depth, unsigned int termset, bool full) :
	Function<VectorBool>(),
	m_maxsize(maxsize),
	m_op(new unsigned int[maxsize]),
	m_root(new unsigned int[maxsize]),
	m_child1(new unsigned int[maxsize]),
	m_child2(new unsigned int[maxsize]),
	m_scal(new float[maxsize]),
	m_bitindex(new unsigned int[maxsize]),
	m_isused(new bool[m_maxsize]),
	m_n(termset)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}
	construct(depth,termset,full);
}

FunctionTree::FunctionTree(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset, unsigned int termset) :
	Function<VectorBool>(),
	m_maxsize(calculSizePb(depths, fnset)),
	m_op(new unsigned int[m_maxsize]),
	m_root(new unsigned int[m_maxsize]),
	m_child1(new unsigned int[m_maxsize]),
	m_child2(new unsigned int[m_maxsize]),
	m_scal(new float[m_maxsize]),
	m_bitindex(new unsigned int[m_maxsize]),
	m_isused(new bool[m_maxsize]),
	m_n(termset)
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_isused[i] = false;
	}
	constructPb(depths,fnset,termset);
}

FunctionTree::~FunctionTree()
{
	if(m_op)
		delete[] m_op;
	if(m_root)
		delete[] m_root;
	if(m_child1)
		delete[] m_child1;
	if(m_child2)
		delete[] m_child2;
	if(m_scal)
		delete[] m_scal;
	if(m_bitindex)
		delete[] m_bitindex;
	if(m_isused)
		delete[] m_isused;
}

FunctionTree& FunctionTree::operator=(const FunctionTree& f)
{
	if(m_maxsize != f.m_maxsize)
	{
		if(m_op)
			delete[] m_op;
		if(m_root)
			delete[] m_root;
		if(m_child1)
			delete[] m_child1;
		if(m_child2)
			delete[] m_child2;
		if(m_scal)
			delete[] m_scal;
		if(m_bitindex)
			delete[] m_bitindex;
		if(m_isused)
			delete[] m_isused;

		m_maxsize = f.m_maxsize;
		m_op = new unsigned int[m_maxsize];
		m_root = new unsigned int[m_maxsize];
		m_child1 = new unsigned int[m_maxsize];
		m_child2 = new unsigned int[m_maxsize];
		m_scal = new float[m_maxsize];
		m_bitindex = new unsigned int[m_maxsize];
		m_isused = new bool[m_maxsize];
	}

	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		m_op[i] = f.m_op[i];
		m_root[i] = f.m_root[i];
		m_child1[i] = f.m_child1[i];
		m_child2[i] = f.m_child2[i];
		m_scal[i] = f.m_scal[i];
		m_bitindex[i] = f.m_bitindex[i];
		m_isused[i] = f.m_isused[i];
	}

	return *this;
}

unsigned int FunctionTree::newNode()
{
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		if(!m_isused[i])
		{
			m_isused[i] = true;
			return i;
		}
	}
	return 0;
}

void FunctionTree::erase(unsigned int node)
{
	if(m_op[node] > 1)
		erase(m_child1[node]);
	if(m_op[node] > 2)
		erase(m_child2[node]);

	m_isused[node] = false;
}

void FunctionTree::replace(unsigned int node, const FunctionTree& f)
{
	if(node)
	{
		unsigned int root = m_root[node];
		erase(node);

		if(m_child1[root] == node)
			m_child1[root] = copy(f,0,root);
		else
			m_child2[root] = copy(f,0,root);
	}
	else
	{
		erase(0);
		copy(f,0);
	}
}

void FunctionTree::cut(unsigned int node)
{
	if(node)
	{
		unsigned int root = m_root[node];
		erase(node);
		if(m_child1[root] == node)
			m_child1[root] = leaf(root);
		else
			m_child2[root] = leaf(root);
	}
	else
	{
		erase(0);
		unsigned int tmp = newNode();

		m_op[tmp] = rand()%2;
		m_root[tmp] = 0;
		m_scal[tmp] = 0.;
		m_bitindex[tmp] = rand()%m_n;
		m_child1[tmp] = 0;
		m_child2[tmp] = 0;
	}
}

void FunctionTree::mutate(unsigned int node)
{
	if(m_op[node] < 2)
	{
		m_op[node] = rand()%2;
		m_bitindex[node] = rand()%m_n;
	}
	else if(m_op[node] == 2)
	{
		m_scal[node] = s_dist(s_gen);
	}
	else
	{
		m_op[node] = 3+rand()%4;
	}
}

unsigned int FunctionTree::leaf(unsigned int root)
{
	unsigned int node = newNode();

	m_op[node] = rand()%2;
	m_root[node] = root;
	m_scal[node] = 0.;
	m_bitindex[node] = rand()%m_n;
	m_child1[node] = 0;
	m_child2[node] = 0;

	return node;
}

unsigned int FunctionTree::copy(const FunctionTree& f, unsigned int fnode, unsigned int root)
{
	unsigned int node = newNode();

	m_op[node] = f.m_op[fnode];
	m_root[node] = root;
	m_scal[node] = f.m_scal[fnode];
	m_bitindex[node] = f.m_bitindex[fnode];

	if(m_op[node] > 1)
		m_child1[node] = copy(f, f.m_child1[fnode], node);
	if(m_op[node] > 2)
		m_child2[node] = copy(f, f.m_child2[fnode], node);

	return node;
}

unsigned int FunctionTree::construct(unsigned int depth, unsigned int termset, bool full, unsigned int root)
{
	unsigned int node = newNode();

	if(!depth || rand()%(1+!full))	//Arity 0
	{
		m_op[node] = rand()%2;
		m_root[node] = root;
		m_scal[node] = 0.;
		m_bitindex[node] = rand()%termset;
		m_child1[node] = 0;
		m_child2[node] = 0;
	}
	else if(!(rand()%5))			//Arity 1
	{
		m_op[node] = 2;
		m_root[node] = root;
		m_scal[node] = s_dist(s_gen);
		m_bitindex[node] = 0;
		m_child1[node] = construct(depth-1, termset, full, node);
		m_child2[node] = 0;
	}
	else							//Arity 2
	{
		m_op[node] = 3+rand()%4;
		m_root[node] = root;
		m_scal[node] = 0.;
		m_bitindex[node] = 0;
		m_child1[node] = construct(depth-1, termset, full, node);
		m_child2[node] = construct(depth-1, termset, full, node);
	}

	return node;
}

unsigned int FunctionTree::constructPb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset, unsigned int termset, unsigned int root)
{
	unsigned int node = newNode();

	m_op[node] = 0;

	for(unsigned int i(0); i < depths.size(); ++i)
	{
		if(depths[i])
		{
			--depths[i];

			m_op[node] = fnset[i];
			m_root[node] = root;
			m_scal[node] = (fnset[i] == 2) ? s_dist(s_gen) : 0.;
			m_bitindex[node] = 0;

			m_child1[node] = constructPb(depths, fnset, termset, node);
			m_child2[node] = (m_op[i] > 2) ? constructPb(depths, fnset, termset, node) : 0;
		}
	}

	if(!m_op[node])
	{
		m_op[node] = rand()%2;
		m_root[node] = root;
		m_scal[node] = 0.;
		m_bitindex[node] = rand()%termset;
		m_child1[node] = 0;
		m_child2[node] = 0;
	}

	return node;
}

unsigned int FunctionTree::calculSizePb(std::vector<unsigned int> depths, const std::vector<unsigned int>& fnset)
{
	unsigned int sum = 1;
	unsigned int prev = 1;

	for(unsigned int i(0); i < depths.size(); ++i)
	{
		for(unsigned int j(0); j < depths[i]; ++j)
		{
			prev *= 1+(fnset[i] > 2);
			sum += prev;
		}
	}

	return sum;
}

float FunctionTree::evaluate(VectorBool& s)
{
	return evaluate(s,0);
}

float FunctionTree::evaluate(VectorBool& s, unsigned int node)
{
	float tmp = 0;
	if(m_op[node] == 0)
		tmp = s[m_bitindex[node]];
	else if(m_op[node] == 1)
		tmp = !s[m_bitindex[node]];
	else if(m_op[node] == 2)
		tmp = m_scal[node]*evaluate(s,m_child1[node]);
	else if(m_op[node] == 3)
		tmp = evaluate(s,m_child1[node]) == evaluate(s,m_child2[node]);
	else if(m_op[node] == 4)
		tmp = std::max(evaluate(s,m_child1[node]),evaluate(s,m_child2[node]));
	else if(m_op[node] == 5)
		tmp = std::min(evaluate(s,m_child1[node]),evaluate(s,m_child2[node]));
	else if(m_op[node] == 6)
		tmp = evaluate(s,m_child1[node]) + evaluate(s,m_child2[node]);

	return tmp;
}

unsigned int FunctionTree::size() const
{
	unsigned int tmp = 0;
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		if(m_isused[i])
			++tmp;
	}
	return tmp;
}

unsigned int FunctionTree::getNode(unsigned int offset) const
{
	unsigned int tmp = 0;
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		if(m_isused[i])
		{
			if(tmp == offset)
			{
				return i;
			}
			tmp++;
		}
	}
	return 0;
}

bool FunctionTree::isLeaf(unsigned int node) const
{
	return m_op[node] < 2;
}

void FunctionTree::show(std::ostream& o, unsigned int depth, unsigned int node) const
{
	for(unsigned int i(0); i < depth; ++i)
	{
		o << "| ";
	}
	o << "-";
	if(m_op[node] == 0)
		o << "X" << m_bitindex[node];
	else if(m_op[node] == 1)
		o << "!X" << m_bitindex[node];
	else if(m_op[node] == 2)
		o << "Scal " << m_scal[node];
	else if(m_op[node] == 3)
		o << "Equal";
	else if(m_op[node] == 4)
		o << "Max";
	else if(m_op[node] == 5)
		o << "Min";
	else if(m_op[node] == 6)
		o << "Plus";
	o << " " << m_isused[node] << "," << node << std::endl;

	if(m_op[node] > 1)
		show(o, depth+1, m_child1[node]);
	if(m_op[node] > 2)
		show(o, depth+1, m_child2[node]);
}

void FunctionTree::oui() const
{
	std::cout << m_maxsize << std::endl;
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		std::cout << "used[" << i << "] = " << m_isused[i] << std::endl;
	}
}






