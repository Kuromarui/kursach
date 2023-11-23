#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    char operator;
    float coef; 
    float power;
} Monom; 

typedef struct monomialStack {
    float coef; 
    float power; 
    char operator; 
    struct monomialStack* next; 
} monomialStack;

void appendMonom(monomialStack** head, float coef, float power, char op){
    monomialStack* newMonom=malloc(sizeof(monomialStack)); 
    newMonom->coef = coef; 
    newMonom->power = power;
    newMonom->operator = op;
    newMonom->next = *head; 
    *head = newMonom; 
}

Monom popMonom(monomialStack** head){ 
    monomialStack* temp = NULL;
    Monom monom; 
    if (*head == NULL){
        exit(-1); 
    }
    temp = *head;
    monom.coef = temp->coef; 
    monom.power = temp->power; 
    *head = temp->next; 
    free(temp); 
    return monom; 
}

void printList(monomialStack* head) {
    monomialStack* ptr = head; // нужно присвоить head переменной, чтобы не потерять начало списка
    while (ptr != NULL) {
        printf("%f", ptr->coef);
        printf("%f", ptr->power);
        ptr = ptr->next; // объявление текущего объекта как следующего в списке
    }
    printf("\n");
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

char popOperator(operandStack ** head){ 
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

Monom performOperation(Monom term1, Monom term2, char operator, monomialStack* stack) { //базовые мат операции 
    switch (operator) {
        case '+':
            if (term1.coef != term2.coef) {
                appendMonom(stack, term2.coef, term2.power, term2.coef = '0');
                appendMonom(stack, term1.coef, term1.power, term1.coef = '+');
                return;
            }

            Monom result;
            result.coef = term1.coef + term2.coef;
            result.power = term1.power;

            return result;

        case '-':
            if (term1.coef != term2.coef) {
                appendMonom(stack, term2.coef,term2.power, term2.coef = '0');
                appendMonom(stack, term1.coef,term1.power, term1.coef = '-');
                return;
            }

            Monom result;
            result.coef = term1.coef - term2.coef;
            result.power = term1.power;
            return result; 
        case '*':
            Monom result;
            result.coef = term1.coef * term2.coef;
            result.power = term1.power + term2.power;
            return result;
        
        case '/':
            if (term2.coef == 0) {
                printf("Ошибка: деление на ноль\n");
                exit(-1);
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


double parsExpression(char* line) { //Парсинг выражения
    monomialStack*  monomsStack =  NULL; 
    operandStack* operatorStack = NULL;
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
            if (line[i + 1] = 'x'){
                monom.coef = operand;
                monom.power = line[i+3];
                appendMonom(&monomsStack, operand); //добавление в стек чисел
            }
            push(&operandStack, operand); //добавление в стек чисел
        }
        else if (line[i] == ')') { //если закрывается скобка
            while (!isStackEmpty(&operatorStack) && operatorStack.data[operatorStack.top] != '(') {
                Monom operand2 = popMonom(&monomsStack); //достаём a
                Monom operand1 = popMonom(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                double result = performOperation(operand1, operand2, opr , operandStack); //производим простое вычисление
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

int main(){

    return 0;
}
