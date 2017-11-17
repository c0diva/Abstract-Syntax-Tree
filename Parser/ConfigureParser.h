#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
// ConfigureParser.h- builds and configures parsers			       //
// version 2.2                                                     //
// Language   :C++, Visual Studio 2015                             //
// Application:Parser, CSE687 - Object Oriented Design			   //
// Source     :Jim Fawcett, Syracuse University, CST 4-187         //
//              jfawcett@twcny.rr.com							   //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv Project1HelpS06.sln
    - cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
         ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.2 : 10 Mar 16
  - Added two more rules and two more actions for every rule
  ver 2.1 : 19 Feb 16
  - Added PrintFunction action to FunctionDefinition rule
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/

//
#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  Parser* Build();

private:
  // Builder must hold onto all the pieces

  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;

  // add Rules and Actions

  BeginningOfScope* pBeginningOfScope;				//Rule to check the beginning of scope
  HandlePush* pHandlePush;	
  EndOfScope* pEndOfScope;							//Rule to ceck the end of the scope	
  HandlePop* pHandlePop;
  PreprocStatement* pPrepocStatement;				//Rule to check the preprocessor statement
  PrintPreproc* pPrintPreproc;
  FunctionDefinition* pFunctionDefinition;			//Rule to check the function defination
  PushFunction* pPushFunction;
  PrintFunction* pPrintFunction;
  Declaration* pDeclaration;						//Rule to check the declaration
  ShowDeclaration* pShowDeclaration;
  Executable* pExecutable;							//Rule to check the executable statements
  ShowExecutable* pShowExecutable;
  ClasandstrucDefination* pClasandstrucDefination;  //Rule to determine class,structure or namespace
  showclassandstruct* pshowclassandstruct;
  PushClassandStruct* pPushClassandStruct;
  ControlStmntDef* pControlStmntDef;				//Rule to determine the flow control statements
  PrintControlStmt* pPrintControlStmt;
  PushControlStmt* pPushControlStmt;

  // prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&) = delete;
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
  void ScopeBegins();
  void scopeEnds();
  void defineFunction();
  void controlStatements();

};


#endif
