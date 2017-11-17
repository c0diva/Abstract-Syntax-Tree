///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.1                                                           //
// Language   : C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Source     : Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com								 //
// Author     : Aditi Gaikwad, Syracuse University,					 //
//				agaikwad@syr.edu									 //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "ITokCollection.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}
void SemiExp::setSemiExp(Toker* pToker) { _pToker = pToker; }

bool SemiExp::get(bool clear)
{
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	_tokens.clear();
	while (true)
	{	std::string token = _pToker->getTok();
		if (token == "")
			break;
		_tokens.push_back(token);
		if (token == ";")
			return true;
		else if (token == "{" || token == "}" ) //Termination condition for open curly brace
			return true;
		else if (token == "#") //Termination condition for '#'
		{	
			return PoundSign(token);
		}
		else
			if (token == "public" || token == "protected" || token == "private") //Termination condition for access specifiers 
			{																	
				return AccessSpecifier(token);
			}
			else
				if (token == "for")   //Termination condition for for loop
				{	
					return	ForLoop(token);
				}
	}
	return false;
}
bool SemiExp::PoundSign(std::string token)
{
	while (true)
	{	
		if (token == "\n" || token == "")
			break;
		token = _pToker->getTok();
		_tokens.push_back(token);		
	}
	return true;
}
bool SemiExp::AccessSpecifier(std::string token)
{
	while (true)
	{
		token = _pToker->getTok();
		_tokens.push_back(token);
		if (token == ":" || token == "" || token == "{")
			break;
	}
	return true;
}
bool SemiExp::ForLoop(std::string token)
{
	while (token != ";")
	{
		token = _pToker->getTok();
		_tokens.push_back(token);
	}
	token = _pToker->getTok();
	_tokens.push_back(token);
	while (token != ";")
	{
		token = _pToker->getTok();
		_tokens.push_back(token);
	}
	token = _pToker->getTok();
	_tokens.push_back(token);

	while (token != "{")
	{
		token = _pToker->getTok();
		_tokens.push_back(token);
		if (token == "{" || token == ";")
				break;
	}
	return true;
}
size_t SemiExp::length()
{
	return _tokens.size();
}
void SemiExp::push_back(const std::string& tok) //tokens are pushed back into the vector
{
	_tokens.push_back(tok);
	return;
}
size_t SemiExp::find(const std::string& token) //To find the token in the vector
{
	/*if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	*/for (size_t i = 0; i < _tokens.size(); i++)
	{
		if (token == _tokens[i])
		{
			return i;
		}
	}
	return -1;
}
void SemiExp::printVector()
{
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		std::cout << " " << _tokens[i];
	}
}
bool SemiExp::remove(const std::string& tok) //removex token by comparing its string
{
	
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		if (tok == _tokens[i])
		{
			_tokens.erase(_tokens.begin() + i);
			return true;
		}
	}
	return false;
}
bool SemiExp::remove(size_t i) //removes token by its position in the vector
{
	if (i < _tokens.size())
	{
		_tokens.erase(_tokens.begin() + i);
		return true;
	}
	else
	{
		std::cout << "\n Invalid token position";
		return false;
	}
}

void SemiExp::toLower()  //changes all the strings of the vector to lowercase
{
	std::locale loc;
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		size_t j = 0;
		std::string s = "";
		while (j < (_tokens[i].length()))
		{
			char c = _tokens[i].at(j);
			if (c <= 'a' && c >= 'z')
			{
				s += c;
				j++;
			}
			else
			{
				s += std::tolower(c, loc);
				j++;
			}
		}
		_tokens[i] = s;
	}
	return;
}

void SemiExp::trimFront() //trims the blank spaces in front of the string
{
	size_t j = 0;
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		for (; j < _tokens[i].length(); j++)
		{
			if (_tokens[i].at(0) != '\n')
				break;
			else
				_tokens[i].erase(0, 1);
			j--;
		}
	}
	
	
	
	return;
}

void SemiExp::clear()
{
	_tokens.clear();
	return;
}

std::string SemiExp::show(bool showNewLines)
{
	std::string s = "";
	if (showNewLines)
	{
		s += "\n";
	}
	for (auto token : _tokens)
		if (token != "\n")
		{
			s += token;
			s += " ";
		}
	if (showNewLines)
	{
		s += "\n";
	}
	return s;
}
size_t SemiExp::currentLineCount()
{
	if (_pToker == nullptr)
		return 0;
	/*
	*  Tokenizer has collected first non-state char when exiting eatChars()
	*  so we need to subtract 1 from the Toker's line count.
	*/
	return _pToker->currentLineCount() - 1;
}
std::string SemiExp::operator[](size_t n) const
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}
std::string& SemiExp::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}
bool SemiExp::isComment(const std::string& tok)
{
	return _pToker->isComment(tok);
}

#ifdef SEMIEXP
int main()
{
	Toker toker;
	std::string fileSpec = "../Parser/ActionsAndRules.h";   //"../Parser/test.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	// std::cout << "\n xcvbnmm,,,,";
	SemiExp semi(&toker);
	semi.printVector();
	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --";
		std::cout << semi.show(true);
	}
	/*
	   May have collected tokens, but reached end of stream
	   before finding SemiExp terminator.
	 */
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		std::cout << semi.show(true);
		std::cout << "\n\n";
	}
	
	return 0;
}
#endif