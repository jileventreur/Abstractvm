#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "IOperand.hpp"
#include "Operand.hpp"
#include <iostream>
#include <string>
#include <typeinfo>

class Factory {
public:
	IOperand const * createOperand( eOperandType type, std::string const & value ) const;
	Factory(void);
	virtual ~Factory(void);
	Factory(Factory const & src);
	Factory & operator=(Factory const & rhs);

private:
	IOperand const * createInt8(std::string const & value ) const;
	IOperand const * createInt16(std::string const & value ) const;
	IOperand const * createInt32(std::string const & value ) const;
	IOperand const * createInt64(std::string const & value ) const;
	IOperand const * createFloat(std::string const & value ) const;
	IOperand const * createDouble(std::string const & value ) const;

	IOperand const * (Factory::*functab[Double + 1])(std::string const &) const = {
		&Factory::createInt8,
		&Factory::createInt16,
		&Factory::createInt32,
		&Factory::createInt64,
		&Factory::createFloat,
		&Factory::createDouble
	};
};
#endif