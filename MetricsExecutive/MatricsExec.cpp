/////////////////////////////////////////////////////////////////////
// MetricAnalysis.h - implements pattern matching for parse file   //
// version 1.0                                                     //
// Language   :C++, Visual Studio 2015                             //
// Application:Parser, CSE687 - Object Oriented Design             //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //
/////////////////////////////////////////////////////////////////////
#include <queue>
#include <string>
#define Util StringHelper
#include "MetricsExec.h"

using namespace Scanner;
using namespace Utilities;

void Req_345()
{
	std::cout << "\n  *************Demonstrating Requirement 3****************\n\n";
	std::cout << "  C++ packages for analyzing function size and complexity metrics\n\n  Please check Parser package and I have used my Tokenizer and \n\n  Semiexpression packages ";
	std::cout << "\n\n\n  **************Demonstrating Requirement 4*****************\n\n";
	std::cout << "  Provides a Parser package with a Parser class that is a container\n\n  for Rules and that provides the interfaces IRule and IAction for rules\n\n  contained in the Parser and actions contained in each rule.";
	
}

#ifdef METRICEXECUTIVE
int main(int argc, char* argv[])
{
	Util::Title("  Demonstrating Requirements of Project 2", '=');
	putline();
	Req_345();
	// collecting tokens from files, named on the command line
	if (argc < 2)
	{
		std::cout<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	for (int i = 1; i<argc; ++i)
	{

		std::vector<std::string> files = FileManager::Directory::getFiles(FileManager::Directory::getCurrentDirectory(), argv[i]); //gets the current directory and file
		std::cout << "\n\n\n  ***************Demonstrating Requirement 7******************\n\n";
		std::cout << "  Files that match the given pattern are : ";
		std::cout << "\n  ======================================== ";
		for (int k = 0; k < files.size(); k++)
		{
			std::cout << "\n  " << files[k];
		}
		std::cout << "\n\n";
		for (int j = 0; j < files.size(); ++j)
		{	
			std::cout << "\n\n\n  ***************Demonstrating Requirement 5******************\n\n";
			std::cout << "   Processing file directory is: ";
			std::cout << "\n  ============================ ";
			//gets full specifications from the filemanager
			std::string fileSpec = FileManager::Path::getFullFileSpec(files[j]);
			std::string msg = "Processing file" + fileSpec;
			Util::title(msg);
			ConfigParseToConsole configure;
			Parser* pParser = configure.Build();		//configures the build
			try
			{
				if (pParser)
				{
					if (!configure.Attach(files[j]))
					{
						std::cout << "\n  could not open file " << fileSpec << std::endl;
						continue;
					}
				}
				else
				{
					std::cout << "\n\n  Parser not built\n\n";
					return 1;
				}
				while (pParser->next())
					pParser->parse();
				std::cout << "\n";
			}
			catch (std::exception& ex)
			{
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
			}
			std::cout << "\n";
	}
}

		
	
#endif