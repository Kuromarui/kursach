#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define MAXDEGREE 128

typedef struct monomialStack {
    float polinom[MAXDEGREE];  
    struct monomialStack* next;
} monomialStack;

void appendPolinom(monomialStack** head, float polinom[MAXDEGREE]) {
    monomialStack* newMonom = malloc(sizeof(monomialStack));
    if (newMonom == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAXDEGREE; i++) {
        newMonom->polinom[i] = polinom[i];
    }
    newMonom->next = *head;
    *head = newMonom;
}


float* popMonom(monomialStack** head) {
    monomialStack* temp = NULL;
    float* poli = NULL;

    if (*head == NULL) {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE); 
    }

    temp = *head;
    poli = malloc(MAXDEGREE * sizeof(float));
    if (poli == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the array elements instead of assigning
    for (int i = 0; i < MAXDEGREE; ++i) {
        poli[i] = temp->polinom[i];
    }
    *head = temp->next; 
    free(temp); 

    return poli; 
}

void printList(float polinom[MAXDEGREE]) {
    for (int i = 0 ; i < MAXDEGREE; i++ ){
        printf('/f ', polinom[i])
    }
}

typedef struct{ 
    char operator; 
    struct operandStack* next;
} operandStack;

void appendOperator(operandStack** head, char operator){
    operandStack* stack = malloc(sizeof(operandStack));
    stack -> operator = operator;
    stack -> next = *head;
    *head = stack;
}

char popOperator(operandStack** head){ 
    operandStack *temp = NULL; 
    char operator; 
    if (head == NULL){ 
        exit(-1); 
    }
    temp = *head; 
    operator = temp->operator; 
    *head = temp->next;
    free(temp); 
    return operator; 
}

bool isMonomialStackEmpty(monomialStack* head) {
    return head == NULL;
}

bool isOperandStackEmpty(operandStack* head) {
    return head == NULL;
}
///////////////////////////////////////////////////
//////////////////////////////////////////////////
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

void performOperation(float term1[MAXDEGREE], float term2[MAXDEGREE], float result[MAXDEGREE], char operator) { //базовые мат операции 
    switch (operator) {
        case '+':
            for (int i = 0 ; i < MAXDEGREE; i++){
                if (term1[i] != 0 || term2[i] != 0){
                    result[i] = term1[i] = term2[i];
                }
            }

        case '-':
            for (int i = 0 ; i < MAXDEGREE; i++){
                if (term1[i] != 0 || term2[i] != 0){
                    result[i] = term1[i] = term2[i];
                }
            }

        case '*':
            for (int i = 0; i < MAXDEGREE; i++){
                if(term1[i] != 0){
                    continue;
                }
                for (int j = 0; i < MAXDEGREE; i++){
                    if (term2[j] != 0){
                        continue;
                    }
                    else{
                        result[i+j] = term1[i] * term2[j];
                    }
                }
            }
        
        case '/':
            for (int i = 0; i < MAXDEGREE; i++){
                if(term1[i] != 0){
                    continue;
                }
                for (int j = 0; i < MAXDEGREE; i++){
                    if (term2[j] != 0){
                        continue;
                    }
                    else{
                        result[i-j] = term1[i] / term2[j];
                    }
                }
            }
        default:
            printf("неправильное действие");
            exit(EXIT_FAILURE);
    }
}

void createMonom(float result[MAXDEGREE], float coef , int power){
    result[power] = coef;
}


float* parsExpression(char* line) { //Парсинг выражения
    monomialStack*  monomsStack =  NULL; 
    operandStack* operatorStack = NULL;
    int i;
    float operand = 0; //число
    int flagX = 0; 
    float temp1[MAXDEGREE];
    float temp2[MAXDEGREE];
    float temp3[MAXDEGREE]; 
    for (i = 0; i < strlen(line); i++) { //Основной цикл 

        if (line[i] == ' ') // при пробеле парсим дальше
            continue;
        else if (line[i] == '(') { //при скобке добавляем её в стек операторов
            appendOperator(&operatorStack, line[i]);
        }
        else if ((isdigit(line[i]) || line[i] == '.') && (flagX = 0)) { //проверка на число int/float
            int decimalCount = 0;//счётчик точки 
            float operand = 0; //число
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
            if (line[i + 1] == 'x' && line[i+2] == '^'){
                flagX = 1;
                
            }
            else if(line[i + 1] == 'x' && line[i+2] != '^'){
                createMonom(temp1, operand, 1 );
                appendPolinom(&monomsStack, temp1);
            }
            else if(line[i+1] != 'x'){
                createMonom(temp2, operand, 0);
                appendPolinom(&monomsStack, temp2);
            }
        }
        else if(flagX == 1 && isdigit(line[i])){
            int power = 0 ;
            while (i < strlen(line) && isdigit(line[i])) { //цикл сборки
                if (line[i] == '.') { // проверка на многоточность 
                    exit(-1);
                }

                power = power * 10 + (line[i] - '0'); // сборка числа
                i++;
            }
            createMonom(temp3, operand, power);
            appendPolinom(&monomsStack, temp3);
        }
        else if (line[i] == ')') { //если закрывается скобка
            while (operatorStack->operator != '(') {
                float result[MAXDEGREE];
                float* term2 = popMonom(&monomsStack); //достаём a
                float* term1 = popMonom(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term2, term1, result , opr); //производим простое вычисление
                appendPolinom(&monomsStack, result);//записываем результат вычислений
            }

            popOperator(&operatorStack); // Удаление '(' из стека
        }
        else if (isOperator(line[i])) {//при нахождении оператора
            float result[MAXDEGREE];
            while (!isOperandStackEmpty(&operatorStack) && getPriority(line[i]) <= getPriority(operatorStack->operator)) { //если приоритет меньше верхушки стека
                float result[MAXDEGREE];
                float* term2 = popMonom(&monomsStack); //достаём a
                float* term1 = popMonom(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term2, term1, result , opr); //производим простое вычисление
                appendPolinom(&monomsStack, result);//записываем результат вычислений
            }

            appendOperator(&operatorStack, line[i]);
        }
    }

    while (!isOperandStackEmpty(&operatorStack)) {
                float result[MAXDEGREE];
                float* term2 = popMonom(&monomsStack); //достаём a
                float* term1 = popMonom(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term2, term1, result , opr); //производим простое вычисление
                appendPolinom(&monomsStack, result);//записываем результат вычислений
    }

    return popMonom(&monomsStack); // Возвращаем результат вычисления выражения
}

int main() {
    char expression[100];

    printf("Enter a mathematical expression: ");
    fgets(expression, 100, stdin);
    expression[strlen(expression) - 1] = '\0'; // Удаление символа новой строки

    double variableValue;
    printf("Enter value of variable: ");
    scanf("%lf", &variableValue);

    double result = parsExpression(expression);

    printf("Result: %lf", result);

    return 0;
}

