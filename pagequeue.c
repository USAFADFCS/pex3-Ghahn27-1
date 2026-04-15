/** pagequeue.c
 * ===========================================================
 * Name: _______________________, __ ___ 2026
 * Section: CS483 / ____
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {

    PageQueue* pq = (PageQueue*) malloc(sizeof(PageQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    pq->maxSize = maxSize;
    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {

    int depth = 0;
    PqNode* current = pq->tail;
    while(current != NULL) { //Break loop when end aka head
        if(current->pageNum == pageNum) { //Conditon met
            pqPop(pq, current);
            return depth;
        } else {
            depth++;
            current = current->prev;
        }
    }
    //Outside of while loop means miss
    PqNode* new = nodeInit(pageNum);
    if(pq->tail == NULL) {
        pq->head = new;
        pq->tail = new;
    } else {
        pq->tail->next = new;
        new->prev = pq->tail;
        pq->tail = new;
    }
    pq->size++;
    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {

    PqNode* current = pq->head;
    while(current != NULL) {
        PqNode* temp = current->next;
        free(current);
        current = temp;
    }
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
}

void pqInsertTail(PageQueue *pq, PqNode* node) {
    if(pq == NULL) {
        return;
    }
    if(pq->tail == NULL) {
        pq->head = node;
        pq->tail = node;
        node->prev = NULL;
        node->next = NULL;
        pq->size++;
        return;
    }
    node->prev = pq->tail;
    pq->tail->next = node;
    pq->tail = node;
    node->next = NULL;
    pq->size++;

    //Check for exceeding size
    if(pq->size > pq->maxSize) {
        PqNode* temp = pq->head;
        pq->head = temp->next;
        pq->head->prev = NULL;
        free(temp);
        pq->size--;
    }
}

PqNode* pqPop(PageQueue* pq, PqNode* current) {

    if (current == pq->tail) {
        return current; //Alr at end of queue
    } 
    if(current == pq->head) {
        pq->head = current->next;
        pq->head->prev = NULL;
    } else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    current->prev = NULL;
    current->next = NULL;
    pq->size--;
    pqInsertTail(pq,current);

    return current;

}

PqNode* nodeInit(unsigned long pageNum) {
    PqNode* node = (PqNode*) malloc (sizeof(PqNode));
    node->pageNum = pageNum;
    node->next = NULL;
    node->prev = NULL;
    return node;
}
