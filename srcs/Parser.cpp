#include "Parser.hpp"

/*
** get token from lexer, check lexical validity of file, strim token to obtain executable list
*/

Parser::tk_list *Parser::parse(void)
{
	bool 	valid_input;
	token	cur;
	token	next;
	tk_list	*lst = new tk_list();

	valid_input = true;
	cur = this->get_next_token(valid_input);
	while (std::get<eToken>(cur) != t_eof)
	{
		next = this->get_next_token(valid_input);
		if (!this->next_is_valid(std::get<eToken>(cur), std::get<eToken>(next)))
		{
			this->syntax_error(&cur, &next);
			valid_input = false;
			this->Lexer::skip_line();
			next.first = t_error;
		}
		lst->push_back(cur);
		cur = next;
	}
	lst->push_back(cur); /* push eof */
	lst->remove_if([](token p) { return \
	(std::get<eToken>(p) == t_comment || std::get<eToken>(p) == t_endl || std::get<eToken>(p) == t_closure); });
	if (!valid_input)
		exit(0);
	return (lst);
}

token Parser::get_next_token(bool &valid_input) /* strim whitespaces */
{
	token	p;

	while (42)
	{
		p = this->Lexer::get_next_token();
		if (std::get<eToken>(p) == t_error)
		{
			this->Lexer::skip_line();
			valid_input = false;
		}
		if (std::get<eToken>(p) != t_white)
			return (p);
	}
}

bool Parser::next_is_valid(eToken cur, eToken next) /* check lexical validity of new token */
{
	if (cur == t_error || next == t_error) /* consider syntax error as always true to parse all file before quit */
		return (true);
	for (int i = 0;;++i)
	{
		if (Parser::valid_next[cur][i] == next)
			return (true);
		else if (Parser::valid_next[cur][i] == t_error)
			break;
	}
	return (false);
}

/*
** main constructor *************************************************************************************************************
*********************************************************************************************************************************
*/

Parser::Parser(std::istream &is, std::string filename, bool cin) : Lexer(is, filename, cin), _filename(filename){}

/*
** error functions **************************************************************************************************************
*********************************************************************************************************************************
*/

void Parser::syntax_error(token *cur, token *next)
{
	dprintf(2, "\033[0;1m%s:%lu:%lu: ", 
	this->_filename.data(), this->Lexer::get_y() , this->Lexer::get_x());
	dprintf(2, "Syntax \033[31mError\033[0;1m : \'%s\' token can't be followed by \'%s\'\033[0m\n",
	Lexer::to_String(std::get<eToken>(*cur)).data(), Lexer::to_String(std::get<eToken>(*next)).data());
	dprintf(2, "\t\t\t%s", this->Lexer::get_line().data());
	dprintf(2, "\t\t\t\033[32;1m");
	for (size_t i = 0; i < this->get_x(); ++i)
		dprintf(2, "~");
	dprintf(2, "^");
	for (size_t i = this->get_x() - 1;
	i <= this->Lexer::get_line().size() && !std::isspace(this->Lexer::get_line()[i + 1]) && this->get_line()[i + 1] != ';'; ++i)
		dprintf(2, "~");
	dprintf(2, "\033[0m\n");
}

/*
** static const attribute assignation *******************************************************************************************
*********************************************************************************************************************************
*/

/* define valid token linking after white strim. t_error is used as delimiter */
const eToken constexpr Parser::valid_next[TOKEN_SIZE][TOKEN_SIZE] = {
{t_int8, t_int16, t_int32, t_int64, t_float, t_double, t_error}, /* push */
{t_comment, t_endl, t_eof, t_error}, /* pop */
{t_comment, t_endl, t_eof, t_error}, /* dump */
{t_int8, t_int16, t_int32, t_int64, t_float, t_double, t_error}, /* assert */
{t_comment, t_endl, t_eof, t_error}, /* add */
{t_comment, t_endl, t_eof, t_error}, /* sub */
{t_comment, t_endl,  t_eof,t_error}, /* mul */
{t_comment, t_endl, t_eof, t_error}, /* div */
{t_comment, t_endl, t_eof, t_error}, /* mod */
{t_comment, t_endl, t_eof, t_error}, /* pow */
{t_comment, t_endl, t_eof, t_error}, /* print */
{t_comment, t_endl, t_eof, t_error}, /* exit */
{t_ivalue, t_error}, /* int8 */
{t_ivalue, t_error}, /* int16 */
{t_ivalue, t_error}, /* int32 */
{t_ivalue, t_error}, /* int64 */
{t_fvalue, t_error}, /* float */
{t_fvalue, t_error}, /* double */
{t_comment, t_endl, t_eof, t_error}, /* closure */
{t_push, t_pop, t_dump, t_assert, t_add, t_sub, t_mul, t_div, t_mod, t_pow, t_print, t_exit, t_endl, t_comment, t_eof, t_error},  /* endl */
{t_error},  /* white (strimed before syntax analysis) */
{t_closure, t_error}, /* i_ivalue */
{t_closure, t_error}, /* fvalue */
{t_endl, t_eof, t_error}, /* comment */
{t_error},  /* eof */
{} /* error */
};

/*
** coplien form functions *******************************************************************************************************
*********************************************************************************************************************************
*/

Parser::Parser(Parser const &src) {*this = src;}
Parser::Parser(void){}
Parser	&Parser::operator=(Parser const &rhs){if (&rhs == this){}return *this;}
Parser::~Parser(void){}

/*
** debug functions **************************************************************************************************************
*********************************************************************************************************************************
*/

void Parser::print_tk_list(tk_list const &lst)
{
	std::for_each(lst.begin(), lst.end(), [this](token const &p){this->Lexer::print_token(p);});
}