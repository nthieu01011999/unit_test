#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "message.h"
#include <string.h>

#define TASK_LIST_LEN 2

typedef struct {
    uint32_t id;
    pthread_t pthread;
    q_msg_t *mailbox;
} task_t;

task_t task_list[TASK_LIST_LEN];

void *task_function(void *arg) {
    task_t *task = (task_t *)arg;
    printf("Task %d started\n", task->id);
    while (1) {
        printf("Task %d waiting for a message\n", task->id);
        ak_msg_t *msg = q_msg_get(task->mailbox);
        if (msg) {
            printf("Task %d received message from task %d with signal %d\n", task->id, msg->header->src_task_id, msg->header->sig);
            if (msg->header->payload != NULL) {
                printf("Payload: %s\n", (char *)msg->header->payload);
            }
            q_msg_free(msg);
        }
    }
    return NULL;
}

void task_post(uint32_t task_dst_id, ak_msg_t *msg) {
    printf("Entering task_post: task_dst_id = %d\n", task_dst_id);
    
    if (task_dst_id >= TASK_LIST_LEN) {
        fprintf(stderr, "Invalid task ID: %d\n", task_dst_id);
        exit(1);
    }

    if (msg != NULL) {
        printf("Message is not NULL\n");

        pthread_mutex_lock(&(task_list[task_dst_id].mailbox->mt));
        printf("Locked the mailbox mutex for task %d\n", task_dst_id);

        msg->header->des_task_id = task_dst_id;
        printf("Set destination task ID in message header to %d\n", task_dst_id);

        q_msg_t *q_msg = task_list[task_dst_id].mailbox;
        printf("Retrieved the mailbox for task %d\n", task_dst_id);

        q_msg_put(q_msg, msg);
        printf("Put the message into the mailbox for task %d\n", task_dst_id);

        pthread_cond_signal(&(task_list[task_dst_id].mailbox->mailbox_cond));
        printf("Signaled the mailbox condition variable for task %d\n", task_dst_id);

        pthread_mutex_unlock(&(task_list[task_dst_id].mailbox->mt));
        printf("Unlocked the mailbox mutex for task %d\n", task_dst_id);
    } else {
        fprintf(stderr, "Message is NULL\n");
        exit(1);
    }

    printf("Exiting task_post\n");
}

ak_msg_t *create_message(uint32_t src_task_id, uint32_t sig, const char *payload) {
    ak_msg_t *msg = (ak_msg_t *)malloc(sizeof(ak_msg_t));
    msg->header = (header_t *)malloc(sizeof(header_t));
    msg->header->src_task_id = src_task_id;
    msg->header->sig = sig;
    msg->header->payload = malloc(strlen(payload) + 1);
    strcpy((char *)msg->header->payload, payload);
    msg->header->len = strlen(payload) + 1;
    printf("Created a message from task %d with signal %d and payload '%s'\n", src_task_id, sig, payload);
    return msg;
}

int main() {
    for (uint32_t i = 0; i < TASK_LIST_LEN; i++) {
        task_list[i].id = i;
        task_list[i].mailbox = (q_msg_t *)malloc(sizeof(q_msg_t));
        q_msg_init(task_list[i].mailbox);

        pthread_create(&task_list[i].pthread, NULL, task_function, &task_list[i]);
        printf("Created task %d\n", i);
    }

    sleep(1);  
    ak_msg_t *msg = create_message(0, 100, "hello world");
    printf("Created a message from task 0 with signal 100\n");
    task_post(1, msg);
    printf("Posted message to task 1\n");

    for (uint32_t i = 0; i < TASK_LIST_LEN; i++) {
        pthread_join(task_list[i].pthread, NULL);
        free(task_list[i].mailbox);
        printf("Joined task %d\n", i);
    }

    return 0;
}
