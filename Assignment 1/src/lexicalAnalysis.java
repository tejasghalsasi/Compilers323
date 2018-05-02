/** 
Tejas Ghalsasi

This class is a lexical analyzer
We use the Java Enum to define the grammer 
The function "driver" operates the DFA. 
The function "nextState" returns the next state given the current state and the input character.
The functions "driver", "getToken" remain the same.
**/


public abstract class lexicalAnalysis extends IO
{					
	public enum State 
       	{ 
	  // non-final states     ordinal number

		Start,             // 0
		Period,            // 1
		E,                 // 2

	  // final states

		identifier,                // 4
		Integer,               // 5
		Real,             // 6
		Add,               // 8
		Sub,               // 9
		Mul,               // 10
		Div,               // 11
		separator,            // 12
		RParen,            // 13
		operator,				   // 14 
		And,			   // 15 
		Not,			   // 16 
		LBrace,			   // 17
		RBrace,			   // 18
		lt,				   // 19
		gt,				   // 20
		eq,				   // 21
		le,				   // 22
		Keyword_while,
		Keyword_for,
		ge,				   // 23
		keyword,
		Keyword_if,	//keyword if
		Keyword_null,	//keyword null
		Keyword_class,	//keyword class
		UNDEF;
		

		private boolean isFinal()
		{
			return ( this.compareTo(State.identifier) >= 0 );  
		}	
	}

	//  we use static char c; to convert the variable a to the char type
	
	public static String t; // holds an extracted token
	public static State state; // the current state of the FA

	private static int driver()

	// This is the driver of the FA. 
	{
		State nextSt;

		t = "";
		state = State.Start;

		if ( Character.isWhitespace((char) a) )
			a = getChar();
		if ( a == -1 ) 
			return -1;

		while ( a != -1 ) 
		{
			c = (char) a;
			nextSt = nextState( state, c );
			if ( nextSt == State.UNDEF )
			{
				if ( state.isFinal() )
					return 1; 
				else 
				{
					t = t+c;
					a = getNextChar();
					return 0; 
				}
			}
			else 
			{
				state = nextSt;
				t = t+c;
				a = getNextChar();
			}
		}
			if ( state.isFinal() )
			return 1; 
		else
			return 0; 
	}

	public static void getToken()

	{
		int i = driver();
		if ( i == 0 )
			displayln(t + " : Lexical Error, invalid token");
	}

	private static State nextState(State s, char c)

	{
		
		
		switch( state )
		{
		case Start:
			
			if ( Character.isLetter(c) )	
				return State.identifier;
			else if ( Character.isDigit(c) )
				return State.Integer;
			else if ( c == '<')	
				return State.operator;	
			else if ( c == '>')
				return State.operator;	
			else if ( c == '=')
				return State.operator;
			else if ( c == '+' )
				return State.operator;
			else if ( c == '-' )
				return State.operator;
			else if ( c == '*' )
				return State.operator;	
			else if ( c == '/' )
				return State.operator;	
			else if ( c == '(' || c == ')' )
				return State.separator;	
			else if ( c == '|' )		
				return State.operator;
			else if ( c == '&' )		
				return State.operator;
			else if ( c == '!' )		
				return State.operator;
			else if ( c == '{')
				return State.separator;	
			else if ( c == '}')
				return State.separator;
			else if( c == '.' )			
				return State.Period;
			else
				return State.UNDEF;
		
		case lt:
			if( c == '=' )	
				 return State.operator;
			else 
				return State.UNDEF;
		case gt:
			if ( c== '=')
				return State.operator;
			else 
				return State.UNDEF;
		case identifier:
			if ( (t + c).equals("if") )
				return State.Keyword_if;
			else if ( (t + c).equals("null") )
				return State.Keyword_null;
			else if ( (t + c).equals("class") )
				return State.Keyword_class;
			else if ( Character.isLetterOrDigit(c) )
				return State.identifier;
			else
				return State.UNDEF;
		//----------------------------   if_keyword DFA state 

		case Keyword_if:	//if
			if( Character.isLetterOrDigit(c))
				return State.keyword;
			else 
				return State.UNDEF;
		
		case Keyword_while:	
			if( Character.isLetterOrDigit(c))
				return State.keyword;
			else 
				return State.UNDEF;
				
		case Keyword_for:	//if
			if( Character.isLetterOrDigit(c))
				return State.keyword;
			else 
				return State.UNDEF;
				
				
		
		
		//---------------------------- null_keyword DFA state 
		case Keyword_null:	//null
			if( Character.isLetterOrDigit(c))
				return State.keyword;
			else 
				return State.UNDEF;
		//---------------------------- class_keyword DFA state 
		case Keyword_class:	//class
			if( Character.isLetterOrDigit(c))
				return State.keyword;
			else 
				return State.UNDEF;

			
		case Add:					
			if ( Character.isDigit(c) )
				return State.Integer;
			else if ( c == '.')
				return State.Period;
			else 
				return State.UNDEF;
		case Period:
			if ( Character.isDigit(c) )
				return State.Real;
			else
				return State.UNDEF;
		case Sub:
			if ( Character.isDigit(c) )
				return State.Integer;
			else if ( c == '.' )
				return State.Period;
			else 
				return State.UNDEF;
		case Real:
			if ( Character.isDigit(c) )
				return State.Real;
			else if ( c == 'e' || c == 'E' )
				return State.E;
		case Integer:	
			if ( Character.isDigit(c) )
				return State.Integer;
			else if ( c == '.' )
				return State.Real;
			else if ( c == 'e' || c == 'E' )
				return State.E;
			else
				return State.UNDEF;
		case E:
			if ( Character.isDigit(c) )
				return State.Real;
			else if ( c == '+' || c == '-' )
				return State.operator;
		
		default:
			return State.UNDEF;
		}
	} // end nextState

