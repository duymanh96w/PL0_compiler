#include "scanner.h"
#include "semantics.h"
#include "virtualmachine.h"
#include "parse.h"


void main(int argc, char *argv[]){
    // printf("%d", argc);
    // parse(argc, argv);
    // toScr();
    // toAsm(argc, argv);
    // interpreter();
    // for(int i = 0; i < 8; i++){
    //     printf("%d\n", stack[i]);
    // }
    // FILE *f;
    // f = fopen(argv[1], "r+");
    // token = getToken(f);
    // if(token == IDENT){
    //     printf("TRUE");
    // } else {
    //     printf("FALSE");
    // }
    // fclose(f);
    // scan(argc, argv);
    if(argc == 2){
        parse(argc, argv);
    } else if (argc == 3){        
        if(strcmp(argv[1], "-s") == 0){
            parse(argc, argv);
            toAsm(argc, argv);
        } else if(strcmp(argv[1], "-b") == 0){
            parse(argc, argv);
            toBin(argc, argv);
        } else if(strcmp(argv[1], "-r") == 0){
            parse(argc, argv);
            interpreter();
        } else {
            printf("\nTham so khong chinh xac!!!\n");
        }
    } else if (argc == 5){
        if(strcmp(argv[1], "-s") == 0){
            if(strcmp(argv[2], "-b") == 0){
                if(strcmp(argv[3], "-r") == 0){
                    parse(argc, argv);
                    toAsm(argc, argv);
                    toBin(argc, argv);
                    interpreter();
                } else {
                    printf("\nTham so khong chinh xac!!!\n");
                }
            } else {
                printf("\nTham so khong chinh xac!!!\n");
            }
        } else {
            printf("\nTham so khong chinh xac!!!\n");
        }
    } else {
        printf("\nTham so khong chinh xac!!!\n");
    }
}