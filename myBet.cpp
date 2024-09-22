#include <iostream>
#include <string>
#include "stack.h"
#include <vector>
#include <fstream>
#include "sexceptions.h"
using namespace std;

struct BetNode
{
	char info;
	BetNode* left;
	BetNode* right;
};

void runTheProgram(ifstream& file);
void infixToPostfix(ifstream& file, stack<char> tokens, string& postfixExpression, string inLine);
bool higherOrEqualPrecedence(char top, char input);
void postToTree(string postfixExpression, BetNode*& root);
void printPrefix(BetNode* t);

int main(){
	//first, a file will be opened to the desired file of operations
	ifstream file("infix.dat", ios::in);
	string inLine;
	if(file.is_open()){
		//the main program will be run with the opened file
		runTheProgram(file);
		file.close();
	}
	else
		cout << "Error opening file. Program will end. \n\n";	
return 0;
}

/*   This primary program will input a file and use three functions:
 *	infixToPostfix, postToTree, and printPrefix
 *   These functions are performed for each line grabbed from the file.
 *   This will perform until there are no lines left to get.
 */
void runTheProgram(ifstream& file){
	stack<char> tokens;
	string inLine;
	while(getline(file, inLine)){
		string postfixExpression;
		infixToPostfix(file, tokens, postfixExpression, inLine);
	
		BetNode* root = NULL;
		postToTree(postfixExpression, root);
		postfixExpression = "";
	
		printPrefix(root);
		cout << "\n\n";
	}
}

//This function turns a string of text grabbed from an input file and analyzes each character
//as it traverses the string. It will determine which order to assemble a new string that will
//represent the postfix expression version of the original operation
void infixToPostfix(ifstream& file, stack<char> tokens, string& postfixExpression, string inLine){	
	//this will attempt to catch any underflow
	try{	
	//this primary for loop traverses the string
	for(int i = 0; i < inLine.size(); i++){
		char c = inLine[i];
		char c2 = inLine[i+2];
		//if the character is a space, skip
		if(c == ' ')
			continue;
		//if the character is a number, it will automatically add it to the end of the string
		if(isdigit(c)){
			postfixExpression = postfixExpression + c + ' ';
		//if the character is an operator, it will determine its precedence before pushing it onto
		//a token operator stack to be popped upon encountering a closing parenthesis
		}else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '('){			
		try{
			if(!isdigit(c2) && !tokens.isEmpty() && higherOrEqualPrecedence(c, tokens.top()) && tokens.top() != '('){
				postfixExpression = postfixExpression + tokens.top() + ' ';
				tokens.pop();
			}
			tokens.push(c);
		}catch(Underflow exc){
			cout << "underflow here" << endl;
			continue;
		}
		//once a closing parenthesis is encountered, the program will pop all operators until another
		//open parenthesis is encountered in which it will pop and move on.
		}else if(c == ')'){
			while(!tokens.isEmpty() && tokens.top() != '('){
				postfixExpression = postfixExpression + tokens.top() + ' ';
				tokens.pop();
			}
			if(!tokens.isEmpty())
				tokens.pop();
			while(!tokens.isEmpty() && tokens.top() != '('){
				postfixExpression = postfixExpression + tokens.top() + ' ';
				tokens.pop();
			}
		}
	}
	//uncomment if the postfixexpression would like to be printed.
	//cout << postfixExpression << "\n"; 	
	}catch(Underflow exc){
		cout << "ooops Underflow: \n";
	}

}

//Returns true if the first character is of equal
//or higher precedence compared to the second one,
//false otherwise.
bool higherOrEqualPrecedence(char top, char input) {
	if(top == input)
		return true;
	if(((top =='*') || (top == '/')) && input == '-')
		return false;
	if((top == '*') || (top == '/'))
		return true;
	if((top == '+') || (top == '-')){
		if((input == '+') || (input == '-'))
			return true;
	}
	if(top == '(')
		return false;
	return false;
}

//This function will take a postfix expression string and create a binary expression tree
//with it.
void postToTree(string postfixExpression, BetNode*& root){
	stack<BetNode*> treeStack;
	for(int i=0; i< postfixExpression.size(); i++){
		char c = postfixExpression[i];
		if(c == ' ')
			continue; //use this to skip any blanks
		//if a digit is encountered, create a null leaf node
		if(isdigit(c)){
			BetNode* newNode = new BetNode;
			newNode -> info = c;
			newNode -> left = NULL;
			newNode -> right = NULL;
			treeStack.push(newNode);
		//otherwise, once an operator is encountered, two numbers will be taken
		//from the stack and be assigned to the operator which will be a new root
		//node.
		}else if(!treeStack.isEmpty()){ 
			BetNode* rightOperand = treeStack.top();
			treeStack.pop();
			BetNode* leftOperand = treeStack.top();
			if(!treeStack.isEmpty())
				treeStack.pop();
			
			BetNode* newNode = new BetNode;
			newNode -> info = c;
			newNode -> left = leftOperand;
			newNode -> right = rightOperand;
			treeStack.push(newNode);
		}
	}
	//once the entire tree is created, the root is declared and sent out of the function
	root = treeStack.top();
	if(!treeStack.isEmpty())
		treeStack.pop();
}

//This function prints a given binary expression tree in prefix form.
void printPrefix(BetNode* t){
	if(t != NULL){
		cout << t-> info << " ";
		printPrefix(t->left);
		printPrefix(t->right);
	}
}
