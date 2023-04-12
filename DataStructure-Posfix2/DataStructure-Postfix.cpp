#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100

typedef char element;

typedef struct {
    element data[MAX_STACK_SIZE];
    int top;
} StackType;

void init_stack(StackType* s)
{
    s->top = -1;
}

int is_full(StackType* s)
{
    return (s->top == MAX_STACK_SIZE - 1);
}

int is_empty(StackType* s)
{
    return (s->top == -1);
}

void push(StackType* s, element item)
{
    if (is_full(s)) {
        fprintf(stderr, "스택 포화 에러\n");
        return;
    }
    else s->data[++(s->top)] = item;
}

element pop(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
    }
    else return s->data[(s->top)--];
}

element peek(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);
    }
    else return s->data[s->top];
}

int prec(char op)
{
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

void infix_to_postfix(char exp[], char postfix[])
{
    int i = 0, j = 0;
    char ch, top_op;
    int len = strlen(exp);
    StackType s;
    init_stack(&s);

    for (i = 0; i < len; i++) {
        ch = exp[i];
        switch (ch) {
        case '+': case '-': case '*': case '/':
            while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
                postfix[j++] = pop(&s);
            }
            push(&s, ch);
            break;
        case '(':
            push(&s, ch);
            break;
        case ')':
            top_op = pop(&s);
            while (top_op != '(') {
                postfix[j++] = top_op;
                top_op = pop(&s);
            }
            break;
        default:
            postfix[j++] = ch;
            break;
        }
    }

    while (!is_empty(&s)) {
        postfix[j++] = pop(&s);
    }
    postfix[j] = '\0';
}

double evaluate_postfix(char* exp)
{
    StackType s;
    init_stack(&s);
    int i = 0;
    char ch;

    while ((ch = exp[i++]) != '\0')
    {
        if (ch >= '0' && ch <= '9') 
            push(&s, ch - '0');
        else { 
            double op2 = pop(&s);
            double op1 = pop(&s);
            switch (ch) {
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
        }
    }
    return pop(&s); 
}
int calculate_postfix(char postfix[])
{
    int i = 0;
    char ch;
    int len = strlen(postfix);
    StackType s;
    init_stack(&s);

    for (i = 0; i < len; i++) {
        ch = postfix[i];
        if (ch >= '0' && ch <= '9') {  
            push(&s, ch - '0');
        }
        else {  
            int op1, op2;
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) {
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
        }
    }

    return pop(&s);
}

int main(void)
{
    char infix[MAX_STACK_SIZE], postfix[MAX_STACK_SIZE] = { 0, };
    int result;
    printf("Enter an infix expression : ");
    scanf_s("%s", infix, MAX_STACK_SIZE);
    infix_to_postfix(infix, postfix);
    printf("Postfix expression : %s\n", postfix);
    result = calculate_postfix(postfix);
    printf("Result : %d\n", result);
    return 0;
}