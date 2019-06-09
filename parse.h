#include <stdio.h>
#include <stdlib.h>
void genCode(OpCode operation, int p, int q);
void LValue(FILE *f, list tableList);
void factor(FILE *f, list tableList); 
void term(FILE *f, list tableList);
void expression(FILE *f, list tableList); 
void condition(FILE *f, list tableList);
void statement(FILE *f, list tableList);
void block(FILE *f, list tableList);
void program(FILE *f);
void parse(int argc, char *argv[]);

void genCode(OpCode operation, int p, int q){
    code[codeLength].op = operation;
    code[codeLength].p = p;
    code[codeLength].q = q;
    codeLength++;
}

void error(int errorCase)
{
    switch (errorCase)
    {
    case 1:
        printf("\nThieu tu khoa program\n");
        exit(1);
    case 2:
        printf("\nThieu ten chuong trinh\n");
        exit(1);
    case 3:
        printf("\nThieu dau .\n");
        exit(1);
    case 4:
        printf("\nThieu dau ;\n");
        exit(1);
    case 5:
        printf("\nCau lenh dieu kien sai cu phap\n");
        exit(1);
    case 6:
        printf("\nThieu dau )\n");
        exit(1);
    case 7:
        printf("\nBieu thuc khong duoc chua ten thu tuc\n");
        exit(1);
    case 8:
        printf("\nThieu dau ]\n");
        exit(1);
    case 9:
        printf("\nThieu dau assign\n");
        exit(1);
    case 10:
        printf("\nLoi cu phap\n");
        exit(1);
    case 11:
        printf("\nThieu END\n");
        exit(1);
    case 12:
        printf("\nThieu THEN\n");
        exit(1);
    case 13:
        printf("\nThieu DO\n");
        exit(1);
    case 14:
        printf("\nThieu TO\n");
        exit(1);
    case 15:
        printf("\nThieu dau =\n");
        exit(1);
    case 16:
        printf("\nSau CALL phai la 1 IDENT\n");
        exit(1);
    case 17:
        printf("\nLoi cu phap, sau ten chuong trinh phai la CONST, VAR, PROCEDURE hoac BEGIN");
        exit(1);
    case 18:
        printf("\nSau CONST phai la IDENT\n");
        exit(1);
    case 19:
        printf("\nSau VAR phai la IDENT\n");
        exit(1);
    case 20:
        printf("\nThieu IDENT sau VAR\n");
        exit(1);
    case 21:
        printf("\nCan gan cho 1 number\n");
        exit(1);
    case 22:
        printf("\nKhai bao du lieu kieu mang can co chi so la number\n");
        exit(1);
    case 23:
        printf("\nThieu BEGIN\n");
        exit(1);
    case 24:
        printf("\nSau cu phap factor\n");
        exit(1);
    case 25:
        printf("\nSau FOR phai la IDENT\n");
        exit(1);
    case 26:
        printf("\nSau PROCEDURE phai la IDENT\n");
        exit(1);
    case 27:
        printf("\nThieu VAR hoac IDENT\n");
        exit(1);
    case 28: 
        printf("\nThieu IDENT\n");
        exit(1);
    case 29:
        printf("\nIDENT %s da ton tai\n", id);
        exit(1);
    case 30:
        printf("\nIDENT %s chua duoc khai bao\n", id);
        exit(1);
    case 31:
        printf("\nSu dung sai cach, IDENT khong phai kieu ARRAY");
        exit(1);
    case 32:
        printf("\nKhong duoc phep thay doi hang so\n");
        exit(1);
    case 33:
        printf("\nThieu chi so cho bien mang\n");
        exit(1);
    case 34: 
        printf("\nPROCEDURE %s chua duoc khai bao\n", id);
        exit(1);
    case 35:
        printf("\nSo luong tham so khac voi khai bao\n");
        exit(1);
    case 36:
        printf("\nTruyen sai kieu tham so\n");
        exit(1);
    case 37:
        printf("\nIDENT khong phai ten PROCEDURE\n");
        exit(1);
    case 38:
        printf("\nKhong duoc su dung PROCEDURE o day\n");
        exit(1);
    case 39:
        printf("\nThieu dau (\n");
        exit(1);
    case 40:
        printf("\nThieu dau [\n");
        exit(1);
    }
}

