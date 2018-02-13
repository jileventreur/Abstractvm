#include "Operand.hpp"

/*
** main class constructor function
*/

template <typename T> Operand<T>::Operand(std::string pValue) 
{
	double tmp;

	try 
	{
		tmp = boost::lexical_cast<double>(pValue);
	}
	catch (std::exception &e) /* should not happen cause error already managed in lexer */
	{
		std::cout << "bad string == " << pValue << std::endl;
		throw MyException::String_cast_error();
	}
	if (tmp > std::numeric_limits<T>::max())
		throw MyException::Overflow();
	else if (tmp < std::numeric_limits<T>::lowest())
		throw MyException::Underflow();
	this->_value = static_cast<T>(tmp);
	this->_svalue = std::to_string(this->_value);
}

/*
** coplien form functions *******************************************************************************************************
*********************************************************************************************************************************
*/

template <typename T> Operand<T> & Operand<T>::operator=(Operand const & rhs)
{
	if (this == &rhs)
		return (*this);
	this->_svalue = rhs._svalue;
	this->_value = rhs._value;
	return (*this);
}

template <typename T> Operand<T>::Operand(Operand const & src) {*this = src;}
template <typename T> Operand<T>::~Operand(void){}
template <typename T> Operand<T>::Operand(void) {} /* private should be never used */

/*
** getter functions *************************************************************************************************************
*********************************************************************************************************************************
*/

template <typename T> eOperandType Operand<T>::getType(void) const 
{
	for (int i = 0; i < Double; ++i)
	{
		if (Operand<T>::_type_hash[i] == typeid(this->_value).hash_code())
			return (static_cast<eOperandType>(i));
	}
	return (Double);
}

template <typename T> int Operand<T>::getPrecision(void) const 
{
	return (static_cast<int> (this->getType()));
}

template <typename T> std::string const &Operand<T>::toString(void) const
{
	return (this->_svalue);
}

/*
** operation overload ***********************************************************************************************************
*********************************************************************************************************************************
*/

template <typename T> const IOperand *Operand<T>::generic_operator(IOperand const &rhs, double (*operation)(double, double)) const 
{
	eOperandType	maxtype;
	double			res;

	maxtype = static_cast<eOperandType>(std::max(rhs.getPrecision(), this->getPrecision()));
	res = (*operation)(std::stod(rhs.toString()), std::stod(this->toString())); // could throw div by 0
	Factory f;
	return (f.createOperand(maxtype, std::to_string(res)));
}

template <typename T> const IOperand *Operand<T>::operator+(IOperand const &rhs) const{return (generic_operator(rhs, Operand<T>::plus));}
template <typename T> const IOperand *Operand<T>::operator-(IOperand const &rhs) const{return (generic_operator(rhs, Operand<T>::minus));}
template <typename T> const IOperand *Operand<T>::operator*(IOperand const &rhs) const{return (generic_operator(rhs, Operand<T>::mul));}
template <typename T> const IOperand *Operand<T>::operator/(IOperand const &rhs) const{return (generic_operator(rhs, Operand<T>::div));}
template <typename T> const IOperand *Operand<T>::operator%(IOperand const &rhs) const{return (generic_operator(rhs, Operand<T>::mod));}
template <typename T> const IOperand *Operand<T>::operator^(IOperand const &rhs) const{return (generic_operator(rhs, Operand<T>::pow));}

/*
** basic arithmetic functions ***************************************************************************************************
*********************************************************************************************************************************
*/

template <typename T> double Operand<T>::plus(double a, double b) {return (a + b);} 
template <typename T> double Operand<T>::minus(double a, double b) {return (a - b);} 
template <typename T> double Operand<T>::mul(double a, double b) {return (a * b);} 
template <typename T> double Operand<T>::div(double a, double b) 
{
	if (b == 0)
		throw MyException::Division_by_zero();
	return (a / b);
} 
template <typename T> double Operand<T>::mod(double a, double b) 
{
	if (b == 0)
		throw MyException::Division_by_zero();
	return (std::fmod(a, b));
} 
template <typename T> double Operand<T>::pow(double a, double b) {return (std::pow(a,b));}

/*
** static const attribute assignation *******************************************************************************************
*********************************************************************************************************************************
*/

template <typename T> const size_t Operand<T>::_type_hash[Double + 1] {
	typeid(int8_t).hash_code(), 
	typeid(int16_t).hash_code(),
	typeid(int32_t).hash_code(),
	typeid(int64_t).hash_code(),
	typeid(float).hash_code(),
	typeid(double).hash_code()
};

/*
** explicit instantiations ******************************************************************************************************
*********************************************************************************************************************************
*/

/* used to separate header and cpp template class and resolve double include conflicts with Factory class */
template class Operand<int8_t>;
template class Operand<int16_t>;
template class Operand<int32_t>;
template class Operand<int64_t>;
template class Operand<float>;
template class Operand<double>;