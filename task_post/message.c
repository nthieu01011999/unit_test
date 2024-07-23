#include "message.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void q_msg_init(q_msg_t *q_msg) {
    pthread_mutex_init(&q_msg->mt, NULL);
    pthread_cond_init(&q_msg->mailbox_cond, NULL);
    q_msg->head = NULL;
    q_msg->tail = NULL;
    q_msg->len = 0;
    printf("Initialized queue: mutex and condition variable\n");
}

void q_msg_put(q_msg_t *q_msg, ak_msg_t *msg) {
    printf("Entering q_msg_put\n");
    pthread_mutex_lock(&(q_msg->mt));
    printf("Locked the queue mutex\n");

    if (q_msg->tail != NULL) {
        msg->prev = NULL;
        msg->next = q_msg->tail;
        q_msg->tail->prev = msg;
        q_msg->tail = msg;
        printf("Inserted message at the end of the queue\n");
    } else {
        msg->next = NULL;
        msg->prev = NULL;
        q_msg->tail = q_msg->head = msg;
        printf("Inserted message into an empty queue\n");
    }

    q_msg->len++;
    printf("Queue length is now %d\n", q_msg->len);

    pthread_cond_signal(&(q_msg->mailbox_cond));
    printf("Signaled the condition variable\n");

    pthread_mutex_unlock(&(q_msg->mt));
    printf("Unlocked the queue mutex\n");
    printf("Exiting q_msg_put\n");
}

ak_msg_t *q_msg_get(q_msg_t *q_msg) {
    printf("Entering q_msg_get\n");
    ak_msg_t *temp_head;
    ak_msg_t *msg = NULL;

    pthread_mutex_lock(&(q_msg->mt));
    printf("Locked the queue mutex\n");

    while (q_msg->head == NULL) {
        printf("Queue is empty, waiting for a message\n");
        pthread_cond_wait(&(q_msg->mailbox_cond), &(q_msg->mt));
        printf("Woke up from waiting for a message\n");
    }

    if (q_msg->head != NULL) {
        msg = q_msg->head;
        if (q_msg->head == q_msg->tail) {
            q_msg->head = q_msg->tail = NULL;
            printf("Queue is now empty after getting the message\n");
        } else {
            temp_head = q_msg->head->prev;
            q_msg->head->prev->next = NULL;
            q_msg->head->prev = NULL;
            q_msg->head = temp_head;
            printf("Got the message from the front of the queue\n");
        }
        q_msg->len--;
        printf("Queue length is now %d\n", q_msg->len);
    }

    pthread_mutex_unlock(&(q_msg->mt));
    printf("Unlocked the queue mutex\n");
    printf("Exiting q_msg_get\n");
    return msg;
}

void q_msg_free(ak_msg_t *msg) {
    if (msg != NULL) {
        if (msg->header != NULL) {
            if (msg->header->payload != NULL) {
                free(msg->header->payload);
            }
            free(msg->header);
        }
        free(msg);
        printf("Freed the message\n");
    } else {
        printf("Attempted to free a NULL message\n");
    }
}

bool q_msg_available(q_msg_t *q_msg) {
    bool ret;
    pthread_mutex_lock(&(q_msg->mt));
    ret = (q_msg->head != NULL && q_msg->tail != NULL);
    pthread_mutex_unlock(&(q_msg->mt));
    return ret;
}
