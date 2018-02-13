#include "MyException.hpp"

MyException::MyException(MyException const &src) {*this = src;}
MyException::MyException(void){}
MyException		 &MyException::operator=(MyException const &rhs){if (&rhs == this){}return *this;}
MyException::~MyException(void){}

MyException::Overflow::Overflow(void) : std::overflow_error ("\033[0;1mExecution \033[31merror\033[0;1m : Overflow\033[0m") {}
MyException::Overflow::Overflow(MyException::Overflow const &src) : std::overflow_error ("\033[0;1mExecution \033[31merror\033[0;1m : Overflow\033[0m") {*this = src;}
MyException::Overflow		&MyException::Overflow::operator=(Overflow const &rhs) {if (&rhs == this){}return *this;}
MyException::Overflow::~Overflow(void){}

MyException::Underflow::Underflow(void) : std::underflow_error ("\033[0;1mExecution \033[31merror\033[0;1m : Underflow\033[0;1m\033[0m") {}
MyException::Underflow::Underflow(MyException::Underflow const &src) : std::underflow_error ("\033[0;1mExecution \033[31merror\033[0;1m : Underflow\033[0m") {*this = src;}
MyException::Underflow		&MyException::Underflow::operator=(Underflow const &rhs){if (&rhs == this){}return *this;}
MyException::Underflow::~Underflow(void){}

MyException::Division_by_zero::Division_by_zero(void) : std::logic_error ("\033[0;1mExecution \033[31merror\033[0;1m : Division by zero\033[0m"){}
MyException::Division_by_zero::Division_by_zero(MyException::Division_by_zero const &src) : std::logic_error ("\033[0;1mExecution \033[31merror\033[0;1m : Division by zero\033[0m") {*this = src;}
MyException::Division_by_zero		&MyException::Division_by_zero::operator=(Division_by_zero const &rhs){if (&rhs == this){}return *this;}
MyException::Division_by_zero::~Division_by_zero(void){}

MyException::String_cast_error::String_cast_error(void) : std::invalid_argument ("\033[0;1mExecution \033[31merror\033[0;1m : String not compatible with type\033[0m"){}
MyException::String_cast_error::String_cast_error(MyException::String_cast_error const &src) : std::invalid_argument ("\033[0;1mExecution \033[31merror\033[0;1m : String not compatible with type\033[0m") {*this = src;}
MyException::String_cast_error		&MyException::String_cast_error::operator=(String_cast_error const &rhs){if (&rhs == this){}return *this;}
MyException::String_cast_error::~String_cast_error(void){}

MyException::Empty_Stack::Empty_Stack(void) : std::out_of_range ("\033[0;1mExecution \033[31merror\033[0;1m : Stack is empty\033[0m"){}
MyException::Empty_Stack::Empty_Stack(MyException::Empty_Stack const &src) : std::out_of_range ("\033[0;1mExecution \033[31merror\033[0;1m : Stack is empty\033[0m") {*this = src;}
MyException::Empty_Stack		&MyException::Empty_Stack::operator=(Empty_Stack const &rhs){if (&rhs == this){}return *this;}
MyException::Empty_Stack::~Empty_Stack(void){}