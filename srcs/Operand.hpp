#ifndef OPERAND_HPP
#define OPERAND_HPP

#include "IOperand.hpp"
#include "Factory.hpp"
#include "MyException.hpp"
#include <iostream>
#include <string>
#include <typeinfo>
#include <cmath>
#include <boost/lexical_cast.hpp>

template <typename T>
class Operand : public IOperand {
public:
	Operand(std::string pValue);
    virtual ~Operand(void);
	Operand(Operand const & src);
	Operand & operator=(Operand const & rhs);
	eOperandType getType(void) const;	
	int getPrecision(void) const;
	std::string const &toString(void) const;
	const IOperand *operator+(IOperand const &rhs) const;
	const IOperand *operator-(IOperand const &rhs) const;
	const IOperand *operator*(IOperand const &rhs) const;
	const IOperand *operator/(IOperand const &rhs) const;
	const IOperand *operator%(IOperand const &rhs) const;
	const IOperand *operator^(IOperand const &rhs) const;
	
private:
	Operand(void);
	const IOperand *generic_operator(IOperand const &rhs, double (*operation)(double, double)) const;
	static double plus(double a, double b);
	static double minus(double a, double b);
	static double mul(double a, double b);
	static double div(double a, double b);
	static double mod(double a, double b);
	static double pow(double a, double b);

/*
** attributes ***************************************************************************
*****************************************************************************************
*/
	T _value;
	static const size_t _type_hash[Double + 1];
	std::string _svalue;
};

#endif