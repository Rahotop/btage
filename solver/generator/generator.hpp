#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

template<class Indiv>
class Generator
{
	public:

	Generator()
	{

	}

	~Generator()
	{

	}

	virtual Indiv generate() const = 0;

	private:


};

#endif