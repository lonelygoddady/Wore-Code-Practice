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

void ls_swap_val(node* head){
        while(head && head->next){
                int tmp = head->val;
                head->val = head->next->val;
                head->next->val = tmp;
                head = head->next->next;
        }
}

node* ls_swap_link(node* head){
        node* ret = NULL;
        if (!head && !head->next)
                return head;
        
        ret = head->next;
        while (head || head->next){

        }
}

node* swap_node(node* new_node){
        node* ret = new_node->next;
        new_node->next = ret->next;
        ret->next = new_node;
        return ret;
}

node* ls_swap_link_recur(node* head){
        if (head && head->next){
                head = swap_node(head);
                head->next->next = ls_swap_link(head->next->next);
        }
        return head;
}


int main(int argc, char **argv){
#ifdef DYNAMIC
        int node_n = 10;
        if (argc == 2){
               sscanf(argv[1], "%d", &node_n);
               printf("creatting %d nodes\n", node_n);
               if (node_n <= 0 || node_n > 100){
                       printf("too many/less nodes!\n");
                       exit(1);
                }
        }
        else if (argc > 2){
                printf("too many arguments!\n");
                exit(1);
        }

        printf("Use dynamic allocation to create linked list\n");
        node* n1 = make_ls(node_n);
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
        head = ls_swap_link_recur(head);
        ls_travel(head);

#ifdef DYNAMIC
        free_ls(head);
        head = NULL;
#endif
        return 0;
}


