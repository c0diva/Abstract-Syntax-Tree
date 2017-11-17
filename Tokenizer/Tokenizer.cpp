/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 1.0                                                         //
// Language   :C++, Visual Studio 2015                             //
// Application:Lexical Scanner, CSE687 - Object Oriented Design    //
// Source     :Jim Fawcett, Syracuse University, CST 4-187         //
//              jfawcett@twcny.rr.com							   //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //		
/////////////////////////////////////////////////////////////////////
/*
* 
* Maintenance History
* ---------------------
* ver 1.0: Feb 8th, 2016
* -Added new functions such as specialchars, quotedstrings and quoted
* characters
*/
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>

namespace Scanner
{
  class ConsumeState
  {
  public:
    ConsumeState();
    ConsumeState(const ConsumeState&) = delete;
    ConsumeState& operator=(const ConsumeState&) = delete;
    virtual ~ConsumeState();
	size_t currentLineCount();
	bool _doReturnComments = false;
	void returnComments(bool doReturnComments = false);
	void attach(std::istream* pIn) 
	{
		_pIn = pIn;
		first = true;
	}
	static bool printComment;
    virtual void eatChars() = 0;
    void consumeChars() 
	{
      _pState->eatChars();
	  _pState = nextState();
    }
	void setSpecialSingleChars(std::string ssc) //adds string to single char vector
	{
		SpecialOneChars.push_back(ssc);
	}
	void setSpecialCharPairs(std::string scp) //adds string to the vector for special char pairs
	{
		SpecialTwoChars.push_back(scp);
	}
	void Print_Comments(bool flag)     //if flag is true then comments are printed 
	{
		printComment = flag;
	}
	void insertIntoVector()			  //inserting special charaters into vector
	{ 
		SpecialOneChars.push_back("<");
		SpecialOneChars.push_back(">");
		SpecialOneChars.push_back("[");
		SpecialOneChars.push_back("]");
		SpecialOneChars.push_back("(");
		SpecialOneChars.push_back(")");
		SpecialOneChars.push_back("{");
		SpecialOneChars.push_back("}");
		SpecialOneChars.push_back(":");
		SpecialOneChars.push_back("=");
		SpecialOneChars.push_back("+");
		SpecialOneChars.push_back("-");
		SpecialOneChars.push_back("*");
		SpecialOneChars.push_back("\\n");
		SpecialTwoChars.push_back("<<");
		SpecialTwoChars.push_back(">>");
		SpecialTwoChars.push_back("++");
		SpecialTwoChars.push_back("::");
		SpecialTwoChars.push_back("==");
		SpecialTwoChars.push_back("+=");
		SpecialTwoChars.push_back("-=");
		SpecialTwoChars.push_back("*=");
		SpecialTwoChars.push_back("/=");
	}

	bool canRead() { return _pIn->good(); }
    std::string getTok() { return token; }
    bool hasTok() { return token.size() > 0; }
  
	bool checkForSingleChar(char ssc)   //checking if the character is single character or not
	{
		for (size_t i = 0; i < SpecialOneChars.size(); i++)
		{
			if (SpecialOneChars[i].at(0)==ssc )
				return true;
		}
		return false;
	}
	bool checkForSpecialPairs(char scp, char nextchar) //checking if the characters are two character strings
	{
		for (size_t i = 0; i < SpecialTwoChars.size(); i++)
		{
			if (SpecialTwoChars[i].at(0) == scp && SpecialTwoChars[i].at(1) == nextchar)
			{
				return true;
			}
		}
		return false;
	}
  
  protected:
    static std::string token;
    static std::istream* _pIn;
    static int prevChar;
    static int currChar;
	ConsumeState* nextState();
	static size_t _lineCount;
    static ConsumeState* _pState;
    static ConsumeState* _pEatCppComment;
    static ConsumeState* _pEatCComment;
    static ConsumeState* _pEatWhitespace;
    static ConsumeState* _pEatPunctuator;
    static ConsumeState* _pEatAlphanum;
    static ConsumeState* _pEatNewline;
	static ConsumeState* _pEatString;
	static ConsumeState* _pEatSpecialChar;
	static ConsumeState* _pEatSpecialPair;
	static ConsumeState* _pEatQuotedChar;
	std::vector<std::string> SpecialOneChars;
	std::vector<std::string> SpecialTwoChars;
	static bool first;
  };
}

using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
size_t ConsumeState::_lineCount=1;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
bool ConsumeState::printComment = false;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatString = nullptr;
ConsumeState* ConsumeState::_pEatSpecialChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialPair = nullptr;
ConsumeState* ConsumeState::_pEatQuotedChar = nullptr;
bool _printCppComment = true;
bool _printCComment = true;

void testLog(const std::string& msg);

