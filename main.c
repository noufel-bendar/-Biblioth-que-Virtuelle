#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//structs

struct person;
struct book;
struct queue;
struct loan_history;

typedef struct person{
    char name[30];
    char address[30];
    int phone_number;
    struct person* next;
}person;

typedef struct queue{
    person* head;
    person* tail;
}queue;

typedef struct book{
    char title[30];
    char author[30];
    bool available;
    struct book* next;
    queue l;
}book;

typedef struct loan_history{
    char name[30];
    char address[30];
    int phone_number;
    char title[30];
    char author[30];
    char date[10];
    struct loan_history* next;
}loan_history;