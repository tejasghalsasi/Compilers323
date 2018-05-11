/*
Tejas Ghalsasi
CPSC 323 Rat18S
*/
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <queue>
#include <stack>
#include "lexer.h"
using namespace std;








void A(), B(), E(), G(), I(), K(), L(), N(), J(), C(), O(), P(), M(), D(),
Q(), R(), S(), T(), U(), V(), W(), X(), Y(), a(), Z(), Zp(), b(), bp(),
c(), d(), F();
void lexerQueue();

string lexeme;
string token;
ofstream outFile;

/* Assignment 3 */
void iA(), iE(), iEp(), iT(), iTp(), iF(), id_insert(string, string),
gen_instr(string, string), back_patch(int);
int get_address(string);
int instr_address = 1;
int id_index = 0;
int memory_address = 2000;
string op_save;	// Used for <Relop>
string id_type;
stack <int> jumpStack;

// <RAT18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>
void A()
{
	// If lexeme = First(<Opt Function Definitions>), @
	if (strcmp(lexeme.c_str(), "@") == 0)
	{
		B();	// Go to <Opt Function Definitions>
	}
	/*else	// Error Message
	{
		cout << "ERROR at '" << lexeme << "'! @ is expected! <RAT18S>" << endl;
	}*/

	// If lexeme = Follow(<Opt Function Definitions>), %%
	if (strcmp(lexeme.c_str(), "%%") == 0)	// Read in %%
	{
		lexerQueue();	// Move to the next item in the queue.
		C();	// Go to <Opt Declaration List>
		D();	// Go to <Statement List>
	}
	else	// Error Message
	{
	//	cout << "ERROR at '" << lexeme << "'! Expecting %%. <RAT18S>" << endl;
	}
}

// <Opt Function Definitions> -> <Function Definitions> | <Empty>
void B()
{
	// If lexeme = First(<Opt Function Definitions>), <Function Definitions>-> @
	if (strcmp(lexeme.c_str(), "@") == 0)
	{
		E();	// <Function Definitions>
	}
	// If lexeme = Follow(<Function Definitions>), %%
	else if (strcmp(lexeme.c_str(), "%%") == 0)
	{
		F();	// <Empty>
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting @ or %%! <Opt Function Definitions>" << endl;
	}
}

// <Function Definitions> -> <Function> | <Function> <Function Definitions>
void E()
{
	// If lexeme = First(<Function>), @
	if (strcmp(lexeme.c_str(), "@") == 0)
	{
		G();	// <Function>
		if (strcmp(lexeme.c_str(), "@") == 0)
		{
			E();	// <Function Definition>
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expected @. <Function Definitions>" << endl;
	}
}

// <Function> -> @<Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>
void G()
{
	if (strcmp(lexeme.c_str(), "@") == 0)
	{
		lexerQueue();
		//H();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting @. <Function>" << endl;
	}

	if (strcmp(token.c_str(), "identifier") == 0)
	{
		lexerQueue();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier! <Function>" << endl;
	}

	if (strcmp(lexeme.c_str(), "(") == 0)
	{
		lexerQueue();
		I();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting '('. <Function>" << endl;
	}

	if (strcmp(lexeme.c_str(), ")") == 0)
	{
		lexerQueue();
		C();
		J();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting ')'. <Function>" << endl;
	}
}

// <Opt Parameter List> -> <Parameter List> | <Empty>
void I()
{
	// If lexeme is an identifier. First(K) = L-> M-> H(identifier)
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		K();
	}
	// If lexeme is part of Follow(I) = ")"
	else if (strcmp(lexeme.c_str(), ")") == 0)
	{
		F();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier or ). <Opt Parameter List>" << endl;
	}
}

// <Parameter List> -> <Parameter> | <Parameter>, <Parameter List>
void K()
{
	// If lexeme is an identifier. First(L) = M-> H(identifier)
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		L();
		if (strcmp(lexeme.c_str(), ",") == 0)
		{
			lexerQueue();
			K();
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier! <Parameter List>" << endl;
	}
}

// <Parameter> -> <IDs> : <Qualifier>
void L()
{
	// If lexeme is an identifier. First(M) = H(identifier)
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		M();
		if (strcmp(lexeme.c_str(), ":") == 0)
		{
			lexerQueue();
			N();
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier! <Parameter>" << endl;
	}
}