void LValue(FILE *f, list tableList){
    if(token == IDENT){
        int ex = checkkeyword(id);
        if(ex == -1){
            coordinates *res = lookupAll(tableList, id);
            if(res->resNode->kind != VARIABLE && res->resNode->kind != ARRAY){
                error(36);
            }
            genCode(OP_LA, res->p, res->q);
            token = getToken(f);
            if(token == LBRACK){
                token = getToken(f);
                expression(f, tableList);
                genCode(OP_ADD, 0, 0);
                if(token == RBRACK){
                    token = getToken(f);
                } else {
                    error(8);
                }
            } else if(res->resNode->kind == ARRAY){
                error(36);
            }
            if(token != COMMA && token != RPARENT){
                error(36);
            }
        } else {
            error(36);
        }        
    } else {
        error(36);
    }
}

void factor(FILE *f, list tableList){
    if(token == NUMBER){
        genCode(OP_LC, 0, num);
        token = getToken(f);
    } else if (token == LPARENT){
        token = getToken(f);
        expression(f, tableList);
        if(token == RPARENT){
            token = getToken(f);
        } else {
            error(6);
        }
    } else if (token == IDENT){
        int ex = checkkeyword(id);
        if(ex == -1){
            int countLBRACK = 0;
            coordinates *res2 = lookupAll(tableList, id);
            if(res2 == NULL){
                error(30);
            }
            if(res2->resNode->kind == SUBPROC){
                error(38);
            }
            //Can sua o day
            if(res2->resNode->kind == REFERENCE){
                genCode(OP_LV, res2->p, res2->q);
            } else {
                genCode(OP_LA, res2->p, res2->q);
            }            
            token = getToken(f);
            if(token == LBRACK){
                countLBRACK = 1;
                if(res2->resNode->kind != ARRAY){
                    error(31);
                }
                token = getToken(f);
                expression(f, tableList);
                genCode(OP_ADD, 0, 0);
                if(token == RBRACK){
                    token = getToken(f);
                } else {
                    error(8);
                }                         
            }
            genCode(OP_LI, 0, 0);
            if(res2->resNode->kind == ARRAY && countLBRACK == 0){
                error(33);
            }
        } else {
            error(7);
        }
    } else {
        error(24);
    }
}

void term(FILE *f, list tableList){
    factor(f, tableList);
    while((token ==  TIMES) || (token == SLASH)){
        TokenType op = token;
        token = getToken(f);
        factor(f, tableList);
        if(op == TIMES){
            genCode(OP_MUL, 0, 0);
        } else {
            genCode(OP_DIV, 0, 0);
        }
    }
}

void expression(FILE *f, list tableList){
    if((token == PLUS) || (token == MINUS)){
        TokenType op = token;
        token = getToken(f);
        if(op == MINUS){
            genCode(OP_NEG, 0, 0);
        }
    }
    term(f, tableList);
    while((token == PLUS) || (token == MINUS)){
        TokenType op2 = token;
        token = getToken(f);
        term(f, tableList);
        if(op2 == PLUS){
            genCode(OP_ADD, 0, 0);
        } else {
            genCode(OP_SUB, 0, 0);
        }
    }
}

void condition(FILE *f, list tableList){
    if(token == IDENT){
        int ex18 = checkkeyword(id);
        if(ex18 == 8){      //ODD
            token = getToken(f);
            expression(f, tableList);
            return;
        }
    }
    expression(f, tableList);
    if(token == EQU || token == NEQ || token == LSS || token == LEQ || token == GRT || token == GEQ){
        TokenType op = token;
        token = getToken(f);
        expression(f, tableList);
        switch(op){
            case EQU:
                genCode(OP_EQ, 0, 0);
                break;
            case NEQ:
                genCode(OP_NE, 0, 0);
                break;
            case LSS:
                genCode(OP_LT, 0, 0);
                break;
            case LEQ:
                genCode(OP_LE, 0, 0);
                break;
            case GRT:
                genCode(OP_GT, 0, 0);
                break;
            case GEQ:
                genCode(OP_GE, 0, 0);
                break;
        }
    } else {
        error(5);
    }
}

