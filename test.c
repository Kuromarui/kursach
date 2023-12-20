#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

int maxDegree = 2; //Максимальная степень в выражении
struct Node { //структура списка операторов 
    double* data;           
    struct Node* next;    
};

struct monomialStack { //структура стека на списке Node
    struct Node* top;   
};

void initStack(struct monomialStack* stack) { // инициализация стека
    stack->top = NULL;
}

void push(struct monomialStack* stack, double* data) { //добавление массива в стек

    struct Node* newNode = (struct Node*)calloc(sizeof(struct Node), 1);
    if (newNode == NULL) {
        printf("Ошибка выделения памяти");
        exit(-1);
    }

    newNode->data = data;
    newNode->next = stack->top;

    stack->top = newNode;
}


double* pop(struct monomialStack* stack) { //извлечение массива из стека
    if (stack->top == NULL) {
        printf("Стек пуст\n");
        return NULL;
    }

    double* data = stack->top->data;

    struct Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);

    return data;

}

void createMonom(struct monomialStack* stack, double coef, int degree) { //инициализация монома в виде массива
    double* arr = (double*)calloc(sizeof(double) , maxDegree * 100 + 1);
    if (arr == NULL) {
        printf("Ошибка выделения памяти");
        exit(-1);
    }
    arr[degree] = coef;
    push(stack, arr);
}

typedef struct operatorStack{ //структура стека оператора
    char operator; 
    struct operatorStack* next;
} operatorStack;

void appendOperator(operatorStack** head, char operator) { //добавление оператора в стек
    operatorStack* stack = (operatorStack*)malloc(sizeof(operatorStack));
    if (stack == NULL) {
        exit(-1);
    }
    stack->operator = operator;
    stack->next = *head;
    *head = stack;
}

char popOperator(operatorStack** head) { //достать оператор с верхушки стека
    if (*head == NULL) { 
        exit(-1);
    }
    operatorStack* temp = *head;
    char op = temp->operator;
    *head = temp->next;
    free(temp);
    return op;
}


bool isOperandStackEmpty(operatorStack* head) { //проверка стека на пустоту
    return head == NULL;
}


