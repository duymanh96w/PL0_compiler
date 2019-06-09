#include <stdio.h>
#include <stdlib.h>

typedef enum {
    OP_LA = 0,   // Load Address:
    OP_LV,   // Load Value:
    OP_LC,   // load Constant
    OP_LI,   // Load Indirect
    OP_INT,  // Increment t
    OP_DCT,  // Decrement t
    OP_J,    // Jump
    OP_FJ,   // False Jump
    OP_HLT,  // Halt
    OP_ST,   // Store
    OP_CALL, // Call
    OP_EP,   // Exit Procedure
    OP_EF,   // Exit Function
    OP_RC,   // Read Char
    OP_RI,   // Read Integer
    OP_WRC,  // Write Char
    OP_WRI,  // Write Int
    OP_WLN,  // WriteLN
    OP_ADD,  // Add
    OP_SUB,  // Substract
    OP_MUL,  // Multiple
    OP_DIV,  // Divide
    OP_NEG,  // Negative
    OP_CV,   // Copy Top
    OP_EQ,   // Equal
    OP_NE,   // Not Equal
    OP_GT,   // Greater
    OP_LT,   // Less
    OP_GE,   // Greater or Equal
    OP_LE,   // Less or Equal
    OP_BP    // Break point.
} OpCode;

typedef struct {
    OpCode op;
    int p;
    int q;
} instruction;

instruction code[2000];
int stack[2000];
int codeLength = 0;

int base(int L, int b){
    int c = b;
    while(L > 0){
        c = stack[c+3];
        L--;
    }
    return c;
}

