#ifndef VM_HPP
#define VM_HPP

#include "Parser.hpp"
#include "IOperand.hpp"
#include "Operand.hpp"
#include "CustomStack.hpp"
#include <iostream>
#include <string>
#include <typeinfo>
#include <fstream>
#include <utility>
#include <list>
#include <functional>

class VM {
public: 
	VM(bool printmode);
	virtual ~VM(void);
	void	run(Parser::tk_list *);
private :
	eOperandType token_to_type(eToken etk) const;
	eToken type_to_token(eOperandType type) const;
	IOperand const *extract_top(void);
	void delete_stack(void);
	void my_push(Parser::tk_list *tk);
	void my_pop(Parser::tk_list *tk);
	void my_dump(Parser::tk_list *tk);

	void generic_arithmetic(std::function<IOperand const *(const IOperand&, IOperand const &)> ptr,
	std::string instrname, Parser::tk_list *lst); 
	
	void my_assert(Parser::tk_list *tk);
	void my_print(Parser::tk_list *tk);
	void my_exit(Parser::tk_list *tk);
	void print_IOperand(IOperand const *p);
	VM(void);
	VM(VM const & src);
	VM & operator=(VM const & rhs);
	std::function<void(VM &, Parser::tk_list *)> _ftab[INSTR_NUM];

/*
** attributes ***************************************************************************
*****************************************************************************************
*/	
	CustomStack	_stack;
	bool		_printmode;
	const std::string _typename[Double + 1] = {"int8", "int16", "int32", "int64", "float", "double"} ;
};

#endif