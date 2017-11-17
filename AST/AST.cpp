/////////////////////////////////////////////////////////////////////
// AST.cpp     - implements new parsing rules and actions          //
// version 1.0                                                     //
// Language   :C++, Visual Studio 2015                             //
// Application:Parser, CSE687 - Object Oriented Design			   //
// Source     :Jim Fawcett, Syracuse University, CST 4-187         //
//              jfawcett@twcny.rr.com							   //
// Author	  :Aditi Gaikwad, Syracuse University				   //
//			   agaikwad@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include "AST.h"
#include "../ScopeStack/ScopeStack.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>


void ASTree::TreeWalk(element* pItem)		//walks through the tree and prints corrsponding children values
{
	static size_t indentLevel = 0;			//setting the indent level to 0
	std::cout << "\n  " << std::string(5 * indentLevel, ' ') << pItem->show();
	auto iter = pItem->_children.begin();
	++indentLevel;
	while (iter != pItem->_children.end())		//iteration through the children of the tree
	{
		TreeWalk(*iter);
		++iter;
	}
	--indentLevel;
}
 element* ASTree::getRoot()		//returns the root of the tree
{
	return root;
}
void ASTree::removeElem(element* currNode, element* removeNode)		//removes the chldren of the tree
{
	if (removeNode == nullptr)
		return;
	std::vector<element*>::iterator it;
	std::vector<element*> *children = currNode->getElem();
	for (it = children->begin(); it != children->end(); )
	{
		if ((*it) == removeNode)
		{
			delete * it;
			it = children->erase(it);
		}
		else
		{
			removeElem((*it), removeNode);
			++it;
		}
	}
}
void ASTree::FindComplexity(element* currNode)			//determines the complexity of the tree
{
	currNode->complexity += 1;
	std::vector<element*>::iterator it;                 //initializing the iterator
	std::vector<element*> *children = currNode->getElem();
	for (it = children->begin(); it != children->end(); it++)
	{
		FindComplexity(*it);							//recursion to determine the complexity
	}
	for (it = children->begin(); it != children->end(); it++)
	{
		currNode->complexity += (*it)->complexity;
	}
}
#ifdef TEST_AST
int main()
{
	std::cout << "\n  Testing Abstract Syntax Tree";
	std::cout << "\n ==============================\n";

	
	ASTree* testAST;
	element* pItem = new element;

	pItem->type = "function";
	pItem->name = "fun1";
	pItem->Start_lineCount = 33;
	pItem->End_lineCount = 76;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST = new ASTree(pItem);

	pItem = new element;
	pItem->type = "ControlStatement";
	pItem->name = "if";
	pItem->Start_lineCount = 38;
	pItem->End_lineCount = 40;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST->getRoot()->addElem(pItem);

	pItem = new element;
	pItem->type = "ControlStatement";
	pItem->name = "for";
	pItem->Start_lineCount = 40;
	pItem->End_lineCount = 50;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST->getRoot()->addElem(pItem);

	pItem->type = "ControlStatement";
	pItem->name = "while";
	pItem->Start_lineCount = 60;
	pItem->End_lineCount = 65;
	pItem->lineCount = pItem->End_lineCount - pItem->Start_lineCount;
	testAST->getRoot()->addElem(pItem);

	std::cout << "\n  Walking simulated AST tree:";
	std::cout << "\n -----------------------------";
	testAST->TreeWalk(testAST->getRoot());
	std::cout << "\n\n";
}

#endif