// <Qualifier> -> "integer" | "boolean" | "floating"
void N()
{
	if (strcmp(lexeme.c_str(), "integer") == 0) 
	{
		id_type = "integer";
		lexerQueue();
	} 
	else if (strcmp(lexeme.c_str(), "floating") == 0)
	{
		id_type = "floating";
		lexerQueue();
	}
	else if(strcmp(lexeme.c_str(), "true") == 0 || strcmp(lexeme.c_str(), "false") == 0)
	{
		id_type = "boolean";
		lexerQueue();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting either: integer, boolean, or floating." << endl;
	}
}

// <Body> -> { <Statement List> }
void J()
{
	if (strcmp(lexeme.c_str(), "{") == 0)
	{
		lexerQueue();
		D();
		if (strcmp(lexeme.c_str(), "}") != 0)
		{
			cout << "ERROR at '" << lexeme << "'! } is expected! <Body>" << endl;
		}
		else
		{
			lexerQueue();
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! { is expected! <Body>" << endl;
	}
}

// <Opt Declaration List> -> <Declaration List> | <Empty>
void C()
{
	// If lexeme is First(<Declaration List>) = int, bool, or float
	if (strcmp(lexeme.c_str(), "integer") == 0 || strcmp(lexeme.c_str(), "true") == 0 ||
		strcmp(lexeme.c_str(), "false") == 0 || strcmp(lexeme.c_str(), "floating") == 0)
	{
		O();
	}
	// if lexeme is part of the Follow(<Opt Declaration List>)
	else if (strcmp(lexeme.c_str(), "{") == 0 || strcmp(token.c_str(), "identifier") == 0 ||
		strcmp(lexeme.c_str(), "if") == 0 || strcmp(lexeme.c_str(), "return") == 0 ||
		strcmp(lexeme.c_str(), "write") == 0 || strcmp(lexeme.c_str(), "read") == 0 ||
		strcmp(lexeme.c_str(), "while") == 0)
	{
		F();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting integer, boolean, floating, {, if, while, read, write, true, or false! <Opt Declaration List>" << endl;
	}
}

// <Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>
void O()
{
	if (strcmp(lexeme.c_str(), "integer") == 0 || strcmp(lexeme.c_str(), "true") == 0 ||
		strcmp(lexeme.c_str(), "false") == 0 || strcmp(lexeme.c_str(), "floating") == 0)
	{
		P();
		if (strcmp(lexeme.c_str(), ";") == 0)
		{
			lexerQueue();
			O();
		}
	}
}

// <Declaration> -> <Qualifier> <IDs>
void P()
{
	if (strcmp(lexeme.c_str(), "integer") == 0 || strcmp(lexeme.c_str(), "boolean") == 0 ||
		strcmp(lexeme.c_str(), "floating") == 0)
	{
		N();
		M();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting integer, boolean, or floating! <Declaration>" << endl;
	}
}

// <IDs> -> <Identifier> | <Identifier>, <IDs>
void M()
{
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		lexerQueue();
		if (strcmp(lexeme.c_str(), ",") == 0)
		{
			lexerQueue();
			M();
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier! <IDs>" << endl;
	}
}

// <Statement List> -> <Statement> | <Statement> <Statement List>
void D()
{
	if (strcmp(lexeme.c_str(), "{") == 0 || strcmp(token.c_str(), "identifier") == 0 ||
		strcmp(lexeme.c_str(), "if") == 0 || strcmp(lexeme.c_str(), "return") == 0 ||
		strcmp(lexeme.c_str(), "write") == 0 || strcmp(lexeme.c_str(), "read") == 0 ||
		strcmp(lexeme.c_str(), "while") == 0)
	{
		Q();
		if (strcmp(lexeme.c_str(), "{") == 0 || strcmp(token.c_str(), "identifier") == 0 ||
			strcmp(lexeme.c_str(), "if") == 0 || strcmp(lexeme.c_str(), "return") == 0 ||
			strcmp(lexeme.c_str(), "write") == 0 || strcmp(lexeme.c_str(), "read") == 0 ||
			strcmp(lexeme.c_str(), "while") == 0)
		{
			D();
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifer or either: {, if, return, write, read, or while! <Statement List>" << endl;
	}
}

// <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
void Q()
{
	if (strcmp(lexeme.c_str(), "{") == 0 || strcmp(token.c_str(), "identifier") == 0 ||
		strcmp(lexeme.c_str(), "if") == 0 || strcmp(lexeme.c_str(), "return") == 0 ||
		strcmp(lexeme.c_str(), "write") == 0 || strcmp(lexeme.c_str(), "read") == 0 ||
		strcmp(lexeme.c_str(), "while") == 0)
	{
		if (strcmp(lexeme.c_str(), "{") == 0)
		{
			R();
		}
		else if (strcmp(token.c_str(), "identifier") == 0)
		{
			S();
		}
		else if (strcmp(lexeme.c_str(), "if") == 0)
		{
			T();
		}
		else if (strcmp(lexeme.c_str(), "return") == 0)
		{
			U();
		}
		else if (strcmp(lexeme.c_str(), "write") == 0)
		{
			V();
		}
		else if (strcmp(lexeme.c_str(), "while") == 0)
		{
			X();
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier or either: {, if, return, write, or while! <Statement>" << endl;
	}
}

// <Compound> -> { <Statement List> }
void R()
{
	if (strcmp(lexeme.c_str(), "{") == 0)
	{
		lexerQueue();
		D();
		if (strcmp(lexeme.c_str(), "}") != 0)
		{
			cout << "ERROR at '" << lexeme << "'! Expecting a }. <Compound>" << endl;
		}
		else
		{
			lexerQueue();
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting a {. <Compound>" << endl;
	}
}

// <Assign> ->  <Identifier>  : = <Expression>;
void S()
{
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		string save = lexeme;
		id_insert(lexeme, id_type);
		lexerQueue();
		if (strcmp(lexeme.c_str(), ":=") == 0)
		{
			lexerQueue();
			Z();
			int addr = get_address(save);
			gen_instr("POPM", to_string(addr));
			if (strcmp(lexeme.c_str(), ";") != 0)
			{
				cout << "ERROR at " << lexeme << "! Expecting ; <Assign>" << endl;
			}
			else
			{
				lexerQueue();
			}
		}
		else
		{
			cout << "ERROR at " << lexeme << "! Expecting := <Assign>" << endl;
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier. <Assign>" << endl;
	}
}

// <If> -> if (<Condition>) <Statement> fi | if (<Condition>) <Statement> else <Statement> fi
void T()
{
	if (strcmp(lexeme.c_str(), "if") == 0)
	{
		int addr = instr_address;
		lexerQueue();
		if (strcmp(lexeme.c_str(), "(") == 0)
		{
			lexerQueue();
			Y();
			if (strcmp(lexeme.c_str(), ")") == 0)
			{
				lexerQueue();
				Q();
				back_patch(instr_address);
				if (strcmp(lexeme.c_str(), "fi") == 0)
				{
					lexerQueue();
				}
				else if (strcmp(lexeme.c_str(), "else") == 0)
				{
					int addr_jump = instr_address;
					jumpStack.push(instr_address);
					gen_instr("JUMPZ", "nil");
					lexerQueue();
					Q();
					back_patch(instr_address);
					if (strcmp(lexeme.c_str(), "fi") == 0)
					{
						lexerQueue();
					}
					else
					{
						cout << "ERROR at '" << lexeme << "'! Expecting fi. <If>" << endl;
					}
				}
				else
				{
					cout << "ERROR at '" << lexeme << "'! Expecting fi or else. <If>" << endl;
				}
			}
			else
			{
				cout << "ERROR at '" << lexeme << "'! Expecting an ). <If>" << endl;
			}
		}
		else
		{
			cout << "ERROR at '" << lexeme << "'! Expecting an (. <If>" << endl;
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an if. <If>" << endl;
	}
}

// <Return> -> return; | return <Expression>;
void U()
{
	gen_instr("STDIN", "nil");
	lexerQueue();
	if (strcmp(lexeme.c_str(), ";") == 0)
	{
		lexerQueue();
	}
	else
	{
		Z();
		if (strcmp(lexeme.c_str(), ";") != 0)
		{
			cout << "ERROR at '" << lexeme << "'! Expecting ';'. <Return>" << endl;
		}
		else
		{
			lexerQueue();
		}
	}
}

// <Write> -> write (<Expression>);
void V()
{
	if (strcmp(lexeme.c_str(), "write") == 0)
	{
		gen_instr("STDOUT", "nil");
		lexerQueue();
		if (strcmp(lexeme.c_str(), "(") == 0)
		{
			Z();
			if (strcmp(lexeme.c_str(), ")") != 0)
			{
				cout << "ERROR at '" << lexeme << "'! Expecting a ) statement! <Write>" << endl;
			}
			else
			{
				lexerQueue();
				if (strcmp(lexeme.c_str(), ";") != 0)
				{
					cout << "ERROR at '" << lexeme << "'! Expecting a ; ! <Write>" << endl;
				}
				else
				{
					lexerQueue();
				}
			}
		}
		else
		{
			cout << "ERROR at '" << lexeme << "'! Expecting a ( ! <Write>" << endl;
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting a write statement! <Write>" << endl;
	}
}

// <Read> -> read (<IDs>)
void W()
{
	if (strcmp(lexeme.c_str(), "read") == 0)
	{
		gen_instr("STDOUT", "nil");
		lexerQueue();
		if (strcmp(lexeme.c_str(), "("))
		{
			lexerQueue();
			int addr = get_address(lexeme);
			gen_instr("POPM", to_string(addr));
			M();
			if (strcmp(lexeme.c_str(), ")") != 0)
			{
				cout << "ERROR at '" << lexeme << "'! Expecting ) ! <Read>" << endl;
			}
			else
			{
				lexerQueue();
			}
		}
		else
		{
			cout << "ERROR at '" << lexeme << "'! Expecting ( ! <Read>" << endl;
		}
	}
	else
	{
		cout << "ERROR at " << lexeme << "! Expecting a read statement! <Read>" << endl;
	}
}

// <While> -> while (<Condition>) <Statement>
void X()
{
	if (strcmp(lexeme.c_str(), "while") == 0)
	{
		int addr = instr_address;
		gen_instr("LABEL", "nil");
		lexerQueue();
		if (strcmp(lexeme.c_str(), "(") == 0)
		{
			lexerQueue();
			Y();
			if (strcmp(lexeme.c_str(), ")") == 0)
			{
				lexerQueue();
				Q();
				gen_instr("JUMP", to_string(addr));
				back_patch(instr_address);
			}
			else
			{
				cout << "ERROR at '" << lexeme << "'! Expecting a ')' ! <While>" << endl;
			}
		}
		else
		{
			cout << "ERROR at '" << lexeme << "'! Expecting a '(' ! <While>" << endl;
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting a while statement! <While>" << endl;
	}
}

// <Condition> -> <Expression> <Relop> <Expression>
void Y()
{
	if (strcmp(token.c_str(), "identifier") == 0 || strcmp(token.c_str(), "integer") == 0 ||
		strcmp(lexeme.c_str(), "(") == 0 || strcmp(token.c_str(), "real") == 0 ||
		strcmp(lexeme.c_str(), "true") == 0 || strcmp(lexeme.c_str(), "false") == 0)
	{
		Z();
		a();
		Z();
		if (strcmp(op_save.c_str(), ":=") == 0)
		{
			gen_instr("EQU", "nil");
			jumpStack.push(instr_address);
			gen_instr("JUMPZ", "nil");
		}
		else if (strcmp(op_save.c_str(), "/=") == 0)
		{
			gen_instr("NEQ", "nil");
			jumpStack.push(instr_address);
			gen_instr("JUMPZ", "nil");
		}
		else if (strcmp(op_save.c_str(), ">") == 0)
		{
			gen_instr("GRT", "nil");
			jumpStack.push(instr_address);
			gen_instr("JUMPZ", "nil");
		}
		else if (strcmp(op_save.c_str(), "<") == 0)
		{
			gen_instr("LES", "nil");
			jumpStack.push(instr_address);
			gen_instr("JUMPZ", "nil");
		}
		else if (strcmp(op_save.c_str(), "=>") == 0)
		{
			gen_instr("GEQ", "nil");
			jumpStack.push(instr_address);
			gen_instr("JUMPZ", "nil");
		}
		else if (strcmp(op_save.c_str(), "<=") == 0)
		{
			gen_instr("LEQ", "nil");
			jumpStack.push(instr_address);
			gen_instr("JUMPZ", "nil");
		}
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier, integer, real, or statements: integer, (, true, or false! <Condition>" << endl;
	}
}

// <Relop> -> ':=' | '=' | '/=' | '>' | '<' | '=>' | '<='
void a()
{
	if (strcmp(lexeme.c_str(), ":=") == 0 || strcmp(lexeme.c_str(), "/=") == 0 ||
		strcmp(lexeme.c_str(), ">") == 0 || strcmp(lexeme.c_str(), "<") == 0 ||
		strcmp(lexeme.c_str(), "=>") == 0 || strcmp(lexeme.c_str(), "<=") == 0)
	{
		op_save = lexeme;
		lexerQueue();
	}
	else
	{
		cout << "ERROR at " << lexeme << "'! Expecting either: :=, /=, >, <, =>, or <=. <Relop>" << endl;
	}
}

/* <Expression> -> <Expression> + <Term> | <Expression> - <Term> | <Term>
<Expression> -> <Term> <Expression Prime> */
void Z()
{
	if (strcmp(token.c_str(), "identifier") == 0 || strcmp(token.c_str(), "integer") == 0 ||
		strcmp(lexeme.c_str(), "(") == 0 || strcmp(lexeme.c_str(), "real") == 0 ||
		strcmp(lexeme.c_str(), "true") == 0 || strcmp(lexeme.c_str(), "false") == 0)
	{
		b();
		Zp();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier, integer, real, or either: (, true, or false. <Expression>" << endl;
	}
}

// <Expression Prime> -> + <Term> <Expresion Prime> | - <Term> <Expression Prime> | Є
void Zp()
{
	if (strcmp(lexeme.c_str(), "+") == 0 || strcmp(lexeme.c_str(), "-") == 0)
	{
		lexerQueue();
		b();
		Zp();
	}
	else if (strcmp(lexeme.c_str(), ";") == 0 || strcmp(lexeme.c_str(), ")") == 0 ||
		strcmp(lexeme.c_str(), ":=") == 0 || strcmp(lexeme.c_str(), "/=") == 0 ||
		strcmp(lexeme.c_str(), ">") == 0 || strcmp(lexeme.c_str(), "<") == 0 ||
		strcmp(lexeme.c_str(), "=>") == 0 || strcmp(lexeme.c_str(), "<=") == 0)
	{
		F();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting +, -, ;, ), or a! <Expression Prime>" << endl;
	}
}

/* <Term> -> <Term> * <Factor> | <Term> / <Factor> | <Factor>
<Term> -> <Factor> <Term Prime> */
void b()
{
	if (strcmp(token.c_str(), "identifier") == 0 || strcmp(token.c_str(), "integer") == 0 ||
		strcmp(lexeme.c_str(), "(") == 0 || strcmp(lexeme.c_str(), "real") == 0 ||
		strcmp(lexeme.c_str(), "true") == 0 || strcmp(lexeme.c_str(), "false") == 0)
	{
		c();
		bp();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting an identifier, integer, real, or either: (, true, or false! <Term>" << endl;
	}
}

// <Term Prime> -> * <Factor> <Term Prime> | / <Factor> <Term Prime> | Є
void bp()
{
	if (strcmp(lexeme.c_str(), "*") == 0 || strcmp(lexeme.c_str(), "/") == 0)
	{
		lexerQueue();
	}
	else if (strcmp(lexeme.c_str(), "+") == 0 || strcmp(lexeme.c_str(), "-") == 0 ||
		strcmp(lexeme.c_str(), ":=") == 0 || strcmp(lexeme.c_str(), "/=") == 0 ||
		strcmp(lexeme.c_str(), ">") == 0 || strcmp(lexeme.c_str(), "<") == 0 ||
		strcmp(lexeme.c_str(), "=>") == 0 || strcmp(lexeme.c_str(), "<=") == 0 ||
		strcmp(lexeme.c_str(), ")") == 0 || strcmp(lexeme.c_str(), ";") == 0)
	{
		F();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting either * or /. <Term Prime>" << endl;
	}
}

// <Factor> -> - <Primary> | <Primary>
void c()
{
	if (strcmp(lexeme.c_str(), "-") == 0)
	{
		lexerQueue();
		d();
	}
	else if (strcmp(token.c_str(), "identifier") == 0 || strcmp(token.c_str(), "integer") == 0 ||
		strcmp(lexeme.c_str(), "(") == 0 || strcmp(lexeme.c_str(), "real") == 0 ||
		strcmp(lexeme.c_str(), "true") == 0 || strcmp(lexeme.c_str(), "false") == 0)
	{
		d();
	}
	else
	{
		cout << "ERROR at '" << lexeme << "'! Expecting either an identifier, real, or either: -, (, integer, true, or false! <Factor>" << endl;
	}
}

// <Primary> -> <Identifier> | <Integer> | <Identifier>[<IDs>] | (<Expression>) | <Real> | "true" | "false"
void d()
{
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		id_insert(lexeme, id_type);
		int addr = get_address(lexeme);
		gen_instr("PUSHM", to_string(addr));
		lexerQueue();
		if (strcmp(lexeme.c_str(), "[") == 0)
		{
			lexerQueue();
			M();
			if (strcmp(lexeme.c_str(), "]") != 0)
			{
				cout << "ERROR at " << lexeme << "! Expecting ]. <Primary>" << endl;
			}
			else
			{
				lexerQueue();
			}
		}
	}
	else if (strcmp(lexeme.c_str(), "(") == 0)
	{
		cout << "<Primary> -> (<Expression>)" << endl;
		Z();
		// Checking for ending paranthesis.
		if (strcmp(lexeme.c_str(), ")") != 0)
		{
			cout << "ERROR at '" << lexeme << "'! Expecting a ). <Primary>" << endl;
		}
		else
		{
			lexerQueue();
		}
	}
	else if (strcmp(token.c_str(), "integer") == 0)
	{
		gen_instr("PUSHI", lexeme);
		lexerQueue();
	}
	else if (strcmp(lexeme.c_str(), "true") == 0)
	{
		lexerQueue();
	}
	else if (strcmp(lexeme.c_str(), "false") == 0)
	{
		lexerQueue();
	}
	else
	{
		cout << "ERROR at " << lexeme << "! Expecting identifier or integer or either: (, true, or false! <Primary>" << endl;
	}
}

// <Empty> -> Є
void F()
{
	//gen_instr("LABEL", "nil");
}

void lexerQueue()
{
	if (!tokenQueue.empty() && !lexemeQueue.empty())
	{
		token = tokenQueue.front();
		lexeme = lexemeQueue.front();
		tokenQueue.pop();
		lexemeQueue.pop();
		/*if (strcmp(token.c_str(), "keyword") == 0 || strcmp(token.c_str(), "operator") == 0 ||
			strcmp(token.c_str(), "integer") == 0)
		{
			cout << "\nTOKEN: " << token << "\t\tLEXEME: " << lexeme << endl;
			outFile << "\nTOKEN: " << token << "\t\tLEXEME: " << lexeme << endl;
		}
		else
		{
			cout << "\nTOKEN: " << token << "\tLEXEME: " << lexeme << endl;
			outFile << "\nTOKEN: " << token << "\tLEXEME: " << lexeme << endl;
		}*/
	}
}

void parse()
{
	outFile.open("testCaseParseResults.txt");
	lexerQueue();
	A();
	outFile.close();
}





struct Instr_t
{
	int address;
	string op;
	string oprnd;
};

struct Symbol_t
{
	string identifier;
	int memoryLocation;
	string type;
};

Instr_t instr_table[200];
Symbol_t symbol_table[200];

void error_message(string msg)
{
	cout << msg << endl;
}

// A -> id := E {gen_instr(POPM, get_address(id))}
void iA()
{
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		string save = lexeme;
		id_insert(save, id_type);
		lexerQueue();
		if (strcmp(lexeme.c_str(), ":=") == 0)
		{
			lexerQueue();
			iE();
			int addr = get_address(save);
			gen_instr("POPM", to_string(addr));
		}
		else
		{
			error_message(":= expected.");
		}
	}
	else
	{
		error_message("id expected.");
	}
}

// E -> T{ } E'{ }
void iE()
{
	iT();
	iEp();
}

// E' -> + T{ gen_instr(ADD, nil) } E'{ }
void iEp()
{
	if (strcmp(lexeme.c_str(), "+") == 0)
	{
		lexerQueue();
		iT();
		gen_instr("ADD", "nil");
		iEp();
	}
}

// T -> F{ } T'{ }
void iT()
{
	iF();
	iTp();
}

// T' -> *F {gen_instr(MUL, nil) } T'{ }
void iTp()
{
	if (strcmp(lexeme.c_str(), "*") == 0)
	{
		lexerQueue();
		iF();
		gen_instr("MUL", "nil");
		iTp();
	}
}

// F -> id {gen_instr(PUSHM, get_address(id)) }
void iF()
{
	if (strcmp(token.c_str(), "identifier") == 0)
	{
		id_insert(lexeme, id_type);
		int addr = get_address(token);
		gen_instr("PUSHM", to_string(addr));
		lexerQueue();
	}
	else
	{
		error_message("id expected.");
	}
}

void gen_instr(string op, string oprnd)
{
	instr_table[instr_address - 1].address = instr_address;
	instr_table[instr_address - 1].op = op;
	instr_table[instr_address - 1].oprnd = oprnd;
	instr_address++;
}

void id_insert(string id, string type)
{
	// Check to see if there is a duplicate identifier.
	bool check = true;
	int address = 0;
	if (id_index == 0)
	{
		check = false;
		address = memory_address;
	}
	else
	{
		for (int i = 0; i < id_index; ++i)
		{
			if (strcmp(id.c_str(), symbol_table[i].identifier.c_str()) != 0)
			{
				check = false;
				address = memory_address + i + 1;
			}
			else
			{
				check = true;
				break;
			}
		}
	}

	if (check == false)
	{
		symbol_table[id_index].identifier = id;
		symbol_table[id_index].memoryLocation = address;
		symbol_table[id_index].type = type;
		id_index++;
		//memory_address++;
	}
}

int get_address(string save)
{
	int address = 0;
	for (int i = 0; i < id_index; ++i)
	{
		if (strcmp(save.c_str(), symbol_table[i].identifier.c_str()) == 0)
		{
			address = memory_address + i;
		}
	}
	return address;
}

void back_patch(int jump_addr)
{
	int addr = jumpStack.top();
	jumpStack.pop();
	instr_table[addr - 1].oprnd = to_string(jump_addr);
}

/* C -> E R E
   R -> =, /=, >, <, =>, <= */
void condition()
{
	iE();
	Y();
	iE();
	/* Test this code later. giving error
	if (strcmp(lexeme.c_str(), "=") == 0 || strcmp(lexeme.c_str(), "/=") == 0 ||
		strcmp(lexeme.c_str(), ">") == 0 || strcmp(lexeme.c_str(), "<") == 0 ||
		strcmp(lexeme.c_str(), "=>") == 0 || strcmp(lexeme.c_str(), "<=") == 0)
	{
		string op = lexeme;
		lexerQueue();
		iE();
		if (strcmp(lexeme.c_str(), "<") == 0)
		{
			gen_instr("LES", "nil");
			jumpStack.push(instr_address);
		}
		else if (strcmp(lexeme.c_str(), ">") == 0)
		{
			gen_instr("GRT", "nil");
		}
		else if (strcmp(lexeme.c_str(), "=") == 0)
		{
			gen_instr("EQU", "nil");
		}
		else if (strcmp(lexeme.c_str(), "/=") == 0)
		{

		}
		else if (strcmp(lexeme.c_str(), "=>") == 0)
		{

		}
		else if (strcmp(lexeme.c_str(), "<=") == 0)
		{

		}
		else
		{
			gen_instr("LABEL", "");
		}
	}*/
}

void print_table()
{
	
	outFile.open("testCase3Result.txt");
	for (int i = 0; i < instr_address; ++i)
	{
		if (strcmp(instr_table[i].op.c_str(), "") != 0)
		{
			cout << instr_table[i].address << " " << instr_table[i].op << " ";
			outFile << instr_table[i].address << " " << instr_table[i].op << " ";
			if (strcmp(instr_table[i].oprnd.c_str(), "nil") == 0)
			{
				cout << endl;
				outFile << endl;
			}
			else
			{
				cout << instr_table[i].oprnd << endl;
				outFile << instr_table[i].oprnd << endl;
			}
		}
	}
	for (int i = 0; i < id_index; ++i)
	{
		if (strcmp(symbol_table[i].identifier.c_str(), "") != 0)
		{
			cout << symbol_table[i].identifier << "\t\t" << symbol_table[i].memoryLocation << "\t\t" << symbol_table[i].type << endl;
			outFile << symbol_table[i].identifier << "\t\t" << symbol_table[i].memoryLocation << "\t\t" << symbol_table[i].type << endl;
		}
	}
	outFile.close();
}

void icg()
{
	lexerQueue();
	A();
	print_table();
}

int main()
{
	lexer();
	icg();
	//system("pause");
}