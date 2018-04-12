#ifndef FUNCTION_HPP_INCLUDED
#define FUNCTION_HPP_INCLUDED

template<class Indiv>
class Function
{
	public:

	Function()
	{

	}
	
	virtual ~Function()
	{
		
	}

	virtual float evaluate(Indiv& ind) = 0;
	virtual float evaluateInc(Indiv& ind, unsigned int bitChanged, unsigned int node = 0)
	{
		bitChanged = bitChanged;
		node = node;
		return evaluate(ind);
	}

	virtual void makeCurrentPrev(unsigned int bitChanged)
	{
		bitChanged = bitChanged;
	}

	private:


};

#endif