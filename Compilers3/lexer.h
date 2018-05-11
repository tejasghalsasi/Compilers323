/*
Tejas Ghalsasi
CPSC 323 Rat18S
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

queue <string> tokenQueue;
queue <string> lexemeQueue;

bool seperator(char input[], int length)
{
	int numSep = 10;
	char seperators[] = "();:,{}[]@%";
	bool sep = false;

	if (length < 2)
	{
		for (int i = 0; i < numSep; ++i)
		{
			if (seperators[i] == input[0])
			{
				sep = true;
				break;
			}
		}
	}
	else
	{
		if (input[0] == '%' && input[1] == '%')
		{
			sep = true;
		}
	}
	return sep;
}

bool operators(char input[], int length)
{
	const int numOps = 7;
	char operators[numOps] = { '<', '>', '+', '-', '*', '/', '=' };
	bool oper = false;

	if (length == 2)
	{
		if ((input[0] == ':' && input[1] == '=') ||
			(input[0] == '=' && input[1] == '=') ||
			(input[0] == '+' && input[1] == '+') ||
			(input[0] == '=' && input[1] == '>') ||
			(input[0] == '<' && input[1] == '=') ||
			(input[0] == '/' && input[1] == '='))
		{
			oper = true;
		}
	}
	else
	{
		for (int i = 0; i < numOps; ++i)
		{
			if (operators[i] == input[0])
			{
				oper = true;
				break;
			}
		}
	}
	return oper;
}

bool keyword(char input[])
{
	const int numKey = 13;
	string keywords[numKey] = { "integer", "if", "else", "fi", "while",
		"return", "read", "write", "for", "true", "false", "boolean",
		"floating" };
	bool key = false;	// Initial state

	for (int i = 0; i < numKey; ++i)
	{
		if (keywords[i] == input)
		{
			key = true;
			break;
		}
	}
	return key;
}

bool idFSM(/*string input*/ char input[], int length)
{
	//bool valid = false;
	int state = 0;
	int col;
	int table[6][2] = {	// q0 = 1; F: {1, 3, 4}
						// l  #
		/*0*/{ 1, 2 },
		/*1*/{ 1, 3 },
		/*2*/{ 2, 2 },	// Invalid State row
		/*3*/{ 4, 2 },
		/*4*/{ 4, 5 },
		/*5*/{ 4, 2 } };

	for (int i = 0; i < length; ++i)
	{
		if (isalpha(input[i]))
		{
			col = 0;
			state = table[state][col];
		}
		else if (input[i] == '#')
		{
			col = 1;
			state = table[state][col];
		}
		// Inputs that enter this else statement are in an invalid state and cannot change due to the way the FSM is set up.
		else
		{
			state = 2;
		}
	}

	if (state == 1 || state == 3 || state == 4)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int numFSM(char input[], int length)
{
	int state = 1;
	int col;
	int table[5][2] = {	// q0 = 1; F: {1(int), 4(real)}
						// d  .
		/*0*/{ 1, 2 },
		/*1*/{ 1, 3 },
		/*2*/{ 2, 2 },	// Invalid State row
		/*3*/{ 4, 2 },
		/*4*/{ 4, 2 } };

	for (int i = 0; i < length; ++i)
	{
		if (isdigit(input[i]))
		{
			col = 0;
			state = table[state][col];
		}
		else if (input[i] == '.')
		{
			col = 1;
			state = table[state][col];
		}
		// num that enters this else statement are in an invalid state.
		else
		{
			state = 2;
		}
	}
	return state;
}

void lexer()
{
	char c;
	char input[20];
	//char seperators[] = "();:,";
	bool firstSep = false;
	string firstSeperator;
	bool lastSep = false;
	string lastSeperator;

	int i = 0;
	ifstream inFile("testCase3.txt");
	ofstream outFile;

	if (!inFile.is_open())
	{
		cout << "Error! Could not open file! " << endl;
	}

	outFile.open("testCase3LexicalResult.txt");
	outFile << "TOKEN\t\t\tLEXEME" << endl;

	while (!inFile.eof())
	{
		c = inFile.get();

		// if c reads an empty space, an end line, or reaches the end of a file; start checking the input.
		if (c == ' ' || c == '\n' || c == '\t' || inFile.eof())
		{
			input[i] = '\0';
			int inputLength = i;
			i = 0;

			// if input is empty, meaning c is just grabbing empty space, don't bother checking.
			if (inputLength > 0)
			{
				// If firstSep reutrns true, output it to file then remove it from input
				if (firstSep == true)
				{
					outFile << "seperator \t\t" << firstSeperator << endl;
					cout << "seperator \t\t" << firstSeperator << endl;
					for (int i = 0; i < inputLength; ++i)
					{
						input[i] = input[i + 1];
					}
					input[inputLength] = '\0';
					inputLength--;
					string input(firstSeperator);
					tokenQueue.push("separator");
					lexemeQueue.push(input);
					firstSep = false;
				}
				// If input ends with ')' or ';' save it for later use then make remove it from input.
				if (input[inputLength - 1] == ')' ||
					input[inputLength - 1] == ';' ||
					input[inputLength - 1] == ',')
				{
					lastSeperator = input[inputLength - 1];
					input[inputLength - 1] = '\0';
					inputLength--;
					lastSep = true;
				}

				// check if seperator.
				if (seperator(input, inputLength) == true)
				{
					outFile << "seperator \t\t";
					cout << "seperator \t\t";
					for (int j = 0; j < inputLength; ++j)
					{
						cout << input[j];
						outFile << input[j];
					}
					cout << endl;
					outFile << endl;
					string inp(input);
					tokenQueue.push("separator");
					lexemeQueue.push(inp);
				}
				// check if operator.
				else if (operators(input, inputLength) == true)
				{
					outFile << "operator \t\t";
					cout << "operator \t\t";
					for (int j = 0; j < inputLength; ++j)
					{
						cout << input[j];
						outFile << input[j];
					}
					cout << endl;
					outFile << endl;
					string inp(input);
					tokenQueue.push("operator");
					lexemeQueue.push(inp);
				}
				// check if keyword.
				else if (keyword(input) == true)
				{
					//cout << input << " is a keyword" << endl;
					outFile << "keyword \t\t";
					cout << "keyword \t\t";
					for (int j = 0; j < inputLength; ++j)
					{
						cout << input[j];
						outFile << input[j];
					}
					outFile << endl;
					cout << endl;
					string inp(input);
					tokenQueue.push("keyword");
					lexemeQueue.push(inp);

					if (lastSep == true)
					{
						outFile << "seperator \t\t" << lastSeperator << endl;
						cout << "seperator \t\t" << lastSeperator << endl;
						lastSep = false;
						string inp(lastSeperator);
						tokenQueue.push("separator");
						lexemeQueue.push(inp);
					}
				}
				// check if identifier
				else if (idFSM(input, inputLength) == true)
				{
					outFile << "identifier \t\t";
					cout << "identifier \t\t";
					for (int j = 0; j < inputLength; ++j)
					{
						cout << input[j];
						outFile << input[j];
					}
					cout << endl;
					outFile << endl;
					string inp(input);
					tokenQueue.push("identifier");
					lexemeQueue.push(inp);

					if (lastSep == true)
					{
						outFile << "seperator \t\t" << lastSeperator << endl;
						cout << "seperator \t\t" << lastSeperator << endl;
						lastSep = false;
						string inp(lastSeperator);
						tokenQueue.push("separator");
						lexemeQueue.push(inp);
					}
				}
				// check if int
				else if (numFSM(input, inputLength) == 1)
				{
					outFile << "integer \t\t";
					cout << "integer \t\t";
					for (int j = 0; j < inputLength; ++j)
					{
						cout << input[j];
						outFile << input[j];
					}
					outFile << endl;
					cout << endl;
					string inp(input);
					tokenQueue.push("integer");
					lexemeQueue.push(inp);
					if (lastSep == true)
					{
						outFile << "seperator \t\t" << lastSeperator << endl;
						cout << "seperator \t\t" << lastSeperator << endl;
						lastSep = false;
						string inp(lastSeperator);
						tokenQueue.push("separator");
						lexemeQueue.push(inp);
					}
				}
				// check if real
				else if (numFSM(input, inputLength) == 4)
				{
					outFile << "real number \t\t";
					cout << "real number \t\t";
					for (int j = 0; j < inputLength; ++j)
					{
						cout << input[j];
						outFile << input[j];
					}
					outFile << endl;
					cout << endl;
					string inp(input);
					tokenQueue.push("real");
					lexemeQueue.push(inp);

					if (lastSep == true)
					{
						outFile << "separator \t\t" << lastSeperator << endl;
						cout << "separator \t\t" << lastSeperator << endl;
						string inp(lastSeperator);
						tokenQueue.push("seperator");
						lexemeQueue.push(inp);
						lastSep = false;
					}
				}
				// If none of the above, it's invalid.
				else
				{
					outFile << "invalid \t\t";
					cout << "invalid \t\t";
					for (int j = 0; j < inputLength; ++j)
					{
						cout << input[j];
						outFile << input[j];
					}
					outFile << endl;
					cout << endl;
					string inp(input);
					tokenQueue.push("invalid");
					lexemeQueue.push(inp);
				}
			}
			// Empty input char array.
			for (int i = 0; i < inputLength; ++i)
			{
				input[i] = '\0';
			}
		}
		else
		{
			// Put the chars c is getting into the char array c.
			input[i] = c;
			i++;

			// Check if there is a '(' or '[' at the start of input.
			if (input[0] == '(')
			{
				// If true, set firstSep to true and save it to firstSeperator for later.
				firstSep = true;
				firstSeperator = input[0];
			}
		}
	}
	outFile.close();
	inFile.close();
}