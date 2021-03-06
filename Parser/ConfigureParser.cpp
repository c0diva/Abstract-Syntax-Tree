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

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  delete pClasandstrucDefination;
  delete pshowclassandstruct;
  delete pPushClassandStruct;
  delete pControlStmntDef;
  delete pPrintControlStmt;
  delete pPushControlStmt;
  delete pPrepocStatement;
  delete pPrintPreproc;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------
void ConfigParseToConsole::ScopeBegins()
{
	pBeginningOfScope = new BeginningOfScope();
	pHandlePush = new HandlePush(pRepo);
	pBeginningOfScope->addAction(pHandlePush);
	pParser->addRule(pBeginningOfScope);
}

void ConfigParseToConsole::scopeEnds()
{
	pEndOfScope = new EndOfScope();
	pHandlePop = new HandlePop(pRepo);
	pEndOfScope->addAction(pHandlePop);
	pParser->addRule(pEndOfScope);
}

void ConfigParseToConsole::defineFunction()
{
	pFunctionDefinition = new FunctionDefinition;
	pPushFunction = new PushFunction(pRepo);  // no action
	pPrintFunction = new PrintFunction(pRepo);
	pFunctionDefinition->addAction(pPushFunction);
	pFunctionDefinition->addAction(pPrintFunction);
	pParser->addRule(pFunctionDefinition);
}

void ConfigParseToConsole::controlStatements()
{
	pControlStmntDef = new ControlStmntDef;
	pPrintControlStmt = new PrintControlStmt(pRepo);
	pPushControlStmt = new PushControlStmt(pRepo);
	pControlStmntDef->addAction(pPushControlStmt);
	pControlStmntDef->addAction(pPrintControlStmt);
	pParser->addRule(pControlStmntDef);
}
Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts
    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues
	ScopeBegins();
	scopeEnds();
	defineFunction();
    // configure to detect and act on function definitions
    // these will stop further rule checking by returning false
	pClasandstrucDefination = new ClasandstrucDefination;
	pshowclassandstruct = new showclassandstruct(pRepo);
	pPushClassandStruct = new PushClassandStruct(pRepo);
	pClasandstrucDefination->addAction(pPushClassandStruct);
	pClasandstrucDefination->addAction(pshowclassandstruct);
	pParser->addRule(pClasandstrucDefination);
	//configure to detect if,switch,for,while,try,catch,while,do
	controlStatements();
	
// configure to check the preprocessor statements

	pPrepocStatement = new PreprocStatement;
	pPrintPreproc = new PrintPreproc(pRepo);
	pPrepocStatement->addAction(pPrintPreproc);
	pParser->addRule(pPrepocStatement);

    // configure to detect and act on declarations and Executables

    pDeclaration = new Declaration;
    pShowDeclaration = new ShowDeclaration;
    pDeclaration->addAction(pShowDeclaration);
    pParser->addRule(pDeclaration);
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
    pExecutable->addAction(pShowExecutable);
    pParser->addRule(pExecutable);

    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
