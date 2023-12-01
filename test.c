#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>


struct Node {
    double* data;           
    struct Node* next;    
};

struct monomialStack {
    struct Node* top;   
};

void initStack(struct monomialStack* stack) {
    stack->top = NULL;
}

void push(struct monomialStack* stack, double* data) {

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }


    newNode->data = data;
    newNode->next = stack->top;

    stack->top = newNode;
}


double* pop(struct monomialStack* stack) {
    if (stack->top == NULL) {
        fprintf(stderr, "Стек пуст\n");
        return NULL;
    }

    double* data = stack->top->data;

    struct Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);

    return data;

}

void createMonom(struct monomialStack* stack, double coef, int degree) {
    double* arr = (double*)calloc((degree+1) , sizeof(double));
    if (arr == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < degree + 1; i++) {
        if (i != degree){
            arr[i] = 0;
        }
        else{
            arr[i] = coef;
            break;
        }
    }

    push(stack, arr);
}

typedef struct operatorStack{ 
    char operator; 
    struct operatorStack* next;
} operatorStack;

void appendOperator(operatorStack** head, char operator) {
    operatorStack* stack = (operatorStack*)malloc(sizeof(operatorStack));
    if (stack == NULL) {
        exit(-1);
    }
    stack->operator = operator;
    stack->next = *head;
    *head = stack;
}

char popOperator(operatorStack** head) {
    if (*head == NULL) { 
        exit(-1);
    }
    operatorStack* temp = *head;
    char op = temp->operator;
    *head = temp->next;
    free(temp);
    return op;
}


bool isOperandStackEmpty(operatorStack* head) {
    return head == NULL;
}


bool isOperator(char ch) { //Проверка на оператор 
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
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



void performOperation(double* term1, double* term2, char operator, int Degree,  struct monomialStack* stack) { //базовые мат операции 
    double* result = (double*)calloc(sizeof(double),  (Degree * Degree) +1);
    switch (operator) {
        case '+':
            for (int i = 0 ; i < Degree; i++){
                if (term1[i] != 0 || term2[i] != 0){
                    result[i] = term2[i] + term1[i];
                }
            }
            push(stack, result);
            break;
        case '-':
            for (int i = 0 ; i < Degree; i++){
                if (term1[i] != 0 || term2[i] != 0){
                    result[i] = term1[i] - term2[i];
                }
            }
            push(stack, result);        
            break;
        case '*':
            for (int i = 0; i < Degree; i++){
                if(term2[i] == 0){
                    continue;
                }
                for (int j = 0; j < Degree; j++){
                    if (term1[j] == 0){
                        continue;
                    }
                    else{
                        result[i+j] = result[i+j] + term1[j] * term2[i];
                    }
                }
            }
            push(stack, result);        
            break;
        case '/':
            for (int i = 0; i < Degree; i++){
                if(term2[i] == 0){
                    continue;
                }
                for (int j = 0; j < Degree; j++){
                    if (term1[j] == 0){
                        continue;
                    }
                    else{
                        result[i-j] = result[i-j] + term2[i] / term1[j];
                    }
                }
            }
            push(stack, result);        
            break;
        default:
            printf("неправильное действие");
            exit(EXIT_FAILURE);
    } 
    free(term1);
    free(term2);
}

double* parsExpression(char* line) { //Парсинг выражения
    struct monomialStack monomsStack; 
    operatorStack* operatorStack = NULL;
    int i;
    int flagX = 0; 
    double operandglobal = 0;
    for (i = 0; i < strlen(line); i++) { //Основной цикл 

        if (line[i] == ' ') // при пробеле парсим дальше
            continue;
        else if (line[i] == '(') { //при скобке добавляем её в стек операторов
            appendOperator(&operatorStack, line[i]);
        }
        else if ((isdigit(line[i]) || line[i] == '.') && (flagX == 0)) { //проверка на число int/float
            int decimalCount = 0;//счётчик точки 
            double operand = 0; //число
            
            while ((i < strlen(line)) && (isdigit(line[i]) || line[i] == '.')) { //цикл сборки
                if (line[i] == '.') { // проверка на многоточность 
                    decimalCount++;
                    i++;
                    if (decimalCount > 1) {
                        printf("Invalid expression");
                        exit(EXIT_FAILURE);
                    }
                }

                operand = operand * 10 + (line[i] - '0'); // сборка числа
                i++;
            }

            i--; // шаг назад 
            operandglobal = operand;
            if (decimalCount == 1) // сборка при float 
                operandglobal /= pow(10, decimalCount);
            if (line[i + 1] == 'x' && line[i+2] == '^'){
                flagX = 1;
                
            }
            else if(line[i + 1] == 'x' && line[i+2] != '^'){
                createMonom(&monomsStack, operandglobal, 1 );
            }
            else if(line[i+1] != 'x'){
                createMonom(&monomsStack, operandglobal, 0);
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
            i--;
            createMonom(&monomsStack, operandglobal, power);
            flagX = 0;
        }
        
        else if (line[i] == ')') { //если закрывается скобка
            while (!isOperandStackEmpty(operatorStack) && operatorStack->operator != '(') {
                double* term2 = pop(&monomsStack); //достаём a
                double* term1 = pop(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term1, term2, opr , 3, &monomsStack); //производим простое вычисление
            }

            popOperator(&operatorStack); // Удаление '(' из стека
        }
        else if (isOperator(line[i])) {//при нахождении оператора
            while (!isOperandStackEmpty(operatorStack) && getPriority(line[i]) <= getPriority(operatorStack->operator)) { //если приоритет меньше верхушки стека
                double* term2 = pop(&monomsStack); //достаём a
                double* term1 = pop(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term1, term2, opr , 3, &monomsStack); //производим простое вычисление
            }

            appendOperator(&operatorStack, line[i]);
        }
    }

    while (!isOperandStackEmpty(operatorStack)) {
                double* term2 = pop(&monomsStack); //достаём a
                double* term1 = pop(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term1, term2, opr , 3, &monomsStack); //производим простое вычисление
    }
    return pop(&monomsStack); // Возвращаем результат вычисления выражения
}

// Пример использования
int main() {
    /*struct monomialStack stack;
    operatorStack* operatorsStack = NULL;
    initStack(&stack);
    createMonom(&stack, 2, 5);
    createMonom(&stack, 6, 3);

    // Извлекаем массивы из стека и выводим их
    double* poppedArr1 = pop(&stack);
    appendOperator(&operatorsStack, '*'); 
    double* poppedArr2 = pop(&stack);
    char operators = popOperator(&operatorsStack);
    performOperation(poppedArr1, poppedArr2, operators, 9, &stack);
    double* result = pop(&stack);
    for (int i = 0 ; i < 50 ; i++){
        printf("%lf ", result[i]);
    }
    int degree = maxDegree(result);
    printf("%d ", degree);
    free(result);
    return 0; */

    //char expression[13] = {'(', '2', 'x', '+', '1' , ')', '*' , '(' , '2', 'x', '+', '1', ')'};
    char expression[100];

    printf("Enter a mathematical expression: ");
    fgets(expression, 100, stdin);
    expression[strlen(expression) - 1] = '\0';

    double* result = parsExpression(expression);

    for (int i = 0 ; i < 6 ; i++){
        printf("%lf ", result[i]);
    } 
    free(result);
    return 0;
}
