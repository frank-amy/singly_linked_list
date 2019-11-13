
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define TASK_NUM                20

typedef struct list {
    uint32_t tid;
    struct list* next;
} task_list_t;

static task_list_t* mTask;/* recording all tasks */

task_list_t* task_list_init(void)
{
    task_list_t* pTask = (task_list_t*)malloc(sizeof(task_list_t));

    memset(pTask, 0, sizeof(task_list_t));

    printf("task init head %p, tid %d\n", (void*)pTask, pTask->tid);

    return pTask;
}

void task_list_insert_tail(task_list_t** pTask, uint32_t tid)
{
    task_list_t* head = *pTask;

    task_list_t* next = (task_list_t*) malloc(sizeof(task_list_t));

    if (head) {
        head->next = next;
        head->next->tid = tid;
    }
}

void task_list_insert_by_tid(task_list_t** pTask, uint32_t tid)
{
    task_list_t* head = *pTask;
    task_list_t* prev = *pTask;

    task_list_t* new = (task_list_t*) malloc(sizeof(task_list_t));

    new->tid = tid;

    while (head) {
        if (head->tid > tid) {
            if (head == prev) {
                new->next = head;
                *pTask = new;
            } else {
                prev->next = new;
                new->next = head;
            }
            break;
        }
        prev = head;
        head = head->next;
    }

    if (!head)
        printf("inserting is failed, tid %d\n", tid);
}

void task_list_free(task_list_t* pTask)
{
    printf("task list free %p, tid %d\n", pTask, pTask->tid);
    free(pTask);
}

/* input the head of task list */
void ascending_by_tid(task_list_t* pTask)
{
    uint32_t tid;
    task_list_t* head = pTask;

    while (head && head->next) {
        if (head->tid > head->next->tid) {
            tid = head->tid;
            head->tid = head->next->tid;
            head->next->tid = tid;
            head = pTask;/* rollback to head */
        } else {
            head = head->next;
        }
    }
}

uint32_t task_list_length(task_list_t* head)
{
    uint32_t length = 0;

    while (head) {
        length++;
        head = head->next;
    }

    return length;
}

void descending_by_tid(task_list_t* pTask)
{
    uint32_t tid;
    task_list_t* head = pTask;

    while (head && head->next) {
        if (head->tid < head->next->tid) {
            tid = head->tid;
            head->tid = head->next->tid;
            head->next->tid = tid;
            head = pTask;
        } else
            head = head->next;
    }
}

void fetching_from_head(task_list_t* pTask)
{
    task_list_t* head = pTask;

    while (head) {
        printf("fetching @%p tid %d\n", head, head->tid);
        head = head->next;
    }
}

void task_list_remove_by_tid(task_list_t** pTask, uint32_t tid)
{
    task_list_t* head = *pTask;
    task_list_t* prev = *pTask;

    while (head) {
        if (head->tid == tid) {
            if (head == prev)/* remove from head */
                *pTask = head->next;
            else/* of remove from body */
                prev->next = head->next;
            task_list_free(head);
            break;
        }
        prev = head;
        head = head->next;
    }
}

void task_list_remove_by_task(task_list_t** pTask, task_list_t* target)
{
    task_list_t* head = *pTask;
    task_list_t* prev = *pTask;

    while (head) {
        if (head == target) {
            if (head == prev) {/* remove from head */
                *pTask = head->next;
            } else {/* remove from body */
                prev->next = head->next;
            }
            task_list_free(head);
            break;
        }
        prev = head;
        head = head->next;
    }
}

int main()
{
    printf("task list - single list.\n");

    mTask = task_list_init();

    task_list_t* head = mTask;

    printf("--------inserting by tid\n");
    for (uint32_t i = 1; i < TASK_NUM; i++) {
        task_list_insert_tail(&head, i);
        printf("task next %p, tid is %d\n", head->next, head->next->tid);
        head = head->next;
    }

    head = mTask;
    printf("---------fetching from head\n");
    fetching_from_head(head);

    printf("---------descending by tid\n");
    descending_by_tid(head);

    head = mTask;
    printf("---------fetching from head %d tasks\n", task_list_length(head));
    fetching_from_head(head);

    printf("---------inserting by tid 0\n");
    task_list_insert_by_tid(&mTask, 0);
    printf("---------inserting by tid 8\n");
    task_list_insert_by_tid(&mTask, 8);
    printf("---------inserting by tid 19\n");
    task_list_insert_by_tid(&mTask, 19);

    head = mTask;
    printf("---------fetching from head %d tasks\n", task_list_length(head));
    fetching_from_head(head);

    printf("---------ascending by tid\n");
    ascending_by_tid(head);

    printf("---------fetching from head\n");
    fetching_from_head(head);

    printf("---------removing by tid 10\n");
    task_list_remove_by_tid(&mTask, 10);

    printf("---------removing by tid 0\n");
    task_list_remove_by_tid(&mTask, 0);

    printf("---------removing by tid 19\n");
    task_list_remove_by_tid(&mTask, 19);

    printf("---------removing by task\n");
    task_list_remove_by_task(&mTask, mTask->next->next);

    head = mTask;
    printf("---------fetching from head\n");
    fetching_from_head(head);

    while (mTask) {
        task_list_free(mTask);
        mTask = mTask->next;
    }

    head = mTask;
    printf("---------fetching from head is starting.\n");
    fetching_from_head(head);
    printf("---------fetching from head is over.\n");

    return 0;
}