	public static void main(String argv[])

	{		

		setIO( "lexicalinput.txt", "lexer_output.txt" );
		
		int i;

		while ( a != -1 ) // while "a" is not end-of-stream
		{
			i = driver(); // extract the next token
			if ( i == 1 )
				displayln( t+"   : "+state.toString() );
			else if ( i == 0 )
				displayln( t+" : Lexical Error, invalid token");
		} 

		closeIO();
	}
}


/*
earlier Map Approach

import java.util.*;
import java.io.File;


public class lexicalAnalysis {
    
    
    static String[] do_lex(String[] input)
    {
    String[] results=new String[input.length];
    
	    Map<String, String> keywords = new HashMap<String, String>()
	{
	    {
		put("keyword1", "int");
		put("keyword3", "float");
		put("keyword4", "boolean");
		put("keyword5", "if");
		put("keyword6", "else");
		put("keyword7", "elseif");
		put("keyword8", "while");
		put("keyword9", "return");
		put("keyword10", "get");
		put("keyword11", "put");
		put("keyword12", "function");
		put("keyword13", "for");
		
	    }
	};
    
	    Map<String, String> separator = new HashMap<String, String>()
	{
	    {
		put("separator1", "(");
		put("separator2", ")");
		put("separator3", "{");
		put("separator4", "}");
		put("separator5", ";");
		put("separator6", ",");
		put("separator7", "=");
			    }
	};
    
        Map<String, String> operator = new HashMap<String, String>()
	{
	    {
		put("operator1", "/");
		put("operator2", "+");
		put("operator3", "-");
		put("operator4", "*");
		put("operator5", "%");
		put("operator6", ">");
		put("operator7", "<");
		put("operator8", "&");
		put("operator9", "|");		
	}
    };
    boolean comment_open=false;
    for(int i=0;i<input.length;i++)
    {
    if((!input[i].equals("!")))
    {
    if(comment_open==false)
    comment_open=true;
    else
    comment_open=false;
    }
    	if((!input[i].equals("")) && (!input[i].equals(null)) && !comment_open)
    	{
    	if(keywords.containsValue(input[i]))
    	{
    		results[i]="keyword	"+input[i];	
    	System.out.println("Results when "+input[i]+" is input and result array is "+results[i]);

    	}
    	else if(separator.containsValue(input[i]))
    	{
   		results[i]="separator	"+input[i];   		
    	System.out.println("Results when "+input[i]+" is input and result array is "+results[i]);

    	}
    	else if(operator.containsValue(input[i]))
    	{
  		results[i]="operator	"+input[i];
    	System.out.println("Results when "+input[i]+" is input and result array is "+results[i]);
    	}
    	else
    	{
    		results[i]="identifier	"+input[i];
    	System.out.println("Results when "+input[i]+" is input and result array is "+results[i]);
    	}
    	}


    }
            
    return results;
    }
    
    public static void main(String[] args) throws Exception {
 
 	String text="";	       
        Scanner sc = new Scanner( new File("lexicalinput.txt"));

//System.out.println("before while");
      
    while (sc.hasNextLine())
    {
  //  System.out.println("inside while");
      text=(text+sc.nextLine());
     } 
     String[] textarr=text.split(" ");
  //    System.out.println("after while");
	//System.out.println("Input code= \n"+Arrays.toString(textarr));
 //       for(int i=0;i<textarr.length;i++)
   //     {
     //   	System.out.print(textarr[i]);
       // }
        System.out.println("Token			Lexeme");
        System.out.println("------------------------------------ \n");
        
        
        if(text.length() < 1) {
            System.out.println("Usage: java Lexer \"((some Scheme) (code to) lex)\".");
            return;
        }
        
        String[] tokens = do_lex(textarr);
        for(int i=0;i<tokens.length;i++) {
            System.out.println(tokens[i]);
        }
    
    }
    
}

*/



 
