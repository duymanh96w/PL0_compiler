#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    NONE = 0, IDENT, NUMBER, BEGIN, CALL, CONST, DO, ELSE, END, FOR, IF, ODD, PROCEDURE, PROGRAM,
    THEN, TO, VAR, WHILE, PLUS, MINUS, TIMES, SLASH, EQU, NEQ, LSS, LEQ, GRT, GEQ, LPARENT, RPARENT, 
    LBRACK, RBRACK, PERIOD, COMMA, SEMICOLON, ASSIGN, PERCENT
} TokenType;

char *tokentype[19] = {"PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS", "LEQ", "GRT", "GEQ", "LPARENT", "RPARENT", "LBRACK", "RBRACK", "PERIOD", "COMMA", "SEMICOLON", "ASSIGN", "PERCENT"};
char *keyword[15] = {"BEGIN", "CALL", "CONST", "DO", "ELSE", "END", "FOR", "IF", "ODD", "PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE"};

TokenType token;
int num;
char id[11];
char c = ' ';

char getCH(FILE *f);
TokenType getToken(FILE *f);
int stringcmp(char *a, char *b);
int checkkeyword(char a[]);
int scan(int argc, char *argv[]);

char getCH(FILE *f){
    char a;
    a = fgetc(f);
    if(a != EOF){
        printf("%c", a);
    }
    return a;
}

TokenType getToken(FILE *f){
    for(int j = 0; j < 11; j++){
        id[j] = 0;
    }
    
    while (c == ' ' || c == '\n' || c == 9){
        c = getCH(f);
    }

    if(c == -1 || c == 0){
        return NONE;
    }

    if(isalpha(c)){
        int i = 0;
        while(isalnum(c)){
            if(i < 10){
                id[i] = c;
                i++;
            }            
            c = getCH(f);
        }
        id[i] = '\0';
        return IDENT;
    } else if (isdigit(c)){
        int i = 0;
        while(isdigit(c)){
            id[i] = c;
            i++;
            c = getCH(f);
        }
        id[i] = '\0';
        if(i > 6){
            printf("So qua lon.\n");
            return NONE;
        }
        num = atoi(id);
        return NUMBER;
    } else if (c == 43){    //Dau +    
        c = getCH(f);         
        return PLUS;
    } else if (c == 45){    //Dau -   
        c = getCH(f);             
        return MINUS;
    } else if (c == 42){    //Dau *                
        c = getCH(f);
        return TIMES;
    } else if (c == 47){    //Dau /                
        c = getCH(f);
        return SLASH;
    } else if (c == 37){    //Dau %  
        c = getCH(f);                   
        return PERCENT;
    } else if (c == 61){    //Dau =                
        c = getCH(f);
        return EQU;
    } else if (c == 60){    //Dau <                
        c = getCH(f);        
        if(c == 62){
            c = getCH(f);
            return NEQ;
        } else if(c == 61){
            c = getCH(f);
            return LEQ;
        }
        return LSS;
    } else if (c == 62){    //Dau >        
        c = getCH(f);
        if(c == 61){
            c = getCH(f);
            return GEQ;
        }
        return GRT;
    } else if (c == 40){    //Dau (       
        c = getCH(f);          
        return LPARENT;
    } else if (c == 41){    //Dau )  
        c = getCH(f);              
        return RPARENT;
    } else if (c == 46){    //Dau .   
        c = getCH(f);             
        return PERIOD;
    } else if (c == 44){    //Dau ,    
        c = getCH(f);                
        return COMMA;
    } else if (c == 59){    //Dau ;     
        c = getCH(f);            
        return SEMICOLON;
    } else if (c == 58){    //Dau :
        c = getCH(f);        
        if (c == 61){    
            c = getCH(f);                    
            return ASSIGN;
        } else {            
            return NONE;
        }
    } else if (c == 91){    //Dau [
        c = getCH(f);                
        return LBRACK;
    } else if (c == 93){    //Dau ]                 
        c = getCH(f);
        return RBRACK;
    }
    return NONE;
}

int stringcmp(char *a, char *b){
    int i = 0;
    while(a[i] != 0 || b[i] != 0){
        if(tolower(a[i]) != tolower(b[i])){
            return -1;
        }
        i++;
    }
    return 0;
}

int checkkeyword(char a[]){
    if(stringcmp(a, "Begin") == 0){
        return 0;
    } else if (stringcmp(a, "Call") == 0){
        return 1;
    } else if (stringcmp(a, "Const") == 0){
        return 2;
    } else if (stringcmp(a, "Do") == 0){
        return 3;
    } else if (stringcmp(a, "Else") == 0){
        return 4;
    } else if (stringcmp(a, "End") == 0){
        return 5;
    } else if (stringcmp(a, "For") == 0){
        return 6;
    } else if (stringcmp(a, "If") == 0){
        return 7;
    } else if (stringcmp(a, "ODD") == 0){
        return 8;
    } else if (stringcmp(a, "Procedure") == 0){
        return 9;
    }  else if (stringcmp(a, "Program") == 0){
        return 10;
    } else if (stringcmp(a, "Then") == 0){
        return 11;
    } else if (stringcmp(a, "To") == 0){
        return 12;
    } else if (stringcmp(a, "Var") == 0){
        return 13;
    } else if (stringcmp(a, "While") == 0){
        return 14;
    } else if (stringcmp(a, "Writeln") == 0){
        return 15;
    } else if (stringcmp(a, "Readln") == 0){
        return 16;
    }
    return -1;
}

int scan(int argc, char *argv[]){
    FILE *f;
    f = fopen(argv[1], "r+");
    while((token = getToken(f)) != NONE){
        // if(token == NUMBER){
        //     printf("NUMBER %d\n", num);
        // } else if(token == IDENT){
        //     int ex = checkkeyword(id);
        //     if(ex != -1){
        //         printf("%s\n", keyword[ex]);
        //     } else {
        //         printf("IDENT %s\n", id);
        //     }
        // } else {
        //     int ex = (int) token - 18;
        //     printf("%s\n", tokentype[ex]);
        // }
    }
    if(!feof(f)){
        printf("Xau loi!!\n");
    }
    fclose(f);
    return 0;
}