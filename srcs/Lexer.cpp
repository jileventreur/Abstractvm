#include "Lexer.hpp"

/*
**	call each automaton for each character until all automatons reject it
**	last automaton accepting the string determines its key
**	default token key is error
*/

token Lexer::get_next_token(void)
{
	size_t			i;
	size_t			last_accept;
	bool			rejected;
	std::string		value = std::string();
	eToken			key;
	eToken			last_hungry;
	Lexer::eState	ret;
	Lexer::charopt	c;

	key = t_error;
	last_hungry = t_error;
	last_accept = 0;
	if (!(c = this->get_next_char()))
		return (token(t_eof, value));
	do
	{
		rejected = true;
		i = 0;
		value += *c;
		++last_accept;
		while (i < t_error)
		{
			if ((ret = (this->_ftab[i])(*this, *c)) != t_reject)
			{
				last_hungry = static_cast<eToken>(i);
				rejected = false;
			}
			if (ret == t_accept)
			{
				if (static_cast<eToken>(i) == t_endl)
					++this->_y;
				key = static_cast<eToken>(i);
				last_accept = 0;
			}
			++i;
		}
	}
	while (!rejected && (c = get_next_char()) && key != t_eof);
	if (key == t_error)
		this->error_suggestion(token(key, value), last_hungry);
	if (c && key != t_eof)
	{
		this->_index -= last_accept;
		this->_x -= last_accept;
		value.pop_back();
	}
	for (i = 0; i < t_error; ++i) /* reset automatons with none arg for next token */
		(this->_ftab[i])(*this, boost::none);
	return (token(key, value));
}

/*
** line to char handling ********************************************************************************************************
*********************************************************************************************************************************
*/

void Lexer::skip_line(void) /* if an error occur jump to the next line or to ';;' token*/
{
	size_t i;

	if (this->_cin && (i = this->_line.find(";;")) != std::string::npos)
	{
		this->_x = i;
		this->_index = i;
	}
	else
	{
		++this->_y;
		this->_index = this->_line.size();
		this->_x = this->_index;
	}
}

Lexer::charopt Lexer::get_next_char(void) /* return current char and  update cursor (_index, _x and _line attribute) */
{
	if (this->_line.size() == this->_index)
	{
		if (!this->_is.good())
			return (boost::none);
		this->_index = 0;
		this->_x = 0;
		std::getline(this->_is, this->_line);
		this->_line.append("\n");
	}
	++this->_x;
	return (this->_line[this->_index++]);
}

/*
** automatons functions *********************************************************************************************************
*********************************************************************************************************************************
*/

Lexer::eState Lexer::f_white(Lexer::charopt copt)
{
	static Lexer::eState state = t_hungry;
	char c;

	if (!copt)
		return (state = t_hungry);
	c = copt.get();
	if (state == t_reject)
		return (state);
	else if (std::isspace(c) && c != '\n')
		state = t_accept;
	else
		state = t_reject;
	return (state);
}

Lexer::eState Lexer::f_ivalue(Lexer::charopt copt)
{
	static std::string str = "";
	static Lexer::eState state = t_hungry;
	char c;

	if (!copt)
	{
		str = std::string();
		return (state = t_hungry);
	}
	c = copt.get();
	str += c;
	if (str.size() == 1 && c == '-')
		return (state);
	if (state != t_reject && std::isdigit(str[str.size() - 1]))
	{
		return (state = t_accept);
	}
	else
		return (state = t_reject);
}

Lexer::eState Lexer::f_fvalue(Lexer::charopt copt)
{
	static std::string str = "";
	static Lexer::eState state = t_hungry;
	static std::regex pattern ("(-?[[:digit:]]+\\.[[:digit:]]+)");
	static std::regex hungry_pattern ("(-?)|(-?[[:digit:]]+\\.[[:digit:]]+)|(-?[[:digit:]]+\\.)|(-?[[:digit:]]+)");
	char c;

	if (!copt)
	{
		str = std::string();
		return (state = t_hungry);
	}
	c = copt.get();
	str += c;
	if (state == t_reject)
		return (state);
	if (std::regex_match(str, pattern))
		return (state = t_accept);
	if (std::regex_match(str, hungry_pattern))
		return (state = t_hungry);
	return (state = t_reject);
}

Lexer::eState Lexer::f_comment(Lexer::charopt copt) /* comment token interpretation is _cin dependant */
{
	static Lexer::eState state = t_hungry;
	static std::string str = "";
	char c;

	if (!copt)
	{
		str = std::string();
		return (state = t_hungry);
	}
	c = copt.get();
	str += c;
	if (str[0] != ';')
		return (state = t_reject);
	else if (c == '\n')
		return (state = t_reject);
	return (state = t_accept);
}

