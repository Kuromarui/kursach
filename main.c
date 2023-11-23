#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define STACK_SIZE 100
///////////ИНИЦИАЛИЗАЦИЯ СТЕКА МОНОМОВ 
typedef struct{
    float coef;
    int power; 
} Monom;

typedef struct {
    Monom monoms[STACK_SIZE]; //основная структура
    int top;
} monomialStack;

void initializeMonomStack(monomialStack* stack) { //Создание стека
    stack->top = -1;
}

bool isMonomStackEmpty(monomialStack* stack) { //Проверка на пустоту 
    return stack->top == -1;
}

bool isMonomStackFull(monomialStack* stack) { // проверка на полноту
    return stack->top == STACK_SIZE - 1; 
}

void pushMonom(monomialStack* stack, Monom monom) { //добавление в стек
    if (isStackFull(stack)) {
        printf("Stack overflow");
        exit(EXIT_FAILURE);
    }

    stack -> monoms[++stack->top] = monom;
}

Monom MonomPop(monomialStack* stack) { // Удаление из стека
    if (isStackEmpty(stack)) {
        printf("Stack underflow");
        exit(EXIT_FAILURE);
    }

    return stack->monoms[(stack->top)--];
}

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





///ПОЛИНОМИАЛЬНАЯ ХУЙНЯ/////////////////////////////////////
////////////////////////////////////////////////////////////
void createMonom(float coef, char* line ){ 
    Monom monom; 
    monom.coef = coef; 
    for 
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


///Работа с числами//////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

Monom performOperation(Monom term1, Monom term2, char operator, monomialStack* stack) { //базовые мат операции 
    switch (operator) {
        case '+':
            if (term1.coef != term2.coef) {
                printf("Ошибка: невозможно сложить полиномы с разными степенями\n");
                pushMonom(stack, term2);
                pushMonom(stack, term1);
                return;
            }

            Monom result;
            result.coef = term1.coef + term2.coef;
            result.power = term1.power;

            return result;

        case '-':
            if (term1.coef != term2.coef) {
                pushMonom(stack, term2);
                pushMonom(stack, term1);
                return;
            }

            Monom result;
            result.coef = term1.coef - term2.coef;
            result.power = term1.power;
            return result; 
        case '*':
            Monom term1 = MonomPop(stack);
            Monom term2 = MonomPop(stack);

            Monom result;
            result.coef = term1.coef * term2.coef;
            result.power = term1.power + term2.power;
            return result;
        
        case '/':
            Monom term1 = MonomPop(stack);
            Monom term2 = MonomPop(stack);

            if (term2.coef == 0) {
                printf("Ошибка: деление на ноль\n");
                pushMonom(stack, term2);
                pushMonom(stack, term1);
                return;
            }
               Monom result;
               result.coef = term1.coef / term2.coef;
               result.power = term1.power - term2.power;
               return result;
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
    monomialStack monomsStack; 
    Stack operatorStack;

    initializeStack(&monomsStack);
    initializeStack(&operatorStack);
    // реализация двух стеков
    int i;
    Monom monom;
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
            if (line[i + 1] = 'x'){
                monom.coef = operand;
                monom.power = line[i+3];
                push(&monomsStack, monom); //добавление в стек чисел
            }
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
int main() {
    char line[100];

    printf("Enter a mathematical expression: ");
    fgets(line, 100, stdin);
    line[strlen(line) - 1] = '\0';

    double result = parsExpression(line);

    printf("Result: %lf", result);

    return 0;
}   