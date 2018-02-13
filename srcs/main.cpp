#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <typeinfo>
#include "Operand.hpp"
#include "Factory.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "MyException.hpp"
#include "VM.hpp"
#include <fstream>

/*
** Parse file or cin and execute vm if no error is found
** third arg could be use to print vm execution 
*/

int main (int argc, char **argv) 
{
	std::unique_ptr<VM>	vm;
	Parser::tk_list	*lst;

	if (argc > 3)
	{
		std::cerr << "usage : ./avm [filename]" << std::endl;
		return (1);
	}
	if (argc >= 2) 
	{
		std::ifstream ifs(argv[1], std::ifstream::in);
		if (!ifs.good())
		{
			std::cerr << "Error : enable to read file" << std::endl;
			return (1);
		}
		lst = Parser::Parser(ifs, argv[1], false).parse();
		ifs.close();
	}
	else
		lst = Parser::Parser(std::cin, "cin", true).parse();
	if (argc == 3 )
	{
		if (std::string("--print").compare(argv[2]) != 0)
		{
			std::cerr << "Error : usage ./avm [file] (--print)?" << std::endl;
			exit(0);
		}
		vm = std::unique_ptr<VM>(new VM(true));
	}
	else
		vm = std::unique_ptr<VM>(new VM(false));
	vm->run(lst);
	return (0);
}