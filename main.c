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

// need this shit to clear the input buffer or else scanf will skip lines

void clear_input_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// book functions

book* search_book(book* b, char title[], char author[]){
    book* temp=b;

    while(temp!=NULL && !(strcmp(temp->title, title)==0 && strcmp(temp->author, author)==0)){
        temp=temp->next;
    }

    if(temp==NULL){
        printf("The book %s by %s was not found.\n", title, author);
    }

    return temp;
};

book* add_book(book* b, char title[], char author[]){

    book* temp=malloc(sizeof(book));
    strcpy(temp->title, title);
    strcpy(temp->author, author);
    temp->available=true;
    temp->l.head=NULL;
    temp->l.tail=NULL;
    temp->next=b;
    b=temp;
    
    return b;
};

loan_history* add_history(loan_history** h, book* b, person* p){
    loan_history* temp=malloc(sizeof(loan_history));

    char date[10];

    printf("Please enter a date in the DD/MM/YYYY format: \n");
    scanf("%10s", date);

    clear_input_buffer();

    strcpy(temp->name, p->name);
    strcpy(temp->address, p->address);
    strcpy(temp->title, b->title);
    strcpy(temp->author, b->author);
    strcpy(temp->date, date);
    temp->phone_number=p->phone_number;
    temp->next=(*h);
    (*h)=temp;
};
