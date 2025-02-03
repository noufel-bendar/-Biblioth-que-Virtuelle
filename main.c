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

// person functions

bool compare_person(person* p, person* t){
    if(strcmp(p->name, t->name)==0 && strcmp(p->address, t->address)==0 && p->phone_number==t->phone_number){
        return true;
    }

    return false;
};

void display_person(person* p){
    printf("Person:\n-Name: %s\n-Adress: %s\n-Phone Number: %d\n\n", p->name, p->address, p->phone_number);
};

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

// queue functions

bool in_queue(queue* q, person* p){
    person* temp=q->head->next;

    while(temp!=NULL && !compare_person(temp, p)){
        temp=temp->next;
    }

    if(temp==NULL){
        return false;
    }

    return true;
};

void enqueue(queue* q, char name[], char address[], int phone){
    person* p=malloc(sizeof(person));

    strcpy(p->name, name);
    strcpy(p->address, address);
    p->phone_number=phone;
    p->next=NULL;

    if(q->tail==NULL){
        q->tail=p;
        q->head=p;
    }
    else{
        q->tail->next=p;
        q->tail=q->tail->next;
    }
};

book* dequeue(book* b, loan_history** h){
    if(b->l.head==NULL){
        printf("The queue is already empty.\n");
        return b;
    }

    add_history(&(*h), b, b->l.head);

    person* temp=b->l.head;

    if(b->l.head==b->l.tail){
        b->l.head=NULL;
        b->l.tail=NULL;
    }
    else{
        b->l.head=b->l.head->next;
    }

    free(temp);

    return b;
};

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

void display_library(book* b){
    book* temp=b;

    while(temp!=NULL){
        display_book(temp);

        temp=temp->next;
    }

    printf("\n");
};// need the rest to be done so i can make the main function

book* start(book* b, loan_history** h){
    char titles[10][30]={"Jojo's Bizarre Adventures", "One Piece", "Sweet Home", "Dog Nigga", "One Punch Man", "Oshi No Ko", "Lookism", "Kaguya-Sama Love Is War", "Wind Breaker", "Wind Breaker"};
    char authors[10][30]={"Araki", "Oda", "Kim Carnby", "DH Animations", "One", "Aka", "PTJ", "Aka", "Jo Yongseuk", "Nii Satoru"};

    char names[10][30]={"Alice Smith", "Bob Johnson", "Charlie Brown", "Daisy White", "Eve Black", "Frank Green", "Grace Blue", "Henry Red", "Ivy Yellow", "Jack Orange"};
    char addresses[10][30]={"123 Main St", "456 Elm St", "789 Oak St", "101 Pine St", "202 Maple St", "303 Cedar St", "404 Walnut St", "505 Birch St", "606 Spruce St", "707 Ash St"};
    int phone_numbers[10]={55512347, 98765432, 12347890, 87632109, 23456901, 89234567, 47890123, 76543098, 21076543, 67890145};

    book* temp;

    for(int i=0; i<10; i++){
        b=add_book(b, titles[i], authors[i]);
    }

    temp=search_book(b, "Dog Nigga", "DH Animations");
    temp->available=false;
    temp=search_book(b, "Jojo's Bizarre Adventures", "Araki");
    temp->available=false;

    for(int i=0; i<5; i++){
        b=loan_book(&(*h), b, names[i], addresses[i], phone_numbers[i], "Dog Nigga", "DH Animations");
        b=loan_book(&(*h), b, names[i], addresses[5+i], phone_numbers[5+i], "Jojo's Bizarre Adventures", "Araki");
    }

    return b;
};

int main(){
    book* b=NULL;
    book* temp_book=NULL;
    loan_history* h=NULL;

    b=start(b, &h);

    int key=-1;

    char temp1[30], temp2[30], temp3[30], temp4[30];
    int x;

    while(key!=0){

        printf("Commands: \n-0: close.\n-1: add a book.\n-2: remove a book.\n-3: loan a book.\n-4: return a book.\n-5: search for a book.\n-6: last borrower of a book.\n-7: list of all books with their queues.\n-8: display history.\n");
        scanf("%d", &key);

        switch (key)
        {

        case 0:

            break;

        case 1:

            printf("Please enter title and author of the book you want to add.\n");
            printf("title: ");
            scanf(" %30[^\n]", temp1);
            clear_input_buffer();
            printf("author: ");
            scanf(" %30[^\n]", temp2);
            clear_input_buffer();

            b=add_book(b, temp1, temp2);
            
            break;

        case 2:

            printf("Please enter title and author of the book you want to remove.\n");
            printf("title: ");
            scanf(" %30[^\n]", temp1);
            clear_input_buffer();
            printf("author: ");
            scanf(" %30[^\n]", temp2);
            clear_input_buffer();

            b=delete_book(b, temp1, temp2);
            
            break;

        case 3:

            printf("Please enter title and author of the book you want to borrow.\n");
            printf("title: ");
            scanf(" %30[^\n]", temp1);
            clear_input_buffer();
            printf("author: ");
            scanf(" %30[^\n]", temp2);
            clear_input_buffer();

            printf("Please enter the name, address and the phone number of the person who wants to borrw the book.\n");
            printf("name: ");
            scanf(" %30[^\n]", temp3);
            clear_input_buffer();
            printf("address: ");
            scanf(" %30[^\n]", temp4);
            clear_input_buffer();
            printf("phone number: ");
            scanf("%d", &x);

            b=loan_book(&h, b, temp3, temp4, x, temp1, temp2);
            
            break;

        case 4:

            printf("Please enter title and author of the book you want to return.\n");
            printf("title: ");
            scanf(" %30[^\n]", temp1);
            clear_input_buffer();
            printf("author: ");
            scanf(" %30[^\n]", temp2);
            clear_input_buffer();

            b=return_book(b, temp1, temp2);
            
            break;

        case 5:

            printf("Please enter title and author of the book you want to search for.\n");
            printf("title: ");
            scanf(" %30[^\n]", temp1);
            clear_input_buffer();
            printf("author: ");
            scanf(" %30[^\n]", temp2);
            clear_input_buffer();

            temp_book=search_book(b, temp1, temp2);

            if(temp_book!=NULL){
                display_book(temp_book);
            }
            
            break;

        case 6:

            printf("Please enter title and author of the book you want to check history of.\n");
            printf("title: ");
            scanf(" %30[^\n]", temp1);
            clear_input_buffer();
            printf("author: ");
            scanf(" %30[^\n]", temp2);
            clear_input_buffer();

            temp_book=search_book(b, temp1, temp2);

            if(temp_book!=NULL){
                search_book_in_history(h, temp_book);
            }
            
            break;

        case 7:

            display_library(b);
            
            break;

        case 8:

            display_all_history(h);
            
            break;
        
        default:
            printf("Invalid command.\n");
            break;
        }
    }

    return 0;
}