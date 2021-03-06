#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/*
/////////////////////////////////////////////////////////////////////
// ActionsAndRules.h - implements new parsing rules and actions    //
// version 2.2                                                     //
// Language   :C++, Visual Studio 2015                             //
// Application:Parser, CSE687 - Object Oriented Design             //
// Source     :Jim Fawcett, Syracuse University, CST 4-187         //
//              jfawcett@twcny.rr.com							   //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //
/////////////////////////////////////////////////////////////////////
 /*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.2 : 10 Mar 16
  - added two rules. One to identify the class, struct and namespace
  - Second one identifies all the flow control statements like for,if,switch,catch,try,while etc.
  - added necessary actions needed for these rules
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release
*/
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../AST/AST.h"
#include "../MetricsAnalysis/MetricsAnalysis.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
/* ToDo:
 * - chanage lineCount to two fields: lineCountStart and lineCountEnd
 */

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
 * ToDo:
 * - add AST Node class
 * - provide field to hold root of AST
 * - provide interface to access AST
 * - provide helper functions to build AST, perhaps in Tree class
 */
class Repository  // application specific
{	
	element* temp;
	ASTree* pAST;
  ScopeStack<element*> stack;
  Scanner::Toker* p_Toker;
  static Repository* pRepository;
public:

  Repository(Scanner::Toker* pToker)
  {
	//Setting the start of the scope as Global namespace
    p_Toker = pToker;
	temp = new element;
	temp->name = "NameSpace";
	temp->type = "Global NameSpace";
	temp->End_lineCount = 0;
	temp->Start_lineCount = 0;
	temp->lineCount = 1;
	pAST = new ASTree(temp);
	stack.push(temp);
  }
  //Return the instance of the repository
  Repository* getRepoInstance()
  {
	  return this;
  }
  //Destructor of the repository
  ~Repository() {
	  std::cout << "\n\n  *************Demonstrating Requirement 6***************\n\n";
	  pAST->FindComplexity(pAST->getRoot());
	  pAST->TreeWalk(pAST->getRoot());
	  std::cout << "\n\n\n";
	  std::cout << "\n\n  ***************Demonstrating Requirement 8 and 9****************\n\n";
	  MetricAnalysis* pMA = new MetricAnalysis(pAST);
	  pMA->StartAnalysis(pAST->getRoot());
  }
  // returns node of the tree
  element* getElem()
  {
	  return temp;
  }
  //used for Abstract Syntax Tree
  ASTree* AST()
  {
	  return pAST;
  }
  //returns the stack
  ScopeStack<element*>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  //returns the linecount
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    element* elem =new element;
    elem->type = "unknown";						//defining the unknown scope
    elem->name = "anonymous";
    elem->lineCount = p_Repos->lineCount();
	p_Repos->scopeStack().top()->addElem(elem);
    p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    if(p_Repos->scopeStack().size() == 0)
      return;
    element* elem = p_Repos->scopeStack().pop();
	elem->Start_lineCount = elem->lineCount - 1;     //Start of the scope
	elem->End_lineCount = p_Repos->lineCount();		 //End of the scope
	elem->lineCount = elem->End_lineCount - elem->Start_lineCount;			//calculating the linecount
	p_Repos->getElem()->End_lineCount = elem->End_lineCount;
	p_Repos->getElem()->lineCount = p_Repos->getElem()->End_lineCount - p_Repos->getElem()->Start_lineCount;
    if(elem->type == "function" || elem->type == "ControlStatement" || elem->type == "classandStruct")
    {
	  std::cout << "\n";
      std::cout << "  " <<elem->type<< " " << elem->name << ", lines = " << elem->lineCount; //prints element type, element name and lines
      std::cout << "\n";
    }
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
	  if((pTc->find("#") < pTc->length())|| (pTc->find("#")<=1))
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
	Repository* p_Repos;
public:
	PrintPreproc(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]= { "for", "while", "switch", "if", "catch" };
	for (int i = 0; i < 5; ++i)
	{
		if (tok == keys[i])
		{
			return true;
		}
	}
		return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // next statement is now  handled in PrintFunction
  