Lexer::eState Lexer::f_eof(Lexer::charopt copt) /* eof token interpretation is _cin dependant */
{
	static Lexer::eState state = t_hungry;
	static size_t i = 0;
	char c ;

	if (!copt)
	{
		i = 0;
		return (state = t_hungry);
	}
	if (!this->_cin)
		return (state = t_reject);
	if (state == t_accept)
		return (state = t_reject);
	c = copt.get();
	if (i == 0 && c == ';')
	{
		++i;
		return (state);
	}
	if (i == 1 && c == ';')
	{
		return (state = t_accept);
	}
	return (state = t_reject);
}

template<eToken tk> Lexer::eState Lexer::generic_automaton(Lexer::charopt copt, std::string str)
{
	static size_t i = 0;
	static Lexer::eState state = t_hungry;
	static std::string strname = str;
	char c ;
	
	if (!copt)
	{
		i = 0;
		return (state = t_hungry);
	}
	c = copt.get();
	if (state == t_reject || state == t_accept)
	{
		state = t_reject;
		return (state);
	}
	if (strname[i++] != c)
		state = t_reject;
	if (strname.size() == i && state == t_hungry)
		state = t_accept;
	return (state);
}

/*
** main constructor *************************************************************************************************************
*********************************************************************************************************************************
*/

/*
** bind generic_automaton into singular functions for each static token
** template with non type value on generic_automaton is used to generate 
** singular function for each binding and resolve static variable issues
*/

Lexer::Lexer(std::istream &is, std::string filename, bool cin) :
_is (is), _filename(filename), _line(std::string()), _x(0), _y(0), _index(0), _cin(cin)
{
	this->_ftab[t_push] = std::bind(&Lexer::generic_automaton<t_push>, this, std::placeholders::_2, std::string("push ")); 
	this->_ftab[t_pop] = std::bind(&Lexer::generic_automaton<t_pop>, this, std::placeholders::_2, std::string("pop"));
	this->_ftab[t_dump] = std::bind(&Lexer::generic_automaton<t_dump>, this, std::placeholders::_2, std::string("dump"));
	this->_ftab[t_assert] = std::bind(&Lexer::generic_automaton<t_assert>, this, std::placeholders::_2, std::string("assert"));
	this->_ftab[t_add] = std::bind(&Lexer::generic_automaton<t_add>, this, std::placeholders::_2, std::string("add"));
	this->_ftab[t_sub] = std::bind(&Lexer::generic_automaton<t_sub>, this, std::placeholders::_2, std::string("sub"));
	this->_ftab[t_mul] = std::bind(&Lexer::generic_automaton<t_mul>, this, std::placeholders::_2, std::string("mul"));
	this->_ftab[t_div] = std::bind(&Lexer::generic_automaton<t_div>, this, std::placeholders::_2, std::string("div"));
	this->_ftab[t_mod] = std::bind(&Lexer::generic_automaton<t_mod>, this, std::placeholders::_2, std::string("mod"));
	this->_ftab[t_pow] = std::bind(&Lexer::generic_automaton<t_pow>, this, std::placeholders::_2, std::string("pow"));
	this->_ftab[t_print] = std::bind(&Lexer::generic_automaton<t_print>, this, std::placeholders::_2, std::string("print"));
	this->_ftab[t_exit] = std::bind(&Lexer::generic_automaton<t_exit>, this, std::placeholders::_2, std::string("exit"));
	this->_ftab[t_int8] = std::bind(&Lexer::generic_automaton<t_int8>, this, std::placeholders::_2, std::string("int8("));
	this->_ftab[t_int16] = std::bind(&Lexer::generic_automaton<t_int16>, this, std::placeholders::_2, std::string("int16("));
	this->_ftab[t_int32] = std::bind(&Lexer::generic_automaton<t_int32>, this, std::placeholders::_2, std::string("int32("));
	this->_ftab[t_int64] = std::bind(&Lexer::generic_automaton<t_int64>, this, std::placeholders::_2, std::string("int64("));
	this->_ftab[t_float] = std::bind(&Lexer::generic_automaton<t_float>, this, std::placeholders::_2, std::string("float("));
	this->_ftab[t_double] = std::bind(&Lexer::generic_automaton<t_double>, this, std::placeholders::_2, std::string("double("));
	this->_ftab[t_closure] = std::bind(&Lexer::generic_automaton<t_closure>, this, std::placeholders::_2, std::string(")"));
	this->_ftab[t_endl] = std::bind(&Lexer::generic_automaton<t_endl>, this, std::placeholders::_2, std::string("\n"));
	this->_ftab[t_white] = &Lexer::f_white;
	this->_ftab[t_ivalue] = &Lexer::f_ivalue;
	this->_ftab[t_fvalue] = &Lexer::f_fvalue;
	this->_ftab[t_comment] = &Lexer::f_comment;
	this->_ftab[t_eof] = &Lexer::f_eof;
}

