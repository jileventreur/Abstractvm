#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "IOperand.hpp"
#include "Operand.hpp"
#include <iostream>
#include <string>
#include <typeinfo>
#include <fstream>
#include <utility>
#include <list>
#include <stdio.h>

class Parser : public Lexer {
public:
	using token = token;
	typedef std::list<token> tk_list;
	Parser(std::istream &source, std::string const filename, bool cin);
	Parser::tk_list *parse(void);
	void print_tk_list(tk_list const &lst); /* debug only */
	virtual ~Parser(void);

private :
	token get_next_token(bool &valid_input);
	void syntax_error(token *cur, token *next);
	bool next_is_valid(eToken cur, eToken next);
	Parser(void);
	Parser(Parser const & src);
	Parser & operator=(Parser const & rhs);
	static const eToken valid_next[TOKEN_SIZE][TOKEN_SIZE];

/*
** attributes ***************************************************************************
*****************************************************************************************
*/	
	std::string const _filename;
};

#endif