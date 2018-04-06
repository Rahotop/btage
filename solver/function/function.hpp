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

	private:


};

#endif