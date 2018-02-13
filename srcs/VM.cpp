#include "VM.hpp"

/*
** List has been strim and now contain only instr token followed by type and value tokens if last instruction required it.
** Example : token(push) -> token(int32) -> token(42) -> token(dump) -> token(pop) -> token(exit) -> token(eof)
** run VM until no instr eof or exit. instr are access through _ftab indexed by token value
*/

void	VM::print_IOperand(IOperand const *p)
{
	std::cout << this->_typename[p->getType()] << "(" << p->toString() << ")";
}

void	VM::run(Parser::tk_list *lst)
{
	Parser::token elem;

	while (!lst->empty() && std::get<eToken>(lst->front()) != t_eof)
	{
		elem = lst->front();
		lst->pop_front();
		try
		{
			(this->_ftab[std::get<eToken>(elem)])(*this, lst);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			delete_stack();
			delete lst;
			exit(0);
		}
		// Lexer::print_token(elem);
	}
	delete lst;
	std::cerr << "\033[0;1mExecution \033[31merror\033[0;1m : program ended without exit instruction\033[0m" << std::endl; 
}

/*
** fonction pop stack twice to get type and value argument and create it with factory method
*/
void VM::my_push(Parser::tk_list *lst)
{
	static const Factory f = Factory();
	Parser::token type;
	Parser::token value;

	type = lst->front();
	lst->pop_front();
	value = lst->front();
	lst->pop_front();
	this->_stack.push(f.createOperand(this->token_to_type(std::get<eToken>(type)), std::get<std::string>(value)));
	if (this->_printmode)
		std::cout << "\033[32mpush " << std::get<std::string>(type) << std::get<std::string>(value) << ")\033[1;0m" << std::endl;
}

eOperandType VM::token_to_type(eToken etk) const
{
	return (static_cast<eOperandType>(etk - INSTR_NUM));
}

void VM::my_pop(Parser::tk_list *lst)
{
	IOperand const *p;
	(void)*lst;
	p = this->extract_top();
	if (this->_printmode)
	{
		std::cout << "\033[32mpop (";
		this->print_IOperand(p);
		std::cout << " is poped)\033[0m" << std::endl;
	}
	delete p;
}

IOperand const *VM::extract_top(void)
{
	IOperand const *p;
	
	if (this->_stack.empty())
		throw MyException::Empty_Stack();
	p = this->_stack.top();
	this->_stack.pop();
	return (p);
}

void VM::my_dump(Parser::tk_list *lst)
{
	(void)*lst;
	std::cout << "\033[33m------- DUMP -------" << std::endl;
	for (CustomStack::iterator it = this->_stack.begin(); it != this->_stack.end(); ++it) 
	{
		std::cout << this->_typename[(*it)->getType()] << "(" << (*it)->toString() << ")" << std::endl;
	}
	std::cout << "--------------------\033[0m" << std::endl;
}

/* 
** same behavior as push fonction on list.
** assert check only value equality without consider type
*/
void VM::my_assert(Parser::tk_list *lst)
{
	IOperand const *arg1;
	IOperand const *arg2;
	static const Factory f = Factory();
	Parser::token type;
	Parser::token value;

	type = lst->front();
	lst->pop_front();
	value = lst->front();
	lst->pop_front();
	arg1 = f.createOperand(this->token_to_type(std::get<eToken>(type)), std::get<std::string>(value));
	arg2 = this->extract_top();
	if (std::stod(arg1->toString()) != std::stod(arg2->toString()))
	{
		std::cerr << "Error : assert (" << arg1->toString() << " != " <<  this->_stack.top()->toString() << ")" << std::endl;
		delete_stack();
		delete arg1;
		delete lst;
		exit(0);
	}
	else if (this->_printmode)
		std::cerr << "\033[32massert (" << arg1->toString() << " == " <<  arg2->toString() << ")\033[1;0m" << std::endl;
	delete arg1;
}