void interpreter(){
    //TEST BELOW
    // code[0].op = OP_J; code[0].q = 1;
    // code[1].op = OP_INT; code[1].q = 5;
    // code[2].op = OP_LA; code[2].p = 0; code[2].q = 4;
    // code[3].op = OP_LC; code[3].q = 5;
    // code[4].op = OP_ST;
    // code[5].op = OP_LA; code[5].p = 0; code[5].q = 4;
    // code[6].op = OP_LC; code[6].q = 10;
    // code[7].op = OP_LV; code[7].p = 0; code[7].q = 4;
    // code[8].op = OP_MUL;
    // code[9].op = OP_ST;
    // code[10].op = OP_LV; code[10].p = 0; code[10].q = 4;
    // code[11].op = OP_WRI;
    // code[12].op = OP_WLN;
    // code[13].op = OP_HLT;
    printf("\n-----------------------\nKET QUA: \n");
    int pc = 0, t = -1, b = 0;
    do {
        switch(code[pc].op){
            case OP_LA:
                // t:=t+1;  s[t]:=base(p)+q;
                t++;
                stack[t] = base(code[pc].p, b) + code[pc].q;
                break;
            case OP_LV:   
                // t:=t+1;  s[t]:=s[base(p)+q];
                t++;
                stack[t] = stack[base(code[pc].p, b) + code[pc].q];
                break;
            case OP_LC:
                // t:=t+1;  s[t]:=q;
                t++;
                stack[t] = code[pc].q;  
                break;
            case OP_LI:  
                // s[t]:=s[s[t]];
                stack[t] = stack[stack[t]];
                break;
            case OP_INT: 
                // t:=t+q;
                t += code[pc].q;
                break; 
            case OP_DCT: 
                // t:=t-q;
                t -= code[pc].q;
                break;
            case OP_J:                  
                pc = code[pc].q - 1;
                break;  
            case OP_FJ: 
                // if  s[t]=0  then  pc:=q-1;  t:=t-1;
                if(stack[t] == 0){
                    pc = code[pc].q - 1;
                }
                t--;
                break;  
            case OP_HLT:
                // printf("\n%d %d %d\n", pc, b, t);
                exit(1);
                break;   
            case OP_ST:
                // s[s[t-1]]:=s[t];  t:=t-2;
                stack[stack[t-1]] = stack[t];
                t -= 2;
                break;   
            case OP_CALL: 
                // s[t+2]:=b; s[t+3]:=pc;  s[t+4]:=base(p);  b:=t+1;  pc:=q;
                // printf("\n%d %d %d\n", pc, b, t);
                stack[t+2] = b;
                stack[t+3] = pc;
                stack[t+4] = base(code[pc].p, b);
                b = t + 1;
                pc = code[pc].q - 1;
                // printf("\n%d %d %d\n", pc, b, t);
                // printf("\n%d %d %d\n", stack[t+2], stack[t+3], stack[t+4]);
                break;
            case OP_EP: 
                // t:=b-1;  pc:=s[b+2];  b:=s[b+1];
                t = b-1;
                pc = stack[b+2];
                b = stack[b+1]; 
                break; 
            case OP_EF:
                // t:=b;  pc:=s[b+2];  b:=s[b+1];
                t = b;
                pc = stack[b+2];
                b = stack[b+1];
                break;   
            case OP_RC:
                break;  
            case OP_RI:
                // read integer to s[s[t]];  t:=t-1;
                scanf("%d", &stack[stack[t]]);
                t--;
                break;    
            case OP_WRC:                
                break;  
            case OP_WRI:
                // write integer from s[t];  t:=t-1;
                printf("%d\n", stack[t]);
                t--;
                break;  
            case OP_WLN:
                printf("\n");
                break;  
            case OP_ADD:
                // t:=t-1;  s[t]:=s[t]+s[t+1];
                t--;
                stack[t] += stack[t+1];
                break;  
            case OP_SUB:
                // t:=t-1;  s[t]:=s[t]-s[t+1];
                t--;
                stack[t] -= stack[t+1];
                break;  
            case OP_MUL:  
                // t:=t-1;  s[t]:=s[t]*s[t+1];
                t--;
                stack[t] *= stack[t+1];
                break;
            case OP_DIV:  
                // t:=t-1;  s[t]:=s[t]/s[t+1];
                t--;
                stack[t] /= stack[t+1];
                break;
            case OP_NEG:
                // s[t]:=-s[t];
                stack[t] = -stack[t];  
                break;
            case OP_CV:   
                // s[t+1]:=s[t];  t:=t+1;
                stack[t+1] = stack[t];
                t++;
                break;
            case OP_EQ:
                // t:=t-1;  if s[t] = s[t+1] then s[t]:=1 else s[t]:=0;
                t--;
                if(stack[t] == stack[t+1]){
                    stack[t] = 1;
                } else {
                    stack[t] = 0;
                }
                break;
            case OP_NE:
                // t:=t-1;  if s[t] != s[t+1] then s[t]:=1 else s[t]:=0;
                t--;
                if(stack[t] != stack[t+1]){
                    stack[t] = 1;
                } else {
                    stack[t] = 0;
                }
                break;
            case OP_GT:   
                // t:=t-1;  if s[t] > s[t+1] then s[t]:=1 else s[t]:=0;
                t--;
                if(stack[t] > stack[t+1]){
                    stack[t] = 1;
                } else {
                    stack[t] = 0;
                }
                break;
            case OP_LT:
                // t:=t-1;  if s[t] < s[t+1] then s[t]:=1 else s[t]:=0;
                t--;
                if(stack[t] < stack[t+1]){
                    stack[t] = 1;
                } else {
                    stack[t] = 0;
                }
                break;
            case OP_GE:
                // t:=t-1;  if s[t] >= s[t+1] then s[t]:=1 else s[t]:=0;
                t--;
                if(stack[t] >= stack[t+1]){
                    stack[t] = 1;
                } else {
                    stack[t] = 0;
                } 
                break;
            case OP_LE:  
                // t:=t-1;  if s[t] <= s[t+1] then s[t]:=1 else s[t]:=0;
                t--;
                if(stack[t] <= stack[t+1]){
                    stack[t] = 1;
                } else {
                    stack[t] = 0;
                } 
                break;
            case OP_BP:    
                break;
        }
        pc++;
        // printf("Command: %d, base: %d \n", pc-1, b);
        // for (int i = 0; i < 30; i++)
        // {            
        //     if(i == t){
        //         printf("----> ");
        //     }
        //     printf("%d: %d\n", i, stack[i]);
        // }

        // printf("+++++++++++++++++++++++++++++++++\n");
    } while (pc < codeLength);
}

void toScr(){
    for(int i = 0; i < codeLength; i++){
        switch(code[i].op){
            case OP_LA:
                printf("LA %d %d\n", code[i].p, code[i].q);
                break;
            case OP_LV:   
                printf("LV %d %d\n", code[i].p, code[i].q);
                break;
            case OP_LC:
                printf("LC %d\n", code[i].q);
                break;
            case OP_LI:  
                printf("LI\n");
                break;
            case OP_INT: 
                printf("INT %d\n", code[i].q);
                break; 
            case OP_DCT: 
                printf("DCT %d\n", code[i].q);
                break;
            case OP_J:                  
                printf("J %d\n", code[i].q);
                break;  
            case OP_FJ: 
                printf("FJ %d\n", code[i].q);
                break;  
            case OP_HLT:
                printf("HLT\n");
                break;   
            case OP_ST:
                printf("ST\n");
                break;   
            case OP_CALL: 
                printf("CALL %d %d\n", code[i].p, code[i].q);
                break;
            case OP_EP: 
                printf("EP\n");
                break; 
            case OP_EF:
                printf("EF\n");
                break;   
            case OP_RC:
                printf("RC\n");
                break;  
            case OP_RI:
                printf("RI\n");
                break;    
            case OP_WRC:     
                printf("WRC\n");           
                break;  
            case OP_WRI:
                printf("WRI\n");
                break;  
            case OP_WLN:
                printf("WLN\n");
                break;  
            case OP_ADD:
                printf("ADD\n");
                break;  
            case OP_SUB:
                printf("SUB\n");
                break;  
            case OP_MUL:  
                printf("MUL\n");
                break;
            case OP_DIV:  
                printf("DIV\n");
                break;
            case OP_NEG:
                printf("NEG\n");
                break;
            case OP_CV:   
                printf("CV\n");
                break;
            case OP_EQ:
                printf("EQ\n");
                break;
            case OP_NE:
                printf("NE\n");
                break;
            case OP_GT:   
                printf("GT\n");
                break;
            case OP_LT:
                printf("LT\n");
                break;
            case OP_GE:
                printf("GE\n");
                break;
            case OP_LE:  
                printf("LE\n");
                break;
            case OP_BP:    
                printf("BP\n");
                break;
        }
    }
}

