#ifndef MYEXCEPTION_HPP
#define MYEXCEPTION_HPP

#include <iostream>
#include <string>
#include <stdexcept>

class MyException {
public:

	class Overflow : public std::overflow_error {
	public:
		Overflow(void);
		Overflow(Overflow const &src);
		Overflow &operator=(Overflow const &rhs);
		virtual ~Overflow(void);
	};

	class Underflow : public std::underflow_error {
	public:
		Underflow(void);
		Underflow(Underflow const &src);
		Underflow &operator=(Underflow const &rhs);
		virtual ~Underflow(void);			
	};

	class Division_by_zero : public std::logic_error {
	public:
		Division_by_zero(void);
		Division_by_zero(Division_by_zero const &src);
		Division_by_zero &operator=(Division_by_zero const &rhs);
		virtual ~Division_by_zero(void);			
	};

	class String_cast_error : public std::invalid_argument {
	public:
		String_cast_error(void);
		String_cast_error(String_cast_error const &src);
		String_cast_error &operator=(String_cast_error const &rhs);
		virtual	~String_cast_error(void);		
	};

	class Empty_Stack : public std::out_of_range {
	public:
		Empty_Stack(void);
		Empty_Stack(Empty_Stack const &src);
		Empty_Stack	&operator=(Empty_Stack const &rhs);
		virtual	~Empty_Stack(void);		
	};

private:
				MyException(void);
				MyException(std::string name,  unsigned int grade);
				MyException(MyException const &src);
MyException		&operator=(MyException const &rhs);
virtual			~MyException(void);
};

#endif