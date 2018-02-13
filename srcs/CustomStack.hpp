#ifndef CUSTOMSTACK_HPP
#define CUSTOMSTACK_HPP
#include <stack>
#include <iterator>
#include "IOperand.hpp"

class CustomStack: public std::stack<IOperand const *>
{
    public:
        typedef typename std::deque<IOperand const *>::reverse_iterator iterator;
        CustomStack(void);
        CustomStack(CustomStack const & src);
        virtual ~CustomStack(void);
        iterator begin(void);
        iterator end(void);
        CustomStack& operator=(CustomStack const & rhs);
};
#endif