void toAsm(int argc, char *argv[]){
    FILE *f;
    int i;
    char asmName[100];
    strcpy(asmName, argv[argc-1]);
    for(i = 0; i < 100; i++){
        if(asmName[i] == '.'){
            asmName[i] = '\0';
            break;
        }
    }
    strcpy(asmName+i, ".asm");
    // printf("%s", asmName);
    // remove(asmName);
    f = fopen(asmName, "w+");
    for (int i = 0; i < codeLength; i++){
        switch (code[i].op){
            case OP_LA:
                fprintf(f, "LA %d %d\n", code[i].p, code[i].q);
                break;
            case OP_LV:
                fprintf(f, "LV %d %d\n", code[i].p, code[i].q);
                break;
            case OP_LC:
                fprintf(f, "LC %d\n", code[i].q);
                break;
            case OP_LI:
                fprintf(f, "LI\n");
                break;
            case OP_INT:
                fprintf(f, "INT %d\n", code[i].q);
                break;
            case OP_DCT:
                fprintf(f, "DCT %d\n", code[i].q);
                break;
            case OP_J:
                fprintf(f, "J %d\n", code[i].q);
                break;
            case OP_FJ:
                fprintf(f, "FJ %d\n", code[i].q);
                break;
            case OP_HLT:
                fprintf(f, "HLT\n");
                break;
            case OP_ST:
                fprintf(f, "ST\n");
                break;
            case OP_CALL:
                fprintf(f, "CALL %d %d\n", code[i].p, code[i].q);
                break;
            case OP_EP:
                fprintf(f, "EP\n");
                break;
            case OP_EF:
                fprintf(f, "EF\n");
                break;
            case OP_RC:
                fprintf(f, "RC\n");
                break;
            case OP_RI:
                fprintf(f, "RI\n");
                break;
            case OP_WRC:
                fprintf(f, "WRC\n");
                break;
            case OP_WRI:
                fprintf(f, "WRI\n");
                break;
            case OP_WLN:
                fprintf(f, "WLN\n");
                break;
            case OP_ADD:
                fprintf(f, "ADD\n");
                break;
            case OP_SUB:
                fprintf(f, "SUB\n");
                break;
            case OP_MUL:
                fprintf(f, "MUL\n");
                break;
            case OP_DIV:
                fprintf(f, "DIV\n");
                break;
            case OP_NEG:
                fprintf(f, "NEG\n");
                break;
            case OP_CV:
                fprintf(f, "CV\n");
                break;
            case OP_EQ:
                fprintf(f, "EQ\n");
                break;
            case OP_NE:
                fprintf(f, "NE\n");
                break;
            case OP_GT:
                fprintf(f, "GT\n");
                break;
            case OP_LT:
                fprintf(f, "LT\n");
                break;
            case OP_GE:
                fprintf(f, "GE\n");
                break;
            case OP_LE:
                fprintf(f, "LE\n");
                break;
            case OP_BP:
                fprintf(f, "BP\n");
                break;
        }
    }
    fclose(f);
}

void toBin(int argc, char *argv[]){
    FILE *f;
    int i;
    char asmName[100];
    strcpy(asmName, argv[argc-1]);
    for(i = 0; i < 100; i++){
        if(asmName[i] == '.'){
            asmName[i] = '\0';
            break;
        }
    }
    strcpy(asmName+i, ".bin");
    // printf("%s", asmName);
    // remove(asmName);
    f = fopen(asmName, "w+b");
    for (int i = 0; i < codeLength; i++){
        int opcode = code[i].op;
        int p = code[i].p;
        int q = code[i].q;
        fwrite(&opcode, sizeof(int), 1, f);
        fwrite(&p, sizeof(int), 1, f);
        fwrite(&q, sizeof(int), 1, f);
    }
    fclose(f);
}