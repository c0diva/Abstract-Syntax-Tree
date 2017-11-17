/////////////////////////////////////////////////////////////////////
// MetricAnalysis.cpp - implements class to analyze the parsed code  //
// version 1.0                                                     //
// Language   :C++, Visual Studio 2015                             //
// Application:Parser, CSE687 - Object Oriented Design             //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include "MetricsAnalysis.h"

void MetricAnalysis::StartAnalysis(element* currNode)
{
	std::cout <<"  "<< "line count = " << currNode->lineCount << "\t\tcomplexity = " << currNode->complexity << "\t element type = " << currNode->type << "\t Element name = " << currNode->name << "\n" ;
	std::vector<element*>::iterator it;						//Iterator is defined 
	std::vector<element*> *children = currNode->getElem();
	for (it = children->begin(); it != children->end(); it++)
	{
		StartAnalysis(*it);		//Starts analyzing each child of the tree
	}
}

#ifdef TEST_MA
int main()
{
	std::cout << "\n  Testing Abstract Syntax Tree";
	std::cout << "\n ==============================\n";


	ASTree* testAST;
	element* pItem = new element;

	pItem->type = "function";
	pItem->name = "fun1";
	pItem->Start_lineCount = 33;
	pItem->End_lineCount = 40;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST = new ASTree(pItem);

	pItem = new element;
	pItem->type = "ControlStatement";
	pItem->name = "if";
	pItem->Start_lineCount = 38;
	pItem->End_lineCount = 60;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST->getRoot()->addElem(pItem);

	pItem = new element;
	pItem->type = "ControlStatement";
	pItem->name = "for";
	pItem->Start_lineCount = 40;
	pItem->End_lineCount = 46;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST->getRoot()->addElem(pItem);

	pItem->type = "ControlStatement";
	pItem->name = "while";
	pItem->Start_lineCount = 60;
	pItem->End_lineCount = 70;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST->getRoot()->addElem(pItem);

	std::cout << "\n  Walking simulated AST tree:";
	std::cout << "\n -----------------------------";
	MetricAnalysis * testMA;
	testMA = new MetricAnalysis(testAST);
	testMA->StartAnalysis(testAST->getRoot());
	std::cout << "\n\n";
}

#endif
