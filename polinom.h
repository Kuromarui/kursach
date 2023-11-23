#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define STACK_SIZE 100
///////////ИНИЦИАЛИЗАЦИЯ СПИСКА
typedef struct{
    float coef;
    int power; 
    char oper; 
    struct Monom* next ;
} Monom;

void appendMonom(Monom** head, float coef, int power, char oper){
    Monom* newMonom = (Monom*)malloc(sizeof(Monom)); 
    newMonom -> coef = coef;
    newMonom -> power = power;
    newMonom -> oper = oper ;
    newMonom -> next = (*head);
   (*head) = newMonom;
}

float popMonomCoef(Monom** head){
    Monom* prev = NULL;
    float coef;
    int power; 
    char oper ;
    if (head == NULL){
        return -1 ;
    }
    prev = (*head);
    (*head) = (*head) -> next; 
    free(prev); 
    return coef; 
}

int popMonomPower(Monom** head){
    Monom* prev = NULL;
    float coef;
    int power; 
    char oper ;
    if (head == NULL){
        return -1 ;
    }
    prev = (*head);
    (*head) = (*head) -> next; 
    free(prev); 
    return power; 
}

char popMonomOper(Monom** head){
    Monom* prev = NULL;
    float coef;
    int power; 
    char oper ;
    if (head == NULL){
        return -1 ;
    }
    prev = (*head);
    (*head) = (*head) -> next; 
    free(prev); 
    return oper; 
}

/////////////////////////////////
///////////////////////////////

// Иницализация стека ///////////////////
////////////////////////////////////////
typedef struct {
    double data[STACK_SIZE]; //основная структура
    int top;
} Stack;

void initializeStack(Stack* stack) { //Создание стека
    stack->top = -1;
}

bool isStackEmpty(Stack* stack) { //Проверка на пустоту 
    return stack->top == -1;
}

bool isStackFull(Stack* stack) { // проверка на полноту
    return stack->top == STACK_SIZE - 1; 
}

void push(Stack* stack, double value) { //добавление в стек
    if (isStackFull(stack)) {
        printf("Stack overflow");
        exit(EXIT_FAILURE);
    }

    stack->data[++(stack->top)] = value;
}

double pop(Stack* stack) { // Удаление из стека
    if (isStackEmpty(stack)) {
        printf("Stack underflow");
        exit(EXIT_FAILURE);
    }

    return stack->data[(stack->top)--];
}
////////////////////////////////////////////////////
///////////////////////////////////////////////////

///Работа с числами//////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

double performOperation(double a, double b, char operator) { //базовые мат операции 
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
            printf("неправильное действие");
            exit(EXIT_FAILURE);
    }
}

bool isOperator(char ch) { //Проверка на оператор 
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

bool isRaz(char ch) { //проверка на раздел 
    return ch == ' ' || ch == '\t' || ch == '\n';
}

int getPriority(char operator) { //Получение приоритетов
    if (operator == '^')
        return 3;
    else if (operator == '*' || operator == '/')
        return 2;
    else if (operator == '+' || operator == '-')
        return 1;
    else
        return 0;
}

double parsExpression(char* line, double variableValue) { //Парсинг выражения
    Stack operandStack; 
    Stack operatorStack;

    initializeStack(&operandStack);
    initializeStack(&operatorStack);
    // реализация двух стеков
    int i;

    for (i = 0; i < strlen(line); i++) { //Основной цикл 
        if (line[i] == ' ') // при пробеле парсим дальше
            continue;
        else if (line[i] == '(') { //при скобке добавляем её в стек операторов
            push(&operatorStack, line[i]);
        }
        else if (isdigit(line[i]) || line[i] == '.') { //проверка на число int/float
            double operand = 0; //число
            int decimalCount = 0;//счётчик точки 

            while ((i < strlen(line)) && (isdigit(line[i]) || line[i] == '.')) { //цикл сборки
                if (line[i] == '.') { // проверка на многоточность 
                    decimalCount++;
                    if (decimalCount > 1) {
                        printf("Invalid expression");
                        exit(EXIT_FAILURE);
                    }
                }

                operand = operand * 10 + (line[i] - '0'); // сборка числа
                i++;
            }

            i--; // шаг назад 

            if (decimalCount == 1) // сборка при float 
                operand /= pow(10, decimalCount);

            push(&operandStack, operand); //добавление в стек чисел
        }
        else if (line[i] == ')') { //если закрывается скобка
            while (!isStackEmpty(&operatorStack) && operatorStack.data[operatorStack.top] != '(') {
                double operand2 = pop(&operandStack); //достаём a
                double operand1 = pop(&operandStack); // b
                char opr = pop(&operatorStack); // оператор

                double result = performOperation(operand1, operand2, opr); //производим простое вычисление
                push(&operandStack, result);//записываем результат вычислений
            }

            pop(&operatorStack); // Удаление '(' из стека
        }
        else if (isOperator(line[i])) {//при нахождении оператора
            while (!isStackEmpty(&operatorStack) && getPriority(line[i]) <= getPriority(operatorStack.data[operatorStack.top])) { //если приоритет меньше верхушки стека
                double operand2 = pop(&operandStack);
                double operand1 = pop(&operandStack);
                char opr = pop(&operatorStack);

                double result = performOperation(operand1, operand2, opr);
                push(&operandStack, result);
            }

            push(&operatorStack, line[i]);
        }
        else if (isalpha(line[i])) { // Если символ является переменной
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
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////