void VM::delete_stack(void)
{
	while (!this->_stack.empty())
		delete VM::extract_top();
}

void VM::my_print(Parser::tk_list *lst) /* dont check if char is printable or not */
{
	static const Factory f = Factory();
	IOperand const *arg;

	arg = this->extract_top();
	if (arg->getType() != Int8)
	{
		std::cerr << "\033[0;1mExecution \033[31merror\033[0;1m : print : top of the stack is not a int8\033[0m" << std::endl;
		delete_stack();
		delete arg;
		delete lst;
		exit(0);
	}
	else if (this->_printmode)
	{
		std::cout << "\033[32mprint (on ";
		this->print_IOperand(arg);
		std::cout << ") : \033[1;0m";
	}
	this->_stack.push(arg);
	std::cout << static_cast<unsigned char>(std::stoi(arg->toString())) << std::endl;
	(void)*lst;
}

void VM::my_exit(Parser::tk_list *lst)
{
	delete_stack();
	delete lst;
	if (this->_printmode)
		std::cout << "\033[32mexit\033[1;0m" << std::endl;
	exit(0);
	(void)*lst;
}

void VM::generic_arithmetic(std::function<IOperand const *(const IOperand&, IOperand const &)> ptr, std::string instrname,
Parser::tk_list *lst)
{
	static std::string name = instrname; 
	(void)*lst;
	IOperand const *arg1;
	IOperand const *arg2;
	IOperand const *res;

	arg1 = this->extract_top();
	arg2 = this->extract_top();
	res = ptr(*arg1 , *arg2);
	if (this->_printmode)
	{
		std::cout << "\033[32m";
		this->print_IOperand(arg2);
		std::cout << " " << instrname << " ";
		this->print_IOperand(arg1);
		std::cout << " = ";
		this->print_IOperand(res);
		std::cout << "\033[1;0m" << std::endl;
	}
	this->_stack.push(res);
	delete arg1;
	delete arg2;
}

VM::VM(bool printmode): _stack (CustomStack()), _printmode(printmode) {
	this->_ftab[t_push] = &VM::my_push;
	this->_ftab[t_pop] = &VM::my_pop;
	this->_ftab[t_dump] = &VM::my_dump;
	this->_ftab[t_assert] = &VM::my_assert;
	
	/*
	** bind member function generic_arithmetic(function_operator, list) into anonym_function(list) with an assigned function_operator
	*/
	this->_ftab[t_add] = std::bind(&VM::generic_arithmetic, this, &IOperand::operator+, std::string("add"), std::placeholders::_2);
	this->_ftab[t_sub] = std::bind(&VM::generic_arithmetic, this, &IOperand::operator-, std::string("sub"), std::placeholders::_2);
	this->_ftab[t_mul] = std::bind(&VM::generic_arithmetic, this, &IOperand::operator*, std::string("mul"), std::placeholders::_2);
	this->_ftab[t_div] = std::bind(&VM::generic_arithmetic, this, &IOperand::operator/, std::string("div"), std::placeholders::_2);
	this->_ftab[t_mod] = std::bind(&VM::generic_arithmetic, this, &IOperand::operator%, std::string("mod"), std::placeholders::_2);
	this->_ftab[t_pow] = std::bind(&VM::generic_arithmetic, this, &IOperand::operator^, std::string("pow"), std::placeholders::_2);
	
	this->_ftab[t_print] = &VM::my_print;
	this->_ftab[t_exit] = &VM::my_exit;
}

eToken VM::type_to_token(eOperandType type) const // to rm ?
{
	return (static_cast<eToken>(type + INSTR_NUM));
}

VM::VM(void) : _stack (CustomStack()), _printmode (false){}
VM::VM(VM const &src) {*this = src;}
VM	&VM::operator=(VM const &rhs){if (&rhs == this){}return *this;}
VM::~VM(void){}