#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"

#define Quantum 10

struct node *head = NULL;

Task *nextTask();
// void tail(struct node **head, Task *task);

void add(char *name, int priority, int burst){
    Task *task_new = (Task *)malloc(sizeof(Task));
    
    task_new -> name = name;
    task_new -> priority = priority;
    task_new -> burst = burst;

    insert(&head, task_new);
}

void schedule(){
    float number = 0;
    float time = 0;
    int f_time = 0;
    struct node *tmp;
    tmp = head;
    

    while (head != NULL){

        // while (tmp -> next != NULL){
        //     tmp = tmp -> next;
        // }

        Task *tmp2 = tmp -> task;

        
        if (tmp2 -> burst > Quantum){
            run (tmp2, Quantum);
            // printf("xxxxx");
            tmp2 -> burst = tmp2 -> burst - Quantum;
            tmp = tmp -> next;
           
            // delete(&head, tmp2);
            // tail(&head, tmp2);
            
            if (tmp != NULL){
                tmp2 = tmp -> task;
            }
            else{
                tmp = head;
                if (tmp != NULL){
                    tmp2 = tmp -> task;
                }
                // else{
                //     return;
                // }
            }
        }
        else{
            int remaining_time = tmp2 -> burst;
            run (tmp2, remaining_time);
            tmp2 -> burst = tmp2 -> burst - remaining_time;
            tmp = tmp -> next;
            delete(&head, tmp2);

            if (tmp != NULL){
                tmp2 = tmp -> task;
            }
            else{
                tmp = head;
                if (tmp != NULL){
                    tmp2 = tmp -> task;
                }
                // else{
                //     return;
                // }
            }
        }

        time = time + tmp2 -> burst;
        f_time = f_time + time;
        number++;
    }

    printf("\n");
    printf("Average wait time = %.2f\n", (f_time - time)/number);
    printf("Average turnaroundtime = %.2f\n", (f_time/number));
    printf("Average response time = %.2f\n", (f_time - time)/number);
}