void statement(FILE *f, list tableList){
    if(token == IDENT){
        int ex = checkkeyword(id);
        if(ex == -1){           //IDENT
            coordinates *res = lookupAll(tableList, id);
            int countLBRACK = 0;
            if(res == NULL){
                error(30);
            }
            if(res->resNode->kind == SUBPROC){
                error(38);
            }
            //Can sua o day
            if(res->resNode->kind == REFERENCE){
                genCode(OP_LV, res->p, res->q);
            } else {
                genCode(OP_LA, res->p, res->q);
            }            
            token = getToken(f);
            if(token == LBRACK){
                countLBRACK = 1;
                if(res->resNode->kind != ARRAY){
                    error(31);
                }
                token = getToken(f);
                expression(f, tableList);
                genCode(OP_ADD, 0, 0);
                if(token == RBRACK){
                    token = getToken(f);
                } else {
                    error(8);
                }
            }
            if(res->resNode->kind == ARRAY && countLBRACK == 0){
                error(33);
            }
            if(token == ASSIGN){
                if(res->resNode->kind == CONSTANT){
                    error(32);
                }
                token = getToken(f);
                expression(f, tableList);
                genCode(OP_ST, 0, 0);
            } else {
                error(9);
            }
        } else if (ex == 1){    //Call
            token = getToken(f);
            if(token == IDENT){
                int ex2 = checkkeyword(id);
                if(ex2 == 15){      //Writeln
                    token = getToken(f);
                    if(token == LPARENT){
                        token = getToken(f);
                        if(token == IDENT){
                            int ex19 = checkkeyword(id);
                            if(ex19 == -1){
                                coordinates *res = lookupAll(tableList, id);
                                if(res == NULL){
                                    error(30);                printf("fjaklsfj;l");
                                }
                                if(res->resNode->kind == SUBPROC){
                                    error(38);
                                }
                                if(res->resNode->kind == VARIABLE || res->resNode->kind == CONSTANT){
                                    genCode(OP_LA, res->p, res->q);
                                    genCode(OP_LI, 0, 0);
                                    genCode(OP_WRI, 0, 0);
                                    token = getToken(f);
                                } else if(res->resNode->kind == REFERENCE){
                                    genCode(OP_LV, res->p, res->q);
                                    genCode(OP_LI, 0, 0);
                                    genCode(OP_WRI, 0, 0);
                                    token = getToken(f);
                                } else if(res->resNode->kind == ARRAY){
                                    token = getToken(f);
                                    if(token == LBRACK){
                                        genCode(OP_LA, res->p, res->q);
                                        token = getToken(f);
                                        expression(f, tableList);
                                        genCode(OP_ADD, 0, 0);
                                        genCode(OP_LI, 0, 0);
                                        genCode(OP_WRI, 0, 0);
                                        if(token == RBRACK){
                                            token = getToken(f);
                                        } else {
                                            error(8);
                                        }
                                    } else {
                                        error(40);
                                    }                                    
                                }                                                            
                                if(token == RPARENT){
                                    token = getToken(f);
                                } else {
                                    error(6);
                                }
                            } else {
                                error(28);
                            }
                        } else {
                            error(28);
                        }
                    } else {
                        genCode(OP_WLN, 0, 0);
                    }
                } else if (ex2 == 16){      //Readln
                    token = getToken(f);
                    if(token == LPARENT){
                        token = getToken(f);
                        if(token == IDENT){
                            int ex20 = checkkeyword(id);
                            if(ex20 == -1){
                                coordinates *res = lookupAll(tableList, id);
                                if(res == NULL){
                                    error(30);
                                }
                                if(res->resNode->kind == CONSTANT){
                                    error(32);
                                }
                                if(res->resNode->kind == SUBPROC){
                                    error(38);
                                }
                                if(res->resNode->kind == VARIABLE){
                                    genCode(OP_LA, res->p, res->q);
                                    genCode(OP_RI, 0, 0);
                                    token = getToken(f);
                                } else if(res->resNode->kind == REFERENCE){
                                    genCode(OP_LV, res->p, res->q);
                                    genCode(OP_RI, 0, 0);
                                    token = getToken(f);
                                } else if(res->resNode->kind == ARRAY){
                                    token = getToken(f);
                                    if(token == LBRACK){
                                        genCode(OP_LA, res->p, res->q);
                                        token = getToken(f);
                                        expression(f, tableList);
                                        genCode(OP_ADD, 0, 0);
                                        genCode(OP_RI, 0, 0);
                                        if(token == RBRACK){
                                            token = getToken(f);
                                        } else {
                                            error(8);
                                        }
                                    } else {
                                        error(40);
                                    }                                    
                                }
                                if(token == RPARENT){
                                    token = getToken(f);
                                } else {
                                    error(6);
                                }
                            } else {
                                error(28);
                            }
                        } else {
                            error(28);
                        }
                    } else {
                        error(39);
                    }
                } else if(ex2 == -1){       //Ident
                    coordinates *res3 = lookupAll(tableList, id);
                    if(res3 == NULL){
                        error(34);
                    }
                    if(res3->resNode->kind != SUBPROC){
                        error(37);
                    }
                    res3->q = res3->resNode->beginLineOfCode;
                    genCode(OP_INT, 0, 4); 
                    token = getToken(f);
                    if(token == LPARENT){
                        int varCount = 0;
                        do{
                            token = getToken(f);
                            if(res3->resNode->arguments[varCount] == 1){
                                LValue(f, tableList);
                            } else {
                                expression(f, tableList);
                            }                                                            
                            varCount++;
                        } while(token == COMMA);
                        if(varCount != res3->resNode->argCount){
                            error(35);
                        }
                        if(token == RPARENT){
                            token = getToken(f);
                        } else {
                            error(6);
                        }
                    } else {
                        if(res3->resNode->argCount != 0){
                            error(35);
                        }
                    }
                    genCode(OP_DCT, 0, 4+res3->resNode->argCount);
                    genCode(OP_CALL, res3->p, res3->q);
                } else {
                    error(16);
                }
            } else {
                error(16);
            }
        } else if (ex == 0){    //Begin
            token = getToken(f);
            statement(f, tableList);
            while(token == SEMICOLON){
                token = getToken(f);
                statement(f, tableList);
            }
            if(token == IDENT){
                int ex3 = checkkeyword(id);
                if(ex3 == 5){   //End
                    token = getToken(f);
                } else {
                    error(11);
                }
            } else {
                error(11);
            }
        } else if (ex == 7){    //If
            token = getToken(f);
            condition(f, tableList);
            int line1 = codeLength;
            genCode(OP_FJ, 0, 0);
            if(token == IDENT){
                int ex4 = checkkeyword(id);
                if(ex4 == 11){      //Then
                    token = getToken(f);
                    statement(f, tableList);
                    code[line1].q = codeLength;
                    if(token == IDENT){
                        int ex5 = checkkeyword(id);
                        if(ex5 == 4){       //Else
                            code[line1].q += 1;
                            int line2 = codeLength;
                            genCode(OP_J, 0, 0);
                            token = getToken(f);
                            statement(f, tableList);
                            code[line2].q = codeLength;
                        }
                    }
                } else {
                    error(12);
                }
            } else {
                error(12);
            }
        } else if (ex == 14){   //While
            token = getToken(f);
            int line1 = codeLength;
            condition(f, tableList);
            int line2 = codeLength;
            genCode(OP_FJ, 0, 0);
            if(token == IDENT){
                int ex6 = checkkeyword(id);
                if(ex6 == 3){   //Do
                    token = getToken(f);
                    statement(f, tableList);
                    genCode(OP_J, 0, line1);
                    code[line2].q = codeLength;
                } else {
                    error(13);
                }
            } else {
                error(13);
            }
        } else if (ex == 6){    //For
            token = getToken(f);
            if(token == IDENT){
                int ex7 = checkkeyword(id);
                if(ex7 == -1){
                    coordinates *res3 = lookupAll(tableList, id);
                    if(res3 == NULL){
                        error(30);
                    }
                    if(res3->resNode->kind == CONSTANT){
                        error(32);
                    }
                    if(res3->resNode->kind == SUBPROC){
                        error(38);
                    }
                    
                    //O day chua biet co can sua khong
                    if(res3->resNode->kind == REFERENCE){
                        genCode(OP_LV, res3->p, res3->q);
                    } else {
                        genCode(OP_LA, res3->p, res3->q);
                    }            
                    genCode(OP_CV, 0, 0);
                    token = getToken(f);                    
                    if(token == ASSIGN){                        
                        token = getToken(f);
                        expression(f, tableList);
                        genCode(OP_ST, 0, 0);
                        if(token == IDENT){
                            int ex8 = checkkeyword(id);
                            if(ex8 == 12){      //To
                                int line1 = codeLength;
                                genCode(OP_CV, 0, 0);
                                genCode(OP_LI, 0, 0);
                                token = getToken(f);
                                expression(f, tableList);
                                genCode(OP_LE, 0, 0);
                                int line2 = codeLength;
                                genCode(OP_FJ, 0, 0);
                                if(token == IDENT){
                                    int ex9 = checkkeyword(id);
                                    if(ex9 == 3){       //Do
                                        token = getToken(f);  
                                        statement(f, tableList);
                                        genCode(OP_CV, 0, 0);
                                        genCode(OP_CV, 0, 0);
                                        genCode(OP_LI, 0, 0);
                                        genCode(OP_LC, 0, 1);
                                        genCode(OP_ADD, 0, 0);
                                        genCode(OP_ST, 0, 0);
                                        genCode(OP_J, 0, line1);
                                        code[line2].q = codeLength;
                                        genCode(OP_DCT, 0, 0);
                                    } else {
                                        error(13);
                                    }
                                } else {
                                    error(13);
                                }
                            } else {
                                error(14);
                            }
                        } else {
                            error(14);
                        }
                    } else {
                        error(9);
                    }
                } else {
                    error(25);
                }
            } else {
                error(25);
            }
        }
    } else {
        
    }
}

