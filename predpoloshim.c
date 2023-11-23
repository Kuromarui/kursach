#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define STACK_SIZE 100

typedef struct {
    double data[STACK_SIZE];
    int top;
} Stack;

// Инициализация стека
void initializeStack(Stack* stack) {
    stack->top = -1;
}

// Проверка, является ли стек пустым
bool isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

// Проверка, является ли стек полным
bool isStackFull(Stack* stack) {
    return stack->top == STACK_SIZE - 1;
}

// Добавление элемента в стек
void push(Stack* stack, double value) {
    if (isStackFull(stack)) {
        printf("Stack overflow");
        exit(EXIT_FAILURE);
    }

    stack->data[++(stack->top)] = value;
}

// Удаление элемента из стека
double pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("Stack underflow");
        exit(EXIT_FAILURE);
    }

    return stack->data[(stack->top)--];
}

// Функция, выполняющая арифметическую операцию
double performOperation(double a, double b, char operator) {
    switch (operator) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        case '^':
            return pow(a, b);
        default:
            printf("Invalid operator");
            exit(EXIT_FAILURE);
    }
}

// Проверка, является ли символ оператором
bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// Проверка, является ли символ разделителем
bool isDelimiter(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

// Получение приоритета оператора
int getOperatorPriority(char operator) {
    if (operator == '^')
        return 3;
    else if (operator == '*' || operator == '/')
        return 2;
    else if (operator == '+' || operator == '-')
        return 1;
    else
        return 0;
}

// Парсинг и вычисление математического выражения
double evaluateExpression(char* expression, double variableValue) {
    Stack operandStack;
    Stack operatorStack;

    initializeStack(&operandStack);
    initializeStack(&operatorStack);

    int i;

    for (i = 0; i < strlen(expression); i++) {
        if (expression[i] == ' ')
            continue;
        else if (expression[i] == '(') {
            push(&operatorStack, expression[i]);
        }
        else if (isdigit(expression[i]) || expression[i] == '.') {
            double operand = 0;
            int decimalCount = 0;

            while (i < strlen(expression) && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    decimalCount++;
                    if (decimalCount > 1) {
                        printf("Invalid expression");
                        exit(EXIT_FAILURE);
                    }
                }

                operand = operand * 10 + (expression[i] - '0');
                i++;
            }

            i--;

            if (decimalCount == 1)
                operand /= pow(10, decimalCount);

            push(&operandStack, operand);
        }
        else if (expression[i] == ')') {
            while (!isStackEmpty(&operatorStack) && operatorStack.data[operatorStack.top] != '(') {
                double operand2 = pop(&operandStack);
                double operand1 = pop(&operandStack);
                char opr = pop(&operatorStack);

                double result = performOperation(operand1, operand2, opr);
                push(&operandStack, result);
            }

            pop(&operatorStack); // Удаление '(' из стека
        }
        else if (isOperator(expression[i])) {
            while (!isStackEmpty(&operatorStack) && getOperatorPriority(expression[i]) <= getOperatorPriority(operatorStack.data[operatorStack.top])) {
                double operand2 = pop(&operandStack);
                double operand1 = pop(&operandStack);
                char opr = pop(&operatorStack);

                double result = performOperation(operand1, operand2, opr);
                push(&operandStack, result);
            }

            push(&operatorStack, expression[i]);
        }
        else if (isalpha(expression[i])) { // Если символ является переменной
            double operand = variableValue;
            push(&operandStack, operand);
        }
    }

    while (!isStackEmpty(&operatorStack)) {
        double operand2 = pop(&operandStack);
        double operand1 = pop(&operandStack);
        char opr = pop(&operatorStack);

        double result = performOperation(operand1, operand2, opr);
        push(&operandStack, result);
    }

    return pop(&operandStack); // Возвращаем результат вычисления выражения
}

int main() {
    char expression[100];

    printf("Enter a mathematical expression: ");
    fgets(expression, 100, stdin);
    expression[strlen(expression) - 1] = '\0'; // Удаление символа новой строки

    double variableValue;
    printf("Enter value of variable: ");
    scanf("%lf", &variableValue);

    double result = evaluateExpression(expression, variableValue);

    printf("Result: %lf", result);

    return 0;
}