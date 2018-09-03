#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct node {
        struct node* next;
        int val;
}node;

node* make_ls(int n){
        node* head = NULL;
        
        while(n > 0){
                node* new_node = (node*) malloc(sizeof(node));
                new_node -> val = n--;
                new_node -> next = head;
                head = new_node;
        }

        return head;
}

void free_ls(node* head){
        node* tmp;

        printf("Free the linked list. \n");
        while(head){
                tmp = head;
                head = head->next;
                free(tmp);
        }
}

void ls_travel(node* head){
        int count = 0;

        while (head){
                count ++;
                printf("node %d val: %d\n", count, head->val);
                head = head->next;
        }
        printf("There are %d nodes in the list\n", count);
}

node* reverse_ls(node* head){
        node* nxt;
        node* cur;

        if (!head || !head->next)
                return head;

        nxt = head -> next;
        head -> next = NULL;
        while(nxt){
                cur = nxt;
                nxt = nxt->next;
                cur->next = head;
                head = cur; 
        }
        return cur; 
}

node* reverse_ls_pp(node* head){
        node *tmp  = NULL;
        while(head){
                node* nxt = head->next;
                head->next = tmp;
                tmp = head;
                head = nxt;
        }
        return tmp;
}

int main(int argc, char **argv){
#ifdef DYNAMIC
        printf("Use dynamic allocation to create linked list\n");
        node* n1 = make_ls(10);
        node* head = n1;
#else 
        node n10 = {NULL, 10};
        node n9 = {&n10, 9};
        node n8 = {&n9, 8};
        node n7 = {&n8, 7};
        node n6 = {&n7, 6};
        node n5 = {&n6, 5};
        node n4 = {&n5, 4};
        node n3 = {&n4, 3};
        node n2 = {&n3, 2};
        node n1 = {&n2, 1};
        node *head = &n1;
#endif 

        ls_travel(head);
        //head = reverse_ls(head);
        head = reverse_ls_pp(head);
        ls_travel(head);

#ifdef DYNAMIC
        free_ls(head);
        head = NULL;
#endif
        return 0;
}


