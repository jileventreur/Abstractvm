#include "CustomStack.hpp"

CustomStack::iterator CustomStack::begin(void)
{
	return (this->c.rbegin());
};

CustomStack::iterator CustomStack::end(void)
{
	return (this->c.rend());
};

/*
** coplien form functions *******************************************************************************************************
*********************************************************************************************************************************
*/

CustomStack& CustomStack::operator=(CustomStack const & rhs)
{
	std::stack<IOperand const *>::operator=(rhs);
	return (*this);
};

CustomStack::CustomStack(void) : std::stack<IOperand const *>(){};
CustomStack::CustomStack(CustomStack const & src) : std::stack<IOperand const *>(src){};
CustomStack::~CustomStack(void){};