/*
** error functions **************************************************************************************************************
*********************************************************************************************************************************
*/

void Lexer::error_suggestion(token tk, eToken last_hungry) const
{
	dprintf(2, "\033[0;1m%s:%lu:%lu: Lexical \033[31merror\033[0;1m : ", 
	this->_filename.data(), this->_y , this->_x);
	if (last_hungry >= t_push && last_hungry <= t_double && (std::get<std::string>(tk)).size() > 1)
	{
		dprintf(2, "unknown symbol \'%s\'; did you mean \'%s\'?\n",
		(std::get<std::string>(tk)).data(), Lexer::to_String(last_hungry).data());
	}
	else if (last_hungry == t_ivalue)
	{
		dprintf(2, "invalid format for integer value (%s) (correct format is :  [-]?[0..9]+)\n",
		(std::get<std::string>(tk)).data());
	}
	else if (last_hungry == t_fvalue)
	{
		dprintf(2, "invalid format for float value (%s) (correct format is : [-]?[0..9]+.[0..9]+)\n",
		(std::get<std::string>(tk)).data());
	}
	else
		dprintf(2, "\'%s\' invalid token\n", std::get<std::string>(tk).data());
	dprintf(2, "\033[0m");
	dprintf(2, "\t\t\t%s", this->_line.data());
	dprintf(2, "\t\t\t\033[32;1m");
	for (size_t i = 0; i < this->_x - 1; ++i)
		dprintf(2, "~");
	dprintf(2, "^");
	for (size_t i = this->_x - 1; i <= this->_line.size() && !std::isspace(this->_line[i + 1]) && this->_line[i + 1] != ';'; ++i)
		dprintf(2, "~");
	dprintf(2, "\033[0m\n");
}

/*
** tools functions **************************************************************************************************************
*********************************************************************************************************************************
*/

std::string const Lexer::to_String(eToken tk)
{
	static const std::string nametab[TOKEN_SIZE] = {
		std::string("push"),
		std::string("pop"),
		std::string("dump"),
		std::string("assert"),
		std::string("add"),
		std::string("sub"),
		std::string("mul"),
		std::string("div"),
		std::string("mod"),
		std::string("pow"),
		std::string("print"),
		std::string("exit"),
		std::string("int8"),
		std::string("int16"),
		std::string("int32"),
		std::string("int64"),
		std::string("float"),
		std::string("double"),
		std::string("closure"),
		std::string("endl"),
		std::string("white"),
		std::string("ivalue"),
		std::string("fvalue"),
		std::string("comment"),
		std::string("eof"),
		std::string("ERROR"),
	};
	return (nametab[static_cast<int>(tk)]);
}

void Lexer::print_state(Lexer::eState s)
{
	if (s == t_accept)
		std::cout << "t_accept" << std::endl;
	else if (s == t_hungry)
		std::cout << "t_hungry" << std::endl;
	else
		std::cout << "t_reject" << std::endl;
}

void Lexer::print_token(token const &p)
{
	std::cout << "{key : " <<  Lexer::to_String(std::get<eToken>(p));
	if (std::get<eToken>(p) == t_endl)
		std::cout << ", value : \"\\n\"}" << std::endl;
	else
		std::cout << ", value : \"" << std::get<std::string>(p) << "\"}" << std::endl;
}

/*
** Geter functions **************************************************************************************************************
*********************************************************************************************************************************
*/

size_t Lexer::get_x(void) {return (this->_x);}
size_t Lexer::get_y(void) {return (this->_y);}
std::string const &Lexer::get_line(void) {return (this->_line);}

/*
** coplien form functions *******************************************************************************************************
*********************************************************************************************************************************
*/

Lexer::Lexer(Lexer const &src) : _is(std::cin), _cin(false) {*this = src;}
Lexer::Lexer(void): _is(std::cin), _cin(false) {}
Lexer	&Lexer::operator=(Lexer const &rhs) {if (&rhs == this){}return *this;}
Lexer::~Lexer(void) {}