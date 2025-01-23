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

// history functions

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
};//no need to use double pointers here since you are returning the pointer

// queue functions

void free_queue(queue* q){
    person* p;

    while(q->head!=NULL){
        p=q->head;

        q->head=q->head->next;

        free(p);
    }

    q->tail=NULL;
}

void display_queue(queue* q){
    person* temp=q->head;

    while(temp!=NULL){
        display_person(temp);

        temp=temp->next;
    }

    printf("\n");
};

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

book* delete_book(book* b, char title[], char author[]){
    book* temp=search_book(b, title, author);

    if(temp!=NULL){
        if(temp==b){
            b=b->next;
            free_queue(&temp->l);
            free(temp);
        }
        else{
            book* temp2=b;

            while(temp2->next!=temp){
                temp2=temp2->next;
            }

            temp2->next=temp->next;
            free_queue(&temp->l);
            free(temp);
        }
    }

    return b;
};

book* loan_book(loan_history** h, book* b, char name[], char address[], int phone, char title[], char author[]){
    book* temp=search_book(b, title, author);
    person* p=malloc(sizeof(person));

    strcpy(p->name, name);
    strcpy(p->address, address);
    p->phone_number=phone;
    p->next=NULL;

    if(temp!=NULL){
        if(temp->l.head!=NULL){
            if(compare_person(temp->l.head, p)){
                if(temp->available==true){
                    temp->available=false;
                    dequeue(temp, &(*h));
                }
                else{
                    printf("Wait for the book to be returned.\n");
                }
            }
            else{
                if(in_queue(&temp->l, p)){
                    printf("Please wait for your turn.\n");
                }
                else{
                    printf("You have been added to the queue, please wait for your turn.\n");
                    enqueue(&temp->l, name, address, phone);
                }
            }
        }
        else{
            if(temp->available==true){
                temp->available=false;
                add_history(&(*h), temp, p);
            }
            else{
                printf("The book is unavailable, please wait for your turn.\n");
                enqueue(&temp->l, name, address, phone);
                
            }
        }
    }
    else{
        printf("The book does not exist.\n");
    }

    free(p);

    return b;
};

book* return_book(book* b, char title[], char author[]){
    book* temp=search_book(b, title, author);

    if(temp!=NULL && temp->available==false){
        temp->available=true;
        printf("Thanks for returning the book.\n");
    }
    else if(temp!=NULL && temp->available==true){
        printf("The book is already in the library, are you sure you are at the right library?\n");
    }

    return b;
};

void display_book(book* b){
    printf("Book:\n-Title: %s\n-Author: %s\n-Avalable: %d\n=Queue=\n", b->title, b->author, b->available);
    display_queue(&b->l);
};// this function will be used to display the whole library
void display_history(loan_history* h){
    printf("Peron Info:\n-Name: %s\n-Address: %s\n-Phone Number: %d\nBook Info:\n-Title: %s\n-Author: %s\nLoan Date: %s\n\n", h->name, h->address, h->phone_number, h->title, h->author, h->date);
};

void search_book_in_history(loan_history* h, book* b){
    loan_history* temp=h;

    while(temp!=NULL && !(strcmp(temp->title, b->title)==0 && strcmp(temp->author, b->author)==0)){
        temp=temp->next;
    }

    if(temp==NULL){
        printf("The book was not found in the history.\n");
    }
    else{
        printf("The book was found.\n");
        display_history(temp);
    }
};

void display_all_history(loan_history* h){
    loan_history* temp=h;

    while(temp!=NULL){
        display_history(temp);

        temp=temp->next;
    }

    printf("\n");
};