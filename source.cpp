#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

//struct for holding LA record
struct LA_output{
    string token;   //category
    string lexeme;  //actual
};

//functions to write to file
template <typename T>
void writetofile(ofstream& ofs, T& x, string token, LA_output & val) {
    val.lexeme = x;
    val.token = token;
    ofs << setw(10) << x << "\t\t\t" << token << endl;
}
template <typename T>
void writetofile(T* x, string token, LA_output & val) {
    val.lexeme = x;
    val.token = token;
    writetofile(*x, token);
}

void SA_output(ofstream & ofs, string production){
    ofs << production;
}
///////////////////////////////////////////////////////////////////////////////////
void m_id_key(ofstream& ofs, char *&p, string * keyword, LA_output & val) {
    //since starting state is 1 and entering this function requires a letter input
    int state = 2;
    bool found_keyword = false;
    string word = "";
    /*
     A finite state machine for identifiers and keywords;
     will continue checking character inputs as long as
     they are valid inputs (letter, digit, $).
     Accepting states 2, 3, 5
     */
    int id_FSM[6][4]{
        //state   L   D   $
        1,        2,  6,  6,
        2,        3,  4,  5,
        3,        3,  4,  5,
        4,        3,  4,  5,
        5,        6,  6,  6,
        6,        6,  6,  6
        
    };
    //while valid input
    while (((isalpha(*p)) || (isdigit(*p))) || (*p == '$')) {
        if (isalpha(*p))
            state = id_FSM[state - 1][1];
        else if (isdigit(*p))
            state = id_FSM[state - 1][2];
        else
            state = id_FSM[state - 1][3];
        word += *p;
        ++p;
    }
    //if accepting state
    if (((state == 2) || (state == 3)) || (state == 5)) {
        //check if keyword
        for (int i = 0; i < 13; i++) {
            if (word == keyword[i]) {
                found_keyword = true;
                writetofile(ofs, word, "keyword", val);
            }
        }
        //otherwise its an identifier
        if (!found_keyword)
            writetofile(ofs, word, "identifier", val);
    }
    //not accepting state
    else
        writetofile(ofs, word, "unknown", val);
}
//determines whether the input characters are integers or real numbers
void m_int_real(ofstream& ofs, char*&p, LA_output & val) {
    int state = 2; //set to 2 since input char was a digit
    /*
     combination of integer and real number FSM
     states 2 is accepting states for integer numbers
     state 4 is the accepting state for real numbers
     state 5 is an error state.
     */
    int int_real_FSM[5][3]{
        //state     D   .
        1,          2,  5,
        2,          2,  3,
        3,          4,  5,
        4,          4,  5,
        5,          5,  5,
    };
    string word = "";
    //while valid input
    while (isdigit(*p) || (*p) == '.') {
        if (isdigit(*p))
            state = int_real_FSM[state - 1][1];
        else
            state = int_real_FSM[state - 1][2];
        word += *p;
        ++p;
    }
    if (state == 2)
        writetofile(ofs, word, "integer", val);
    else if (state == 4)
        writetofile(ofs, word, "real", val);
    else
        writetofile(ofs, word, "unknown", val);
}

//determines if the character is a separator
bool is_sep(ofstream& ofs, char *&p, char * sep, LA_output & val) {
    for (int i = 0; i < 10; i++){
        if (*p == sep[i]){
            if (*p == '%'){
                //if next char is also %
                if (*(++p) == '%'){
                    writetofile(ofs, "%%","separator", val);
                    ++p;
                    return true;
                }
                else{
                    //decrement to original p for error message
                    --p;
                    char tmp = p[0];
                    writetofile(ofs, tmp,"unknown", val);
                    ++p;
                    return true;
                }
            }
            else{
                char tmp = p[0];
                writetofile(ofs, tmp,"separator", val);
                ++p;
                return true;
            }
        }
    }
    return false;
}
//determines if the current character and next char are operators
bool is_op(ofstream& ofs, char *&p, char * ope, LA_output & val) {
    string two_char_operator [4] = {"==", "=>", "=<", "^="};
    
    string word_ope = "";
    for (int i = 0; i < 8; i++) {
        if (*p == ope[i]) {
            word_ope += *p;
            ++p;
            //second loop to check if next char is part of operator
            for (int k = 0; k < 8; k++) {
                if (*p == ope[k]) {
                    word_ope += *p;
                    for (int j = 0; j < 4; j++){
                        if (word_ope == two_char_operator[j]){
                            writetofile(ofs, word_ope, "operator", val);
                            //cout << word_ope << " is valid" << endl;
                            ++p;
                            return true;
                        }//end if two_char operator
                    }//end for checking valid two_char operator
                    //if second char isn't a valid operator
                    ++p;
                    writetofile(ofs, word_ope, "unknown", val);
                    //cout << word_ope << " is not valid" << endl;
                    return true;
                }//end if second char is an operator
            }//end for k
            writetofile(ofs, word_ope, "operator", val);
            //cout << word_ope << " is valid" << endl;
            return true;
        }//end if first char is an operator
    }
    //cout << word_ope << " is not valid" << endl;
    return false;
}


