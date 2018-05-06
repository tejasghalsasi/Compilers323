//TEJAS GHALSASI 
import java.util.*;
import java.io.*;

public class topDownParse
{

static String lexeme="";
static String token="";
static Queue <String> tokenQueue= new LinkedList<String>();
static Queue <String> lexemeQueue=new LinkedList<String>();

// <Rat18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>
static void A()
{
	// If lexeme = First(<Opt Function Definitions>), @
	if (lexeme.compareToIgnoreCase("@") == 0)
	{
		System.out.println("<RAT18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>");
		B();	// Go to <Opt Function Definitions>
	}
	else	// Error Message
	{
		System.out.println("ERROR at '" + lexeme + "'! @ is expected! <RAT18S>");
		// "<RAT18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" );
	}
	// If lexeme = Follow(<Opt Function Definitions>), %%
	if (lexeme.compareToIgnoreCase("%%") == 0)	// Read in %%
	{
		lexerQueue();	// Move to the next item in the queue.
		C();	// Go to <Opt Declaration List>
		D();	// Go to <Statement List>
	}
	else	// Error Message
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting %%. <RAT18S>");
	}
}

// <Opt Function Definitions> -> <Function Definitions> | <Empty>
static void B()
{
	// If lexeme = First(<Opt Function Definitions>), <Function Definitions>-> @
	if (lexeme.compareToIgnoreCase("@") == 0)
	{
		System.out.println("<Opt Function Definitions> -> <Function Definitions>");
		E();	// <Function Definitions>
	}
	// If lexeme = Follow(<Function Definitions>), %%
	else if (lexeme.compareToIgnoreCase("%%") == 0 )
	{
		System.out.println("<Opt Function Definitions> -> <Empty>" );
		F();	// <Empty>
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting @ or %%! <Opt Function Definitions>");
		
	}
}

// <Function Definitions> -> <Function> | <Function> <Function Definitions>
static void E()
{
	// If lexeme = First(<Function>), @
	if (lexeme.compareToIgnoreCase("@") == 0)
	{
		System.out.println("<Function Definitions> -> <Function>" );
		
		G();	// <Function>
		if (lexeme.compareToIgnoreCase("@") == 0)
		{
			System.out.println("<Function Definitions> -> <Function> <Function Definitions>" );
			E();	// <Function Definition>
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expected @. <Function Definitions>" );
	}
}

// <Function> -> @<Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>
static void G()
{
	if (lexeme.compareToIgnoreCase("@") == 0)
	{
		System.out.println("<Function> -> @<Identifier> (<Opt Declaration List>) <Opt Declaration List> <Body>" );
		lexerQueue();
		//H();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting @. <Function>" );
	
	}

	if (lexeme.compareToIgnoreCase("identifier") == 0)
	{
		lexerQueue();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier! <Function>" );
		
	}

	if (lexeme.compareToIgnoreCase("(") == 0)
	{
		lexerQueue();
		I();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting '('. <Function>" );
		
	}

	if (lexeme.compareToIgnoreCase(")") == 0)
	{
		lexerQueue();
		C();
		J();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting ')'. <Function>" );
		
	}
}

// <Opt Parameter List> -> <Parameter List> | <Empty>
static void I()
{
	// If lexeme is an identifier. First(K) = L-> M-> H(identifier)
	if (token.compareToIgnoreCase("identifier") == 0)
	{
		System.out.println("<Opt Parameter List> -> <Parameter List>" );
		
		K();
	}
	// If lexeme is part of Follow(I) = ")"
	else if (lexeme.compareToIgnoreCase(")") == 0)
	{
		System.out.println("<Opt Parameter List> -> <Empty>" );
		
		F();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier or ). <Opt Parameter List>" );
		
	}
}

