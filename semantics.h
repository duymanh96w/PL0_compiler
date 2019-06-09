#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {CONSTANT = 50, VARIABLE, ARRAY, SUBPROC, REFERENCE} VarType;

typedef struct node {
    char name[11];
    VarType kind;
    int argCount;
    int size;
    int offset;
    int beginLineOfCode;
    struct node *down;
    int arguments[10];
} node, *table;

typedef struct tablePointer {
    table tab;
    int size;
    struct tablePointer *parent;
} *list, tablePointer;

typedef struct {
    node *resNode;
    int p;
    int q;
} coordinates;

node *newNode(char _name[11], VarType _kind, int _size){
    node *temp = (node *) malloc(sizeof(node));
    strcpy(temp->name, _name);
    temp->kind = _kind;
    temp->size = _size;
    temp->down = NULL;
    return temp;
}

table newTable(){
    node *a = (node *) malloc(sizeof(node));
    strcpy(a->name, "112358");
    a->down = NULL;
    return a;
}

table pushIdent(table top, char _name[11], VarType _kind, int _size){
    node *new_node = newNode(_name, _kind, _size);
    if(top->down == NULL){
        new_node->offset = 4;
    } else {
        new_node->offset = top->offset + top->size;
    }
    new_node->down = top;
    top = new_node;
    return top;
}

void printTable(table top){
    int i;
    node *a = top;
    while(a != NULL){
        printf("name: %s, size: %d, offset: %d, kind: %d, begin line: %d, argCount: %d arguments:  ", a->name, a->size, a->offset, a->kind, a->beginLineOfCode, a->argCount);
        for(i = 0; i < 10; i++){
            printf("%d ", a->arguments[i]);
        }
        printf("\n");
        a = a->down;
    }
}

tablePointer *newTablePointer(table existTable){
    tablePointer *a = (tablePointer *) malloc(sizeof(tablePointer));
    a->tab = (node *) malloc(sizeof(node));
    strcpy(a->tab->name, existTable->name);
    a->tab->kind = existTable->kind;
    a->tab->down = existTable->down;
    a->tab->size = existTable->size;
    a->parent = NULL;
    return a;
}

list newList(){
    tablePointer *a = (tablePointer *) malloc(sizeof(tablePointer));
    a->parent = NULL;
    return a;
}

list pushTable(list currentTable, table existTable){
    tablePointer *newTabPointer = newTablePointer(existTable);
    newTabPointer->parent = currentTable;
    currentTable = newTabPointer;
    return currentTable;
}

void printList(list currentTable){
    tablePointer *a = currentTable;
    while(a != NULL){
        printTable(a->tab);
        a = a->parent;
    }
}

// node *lookupAll(list currentTable, char x[11]){
//     tablePointer *a = currentTable;
//     while(a != NULL){
//         table b = a->tab;
//         node *c = b;
//         while(c != NULL){
//             if(stringcmp(c->name, x) == 0){
//                 return c;
//             }
//             c = c->down;
//         }
//         a = a->parent;
//     }
//     return NULL;
// }

coordinates *lookupAll(list currentTable, char x[11]){
    tablePointer *a = currentTable;
    coordinates *tmp = (coordinates *) malloc(sizeof(coordinates));
    tmp->p = 0;
    while(a != NULL){
        tmp->q = 3;
        table b = a->tab;
        node *c = b;
        while(c != NULL){
            if(stringcmp(c->name, x) == 0){
                tmp->resNode = c;
                tmp->q = c->offset;
                return tmp;
            }
            c = c->down;
        }
        a = a->parent;
        tmp->p += 1;
    }
    return NULL;
}

node *lookupCurrent(list currentTable, char x[11]){
    tablePointer *a = currentTable;
    node *b = a->tab;
    while(b != NULL){
        if(stringcmp(b->name, x) == 0){
            return b;
        }
        b = b->down;
    }
    return NULL;
}

int getArgCount(list currentTable){
    list a = currentTable;
    if(a->parent->parent == NULL){
        return 0;
    } else {
        return a->parent->tab->argCount;
    }
}