//checks for comments
bool is_comment(char *&p, fstream & fin, string & x){
    if (*p == '!'){
        ++p;
        while (*p != '!')
        {
            if (p != &x[x.length()])
                ++p;
            else
            {
                getline(fin, x);
                p = &x[0];
            }
        }
        ++p;
        return true;
    }
    return false;
}

//the lexer function considers all valid inputs
LA_output lexer(char *& p, ofstream & outfile, fstream & fin, string & x){
    //valid keywords, separators, and operators
    string keywords[13] = { "int","boolean","real","function","return","true",
        "false","if","else","endif","while","get","put" };
    char separators[10] = { '(',')',':',',','{','}', ']', '[', ';', '%' };
    char operators[8] = { '<','>','=','^','+','-','*','/' };
    
    LA_output ex;
    if (isalpha(*p))m_id_key(outfile, p, keywords, ex);
    else if (isdigit(*p))m_int_real(outfile, p, ex);
    else if (is_op(outfile, p, operators, ex)) {}
    else if (is_sep(outfile, p, separators, ex)) {}
    //else if (((((*p) == 32) || (*p == '\t')) || ((*p == '\n') || (int)(*p) == 13)) || ((int)(*p) == 0)) {
    else if ((int)*p <= 32){
        if ((int)(*p) == 13) ex.token = "return carridge";
        else if ((int)*p <= 0) ex.token = "error";
        else {
            cout << (int)(*p) << endl;
            ex.token = "undefined space";
        }
        ++p;
    }
    //else if (is_comment(p, fin, x)){
    //}
    else if (*p =='!'){
        ++p;
        while (*p != '!')
        {
            if (p != &x[x.length()])
                ++p;
            else
            {
                getline(fin, x);
                p = &x[0];
            }
        }
        ex.lexeme = "";
        ++p;
        //lexer(p, outfile, fin, x);
        //ex.token = "unknown";
    }
    else {
        //char tmp = p[0];
        //writetofile(outfile, tmp, "unknown", ex);
        ++p;
        //error
        ex.token = "unknown";
    }
    return ex;
}

bool is_space(char x){
    if ((((((x) == 32) || (x == '\t')) || ((x == '\n') || (int)(x) == 13)) || (x == '\r')) || ((int)x == 0)) cout << "true" <<endl; return true;
    return false;
}

