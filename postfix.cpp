#include <iostream>
#include <string>
#include "stack.h"
#include <vector>
#include <limits>
#include <cstdlib>
#include "sexceptions.h" 
using namespace std;

void runTheProgram(stack<int> tokens);
int mathOp(stack<int>& inStack, char token);
void mathPop(stack<int>& inStack);

int main(){
	//begin by setting up a stack that will be erased upon completion
	//of the program run.
	stack<int> tokens;
	//begins the primary evaluator function, taking in a token stack.
	runTheProgram(tokens);
/* 	Here is the primary while loop responsible for asking the user for
 *  	input on whether or not they wish to run the calculator program again.
 *	The following options are:
 *	Y: will run the postfix evaluator again
 *	N: will set the while loop to end on following completion
 *	The default will cover any other invalid inputs
 *
 *	The user will be notified when the calculator program has ceased.    */
	char userChoice;
	bool codeRun = true;
	while(codeRun){
		cout << endl << "Go Again? (Y/N) ";
		cin >> userChoice;
		switch(userChoice){
			case 'Y':
				runTheProgram(tokens); break;
			case 'N':
				codeRun = false; break;
			default:
				cout << "Invalid input try again!" << endl; break;
		}	
	}
	cout << "! Ending calculator Program." << endl;
return 0;
}

/*  Here is the primary program activation function.
 *  It will take in an empty integer tokens stack
 *  It will utilize two other functions including:
 *  mathOp and mathPop				  */ 
void runTheProgram(stack<int>tokens){
	cout << "@@@@@@@@@@@@@@@@@ THE POSTFIX EVALUATOR @@@@@@@@@@@@@@@@@" << endl;	
	cout << endl << "Your RPN Expression : ";
	cout.clear();
	//Read in a user string input
	string userStringCommand;	
	cin.clear();
	cin.sync();
	getline(cin, userStringCommand, ';');
	userStringCommand = userStringCommand + ';';
	//clean up any unnecessary inputs just in case, as well as making sure the token
	//stack is absolutely empty upon a re-run
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	while (!tokens.isEmpty()) {
  		tokens.pop();
	}
	//using a for loop, the characters within the string are separated into
	//tokens that will then be analyzed and utilized in the mathematical operations.
	int token;
	char tokenOp;
	bool isNeg;
	int popResult;
	for(int i = 0; i < userStringCommand.size(); i++){
		char c = userStringCommand[i];
		//if there is a space, skip over it
		if(c == ' ' || c == '\n')
			continue;
		//if there is a underscore, consider the number adjacent to be negative
		if(c == '_'){
			isNeg = true;
			continue;
		}
		//if the current character is a mathematical operator or digit, the if statement will
		//convert the character into a usable piece of the token union.
		if(isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/'|| c == ';' || c == '%') {
			//if a digit is detected, check to see if there are recurring digits afterwards.
			if(isdigit(c)){
				string numStr;
				//while there are recurring digits, combine into a string.
				while(i < userStringCommand.size() && isdigit(userStringCommand[i])){
					numStr += userStringCommand[i];
					i++;
				}
				i--;
				
				//make the string become a integer under token after converting
				//the string to an int and then checking if the number is negative.
				token = (isNeg ? -1 : 1) * atoi(numStr.c_str());
				isNeg = false;
				
				//will attempt to catch an overflow if there are too many inputs in
				//the expression given.
				try{	
				tokens.push(token);
				}catch(Overflow exc){
				cout << endl << "Stack Overflow " << endl;
				tokens.pop();
				}
				cout << "Token = " << token << "	";
				cout << "Push = " << token << endl;
				token = 0;
			//if the negative flag is true and there is no digit following it then there
			//will be a breaking error for an invalid expression.	
			}else if(isNeg == true && !isdigit(c)){
				cout << "Negative sign in front of non digit error! \n\n";
				break;
			//once the code reaches the input breaking semicolon, the token will be displayed
			//before the final answer is displayed afterwards.
			}else if(c == ';'){
				cout << "Token = " << c << "	"; 
				if(!tokens.isEmpty()){
					mathPop(tokens);
				}else{
					cout << "\nNon existent expression or answer --  Exception!";
				}
				cout << "\n\n";
				break;
			//if the input was one of any of the desired operators, this else statement will perform
			//the necessary mathematical functions depending on the operator.
			}else{
	       			tokenOp = c;
				cout << "Token = " << tokenOp << "	";   
				//attempts to catch an exception of division by zero and underflow.
				try{				
				popResult = mathOp(tokens, tokenOp);
				}catch(DivByZero exc){
					cout << endl << "Division by Zero Exception -- Now Exiting. \n\n" << endl; break;
				}catch(Underflow exc){
					cout << endl << "Underflow Exception" << endl; break;
				}
				//will push the output answer back into the stack.
				cout << "Push = " << popResult << endl;
				tokens.push(popResult);
				tokenOp = '0';
			}
  		//if the user inputs an invalid expression that includes improper characters, the code will end.
		}else{
			cout << "\nException -- You Have entered An Invalid Expression.\n";
			break;
		}
	}
	//one more attempt at clearing the stack for final.
	while(!tokens.isEmpty()){
		tokens.pop();
	}
	
}


//this function takes in the stack and a singular operator to perfrom a
//mathematical funciton based on the two top values within the stack. It
//will return the updated stack once the process is complete.
int mathOp(stack<int>& inStack, char tokenOp){
	int opVal1; int opVal2; int finVal;
	switch(tokenOp){
		case '+':
			opVal1 = inStack.top();
			mathPop(inStack);
			opVal2 = inStack.top();
			mathPop(inStack);
			finVal = opVal2 + opVal1;
			return finVal; break;
		case '-':
			opVal1 = inStack.top();
			mathPop(inStack);
			opVal2 = inStack.top();
			mathPop(inStack);
			finVal = opVal2 - opVal1;
			return finVal; break;
		case '*':
			opVal1 = inStack.top();
			mathPop(inStack);
			opVal2 = inStack.top();
			mathPop(inStack);
			finVal = opVal2 * opVal1;
			return finVal; break;
		case '/':
			opVal1 = inStack.top();
			mathPop(inStack);
			opVal2 = inStack.top();
			mathPop(inStack);
			if(opVal1 == 0){
				throw DivByZero();
			}
			finVal = opVal2 / opVal1;
			return finVal; break;	
		case '%':
			opVal1 = inStack.top();
			mathPop(inStack);
			opVal2 = inStack.top();
			mathPop(inStack);
			if(opVal1 == 0){
				throw DivByZero();
			}
			finVal = opVal2 % opVal1;
			return finVal; break;
		default: break;
	}
}

//This function is used within the mathOp function where the action of
//popping the stack is repeated many times. It will display the popped
//value.
void mathPop(stack<int>& inStack){
	int popVal;
	popVal = inStack.top();
	cout << "Pop " << popVal << "		";
	inStack.pop();
}



