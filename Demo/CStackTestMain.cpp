//
// Created by wjc on 2023/3/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Link
{
    int data;
    struct Link* next;
};

struct CStack
{
    struct Link* head;
    int size;
};

void StackInit(struct CStack* stack)
{
    stack->head = NULL;
    stack->size = 0;
}

void StackPush(struct CStack* stack, const int data)
{
    struct Link* node;
    node = (struct Link*)malloc(sizeof(struct Link));
    assert(node != NULL);
    node->data = data;
    node->next = stack->head;
    stack->head = node;
    ++stack->size;
}

int StackEmpty(struct CStack* stack)
{
    return (stack->size == 0);
}

int StackPop(struct CStack* stack, int* data)
{
    if (StackEmpty(stack))
    {
        return 0;
    }

    struct Link* tmp = stack->head;
    *data = stack->head->data;
    stack->head = stack->head->next;
    free(tmp);
    --stack->size;

    return 1;
}

void StackCleanup(struct CStack* stack)
{
    struct Link* tmp;
    while (stack->head)
    {
        tmp = stack->head;
        stack->head = stack->head->next;
        free(tmp);
    }

    stack->size = 0;
}

int mainc100(void)
{
    struct CStack stack;
    StackInit(&stack);
    int i;
    for (i = 0; i < 5; i++)
    {
        StackPush(&stack, i);
    }

    while (!StackEmpty(&stack))
    {
        StackPop(&stack, &i);
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}