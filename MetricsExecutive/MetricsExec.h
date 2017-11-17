#ifndef METRICSEXEC_H
#define METRICSEXEC_H

/////////////////////////////////////////////////////////////////////
// MetricAnalysis.h - implements pattern matching for parse file   //
// version 1.0                                                     //
// Language   :C++, Visual Studio 2015                             //
// Application:Parser, CSE687 - Object Oriented Design             //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
provide a MetricsExecutive package that enables collecting metrics for all 
the packages with names that match a specified pattern in a directory tree 
rooted at a specified path. Please define the path and file patterns on the 
MetricsExecutive command line.


Build Process:
==============
Required files
 -Utilities.h , Tokenizer.h , SemiExp.h , Parser.h , ActionsAndRules.h ,
  ConfigureParser.h , MetricsAnalysis.h , MetricsExec.h, Metricsexec.cpp

Maintenance History:
====================
ver 1.0 : 10 Mar 16
- first release
*/

#include "../FileSystem-Windows/FileSystemDemo/FileManager.h"

#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../MetricsAnalysis/MetricsAnalysis.h"

#endif