ConsumeState* ConsumeState::nextState()
{
  if (!(_pIn->good()))
  {
    return nullptr;
  }
  int chNext = _pIn->peek();
  if (chNext == EOF)
  {
    _pIn->clear();
    // if peek() reads end of file character, EOF, then eofbit is set and
    // _pIn->good() will return false.  clear() restores state to good
  }
  if (std::isspace(currChar) && currChar != '\n')
  {
    testLog("state: eatWhitespace");
    return _pEatWhitespace;
  }
  if (currChar == '\'')
  {
	  testLog("state: eatQuotedChar");   //State for quoted strings
	  return _pEatQuotedChar;
  }
  if (currChar == '/' && chNext == '/')
  {
    testLog("state: eatCppComment");
    return _pEatCppComment;
  }
  if (currChar == '/' && chNext == '*')
  {
    testLog("state: eatCComment");
    return _pEatCComment;
  }
  if (currChar == '\n')
  {
    testLog("state: eatNewLine");
    return _pEatNewline;
  }
  if (std::isalnum(currChar) || currChar == '_')
  {
    testLog("state: eatAlphanum");
    return _pEatAlphanum;
  }
  if (checkForSpecialPairs(currChar, chNext))  //State for special two character tokens
  {
	  testLog("state: eatSpecialPair");
	  return _pEatSpecialPair;
  }
  if (checkForSingleChar(currChar))            //State for special one character tokens
  {
	  testLog("state: eatSingleSpecialChar");
		  return _pEatSpecialChar;
  }
  if (currChar == '"')						   //State for quoted strings
  {
	  testLog("state: eatQuotedString");
	  return _pEatString;
  }
  if (ispunct(currChar))                     //State for quoted characters
  {
    testLog("state: eatPunctuator");
    return _pEatPunctuator;
  }
  if (!_pIn->good())
  {
    return _pEatWhitespace;
  }
  throw(std::logic_error("invalid type"));
}
class EatQuotedString : public ConsumeState	
{
public:
	virtual void eatChars()
	{
		token.clear();
		do
		{
			if(currChar!='"' && currChar!='\\')
			token += currChar;
			else if (currChar == '\\')
			{
				if (/*_pIn->peek() == '"' ||*/ _pIn->peek()=='\\') //Checks for quoted string inside 
				{											   //the double quoted string
					currChar = _pIn->get();
					token += currChar;
				}
				else
				{
					token += currChar;
				}
			}
			if (!_pIn->good())
				return;
			currChar = _pIn->get();
		} while (currChar!='"');
		currChar = _pIn->get();
	}
};
class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating whitespace";
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};
class EatQuotedChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		do
		{
			if (currChar != '\'' && currChar != '\\') //Condition for quoted characters
				token += currChar;
			else if (currChar == '\\' )
			{
				if (_pIn->peek() == '\'' || _pIn->peek()=='\\')
				{
					currChar = _pIn->get();
					token += currChar;
				}
				else
				{
					token += currChar;
				}
			}
			if (!_pIn->good())
				return;
			currChar = _pIn->get();
		} while (currChar != '\'');
		currChar = _pIn->get();
	}
};
class EatCppComment : public ConsumeState   //Takes whole c++ comment as a single token
{
public:
	virtual void eatChars()
	{
			token.clear();
			do {
				if (printComment)
					token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			} while (currChar != '\n');
	}
};
class EatSpecialChar : public ConsumeState   //Extracts special one character tokens
{
public :
	virtual void eatChars()
	{
		token.clear();
		token += currChar;
		if (currChar == '\\' && _pIn->peek() == 'n')
		{
			currChar = _pIn->get();
			token += currChar;
		}
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};
class EatSpecialPair : public ConsumeState   //Extracts special two character tokens
{
public:
	virtual void eatChars()
	{
		token.clear();
			token += currChar;
			currChar = _pIn->get();
			token += currChar;		
			currChar = _pIn->get();
	}
};
class EatCComment : public ConsumeState   //Extracts all c comments
{
public:
  virtual void eatChars()
  { 
		  token.clear();
		  do {
			  if (printComment)
			  {
				  token += currChar;
			  }
			  if (!_pIn->good())  // end of stream
				  return;
			  currChar = _pIn->get();
		  } while (currChar != '*' || _pIn->peek() != '/');
		  if (printComment)
			  token += currChar;
		  currChar = _pIn->get();
		  if (printComment)
			  token += currChar;
		  currChar = _pIn->get();
  }
};

class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (ispunct(currChar));
  }
};
class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (isalnum(currChar) || currChar == '_');
  }
};
class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    token += currChar;
	_lineCount++;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};
bool ConsumeState::first = true;
ConsumeState::ConsumeState()
{	
  if (first)
  {
    first = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
	_pEatString = new EatQuotedString();
	_pEatSpecialChar = new EatSpecialChar();
	_pEatSpecialPair = new EatSpecialPair();
	_pEatQuotedChar = new EatQuotedChar();
	insertIntoVector();
    _pState = _pEatWhitespace;
  }
}
//----< return number of newlines collected from stream >------------

size_t ConsumeState::currentLineCount() { return _pState->_lineCount; }


ConsumeState::~ConsumeState()
{
  ConsumeState::first = true;
  static bool firstd = true;
  if (firstd)
  {
    firstd = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pEatString;
	delete _pEatSpecialChar;
	delete _pEatSpecialPair;
	delete _pEatQuotedChar;
  }
}
Toker::Toker() : pConsumer(new EatWhitespace()) {}
Toker::Toker(bool flag) : pConsumer(new EatWhitespace())
{
	pConsumer->Print_Comments(flag);
}
Toker::~Toker() { delete pConsumer; }
bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}


bool Toker::isComment(const std::string& tok)
{
	if (tok.find("//") < tok.size() || tok.find("/*") < tok.size())
		return true;
	return false;
}

std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}
bool Toker::canRead() { return pConsumer->canRead(); }
size_t Toker::currentLineCount()
{
	return pConsumer->currentLineCount();
}
void Toker::returnComments(bool doReturnComments)
{
	pConsumer->returnComments(doReturnComments);
}

void ConsumeState::returnComments(bool doReturnComments)
{
	_pState->_doReturnComments = doReturnComments;
}


void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}
//----< test stub >--------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  //std::string fileSpec = "../Tokenizer/Tokenizer.h";
  std::string fileSpec = "../Tokenizer/Test.txt";
  std::ifstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  Toker toker;
  toker.attach(&in);
  do
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = " newline";
    std::cout << "\n -- " << tok;
  } while (in.good());
  std::cout << "\n\n";
  return 0;
}
#endif
