#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"


struct node *head = NULL;

Task *nextTask();

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
    Task *current;

    while(head != NULL){
        current = nextTask();

        run(current, current -> burst);
        delete(&head, current);

        time = time + current -> burst;
        f_time = f_time + time;
        number++;
    }

    printf("\n");
    printf("Average wait time = %.2f\n", (f_time - time)/number);
    printf("Average turnaroundtime = %.2f\n", (f_time/number));
    printf("Average response time = %.2f\n", (f_time - time)/number);
}

Task *nextTask(){
    struct node *tmp;
    tmp = head -> next;
    Task *pos = head -> task;
    
    while (tmp != NULL){
        pos = tmp -> task;
        tmp = tmp -> next;
    }
    return pos;
}