    // pop anonymous scope
	  p_Repos->AST()->removeElem(p_Repos->AST()->getRoot(), p_Repos->scopeStack().top());
    p_Repos->scopeStack().pop();
    std::string name = (*pTc)[pTc->find("(") - 1];
    element* elem = new element;
    elem->type = "function";
    elem->name = name;
    elem->lineCount = p_Repos->lineCount();
	p_Repos->scopeStack().top()->addElem(elem);
    p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    size_t len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end >= tc.length())
        end = tc.find(">::");
      if (end >= tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch","try","do","else" };
    for (int i = 0; i<8; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]))
          continue;
        if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
    std::cout << "\n  Declaration: " << se.show();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end >= tc.length())
        end = tc.find(">::");
      if (end >= tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
  }
  
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch","else","do","try" };
    for (int i = 0; i<8; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]))
          continue;
        if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
          se.push_back(tc[i]);
      }
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    std::cout << "\n  Executable: " << se.show();
  }
};

/////////////////////////////////////////////////////////
// Rule to detect class, struct and namespace

class ClasandstrucDefination:public IRule
{
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "class", "struct", "namespace"};		//Check for special keyword
		for (int i = 0; i<3; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")		//find closing curly brace
		{
			for (int i = 0; i < tc.find("{")-1; i++)
			{
				if (isSpecialKeyWord(tc[i]))
				{
					doActions(pTc);
					return true;
				}
			}
		}
		return true;
	}
};

////////////////////////////////////////////////////
//Action to show or print class, struct and namespace

class showclassandstruct : public IAction
{
	Repository* p_Repos;
public:
	showclassandstruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{	
		std::string typeName = pTc->show().c_str();
		if((typeName.find("class"))<typeName.length())
			std::cout << "\n Class Def : " << pTc->show().c_str();	//printing a class defination
		else
			if((typeName.find("struct"))<typeName.length())
				std::cout << "\n Structure Def: " << pTc->show().c_str();		//printing a structure defination
			else
				if((typeName.find("namespace"))<typeName.length())
					std::cout << "\n Namespace : " << pTc->show().c_str();			//printing namespace
	}
};

////////////////////////////////////////////////////
//Action to push class,struct and namespace

class PushClassandStruct : public IAction
{
	Repository* p_Repos;
public:
	PushClassandStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "class", "struct", "namespace" };
		for (int i = 0; i<3; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->AST()->removeElem(p_Repos->AST()->getRoot(), p_Repos->scopeStack().top());
		p_Repos->scopeStack().pop();
		
		// push function scope
		std::string name = "";
		for (int i = 0; i < pTc->find("{") - 1; i++)
		{
			if (isSpecialKeyWord((*pTc)[i]))		//Finding the name of element to print
			{

				name = (*pTc)[++i];
				break;
			}
		}
		element* elem = new element;
		std::string typeName = pTc->show().c_str();
		if ((typeName.find("class")) < typeName.length())
			elem->type = "class";
		else
			if ((typeName.find("struct")) < typeName.length())
				elem->type = "struct";
			else
				if ((typeName.find("namespace")) < typeName.length())
					elem->type = "namespace";
		elem->name = name;
		elem->lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addElem(elem);
		p_Repos->scopeStack().push(elem);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
///// Rule for flow control statements like for,if,switch,while, else, try, catch, do

class ControlStmntDef : public IRule
{

		bool isSpecialKeyWord(const std::string& tok)
		{
			const static std::string keys[]
				= { "for", "if", "while","switch","else","try","catch","do"}; //set of special keywords
			for (int i = 0; i<8; ++i)
				if (tok == keys[i])
					return true;
			return false;
		}
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				for (int i = 0; i < tc.find("{"); i++) 
				{
					if (isSpecialKeyWord(tc[tc.find("{") - i - 1]))
					{
							doActions(pTc);
							return true;
					}
				}
			}
			return true;
		}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
///// Action to show or print flow control statements like for,if,switch,while, else, try, catch, do

class PrintControlStmt : public IAction
{
	Repository* p_Repos;
public:
	PrintControlStmt(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  ControlStatement: " << pTc->show().c_str();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////
//// Action for pushing flow control statements lke for,if,switch,while, else, try, catch, do

class PushControlStmt : public IAction
{
	Repository* p_Repos;
public:
	PushControlStmt(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "if", "while","switch","else","try","catch","do" };
		for (int i = 0; i<8; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->AST()->removeElem(p_Repos->AST()->getRoot(), p_Repos->scopeStack().top());
		p_Repos->scopeStack().pop();
		int i;
		for (i = 0; i < pTc->length(); i++)
		{
			if (isSpecialKeyWord((*pTc)[i]))
			{
				break;
			}
		}
		// push function scope
		std::string name = (*pTc)[i];
		element* elem = new element;
		elem->type = "ControlStatement";
		elem->name = name;
		elem->lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addElem(elem);
		p_Repos->scopeStack().push(elem);
	}
};

#endif
