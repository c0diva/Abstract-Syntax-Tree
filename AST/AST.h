#ifndef AST_H
#define AST_H
/////////////////////////////////////////////////////////////////////
// AST.h     - implements new parsing rules and actions            //
// version 1.0                                                     //
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
This module defines the structre for creating nodes and trees. 
Also defines functions to remove node, to get the root of the
tree. This module will implement the Abstract Syntax tree.

Public Interface:
=================
ASTree(element* elem) : root(elem) { }						//constructor of Class that implements AST
void TreeWalk(element* pItem);								//Walks through the tree 
element* getRoot();											//Gets the root of the tree
void removeElem(element* currNode, element* removeNode);	//used to remove the node of tree
void FindComplexity(element* currNode);						//determines the complexity

Build Process:
==============
Required files
- AST.h,AST.cpp
- Libraries used are sstream, string, vector, stack, queue, memory, iostream

Maintenance History:
====================
ver 1.0 : 10 Mar 16
- first release
*/

#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <memory>
#include <iostream>

	/////////////////////////////////////////////////////////////////////////////
	// Define tree node - used to wire up ad hoc tree

struct element
{
	std::string type;					//type of an element
	std::string name;					//name of the element
	size_t lineCount;					//linecount of the particular scope
	int complexity = 0;					//complexity of scopes
	size_t Start_lineCount = 1;			//start of the linecount
	size_t End_lineCount;				//end of the linecount
	std::vector<element*> _children;	//children of the tree
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ")";
		return temp.str();
	}

	void addElem(element* elem) {		//adding an element i.e. child to the tree
		_children.push_back(elem);
	}
	
	std::vector<element*>* getElem() {  //get the value of the node
		return &_children;
	}

	bool isEqual(element* elem1)  //check whether two nodes are eqal or not
	{
		if (name == elem1->name && type == elem1->type)
			return true;
		else
			return false;
	}
};

class ASTree {
public:
	
	ASTree(element* elem) : root(elem) { }
	void TreeWalk(element* pItem);
	element* getRoot();
	void removeElem(element* currNode, element* removeNode);
	void FindComplexity(element* currNode);

private:
	element* root;
};

#endif