// <Parameter List> -> <Parameter> | <Parameter>, <Parameter List>
static void K()
{
	// If lexeme is an identifier. First(L) = M-> H(identifier)
	if (token.compareToIgnoreCase("identifier") == 0)
	{
		System.out.println("<Parameter List> -> <Parameter>" );
		
		L();
		if (lexeme.compareToIgnoreCase(",") == 0)
		{
			System.out.println("<Parameter List> -> <Parameter>, <Parameter List>" );
			
			lexerQueue();
			K();
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier! <Parameter List>" );
		
	}
}

// <Parameter> -> <IDs> : <Qualifier>
static void L()
{
	// If lexeme is an identifier. First(M) = H(identifier)
	if (token.compareToIgnoreCase("identifier") == 0)
	{
		System.out.println("<Parameters> -> <IDs> : <Qualifier>" );
		
		M();
		if (lexeme.compareToIgnoreCase(":") == 0)
		{
			lexerQueue();
			N();
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier! <Parameter>" );
		
	}
}

// <Qualifier> -> "integer" | "boolean" | "floating"
static void N()
{
	if (lexeme.compareToIgnoreCase("integer") == 0 || lexeme.compareToIgnoreCase("true") == 0 ||
		lexeme.compareToIgnoreCase("false") == 0 || lexeme.compareToIgnoreCase("floating") == 0)
	{
		System.out.println("<Qualifier> -> " + lexeme );
		lexerQueue();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting either: integer, boolean, or floating." );
	}
}

// <Body> -> { <Statement List> }
static void J()
{
	if (lexeme.compareToIgnoreCase("{") == 0)
	{
		System.out.println("<Body> -> { <IDs> }" );
	
		lexerQueue();
		D();
		if(lexeme.compareToIgnoreCase("}") != 0)
		{
			System.out.println("ERROR at '" + lexeme + "'! } is expected! <Body>" );
	
		}
		else
		{
			lexerQueue();
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! { is expected! <Body>" );
	
	}
}

// <Opt Declaration List> -> <Declaration List> | <Empty>
static void C()
{
	// If lexeme is First(<Declaration List>) = int, bool, or float
	if (lexeme.compareToIgnoreCase("integer") == 0 || lexeme.compareToIgnoreCase("true") == 0 || 
		lexeme.compareToIgnoreCase("false") == 0 || lexeme.compareToIgnoreCase("floating") == 0)
	{
		System.out.println("<Opt Declaration List> -> <Declaration List>" );
	
		O();
	}
	// if lexeme is part of the Follow(<Opt Declaration List>)
	else if (lexeme.compareToIgnoreCase("{") == 0 || token.compareToIgnoreCase("identifier") == 0 ||
		lexeme.compareToIgnoreCase("if") == 0 || lexeme.compareToIgnoreCase("return") == 0 ||
		lexeme.compareToIgnoreCase("write") == 0 || lexeme.compareToIgnoreCase("read") == 0 ||
		lexeme.compareToIgnoreCase("while") == 0)
	{
		System.out.println("<Opt Declaration List> -> <Empty>" );
	
		F();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting integer, boolean, floating, {, if, while, read, write, true, or false! <Opt Declaration List>" );
	
	}
}

// <Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>
static void O()
{
	if (lexeme.compareToIgnoreCase("integer") == 0 || lexeme.compareToIgnoreCase("true") == 0 ||
		lexeme.compareToIgnoreCase("false") == 0 || lexeme.compareToIgnoreCase("floating") == 0)
	{
		System.out.println("<Declaration List> -> <Declaration>;" );
	
		P();
		if (lexeme.compareToIgnoreCase(";") == 0)
		{
			System.out.println("<Declaration List> -> <Declaration>; <Declaration List>" );
	
			lexerQueue();
			O();
		}
	}
}


// <Declaration> -> <Qualifier> <IDs>
static void P()
{
	if (lexeme.compareToIgnoreCase("integer") == 0 || lexeme.compareToIgnoreCase("boolean") == 0 ||
		lexeme.compareToIgnoreCase("floating") == 0)
	{
		System.out.println("<Declaration> -> <Qualifier> <IDs>" );
	
		N();
		M();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting integer, boolean, or floating! <Declaration>" );
	
	}
}

// <IDs> -> <Identifier> | <Identifier>, <IDs>
static void M()
{
	if (token.compareToIgnoreCase("identifier") == 0)
	{
		System.out.println("<IDs> -> <Identifier>" );
	
		lexerQueue();
		if (lexeme.compareToIgnoreCase(",") == 0)
		{
			System.out.println("<IDs> -> <Identifier>, <IDs>" );
	
			lexerQueue();
			M();
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier! <IDs>" );
	
	}
}

// <Statement List> -> <Statement> | <Statement> <Statement List>
static void D()
{
	if (lexeme.compareToIgnoreCase("{") == 0 || token.compareToIgnoreCase("identifier") == 0 ||
		lexeme.compareToIgnoreCase("if") == 0 || lexeme.compareToIgnoreCase("return") == 0 ||
		lexeme.compareToIgnoreCase("write") == 0 || lexeme.compareToIgnoreCase("read") == 0 ||
		lexeme.compareToIgnoreCase("while") == 0)
	{
		System.out.println("<Statement List> -> <Statement>" );
	
		Q();
		if (lexeme.compareToIgnoreCase("{") == 0 || token.compareToIgnoreCase("identifier") == 0 ||
			lexeme.compareToIgnoreCase("if") == 0 || lexeme.compareToIgnoreCase("return") == 0 ||
			lexeme.compareToIgnoreCase("write") == 0 || lexeme.compareToIgnoreCase("read") == 0 ||
			lexeme.compareToIgnoreCase("while") == 0)
		{
			System.out.println("<Statement List> -> <Statement> <Statement List>" );
	
			D();
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifer or either: {, if, return, write, read, or while! <Statement List>" );
	
	}
}

// <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
static void Q()
{
	if (lexeme.compareToIgnoreCase("{") == 0 || token.compareToIgnoreCase("identifier") == 0 || 
		lexeme.compareToIgnoreCase("if") == 0 || lexeme.compareToIgnoreCase("return") == 0 || 
		lexeme.compareToIgnoreCase("write") == 0 || lexeme.compareToIgnoreCase("read") == 0 ||
		lexeme.compareToIgnoreCase("while") == 0)
	{
		if (lexeme.compareToIgnoreCase("{") == 0)
		{
			System.out.println("<Statement> -> <Compound>" );
	
			R();
		}
		else if (token.compareToIgnoreCase("identifier") == 0)
		{
			System.out.println("<Statement> -> <Assign>" );
	
			S();
		}
		else if (lexeme.compareToIgnoreCase("if") == 0)
		{
			System.out.println("<Statement> -> <If>" );
	
			T();
		}
		else if (lexeme.compareToIgnoreCase("return") == 0)
		{
			System.out.println("<Statement> -> <Return>" );
	
			U();
		}
		else if (lexeme.compareToIgnoreCase("write") == 0)
		{
			System.out.println("<Statement> -> <Write>" );
	
			V();
		}
		else if (lexeme.compareToIgnoreCase("while") == 0)
		{
			System.out.println("<Statement> -> <While>" );
	
			X();
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier or either: {, if, return, write, or while! <Statement>" );
	
	}
}

// <Compound> -> { <Statement List> }
static void R()
{
	if (lexeme.compareToIgnoreCase("{") == 0)
	{
		System.out.println("<Compound> -> { <Statement List> }" );
	
		lexerQueue();
		D();
		if (lexeme.compareToIgnoreCase("}") != 0)
		{
			System.out.println("ERROR at '" + lexeme + "'! Expecting a }. <Compound>" );
	
		}
		else
		{
			lexerQueue();
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting a {. <Compound>" );
	
	}
}

// <Assign> -> <Identifier> := <Expression> ;
static void S()
{
	if (token.compareToIgnoreCase("identifier") == 0)
	{
		System.out.println("<Assign> -> <Identifier>" );
	
		lexerQueue();
		if (lexeme.compareToIgnoreCase(":=") == 0)
		{
			lexerQueue();
			Z();
			if (lexeme.compareToIgnoreCase(";") != 0)
			{
				System.out.println("ERROR at " + lexeme + "! Expecting ; <Assign>" );
	
			}
			else
			{
				lexerQueue();
			}
		}
		else
		{
			System.out.println("ERROR at " + lexeme + "! Expecting := <Assign>" );
	
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier. <Assign>" );
	
	}
}

// <If> -> if (<Condition>) <Statement> fi | if (<Condition>) <Statement> else <Statement> fi
static void T()
{
	if (lexeme.compareToIgnoreCase("if") == 0)
	{
		System.out.println("<If> -> if (<Condition>) <Statement>" );
	
		lexerQueue();
		if (lexeme.compareToIgnoreCase("(") == 0)
		{
			lexerQueue();
			Y();
			if (lexeme.compareToIgnoreCase(")") == 0)
			{
				lexerQueue();
				Q();
				if (lexeme.compareToIgnoreCase("fi") == 0)
				{
					System.out.println("<ID> -> if (<Condition>) <Statement> fi" );
	
					lexerQueue();
				}
				else if (lexeme.compareToIgnoreCase("else") == 0)
				{
					System.out.println("if (<Condition>) <Statement> else <Statement> fi" );
	
					lexerQueue();
					Q();
					if (lexeme.compareToIgnoreCase("fi") == 0)
					{
						lexerQueue();
					}
					else
					{
						System.out.println("ERROR at '" + lexeme + "'! Expecting fi. <If>" );
	
					}
				}
				else
				{
					System.out.println("ERROR at '" + lexeme + "'! Expecting fi or else. <If>" );
	
				}
			}
			else
			{
				System.out.println("ERROR at '" + lexeme + "'! Expecting an ). <If>" );
	
			}
		}
		else
		{
			System.out.println("ERROR at '" + lexeme + "'! Expecting an (. <If>" );
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an if. <If>" );
	}
}

// <Return> -> return; | return <Expression>;
static void U()
{
	lexerQueue();
	if (lexeme.compareToIgnoreCase(";") == 0)
	{
		System.out.println("<Return> -> return;" );
		lexerQueue();
	}
	else
	{
		System.out.println("<Return> -> return <Expression>;" );
		Z();
		if (lexeme.compareToIgnoreCase(";") != 0)
		{
			System.out.println("ERROR at '" + lexeme + "'! Expecting ';'. <Return>" );
		}
		else
		{
			lexerQueue();
		}
	}
}

// <Write> -> write (<Expression>);
static void V()
{
	if (lexeme.compareToIgnoreCase("write") == 0)
	{
		System.out.println("<Write> -> write (<Expression>);" );
		lexerQueue();
		if (lexeme.compareToIgnoreCase("(") == 0)
		{
			Z();
			if (lexeme.compareToIgnoreCase(")") != 0)
			{
				System.out.println("ERROR at '" + lexeme + "'! Expecting a ) statement! <Write>" );
			}
			else
			{
				lexerQueue();
				if (lexeme.compareToIgnoreCase(";") != 0)
				{
					System.out.println("ERROR at '" + lexeme + "'! Expecting a ; ! <Write>" );
				}
				else
				{
					lexerQueue();
				}
			}
		}
		else
		{
			System.out.println("ERROR at '" + lexeme + "'! Expecting a ( ! <Write>" );
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting a write statement! <Write>" );
	}
}

// <Read> -> read (<IDs>)
static void W()
{
	if (lexeme.compareToIgnoreCase("read") == 0)
	{
		System.out.println("<Read> -> read (<IDs>)" );
    	lexerQueue();
		if (lexeme.compareToIgnoreCase("(")!= 0)
		{
			lexerQueue();
			M();
			if (lexeme.compareToIgnoreCase(")") != 0)
			{
				System.out.println("ERROR at '" + lexeme + "'! Expecting ) ! <Read>" );
			}
			else
			{
				lexerQueue();
			}
		}
		else
		{
			System.out.println("ERROR at '" + lexeme + "'! Expecting ( ! <Read>" );
		}
	}
	else
	{
		System.out.println("ERROR at " + lexeme + "! Expecting a read statement! <Read>" );
		}
}

// <While> -> while (<Condition>) <Statement>
static void X()
{
	if (lexeme.compareToIgnoreCase("while") == 0)
	{
		System.out.println("<While> -> while (<Condition>) <Statement>" );
		lexerQueue();
		if (lexeme.compareToIgnoreCase("(") == 0)
		{
			lexerQueue();
			Y();
			if(lexeme.compareToIgnoreCase(")") == 0)
			{
				lexerQueue();
				Q();
			}
			else
			{
				System.out.println("ERROR at '" + lexeme + "'! Expecting a ')' ! <While>" );
			}
		}
		else
		{
			System.out.println("ERROR at '" + lexeme + "'! Expecting a '(' ! <While>" );
		}
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting a while statement! <While>" );
	}
}

// <Condition> -> <Expression> <Relop> <Expression>
static void Y()
{
	if (token.compareToIgnoreCase("identifier") == 0 || token.compareToIgnoreCase( "integer") == 0 ||
		token.compareToIgnoreCase("(") == 0 || token.compareToIgnoreCase("real") == 0 ||
		lexeme.compareToIgnoreCase("true") == 0 ||lexeme.compareToIgnoreCase("false") == 0)
	{
		System.out.println("<Condition> -> <Expression> <Relop> <Expression>" );
		Z();
		a();
		Z();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier, integer, real, or statements: integer, (, true, or false! <Condition>" );
	}
}

// <Relop> -> := | '=' | '/=' | '>' | '<' | '=>' | '<='
static void a()
{
	if (lexeme.compareToIgnoreCase(":=") == 0 || lexeme.compareToIgnoreCase("/=") == 0 ||
		lexeme.compareToIgnoreCase(">") == 0 || lexeme.compareToIgnoreCase("<") == 0 ||
		lexeme.compareToIgnoreCase("=>") == 0 || lexeme.compareToIgnoreCase("<=") == 0)
	{
		System.out.println("<Relop> -> " + lexeme );
		lexerQueue();
	}
	else
	{
		System.out.println("ERROR at " + lexeme + "'! Expecting either: :=, /=, >, <, =>, or <=. <Relop>" );
	}
}

/* <Expression> -> <Expression> + <Term> | <Expression> - <Term> | <Term>
   <Expression> -> <Term> <Expression Prime> */
static void Z()
{
	if (token.compareToIgnoreCase("identifier") == 0 || token.compareToIgnoreCase("integer") == 0 ||
		lexeme.compareToIgnoreCase("(") == 0 || lexeme.compareToIgnoreCase("real") == 0 ||
		lexeme.compareToIgnoreCase("true") == 0 || lexeme.compareToIgnoreCase("false") == 0)
	{
		System.out.println("<Expression> -> <Term> <Expression Prime>" );
		b();
		Zp();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier, integer, real, or either: (, true, or false. <Expression>" );
	}
}

// <Expression Prime> -> + <Term> <Expresion Prime> | - <Term> <Expression Prime> | Є
static void Zp()
{
	if (lexeme.compareToIgnoreCase("+") == 0 || lexeme.compareToIgnoreCase("-") == 0)
	{
		System.out.println("<Expression Prime> -> " + lexeme + " <Term> <Expression Prime>" );
		lexerQueue();
		b();
		Zp();
	}
	else if (lexeme.compareToIgnoreCase(";") == 0 || lexeme.compareToIgnoreCase(")") == 0 ||
		lexeme.compareToIgnoreCase(":=") == 0 || lexeme.compareToIgnoreCase("/=") == 0 ||
		lexeme.compareToIgnoreCase(">") == 0 || lexeme.compareToIgnoreCase("<") == 0 ||
		lexeme.compareToIgnoreCase("=>") == 0 || lexeme.compareToIgnoreCase("<=") == 0)
	{
		System.out.println("<Expression Prime> -> <Empty>" );
		F();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting +, -, ;, ), or a! <Expression Prime>" );
	}
}

/* <Term> -> <Term> * <Factor> | <Term> / <Factor> | <Factor>
   <Term> -> <Factor> <Term Prime> */
static void b()
{
	if (token.compareToIgnoreCase("identifier") == 0 || token.compareToIgnoreCase("integer") == 0 ||
		lexeme.compareToIgnoreCase("(") == 0 || lexeme.compareToIgnoreCase("real") == 0 ||
		lexeme.compareToIgnoreCase("true") == 0 || lexeme.compareToIgnoreCase("false") == 0)
	{
		System.out.println("<Term> -> <Factor> <Term Prime>" );
		c();
		bp();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting an identifier, integer, real, or either: (, true, or false! <Term>" );
	}
}

// <Term Prime> -> * <Factor> <Term Prime> | / <Factor> <Term Prime> | Є
static void bp()
{
	if (lexeme.compareToIgnoreCase("*") == 0 || lexeme.compareToIgnoreCase("/") == 0)
	{
		System.out.println("<Expression Prime> -> " + lexeme + " <Term> <Expression Prime>" );
		lexerQueue();
	}
	else if (lexeme.compareToIgnoreCase("+") == 0 || lexeme.compareToIgnoreCase("-") == 0 ||
		lexeme.compareToIgnoreCase(":=") == 0 || lexeme.compareToIgnoreCase("/=") == 0 ||
		lexeme.compareToIgnoreCase(">") == 0 || lexeme.compareToIgnoreCase("<") == 0 ||
		lexeme.compareToIgnoreCase("=>") == 0 || lexeme.compareToIgnoreCase("<=") == 0 ||
		lexeme.compareToIgnoreCase(")") == 0 || lexeme.compareToIgnoreCase(";") == 0)
	{
		System.out.println("<Term Prime> -> <Empty>" );
		F();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting either * or /. <Term Prime>" );
	}
}

// <Factor> -> - <Primary> | <Primary>
static void c()
{
	if (lexeme.compareToIgnoreCase("-") == 0)
	{
		System.out.println("<Factor> -> - <Primary>" );
		lexerQueue();
		d();
	}
	else if(token.compareToIgnoreCase("identifier") == 0 || token.compareToIgnoreCase("integer") == 0 ||
		lexeme.compareToIgnoreCase("(") == 0 || lexeme.compareToIgnoreCase("real") == 0 ||
		lexeme.compareToIgnoreCase("true") == 0 || lexeme.compareToIgnoreCase("false") == 0)
	{
		System.out.println("<Factor> -> <Primary>" );
		d();
	}
	else
	{
		System.out.println("ERROR at '" + lexeme + "'! Expecting either an identifier, real, or either: -, (, integer, true, or false! <Factor>" );
	}
}

// <Primary> -> <Identifier> | <Integer> | <Identifier>[<IDs>] | (<Expression>) | <Real> | "true" | "false"
static void d()
{
	if (token.compareToIgnoreCase("identifier") == 0)
	{
		System.out.println("<Primary> -> <Identifier>" );
		lexerQueue();
		if (lexeme.compareToIgnoreCase("[") == 0)
		{
			System.out.println("<Primary> -> <Identifier> [ <IDs> ]" );
			lexerQueue();
			M();
			if (lexeme.compareToIgnoreCase("]") != 0)
			{
				System.out.println("ERROR at " + lexeme + "! Expecting ]. <Primary>" );
			}
			else
			{
				lexerQueue();
			}
		}
	}
	else if (lexeme.compareToIgnoreCase("(") == 0)
	{
		System.out.println("<Primary> -> (<Expression>)" );
		Z();
		// Checking for ending paranthesis.
		if (lexeme.compareToIgnoreCase(")") != 0)
		{
			System.out.println("ERROR at '" + lexeme + "'! Expecting a ). <Primary>" );
		}
		else
		{
			lexerQueue();
		}
	}
	else if (token.compareToIgnoreCase("integer") == 0)
	{
		System.out.println("<Primary> -> <Integer>" );
		lexerQueue();
	}
	else if (lexeme.compareToIgnoreCase("true") == 0)
	{
		System.out.println("<Primary> -> true" );
		lexerQueue();
	}
	else if (lexeme.compareToIgnoreCase("false") == 0)
	{
		System.out.println("<Primary> -> false" );
		lexerQueue();
	}
	else
	{
		System.out.println("ERROR at " + lexeme + "! Expecting identifier or integer or either: (, true, or false! <Primary>" );
	}
}

// <Empty> -> Є
static void F()
{
	System.out.println("<Empty> -> Epsilon" );
}

static void lexerQueue()
{
	if (!tokenQueue.isEmpty() && !lexemeQueue.isEmpty())
	{
		token = tokenQueue.peek();
		lexeme = lexemeQueue.peek();
		tokenQueue.poll();
		lexemeQueue.poll();
		if (token.compareToIgnoreCase("keyword") == 0 || token.compareToIgnoreCase("operator") == 0 ||
			token.compareToIgnoreCase("integer") == 0)
		{
			System.out.println("\nTOKEN: " + token + "\t\tLEXEME: " + lexeme );
		}
		else
		{
			System.out.println("\nTOKEN: " + token + "\tLEXEME: " + lexeme );
		}
	}
}

static void parse()
{
     Scanner sc=null;
        try
		{
    sc = new Scanner( new File("abc.txt"));
        }
		catch(Exception e)
		{
			e.printStackTrace();
		}
	lexerQueue();
	A();
	sc.close();
}

public static void main(String args[])
{
	lexicalAnalyzer a=new lexicalAnalyzer();
	a.lexer();
	parse();
	
}
}