bool isOperator(char ch) { //Проверка на оператор 
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

bool isAlert(char ch){ //проверка на запрещённый символ
    return ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '&' || ch == '_' || ch == '=' || ch == '|' || ch == '<' || ch == '>' || ch == '?' || ch == ';' || ch == ':' || ch == '\'' || ch == '\"' || ch == '\\' || ch == '\{' || ch == '\[' || ch == '}' || ch == ']';
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



void performOperation(double* term1, double* term2, char operator, struct monomialStack* stack) { //базовые мат операции 
    double* result = (double*)calloc(sizeof(double) , maxDegree * 100 + 1);
    int length1 = 0, length2 = 0, monoCounter = 0;
    for (int i = maxDegree * 100 + 1; i != -1; i-- ){
        if (term1[i] != 0 ){
            length1 = i;
            break;
        }
    }
    for (int i= maxDegree * 100 + 1; i != -1; i-- ){
        if (term2[i]!= 0){
            length2 = i;
            break;
        }
    }
    switch (operator) {
        case '+':
            for (int i = 0 ; i < maxDegree + 1; i++){
                result[i] = term1[i] + term2[i];
            }
            push(stack, result);
            break;
        case '-':
            for (int i = 0 ; i < maxDegree + 1; i++){
                result[i] = term1[i] - term2[i];
            }
            push(stack, result);         
            break;
        case '*':
            for (int i = 0; i < length1+ 1; i++) {
                for (int j = 0; j <length2 + 1; j++) {
                    result[i + j] += term1[i] * term2[j];
                }
            }
            push(stack, result);
            break;
        case '/':
            if (term2[length2] == 0){
                printf("Деление на ноль");
                exit(-1);
            }
            for (int i = 0 ; i < maxDegree * 100 + 1; i++){ //цикл поиска многочлена
                if (term2[i] != 0){
                    monoCounter++;
                }
            }
            if (monoCounter > 1){ //проверка деления на многочлен 
                printf("Деление на многочлен не поддерживается");
                exit(-1);
            }
            for (int i = 0 ; i < length1+1 ; i++){
                if (term1[i] != 0){
                    if (i - length2 < 0){
                        printf("Отрицательная степень");
                        exit(-1);
                    }
                    else{
                        result[i - length2] = term1[i] / term2[length2];
                    }
                }
            }
            push(stack, result);
            break;
        default:
            printf("неправильное действие");
            exit(-1);
    } 
    free(term1);
    free(term2);
}

void findMaxDegree(char* line){ //поиск максимальной степени в введёном выражении
    bool flag = false;
    for (int i = 0 ; i < strlen(line); i++){
        if (line[i] == '^'){
            flag = true;
        }
        else if(flag == true && isdigit(line[i])){
            int power = 0 ;
            while (i < strlen(line) && isdigit(line[i])) { //цикл сборки
                if (line[i] == '.' || line[i] == ',') { // проверка на многоточность 
                    exit(-1);
                }

                power = power * 10 + (line[i] - '0'); // сборка числа
                i++;
            }
            i--;
            maxDegree = power;
            flag = false;
        }
    }
}

double* parsExpression(char* line) { //Парсинг выражения
    struct monomialStack monomsStack; 
    operatorStack* operatorStack = NULL;
    int i;
    int flagX = 0; 
    double operandglobal = 0;
    if (line[0] == '-'){
        printf("Пожалуйста введите первым положительное значение");
        exit(-1);
    }
    for (i = 0; i < strlen(line); i++) { //Основной цикл 
        if(line[i] == '+' && i == 0){
            continue;
        }
        if (line[i] == ' ') // при пробеле парсим дальше
            continue;
        else if (line[i] == '(') { //при скобке добавляем её в стек операторов
            appendOperator(&operatorStack, line[i]);
        }
        else if ((isdigit(line[i]) || line[i] == '.' || line[i] == ',') && (flagX == 0)) { //проверка на число int/float
            int decimalCount = 0;//счётчик точки 
            double operand = 0; //число
            int afterpointNumbers = 0; //счётчик количества символов после точки
            while ((i < strlen(line)) && (isdigit(line[i]) || line[i] == '.' || line[i] == ',' )) { //цикл сборки
                if (line[i] == '.' || line[i] == ',') { // проверка на многоточность 
                    decimalCount++;
                    i++;
                    if (decimalCount > 1) {
                        printf("Неправильное выражение");
                        exit(-1);
                    }
                }
                if (decimalCount == 1){
                    afterpointNumbers++;
                }
                operand = operand * 10 + (line[i] - '0'); // сборка числа
                i++;
            }

            i--; // шаг назад 
            operandglobal = operand;
            if (decimalCount == 1){ // сборка при float 
                operandglobal /= pow(10, afterpointNumbers);
            }
            
            if (line[i + 1] == 'x' && line[i+2] == '^'){ // при x^
                if (!isdigit(line[i+3])){
                    printf("Неправильное выражение");
                    exit(-1);  
                }
                else{
                    flagX = 1;
                }
            }

            else if(line[i + 1] == 'x' && line[i+2] != '^'){
                createMonom(&monomsStack, operandglobal, 1 );
            }

            else if(line[i+1] != 'x'){
                createMonom(&monomsStack, operandglobal, 0);
            }
        }
        else if (isalpha(line[i])){ // проверка на другой оператор 
            if (line[i] != 'x'){
                printf("Введите x а не другой оператор");
                exit(-1);
            }
            else{
                if (!isdigit(line[i-1]) && line[i+1] != '^'){
                    createMonom(&monomsStack, 1, 1);
                }
                else if(!isdigit(line[i-1]) && line[i+1] == '^'){
                    flagX = 1;
                    if (!isdigit(line[i+2])){
                        printf("Неправильное выражение");
                        exit(-1);
                    }
                    
                }
            }

        }
        else if(line[i] == '^' && line[i-1] != 'x'){ // ошибка при 2^
            printf("Возведение чисел в степень не поддерживается");
            exit(-1);
        }
        else if(isAlert(line[i])){
            printf("Недопустимый символ");
            exit(-1);
        }
        else if(flagX == 1 && isdigit(line[i]) && isdigit(line[i-3])){ // при ЧИСЛО x
            int power = 0 ;
            while (i < strlen(line) && isdigit(line[i])) { //цикл сборки
                if (line[i] == '.' || line[i] == ',') { // проверка на многоточность 
                    exit(-1);
                }

                power = power * 10 + (line[i] - '0'); // сборка числа
                i++;
            }
            i--;
            createMonom(&monomsStack, operandglobal, power);
            flagX = 0;
        }
        else if(flagX == 1 && isdigit(line[i]) && (!isdigit(line[i-3]) || (i-3<0))){ // при x
            int power = 0 ;
            while (i < strlen(line) && isdigit(line[i])) { //цикл сборки
                if (line[i] == '.' || line[i] == ',') { // проверка на многоточность 
                    printf("Некорректное число");
                    exit(-1);
                }

                power = power * 10 + (line[i] - '0'); // сборка числа
                i++;
            }
            i--;
            createMonom(&monomsStack, 1, power);
            flagX = 0;
        }
        else if (line[i] == ')') { //если закрывается скобка
            if (line[i+1] == '('){
                printf("Введите оператор между скобками");
                exit(-1);
            }
            while (!isOperandStackEmpty(operatorStack) && operatorStack->operator != '(') {
                double* term2 = pop(&monomsStack); //достаём a
                double* term1 = pop(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор ё

                performOperation(term1, term2, opr , &monomsStack); //производим простое вычисление
            }

            popOperator(&operatorStack); // Удаление '(' из стека
        }
        else if (isOperator(line[i])) {//при нахождении оператора
            if(isOperator(line[i+1])){
                printf("Двойной оператор");
                exit(-1);
            }
            if (line[i] == '*' && line[i+1] == '0'){
                printf("Введите выражение без умножения на ноль");
                exit(-1);
            }
            while (!isOperandStackEmpty(operatorStack) && getPriority(line[i]) <= getPriority(operatorStack->operator)) { //если приоритет меньше верхушки стека
                double* term2 = pop(&monomsStack); //достаём a
                double* term1 = pop(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term1, term2, opr , &monomsStack); //производим простое вычисление
            }
            appendOperator(&operatorStack, line[i]);
        }
    }

    while (!isOperandStackEmpty(operatorStack)) {
                double* term2 = pop(&monomsStack); //достаём a
                double* term1 = pop(&monomsStack); // b
                char opr = popOperator(&operatorStack); // оператор

                performOperation(term1, term2, opr , &monomsStack); //производим простое вычисление
    }
    return pop(&monomsStack); // Возвращаем результат вычисления выражения
}

void checkBracket(char line[]){ // проверка на парность скобок
    int inbracket = 0, outbracket =0;
    for (int i = 0 ; i < strlen(line); i++){
        if (line[i] =='('){
            inbracket ++;
        }
        else if(line[i] == ')'){
            outbracket++;
        }
    }
    if (inbracket != outbracket){
        printf("Введите скобки правильно");
        exit(-1);
    }
}

void resultOutput(double* polinom, int degree) { //вывод полинома
    bool isFirstTerm = true; // флаг для определения первого элемента
    bool nullflag = false; // флаг пустого массива
    for (int i = degree; i >= 0; i--) {
        if (polinom[i] != 0) {
            if (!isFirstTerm) {
                if (polinom[i] > 0) {
                    printf("+ ");
                } else {
                    printf("- ");
                }
            } else {
                isFirstTerm = false;
                if (polinom[i] < 0) {
                    printf("- ");
                }
            }

            double absCoef = fabs(polinom[i]);
            if (i == 0) {
                printf("%lf ", absCoef);
                nullflag = true;
            } else if (i == 1) {
                printf("%lfx ", absCoef);
                nullflag = true;
            } else {
                printf("%lfx^%i ", absCoef, i);
                nullflag = true;
            }
        }
    }
    if (nullflag == false){
        printf("0");
    }
}

void removeRaz(char* str) { //удаление разделителей
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (!isRaz(str[i])) {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}


// Пример использования
int main() {

    char expression[100];
    //char expression[5] = {"12x^2"};
    printf("Enter a mathematical expression: ");
    fgets(expression, 100, stdin);
    
    removeRaz(expression);
    checkBracket(expression);
    findMaxDegree(expression);
    if (maxDegree > 100000){
        printf("Слишком большая степень");
        exit(-1);
    }
    
    double* result = parsExpression(expression);
    printf("Результат вычислений ");
    resultOutput(result, maxDegree * 100);
    free(result);
    return 0;
}