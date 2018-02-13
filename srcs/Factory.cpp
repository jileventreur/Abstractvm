#include "Factory.hpp"

const IOperand * Factory::createOperand(eOperandType type, const std::string &value) const
{
	return ((this->*functab[static_cast<int>(type)])(value));
}

IOperand const * Factory::createInt8( std::string const & value ) const { return ( new Operand<int8_t>(value));}
IOperand const * Factory::createInt16( std::string const & value ) const { return ( new Operand<int16_t>(value));}
IOperand const * Factory::createInt32( std::string const & value ) const { return ( new Operand<int32_t>(value));}
IOperand const * Factory::createInt64( std::string const & value ) const { return ( new Operand<int64_t>(value));}
IOperand const * Factory::createFloat( std::string const & value ) const { return ( new Operand<float>(value));}
IOperand const * Factory::createDouble( std::string const & value ) const { return ( new Operand<double>(value));}

/*
coplien form functions **********************************************************************************************************
*********************************************************************************************************************************
*/

Factory::Factory(void){}
Factory::~Factory(void){}
Factory &Factory::operator=(Factory const & rhs) {if (&rhs == this) {} return (*this);}
Factory::Factory(Factory const &src) {*this = src;}