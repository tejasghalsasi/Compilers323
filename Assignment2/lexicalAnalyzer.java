import java.util.*;
import java.io.*;
/*

Tejas Ghalsasi

This class is a lexical analyzer i have re written.
*/

public class lexicalAnalyzer
{
static Queue <String> tokenQueue= new LinkedList<String>();
static Queue <String> lexemeQueue=new LinkedList<String>();

public static BufferedReader inStream;
public static PrintWriter outStream;

	public static int a; 
	public static char c; 

	public lexicalAnalyzer()
	{
		setIO("abc.txt","xyz.txt");
	}
	Queue <String> gettokenQueue()
	{
		return tokenQueue;
	}

Queue <String> getlexemeQueue()
	{
		return lexemeQueue;
	}
	

	public static int getNextChar()
	{
		int ret=0;
		try
		{
			return inStream.read();
		}
		catch(IOException e)
		{
			e.printStackTrace();
	
		}
		return ret;
	}

	public static int getChar()

	
	{
		int i = getNextChar();
		while ( Character.isWhitespace((char) i) )
			i = getNextChar();
		return i;
	}

	public static void display(String s)
	{
		setIO("abc.txt","xyz.txt");
		outStream.print(s);
	}

	public static void displayln(String s)
	{
		setIO("abc.txt","xyz.txt");
		outStream.println(s);
	}