//super header
void SA(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void new_line(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);



//if at the end of the line, get a new line, if it exists, else throw an error
bool end_line(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    //return (record.lexeme[0] == line_code[line_code.length()-1]);
    //cout << (int)line_code[line_code.length()-1] << endl;
    //cout << &p[0] << endl;
    //cout << (int)*p << endl;
    //return (p == &line_code[line_code.length()]);
    //return ((int)*p == (int)line_code[line_code.length()-1]);
    return (false);
}

//the end of the line doesn't mean the end of SA
void get_token(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    
    //if record is a new line carridge, then if new line, get new line; else, empty file
    //else if its a space, keep calling until we get a real token
    
    //cout << (int)record.lexeme[0] <<"\t" << "13" << endl;
    record = lexer (p, outfile, fin, line_code);
    
    /*
    int too_many_spaces = 0;
    while (too_many_spaces != 5){
        if (record.lexeme.length() == 0){
            record = lexer(p, outfile, fin, line_code);
        }
        too_many_spaces++;
    }
     */
    
    
    
    bool prob = true;
    //int zero_len = 0;
    //cout << (int)*(--p) << endl;
    
    while (prob){
        //cout << (int)record.lexeme[0] << endl;
        if (record.token == "unknown"){
            if (getline(fin, line_code)){
                p = &line_code[0];
                //get_token(p, outfile, fin, record, line_code);
            }
            else{
                cout << "empty file" << endl;
                outfile.close();
                fin.close();
                exit(0);
            }
            record = lexer(p, outfile, fin, line_code);
            
        }
        else if((record.token == "return carridge") || (record.token == "error")){
            new_line(p, outfile, fin, record, line_code);
            record = lexer(p, outfile, fin, line_code);
        }
        else if (record.lexeme.length() == 0){
            record = lexer(p, outfile, fin, line_code);
        }
        else{
            prob = false;
            cout << "all good" << endl;
        }
        
    }
    
    
}

//when we run into an error, we can call new_line
void new_line(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    if (getline(fin, line_code)){
        p = &line_code[0];
        //record = lexer(p, outfile, fin, line_code);
        //get_token(p, outfile, fin, record, line_code);
        //SA (p, outfile, fin, record, line_code);
    }
    else {
        cout << "empty file" << endl;
        outfile.close();
        fin.close();
        exit(0);
    }
}

//function headers
void functiondefinitions(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void statementlistP(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void declarationlist(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void parameterlist(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void term (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void expression (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void qualifier (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void statementlist(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void statement(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);
void ids(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code);

//r29
void empty(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Empty>\n");
    //get_token(p, outfile, fin, record, line_code);
    SA(p, outfile, fin, record, line_code);
}

//r28

//problem with <identifier>:
void primary(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output (outfile, "<Primary> ->  <Identifier> | <Integer> | <Identifier> (<IDs> ) | (<Expression>) | <Real> | true | false\n");

    //automatically called
    if (record.token == "identifier"){
        get_token(p, outfile, fin, record, line_code);
        //what if <identifier> (<ids>)?
        SA_output(outfile, "<Primary> -> <Identifier>\n");
        
        if (record.lexeme == "("){
            get_token(p, outfile, fin, record, line_code);
            ids(p, outfile, fin, record, line_code);
            if (record.lexeme == ")"){
                get_token(p, outfile, fin, record, line_code);
                //SA(p, outfile, fin, record, line_code);
            }
            else{
                SA_output(outfile,"error(4): primary\n");
                new_line(p, outfile, fin, record, line_code);
                //SA(p, outfile, fin, record, line_code);
            }
        }
        else{
            
        }
        
    }
    else if (record.token == "integer"){
        get_token(p, outfile, fin, record, line_code);
        SA_output(outfile, "<Primary> -> <integer>\n");
    }
    else if (record.lexeme == "("){
        get_token(p, outfile, fin, record, line_code);
        SA_output(outfile, "<Primary> -> (<Expression>)\n");
        expression(p, outfile, fin, record, line_code);
        if (record.lexeme == ")"){
            get_token(p, outfile, fin, record, line_code);
            //SA(p, outfile, fin, record, line_code);
        }
        else{
            SA_output(outfile,"error(2): primary\n");
            new_line(p, outfile, fin, record, line_code);
            //SA(p, outfile, fin, record, line_code);
        }
    }
    else if (record.token == "Real"){
        get_token(p, outfile, fin, record, line_code);
        SA_output(outfile, "<Primary> -> <real>\n");
    }
    else if (record.lexeme == "true"){
        get_token(p, outfile, fin, record, line_code);
        SA_output(outfile, "<Primary> -> true\n");

    }
    else if (record.lexeme == "false"){
        get_token(p, outfile, fin, record, line_code);
        SA_output(outfile, "<Primary> -> false\n");

    }
    else{
        SA_output(outfile,"error(1): primary\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);

    }
}

//r27
void factor (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output (outfile, "<Factor> -> -<Primary> | <Primary>\n");
    //automatically called
    
    if (record.lexeme == "-"){
        get_token(p, outfile, fin, record, line_code);
        primary(p, outfile, fin, record, line_code);
        SA_output (outfile, "<Factor> -> - <primary>\n");
    }
    else {
        SA_output (outfile, "<Factor> -> <primary>\n");
        primary(p, outfile, fin, record, line_code);
    }
    
}


//r26 prime
void termP (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output (outfile, "<Term Prime> -> *<Factor><Term> | /<Factor><Term> |<Empty>\n");
    //automatically called
    if (record.lexeme == "*"){
        SA_output (outfile, "<Term Prime> -> * <Factor> <Term Prime>\n");
        get_token(p, outfile, fin, record, line_code);
        //term(p, outfile, fin, record, line_code);
        
        factor(p, outfile, fin, record, line_code);
        termP(p, outfile, fin, record, line_code);
    }
    else if (record.lexeme == "/"){
        SA_output (outfile, "<Term Prime> -> / <Factor> <Term Prime>\n");
        get_token(p, outfile, fin, record, line_code);
        //term(p, outfile, fin, record, line_code);
        factor (p, outfile, fin, record, line_code);
        termP(p, outfile, fin, record, line_code);
    }
    /*
    else if (record.lexeme == ";"){// if ((end_line(p, outfile, fin, record, line_code)) || (record.lexeme == ";")){
        
     
     SA_output (outfile, "<Term Prime> -> Epsilon\n");
        get_token(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
        //empty(p, outfile, fin, record, line_code);
    }
     */
    else{
        //remove dangling ; if it remains
        if (record.lexeme == ";"){
            cout << "check here" << endl;
            //get_token(p, outfile, fin, record, line_code);
        }
    }
}

//r26
void term (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output (outfile, "<Term> -> <Factor> <Term Prime>\n");
    factor(p, outfile, fin, record, line_code);
    termP(p, outfile, fin, record, line_code);
}


//r25 prime
void expressionP(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output (outfile, "<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | Empty\n");
    //automatically called
    if ((record.lexeme == "+") || (record.lexeme == "-")){
        get_token(p, outfile, fin, record, line_code);
        term(p, outfile, fin, record, line_code);
        //expression(p, outfile, fin, record, line_code);
        expressionP(p, outfile, fin, record, line_code);
    }
    //else if end of line code, we good
    /*
    else if (end_line(p, outfile, fin, record, line_code)){
        SA_output (outfile, "<Expression Prime> -> Epsilon\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
    */
    else{
        //remove dangingling ; if it reamins
        if (record.lexeme == ";"){
            get_token(p, outfile, fin, record, line_code);
        }
        //term(p, outfile, fin, record, line_code);
        //SA_output(outfile,"expression prime: error\n");
        //new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
}

//r25
void expression (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Expression> -> <Term> <Expression Prime>\n");
    term(p, outfile, fin, record, line_code);
    expressionP(p, outfile, fin, record, line_code);
}

//r24
void relop (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Relop> -> == | ^= | > | < | => | =< \n");
    
    if ((record.lexeme == "==") || (record.lexeme == "^=")){
        get_token(p, outfile, fin, record, line_code);
    }
    else if ((record.lexeme == ">") || (record.lexeme == "<")){
        get_token(p, outfile, fin, record, line_code);
    }
    else if ((record.lexeme == "=>") || (record.lexeme == "=<")){
        get_token(p, outfile, fin, record, line_code);
    }
    else{
        SA_output(outfile,"error: relop\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }

}

//r23
void condition (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Condition> -> <Expression> <Relop> <Expression>\n");
    expression(p, outfile, fin, record, line_code);
    relop(p, outfile, fin, record, line_code);
    expression(p, outfile, fin, record, line_code);
    
}

//r22
void while_r(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<While> -> while (<Condition>) <Statement>\n");
    
    //assume while
    if (record.lexeme == "("){
        get_token(p, outfile, fin, record, line_code);
        condition(p, outfile, fin, record, line_code);
        if (record.lexeme == ")"){
            get_token(p, outfile, fin, record, line_code);
            statement(p, outfile, fin, record, line_code);
        }
        else{
            SA_output(outfile,"error(2): while_r\n");
            new_line(p, outfile, fin, record, line_code);
            //SA(p, outfile, fin, record, line_code);
        }
    }
    else{
        SA_output(outfile,"error(1): while_r\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
    
}

//r21
void scan (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Scan> -> get (<Scan>);\n");
    //assume get
    if (record.lexeme == "get"){
        get_token(p, outfile, fin, record, line_code);
        if (record.lexeme == "("){
            get_token(p, outfile, fin, record, line_code);
            ids(p, outfile, fin, record, line_code);
            if(record.lexeme == ")"){
                get_token(p, outfile, fin, record, line_code);
                if (record.lexeme == ";"){
                    get_token(p, outfile, fin, record, line_code);
                    //? not sure if I should use empty after semicolons
                    empty(p, outfile, fin, record, line_code);
                }
                else{
                    SA_output(outfile,"error (4): scan\n");
                    new_line(p, outfile, fin, record, line_code);
                    //SA(p, outfile, fin, record, line_code);
                }
            }
            else{
                SA_output(outfile,"error(3): scan\n");
                new_line(p, outfile, fin, record, line_code);
                //SA(p, outfile, fin, record, line_code);
            }
        }
        else{
            SA_output(outfile,"error(2): scan\n");
            new_line(p, outfile, fin, record, line_code);
            //SA(p, outfile, fin, record, line_code);
        }
    }
    else{
        SA_output(outfile,"error(1): scan\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }

}

//r20
void print(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    //assume token put
    SA_output(outfile, "<Print> -> put ( <Expression> );\n");
    if (record.lexeme == "("){
        get_token(p, outfile, fin, record, line_code);
        expression(p, outfile, fin, record, line_code);
        if (record.lexeme == ")"){
            get_token(p, outfile, fin, record, line_code);
            if (record.lexeme == ";"){
                get_token(p, outfile, fin, record, line_code);
                /*
                if (end_line(p, outfile, fin, record, line_code)){
                    new_line(p, outfile, fin, record, line_code);
                    SA(p, outfile, fin, record, line_code);
                }
                else{
                    SA_output(outfile,"error(4): print\n");
                    new_line(p, outfile, fin, record, line_code);
                    SA(p, outfile, fin, record, line_code);
                }
                 */
                
            }
            else{
                SA_output(outfile,"error(3): print\n");
                new_line(p, outfile, fin, record, line_code);
                SA(p, outfile, fin, record, line_code);
            }
        }
        else{
            SA_output(outfile,"error(2): print\n");
            new_line(p, outfile, fin, record, line_code);
            SA(p, outfile, fin, record, line_code);
        }
    }
    else{
        SA_output(outfile,"error(1): print\n");
        new_line(p, outfile, fin, record, line_code);
        SA(p, outfile, fin, record, line_code);
    }
}



//r19p
void return_rp(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<ReturnP> -> ;|<Expression>;\n");
    
    if (record.lexeme == ";"){
        //good
        get_token(p, outfile, fin, record, line_code);
        empty(p, outfile, fin, record, line_code);
    }
    else{
        expression(p, outfile, fin, record, line_code);
        if (record.lexeme == ";"){
            get_token(p, outfile, fin, record, line_code);
            /*
            if (end_line(p, outfile, fin, record, line_code)){
                new_line(p, outfile, fin, record, line_code);
                SA(p, outfile, fin, record, line_code);
            }
            */
            //else?
        }
        else{
            //SA_output(outfile,"error (1): return_rp\n");
            //new_line(p, outfile, fin, record, line_code);
            //SA(p, outfile, fin, record, line_code);
        }
    }

}

//r19
void return_r (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Return> -> return <ReturnP>\n");
    
    if (record.lexeme == "return"){
       get_token(p, outfile, fin, record, line_code);
       return_rp (p, outfile, fin, record, line_code);
    }
    else{
        SA_output(outfile,"error: return_r\n");
        new_line(p, outfile, fin, record, line_code);
        SA(p, outfile, fin, record, line_code);
    }
    
}

//r18p
void if_rp(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<If Prime> -> endif | else <Statement> endif\n");
    
    if (record.lexeme == "endif"){
        get_token(p, outfile, fin, record, line_code);
    }
    else if (record.lexeme == "else"){
        get_token(p, outfile, fin, record, line_code);
        statement(p, outfile, fin, record, line_code);
        if (record.lexeme == "endif"){
            //good
            get_token(p, outfile, fin, record, line_code);
            //?
            SA(p, outfile, fin, record, line_code);
        }
        else{
            SA_output(outfile,"error (2): if_rp\n");
            new_line(p, outfile, fin, record, line_code);
            SA(p, outfile, fin, record, line_code);
        }
        
    }
    else{
        SA_output(outfile,"error (1): if_rp\n");
        new_line(p, outfile, fin, record, line_code);
        SA(p, outfile, fin, record, line_code);
    }

}


//r18
void if_r (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<If> -> if ( <Condition> ) <Statement> <ifP> \n");
    //assume if
    
    //if not endline
    if (record.lexeme == "("){
        get_token(p, outfile, fin, record, line_code);
        condition(p, outfile, fin, record, line_code);
        if(record.lexeme == ")"){
            get_token(p, outfile, fin, record, line_code);
            statement(p, outfile, fin, record, line_code);
            if_rp(p, outfile, fin, record, line_code);
        }
        else{
            SA_output(outfile,"error(2): if_r\n");
            new_line(p, outfile, fin, record, line_code);
            SA(p, outfile, fin, record, line_code);
        }
    }
    else{
        SA_output(outfile,"error(1): if_r\n");
        new_line(p, outfile, fin, record, line_code);
        SA(p, outfile, fin, record, line_code);
    }
}

//r17
void assign (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Assign> -> <Identifier> = <Expression>\n");
    
    if (record.token == "identifier"){
        get_token(p, outfile, fin, record, line_code);
    }
        if (record.lexeme == "="){
            get_token(p, outfile, fin, record, line_code);
            if (record.token == "identifier"){
                //get_token (p, outfile, fin, record, line_code);
                expression(p, outfile, fin, record, line_code);
            }
            else{
                SA_output(outfile,"error (2): assign\n");
                new_line(p, outfile, fin, record, line_code);
                //SA(p, outfile, fin, record, line_code);
            }
        }
        else {
            SA_output(outfile, "error (1): assign\n");
            new_line(p, outfile, fin, record, line_code);
            //SA(p, outfile, fin, record, line_code);
        }
    
}

//r16
void compound (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Compound> -> { <Statement List> }\n");
    
    if (record.lexeme == "{"){
        get_token(p, outfile, fin, record, line_code);
        statementlist(p, outfile, fin, record, line_code);
        if (record.lexeme == "}"){
            get_token(p, outfile, fin, record, line_code);

        }
        else{
            SA_output(outfile,"error (2): compound\n");
            new_line(p, outfile, fin, record, line_code);
            SA(p, outfile, fin, record, line_code);
        }
    }
    else{
        SA_output(outfile,"error(1): compound\n");
        new_line(p, outfile, fin, record, line_code);
        SA(p, outfile, fin, record, line_code);
    }
    
}

//r15
void statement(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>\n");
    
    if (record.lexeme == "{"){
        compound (p, outfile, fin, record, line_code);
    }
    else if (record.token == "identifier"){
        assign(p, outfile, fin, record, line_code);
    }
    else if (record.lexeme == "if"){
        get_token(p, outfile, fin, record, line_code);
        if_r(p, outfile, fin, record, line_code);
    }
    else if (record.lexeme == "return"){
        return_r(p, outfile, fin, record, line_code);
    }
    else if (record.lexeme == "put"){
        get_token(p, outfile, fin, record, line_code);
        print(p, outfile, fin, record, line_code);
    }
    else if (record.lexeme == "get"){
        scan(p, outfile, fin, record, line_code);
    }
    else if (record.lexeme == "while"){
        get_token(p, outfile, fin, record, line_code);
        while_r(p, outfile, fin, record, line_code);
    }
    else{
        SA_output(outfile,"error: statement\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
}

//r14p
void statementlistP(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Statement ListP> -><Empty>|<Statement List\n");

    if ((record.token == "identifier") || (record.token == "keyword")){
        statement(p, outfile, fin, record, line_code);
    }
    else{
        
    }
}

//r14
void statementlist(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Statement List> -><Statement><Statement ListP\n");
    statement(p, outfile, fin, record, line_code);
    statementlistP(p, outfile, fin, record, line_code);
}

//r13p
void idsP(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Ids Prime> ->  <Empty> |, <Ids>\n");
    
    
    if (record.lexeme == ","){
        get_token(p, outfile, fin, record, line_code);
        ids(p, outfile, fin, record, line_code);
        //scan(p, outfile, fin, record, line_code);
    }
    else{
        
    }
    
    
}

//r13
void ids(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<IDs> ->  <identifier> <IDs Prime>\n");
    
    if (record.token == "identifier"){
        get_token(p, outfile, fin, record, line_code);
        idsP(p, outfile, fin, record, line_code);
        
    }
    else{
        
        SA_output(outfile,"error: scan_r\n");
        new_line(p, outfile, fin, record, line_code);
        SA(p, outfile, fin, record, line_code);
    }
}

//r12
void declaration(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Declaration> ->  <Qualifier> <Ids>\n");
    qualifier(p, outfile, fin, record, line_code);
    ids(p, outfile, fin, record, line_code);
    
}

//r11p
void declarationlistP(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Declaration List Prime> ->  <Empty> | <Declaration List>\n");
    
    
    if ((record.token == "integer") || (record.token == "real")){
        declarationlist(p, outfile, fin, record, line_code);
    }
    else if ((record.lexeme == "true") || (record.lexeme == "false")){
        declarationlist(p, outfile, fin, record, line_code);
    }
    else{
        
    }

}

//r11
void declarationlist(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Declaration List> ->  <Declaration> ; <Declaration List Prime>\n");
    declaration(p, outfile, fin, record, line_code);
    if (record.lexeme == ";"){
        get_token(p, outfile, fin, record, line_code);
        declarationlistP(p, outfile, fin, record, line_code);
    }
    else{
        SA_output(outfile, "error: declarationlist\n");
    }
}


//r10
void optDeclarationlist(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Opt Declaration List> ->  <Declaration List> | <Empty>\n");
    
     if (((record.lexeme == "int") || (record.lexeme == "real")) || (record.lexeme == "boolean")){
        declarationlist(p, outfile, fin, record, line_code);
    }
    //empty
    else{
        
    }
    
    
}

//r9
void body (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Body> ->  { <Statement List> }\n");
    
    if (record.lexeme == "{"){
        get_token(p, outfile, fin, record, line_code);
        statementlist(p, outfile, fin, record, line_code);
        if (record.lexeme == "}"){
            get_token(p, outfile, fin, record, line_code);
            //?
            new_line(p, outfile, fin, record, line_code);
            //good
        }
        else{
            SA_output(outfile,"error (2): body\n");
            new_line(p, outfile, fin, record, line_code);
            SA(p, outfile, fin, record, line_code);
        }
    }
    else{
        SA_output(outfile,"error (1): body\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
}

//r8
void qualifier (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Qualifier> -> int | boolean | real\n");
    
    if (((record.lexeme == "int") || (record.lexeme == "real")) || (record.lexeme == "boolean")){
        get_token(p, outfile, fin, record, line_code);
    }
    else{
        SA_output(outfile,"error: qualifier\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
}

//r7
void parameter (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Parameter> -> <Ids> : <Qualifier>\n");
    
    ids(p, outfile, fin, record, line_code);
    if (record.lexeme == ":"){
        get_token(p, outfile, fin, record, line_code);
        qualifier(p, outfile, fin, record, line_code);
    }
    else{
        SA_output(outfile,"error: parameter\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
}

//r6p
void parameterlistP(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Parameter List Prime> -> <Empty> | , <Parameter List>\n");
    
    
    if (record.lexeme == ","){
        parameterlist(p, outfile, fin, record, line_code);
    }
    //empty
    else{
        
    }
    
}

//r6
void parameterlist(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Parameter List> -> <Parameter> <Parameter List Prime>\n");
    parameter(p, outfile, fin, record, line_code);
    parameterlistP(p, outfile, fin, record, line_code);

}

//r5
void optParameterlist (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Opt Parameter List> -> <Parameter List> | <Empty>\n");
    
    
    if (record.token == "identifier"){
        parameterlist(p, outfile, fin, record, line_code);
        
    }
    //empty
    else{
        
    }
    
}

//r4
void function_r(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Function> -> function <Identifier> [ <Opt Parameter List> ] <Opt Declaration List> <Body> <Empty>\n");
    
    if (record.lexeme == "function"){
        get_token(p, outfile, fin, record, line_code);
        if (record.token == "identifier"){
            get_token(p, outfile, fin, record, line_code);
            if (record.lexeme == "["){
                get_token(p, outfile, fin, record, line_code);
                optParameterlist(p, outfile, fin, record, line_code);
                if (record.lexeme == "]"){
                    get_token(p, outfile, fin, record, line_code);
                    optDeclarationlist(p, outfile, fin, record, line_code);
                    body(p, outfile, fin, record, line_code);
                    /*
                    if (end_line(p, outfile, fin, record, line_code)){
                        empty(p, outfile, fin, record, line_code);
                    }
                    else{
                        SA_output(outfile,"error(5): function_r\n");
                        new_line(p, outfile, fin, record, line_code);
                        //SA(p, outfile, fin, record, line_code);
                    }
                     */
                }
                else{
                    SA_output(outfile,"error(4): function_r\n");
                    new_line(p, outfile, fin, record, line_code);
                    //SA(p, outfile, fin, record, line_code);
                }
            }
            else{
                SA_output(outfile,"error (3): function_r\n");
                new_line(p, outfile, fin, record, line_code);
                //SA(p, outfile, fin, record, line_code);
            }
        }
        else{
            SA_output(outfile,"error (2): function_r\n");
            new_line(p, outfile, fin, record, line_code);
            //SA(p, outfile, fin, record, line_code);
        }
    }

    else{
        SA_output(outfile,"error (1): function_r\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }
  
}

//r3p
void functiondefinitionsP(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Function Definitions Prime> -> <Empty> | <Function Definitions>\n");
    
    
    /*
    if (end_line(p, outfile, fin, record, line_code)){
        empty(p, outfile, fin, record, line_code);
    }
    else{
        functiondefinitions(p, outfile, fin, record, line_code);
    }
     */
    
    if (record.lexeme == "function"){
        functiondefinitions(p, outfile, fin, record, line_code);
    }
    else{
        
    }
    
}

//r3
void functiondefinitions(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Function Definitions> -> <Function> <Function Definitions Prime>\n");
    function_r(p, outfile, fin, record, line_code);
    functiondefinitionsP(p, outfile, fin, record, line_code);
}

//r2
void optFunctiondefinitions(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<Opt Function Definitions> -> <Opt Function Definitions> | <Empty> \n");
   
    
    /*if (end_line(p, outfile, fin, record, line_code)){
        empty(p, outfile, fin, record, line_code);
    }
    else{
        
    }
     */
    
    if (record.lexeme == "function"){
        functiondefinitions(p, outfile, fin, record, line_code);
    }
    else{
        
    }
}

//r1
void rat18s (char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    SA_output(outfile, "<rat18s> -><Opt Function Definitions > %% <Opt Declaration List> <Statement List>\n");
    
    optFunctiondefinitions(p, outfile, fin, record, line_code);
    if (record.lexeme == "%%"){
        get_token(p, outfile, fin, record, line_code);
        optDeclarationlist(p, outfile, fin, record, line_code);
        statementlist(p, outfile, fin, record, line_code);
    }
    else{
        SA_output(outfile,"error: rat18s\n");
        new_line(p, outfile, fin, record, line_code);
        //SA(p, outfile, fin, record, line_code);
    }

}

//The start of the syntax analyzation
void SA(char *&p, ofstream & outfile, fstream & fin, LA_output & record, string & line_code){
    new_line(p, outfile, fin, record, line_code);
    get_token(p, outfile, fin, record, line_code);
    //cout << record.token << "\t" << record.lexeme << endl;
    if (record.lexeme == "function"){
        rat18s(p, outfile, fin, record, line_code);
    }
    else if (((record.lexeme == "int") || (record.lexeme == "real")) || (record.lexeme == "boolean")){
        optDeclarationlist(p, outfile, fin, record, line_code);
    }
    else if ((record.token == "identifier") || (record.token == "keyword")){
        statement(p, outfile, fin, record, line_code);
    }
}
/////////////////////////////////////////////////////////////////

int main() {
    
    //read file
    fstream fin;
    
    //candidate lexeme
    char * lex_candid;
    
    //holds each input file line
    string line_code;
    string file_name;
    
    //structure containing lexer function results
    LA_output record;
    
    //prompt user to enter file name
    cout << "What is the name of the txt file you want to test:  " << endl;
    cin >> file_name;
    //file_name = "t1.txt";
    fin.open(file_name);
        
    //while unable to open file prompt user to re-enter file name
    while (!(fin.is_open())){
        cout << "Unable to open file; please re-enter file name" << endl;
        cin >> file_name;
        fin.open(file_name);
    }
    
    //creating and formatting output file
    ofstream outfile("output.txt");
    outfile << setw(10)<<"token"<<"\t\t\t" << "lexeme" <<endl;
    outfile << "--------------------------------------------------------------\n";
    
    //new_line(lex_candid, outfile, fin, record, line_code);
    SA(lex_candid, outfile, fin, record, line_code);
    
    
    fin.close();
    outfile.close();
    return 0;
}

