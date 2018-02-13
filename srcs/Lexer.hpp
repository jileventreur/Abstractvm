#ifndef LEXER_HPP
#define LEXER_HPP

/*
** 	token order in enum and fautomaton have to be identical to use enum as an index to fautomoton.
** 	token are ordered firstly by instruction to use the last one (exit) as instruction number
**	error must always be the last elem to define size of the function array
** 	eof must be penultimate to respect token recognition priority
*/

enum eToken {
	t_push, 
	t_pop,
	t_dump,
	t_assert,
	t_add,
	t_sub,
	t_mul,
	t_div,
	t_mod,
	t_pow,
	t_print,
	t_exit,
	t_int8,
	t_int16,
	t_int32,
	t_int64,
	t_float,
	t_double,
	t_closure,
	t_endl,
	t_white,
	t_ivalue,
	t_fvalue, 
	t_comment,
	t_eof,
	t_error
};

#define TOKEN_SIZE (t_error + 1)
#define INSTR_NUM (t_exit + 1)

#include <iostream>
#include <string>
#include <typeinfo>
#include <fstream>
#include <regex>
#include <functional>
#include <boost/optional.hpp>
#include <utility>
#include "IOperand.hpp"
#include <stdio.h>

typedef std::pair<eToken, std::string> token;

class Lexer {
public:
	Lexer(std::istream &is, std::string filename, bool cin);
	virtual ~Lexer(void);
	token get_next_token(void);
	void print_token(token const &p); /* debug only */
	static std::string const to_String(eToken);

protected:
	void skip_line(void);
	size_t get_x(void);
	size_t get_y(void);
	std::string const &get_line(void);
	Lexer(void);

private:
	typedef boost::optional<char> charopt;
	void go_next_line(void);
	enum eState {t_reject, t_hungry, t_accept};
	template <eToken tk> eState generic_automaton(charopt copt, std::string str);
	eState f_white(charopt c);
	eState f_ivalue(charopt c);
	eState f_fvalue(charopt c);
	eState f_comment(charopt c);
	eState f_eof(charopt c);
	void error_suggestion(token tk, eToken last_hungry) const;
	charopt get_next_char(void);
	void print_state(Lexer::eState s); /* debug only */
	Lexer(Lexer const & src);
	Lexer & operator=(Lexer const & rhs);

/*
** attributes ***************************************************************************
*****************************************************************************************
*/	
	std::function<eState(Lexer &, charopt c)> _ftab[TOKEN_SIZE];
	std::istream &_is;
	std::string const _filename;
	std::string _line;
	size_t	_x;
	size_t	_y;
	size_t	_index;
	bool _cin;
};

#endif