	public static void setIO(String inFile, String outFile)
	{	
		inFile="/home/aibot/Desktop/GIT_folder/Compilers323-master/Compilers323/Assignment2/abc.txt";
		outFile="/home/aibot/Desktop/GIT_folder/Compilers323-master/Compilers323/Assignment2/xyz.txt";
	
		try
		{
			inStream = new BufferedReader( new FileReader(new File(inFile)));
			outStream = new PrintWriter( new FileOutputStream(new File(outFile)));
			a = inStream.read();
		}
		catch(FileNotFoundException e)
		{
			e.printStackTrace();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}

	public static void closeIO()
	{
		try
		{
			inStream.close();
			outStream.close();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}

boolean seperator(char input[], int length)
{
	int numSep = 10;
    String temp= "();:,{}[]@%";
	char seperators[]=temp.toCharArray(); 
	boolean sep = false;

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

boolean operators(char input[], int length)
{
	int numOps = 7;
	char operators[] = { '<', '>', '+', '-', '*', '/', '=' };
	boolean oper = false;

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

boolean keyword(char input[])
{
	  int numKey = 13;
	String keywords[] = { "integer", "if", "else", "fi", "while",
		"return", "read", "write", "for", "true", "false", "Boolean",
		"floating" };
	boolean key = false;	// Initial state
    String inputString=new String(input);
	for (int i = 0; i < numKey; ++i)
	{

		if (keywords[i].equals(inputString))
		{
			key = true;
			break;
		}
	}
	return key;
}

boolean idFSM(/*String input*/ char input[], int length)
{
	int state = 0;
	int col;
	int table[][] =new int[][] {	// q0 = 1; F: {1, 3, 4}
						// l  #
					/*0*/{ 1, 2 },
					/*1*/{ 1, 3 },
					/*2*/{ 2, 2 },	
					/*3*/{ 4, 2 },
					/*4*/{ 4, 5 },
					/*5*/{ 4, 2 } };

	for (int i = 0; i < length; ++i)
	{
		if (Character.isLetter(input[i]))
		{
			col = 0;
			state = table[state][col];
		}
		else if (input[i] == '#')
		{
			col = 1;
			state = table[state][col];
		}
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
	int table[][] = new int[][]{	// q0 = 1; F: {1(int), 4(real)}
						// d  .
					/*0*/{ 1, 2 },
				    /*1*/{ 1, 3 },
				    /*2*/{ 2, 2 },	// Invalid State row
					/*3*/{ 4, 2 },
					/*4*/{ 4, 2 } };

	for (int i = 0; i < length; ++i)
	{
		if (Character.isDigit(input[i]))
		{
			col = 0;
			state = table[state][col];
		}
		else if (input[i] == '.')
		{
			col = 1;
			state = table[state][col];
		}
		else
		{
			state = 2;
		}
	}
	return state;
}

void lexer()
{

    
	char c=' ';
	char input[]=new char[1000];
	//char seperators[] = "();:,";
	boolean firstSep = false;
	String firstSeperator="";
	boolean lastSep = false; 
	String lastSeperator="";

	int i = 0;
	File inFile = new File("abc.txt");
        Scanner sc=null;
	displayln("TOKEN\t\t\tLEXEME");
        try
		{
sc = new Scanner( inFile);
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}

    while (sc.hasNextLine())
	{
		c +=sc.next().charAt(0);

		if (c == ' ' || c == '\n' || c == '\t')
		{
			input[i] = '\0';
			int inputLength = i;
			i = 0;

			if (inputLength > 0)
			{
				if (firstSep == true)
				{
					displayln("seperator \t\t"+firstSeperator);
					System.out.println( "seperator \t\t"+firstSeperator);
					for (int j = 0; j < inputLength; ++j)
					{
						input[j] = input[j + 1];
					}
					input[inputLength] = '\0';
					inputLength--;
					input=(firstSeperator).toCharArray();
					tokenQueue.add("separator");
					lexemeQueue.add(""+input);
					firstSep = false;
				}
				if (input[inputLength - 1] == ')' ||
					input[inputLength - 1] == ';' ||
					input[inputLength - 1] == ',')
				{
					lastSeperator =""+input[inputLength - 1];
					input[inputLength - 1] = '\0';
					inputLength--;
					lastSep = true;
				}

				if (seperator(input, inputLength) == true)
				{
					displayln("seperator \t\t");
					System.out.print( "seperator \t\t");
					for (int j = 0; j < inputLength; ++j)
					{
						System.out.print( input[j]);
					}
					System.out.println();
					displayln(" ");
					String inp=new String(input);
					tokenQueue.add("separator");
					lexemeQueue.add(inp);
				}
				// check if operator.
				else if (operators(input, inputLength) == true)
				{
					displayln("operator \t\t");
					System.out.print( "operator \t\t");
					for (int j = 0; j < inputLength; ++j)
					{
						System.out.print( input[j]);
						//display(input[j]);
					}
					System.out.println();
					displayln("");
					String inp=new String(input);
					tokenQueue.add("operator");
					lexemeQueue.add(inp);
				}
				// check if keyword.
				else if (keyword(input) == true)
				{
					System.out.println( input + " is a keyword");
					displayln("keyword \t\t");
					System.out.print( "keyword \t\t");
					for (int j = 0; j < inputLength; ++j)
					{
						System.out.print( input[j]);
					//	display(input[j]);
					}
					displayln("");
					System.out.println();
					String inp=new String(input);
					tokenQueue.add("keyword");
					lexemeQueue.add(inp);

					if (lastSep == true)
					{
						displayln("seperator \t\t"+lastSeperator);
						System.out.println("seperator \t\t"+lastSeperator);
						lastSep = false;
						inp=(lastSeperator);
						tokenQueue.add("separator");
						lexemeQueue.add(inp);
					}
				}
				// check if identifier
				else if (idFSM(input, inputLength) == true)
				{
						System.out.print( "identifier \t\t");
					for (int j = 0; j < inputLength; ++j)
					{
						System.out.print( input[j]);
					}
					System.out.println();
					displayln("");
					String inp=new String(input);
					tokenQueue.add("identifier");
					lexemeQueue.add(inp);

					if (lastSep == true)
					{
						displayln("seperator \t\t"+lastSeperator);
						System.out.print( "seperator \t\t"+lastSeperator);
						lastSep = false;
						inp=(lastSeperator);
						tokenQueue.add("separator");
						lexemeQueue.add(inp);
					}
				}
				// check if int
				else if (numFSM(input, inputLength) == 1)
				{
					display("integer \t\t");
					System.out.print( "integer \t\t");
					for (int j = 0; j < inputLength; ++j)
					{
						System.out.print( input[j]);
						//display(input[j]);
					}
					displayln("");
					System.out.println();
					String inp=new String(input);
					tokenQueue.add("integer");
					lexemeQueue.add(inp);
					if (lastSep == true)
					{
						displayln("seperator \t\t"+ lastSeperator);
						System.out.println( "seperator \t\t"+ lastSeperator);
						lastSep = false;
						inp=(lastSeperator);
						tokenQueue.add("separator");
						lexemeQueue.add(inp);
					}
				}
				// check if real
				else if (numFSM(input, inputLength) == 4)
				{
					display("real number \t\t");
					System.out.print( "real number \t\t");
					for (int j = 0; j < inputLength; ++j)
					{
						System.out.print( input[j]);
						//display(input[j]);
					}
					displayln("");
					System.out.println();
					String inp=new String(input);
					tokenQueue.add("real");
					lexemeQueue.add(inp);

					if (lastSep == true)
					{
						displayln("separator \t\t");
						System.out.println( "separator \t\t");
						inp=(lastSeperator);
						tokenQueue.add("seperator");
						lexemeQueue.add(inp);
						lastSep = false;
					}
				}
				else
				{
					display("invalid \t\t");
					System.out.print("invalid \t\t");
					for (int j = 0; j < inputLength; ++j)
					{
						System.out.print(input[j]);
					}
                    displayln(" ");
					System.out.println();
					String inp=""+(input);
					tokenQueue.add("invalid");
					lexemeQueue.add(inp);
				}
			}
			// Empty input char array.
			for (int j = 0; j < inputLength; ++j)
			{
				input[j] = '\0';
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
				firstSeperator =""+input[0];
			}
		}
	}
}
public static void main(String args[])
{
	lexicalAnalyzer a=new lexicalAnalyzer();
	a.lexer();

	displayln("this thing is not working madarchod");
}




}