void block(FILE *f, list tableList){
    genCode(OP_INT, 0, 4);
    if(token == IDENT){
        int ex10 = checkkeyword(id);
        if(ex10 == 2){          //Const      
            int count = 0;      
            do{
                token = getToken(f);
                if(token == IDENT){
                    int ex12 = checkkeyword(id);
                    if(ex12 == -1){
                        if(lookupCurrent(tableList, id) == NULL){
                            tableList->tab = pushIdent(tableList->tab, id, CONSTANT, 1);
                        } else {
                            error(29);
                        }
                        count ++;
                        token = getToken(f);
                        if(token == EQU){
                            token = getToken(f);
                            if(token == NUMBER){
                                genCode(OP_LC, 0, num);
                                token = getToken(f);
                            } else {
                                error(21);
                            }
                        } else {
                            error(15);
                        }
                    } else {
                        error(18);
                    }
                }
            } while (token == COMMA);
            if(count == 0){
                error(18);
            }
            if(token == SEMICOLON){
                token = getToken(f);
                ex10 = checkkeyword(id);
            } else {
                error(4);
            }
        } 
        if (ex10 == 13){     //Var
            int count = 0;
            do{
                token = getToken(f);
                if(token == IDENT){
                    int ex13 = checkkeyword(id);
                    if(ex13 == -1){
                        genCode(OP_INT, 0, 1);
                        if(lookupCurrent(tableList, id) == NULL){
                            tableList->tab = pushIdent(tableList->tab, id, VARIABLE, 1);
                        } else {
                            error(29);
                        }
                        count++;
                        token = getToken(f);
                        if(token == LBRACK){
                            token = getToken(f);
                            if(token == NUMBER){
                                tableList->tab->kind = ARRAY;
                                tableList->tab->size = num;
                                genCode(OP_INT, 0, num-1);
                                token = getToken(f);
                                if(token == RBRACK){
                                    token = getToken(f);
                                } else {
                                    error(8);
                                }
                            } else {
                                error(22);
                            }
                        }
                    } else {
                        error(19);
                    }
                } else {
                    error(19);
                }  
            } while(token == COMMA);
            if(count == 0){
                error(20);
            }
            if(token == SEMICOLON){
                token = getToken(f);
                ex10 = checkkeyword(id);
            } else {
                error(4);
            }
        } 
        if (ex10 == 9){      //Procedure
            do{                          
                token = getToken(f);
                if(token == IDENT){
                    int ex14 = checkkeyword(id);
                    if(ex14 == -1){         
                        int line1 = codeLength;               
                        genCode(OP_J, 0, 0);                                               
                        if(lookupCurrent(tableList, id) == NULL){
                            tableList->tab = pushIdent(tableList->tab, id, SUBPROC, 0);
                            tableList->tab->beginLineOfCode = codeLength;
                        } else {
                            error(29);
                        }
                        table newT = newTable();
                        tableList = pushTable(tableList, newT);           
                        token = getToken(f);
                        if(token == LPARENT){
                            int varCount = 0;
                            do{
                                token = getToken(f);
                                if(token == IDENT){
                                    int ex15 = checkkeyword(id);
                                    if(ex15 == 13){     //Var
                                        genCode(OP_INT, 0, 1);
                                        tableList->parent->tab->arguments[varCount] = 1;
                                        varCount++;                                        
                                        token = getToken(f);
                                        if(token == IDENT){
                                            int ex16 = checkkeyword(id);
                                            if(ex16 == -1){
                                                if(lookupCurrent(tableList, id) == NULL){
                                                    tableList->tab = pushIdent(tableList->tab, id, REFERENCE, 1);
                                                } else {
                                                    error(29);
                                                }                                                
                                                token = getToken(f);
                                            } else {
                                                error(28);
                                            }
                                        } else {
                                            error(28);
                                        }
                                    } else if(ex15 == -1){
                                        genCode(OP_INT, 0, 1);
                                        tableList->parent->tab->arguments[varCount] = -1;
                                        varCount++;                                        
                                        if(lookupCurrent(tableList, id) == NULL){
                                            tableList->tab = pushIdent(tableList->tab, id, VARIABLE, 1);
                                        } else {
                                            error(29);
                                        }
                                        token = getToken(f);
                                    } else {
                                        error(27);
                                    }
                                } else {
                                    error(27);
                                }
                            } while (token == SEMICOLON);
                            tableList->parent->tab->argCount = varCount;
                            if(token == RPARENT){
                                token = getToken(f);
                            } else {
                                error(6);
                            }                            
                        }                        
                        if(token == SEMICOLON){
                            token = getToken(f);
                        } else {
                            error(4);
                        }
                        block(f, tableList);
                        genCode(OP_EP, 0, 0);
                        code[line1].q = codeLength;
                        tableList->parent->tab->size = 0;
                        tableList = tableList->parent;
                        if(token == SEMICOLON){
                            token = getToken(f);
                            ex10 = checkkeyword(id);
                        } else {
                            error(4);
                        }
                    } else {
                        error(26);
                    }
                } else {
                    error(26);
                }
            } while(ex10 == 9);            
        } 
        if (ex10 == 0){      //Begin
            token = getToken(f);
            statement(f, tableList);
            while(token == SEMICOLON){
                token = getToken(f);
                statement(f, tableList);
            }
            if(token == IDENT){
                int ex11 = checkkeyword(id);
                if(ex11 == 5){      //End
                    token = getToken(f);
                } else {
                    error(4);
                }
            } else {
                if(token == PERIOD){
                    error(11);
                } else {
                    error(4);
                }                
            }
        } else {
            error(23);
        }
    } else {
        error(17);
    }
    // printList(tableList);
}

void program(FILE *f){
    list tableList = newList();
    table first = newTable();
    tableList = pushTable(tableList, first);
    if(token == IDENT){
        int ex = checkkeyword(id);
        if(ex == 10){       //Program
            token = getToken(f);
            if(token == IDENT){
                int ex17 = checkkeyword(id);
                if(ex17 == -1){
                    token = getToken(f);
                    if(token == SEMICOLON){
                        token = getToken(f);
                        block(f, tableList);
                        if(token == PERIOD){
                            genCode(OP_HLT, 0, 0);
                            printf("\nThanh cong!!!\n");
                        } else {
                            error(3);
                        }
                    } else {
                        error(4);
                    }
                } else {
                    error(2);
                }
            } else {
                error(2);
            }
        } else {
            error(1);
        }
    } else {
        error(1);
    }
}

void parse(int argc, char *argv[])
{
    FILE *f;
    f = fopen(argv[argc-1], "r+");
    token = getToken(f);
    program(f);
    fclose(f);
}

