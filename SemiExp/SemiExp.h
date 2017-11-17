#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
/////////////////////////////////////////////////////////////////////
// SemiExp.h - collects tokens for analysis		                   //
// ver 1.0                                                         //
// Language   :C++, Visual Studio 2015                             //
// Application:Lexical Scanner, CSE687 - Object Oriented Design    //
// Source     :Jim Fawcett, Syracuse University, CST 4-187         //
//              jfawcett@twcny.rr.com							   //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //		
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a 
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
* Also other terminators include public:, private: and protected:
* It also considers special conditions such as for loop i.e. it will
* consider whole (;;) statement as one single token.
*
* Build Process:
* --------------
* Required Files: 
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp, ITokCollection.h
* 
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0: Feb 8th, 2016
* -implemented with scanner interface, ITokCollection.
*
*/
#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "ITokCollection.h"

namespace Scanner
{
  using Token = std::string;
  class SemiExp : public ITokCollection
  {
  public:
SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
    SemiExp& operator=(const SemiExp&) = delete;
    bool get(bool clear = true);
	bool isComment(const std::string& tok) ;

	std::string& operator[](size_t n);
	std::string operator[](size_t n) const;
	size_t find(const std::string& token);
	void push_back(const std::string& tok);
	bool remove(const std::string& tok);
	bool remove(size_t i);
	std::string show(bool showNewLines = false);
	void toLower();
	void trimFront();
	void clear();
    size_t length();
	void setSemiExp(Toker* pToker);
	void printVector();
	bool ForLoop(std::string token);
	bool AccessSpecifier(std::string token);
	bool PoundSign(std::string token);
	size_t currentLineCount();
  private:
	 std::vector<Token> _tokens;
    Toker* _pToker;
//	bool hasFor = false;
  };
}
#endif
