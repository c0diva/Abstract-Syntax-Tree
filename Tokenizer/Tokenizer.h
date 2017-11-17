#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 1.0                                                           //
// Language   : C++, Visual Studio 2015                              //
// Application: Lexical Scanner, CSE687 - Object Oriented Design     //
// Source     : Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com								 //
// Author     : Aditi Gaikwad, Syracuse University					 //
//              agaikwad@syr.edu									 //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design. It also return quoted 
 * strings as single token. It must handle \" and \' correctly. It Returns 
 * [, ], {, }, (, ), <, >, :, ; as single character tokens and  * - Returns 
 * <<, >>, +=, -=, *=, /=, :: as two character tokens.
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Tokenizer.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : Feb 08, 2016
 * -Added new toker constructor
 */ 
 
#include <iosfwd>
#include <string>

namespace Scanner
{
  class ConsumeState;
  class Toker
  {
  public:
    Toker();
	Toker(bool flag);
    Toker(const Toker&) = delete;
    ~Toker();
    Toker& operator=(const Toker&) = delete;
    bool attach(std::istream* pIn);
	bool isComment(const std::string & tok);
	std::string getTok();
    bool canRead();
	size_t currentLineCount();
	void returnComments(bool doReturnComments=false);
  private:
    ConsumeState* pConsumer;
  };
}
#endif