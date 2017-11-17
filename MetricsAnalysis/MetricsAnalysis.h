#ifndef METRICSANALYSIS_H
#define METRICSANALYSIS_H
/*
/////////////////////////////////////////////////////////////////////
// MetricAnalysis.h - implements class to analyze the parsed code  //
// version 1.0                                                     //
// Language   :C++, Visual Studio 2015                             //
// Application:Parser, CSE687 - Object Oriented Design             //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines StartAnalysis method to analyze the code which in 
turn provides the complexity and linecounts of the various scopes of 
the given code.

Public Interface:
=================
void StartAnalysis(element* currNode);			//Analyzes the whole code 

Build Process:
==============
Required files
- AST.h

Maintenance History:
====================
ver 1.0 : 10 Mar 16
- first release
*/

#include "../AST/AST.h"

class MetricAnalysis {
	public:
		MetricAnalysis(ASTree* ast) :pAST(ast)		//Constructor of the MetricAnalysis
		{			
		}
		void StartAnalysis(element* currNode);		//Function to analyze the code

	private:
		ASTree* pAST;
};

#endif
