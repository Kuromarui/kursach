#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Структура для представления полинома
typedef struct Polynomial {
    int degree; // Степень полинома
    int* coefficients; // Коэффициенты полинома
} Polynomial;

// Функция для создания нового полинома
Polynomial* createPolynomial(int degree) {
    // Выделение памяти под полином
    Polynomial* polynomial = (Polynomial*)malloc(sizeof(Polynomial));
    polynomial->degree = degree;

    // Выделение памяти под массив коэффициентов
    polynomial->coefficients = (int*)malloc((degree + 1) * sizeof(int));

    // Инициализация коэффициентов нулями
    for (int i = 0; i <= degree; i++) {
        polynomial->coefficients[i] = 0;
    }

    return polynomial;
}

// Функция для освобождения памяти, занятой полиномом
void freePolynomial(Polynomial* polynomial) {
    free(polynomial->coefficients);
    free(polynomial);
}

// Функция для вычисления значения полинома при заданном значении переменной x
int evaluatePolynomial(Polynomial* polynomial, int x) {
    int result = 0;

    // Вычисление значения полинома
    for (int i = 0; i <= polynomial->degree; i++) {
        result += polynomial->coefficients[i] * pow(x, polynomial->degree - i);
    }

    return result;
}

// Функция для выполнения операции сложения полиномов
Polynomial* addPolynomials(Polynomial* polynomial1, Polynomial* polynomial2) {
    int maxDegree = (polynomial1->degree > polynomial2->degree) ? polynomial1->degree : polynomial2->degree;
    Polynomial* result = createPolynomial(maxDegree);

    // Сложение коэффициентов полиномов
    for (int i = 0; i <= polynomial1->degree; i++) {
        result->coefficients[result->degree - (polynomial1->degree - i)] += polynomial1->coefficients[i];
    }
    for (int i = 0; i <= polynomial2->degree; i++) {
        result->coefficients[result->degree - (polynomial2->degree - i)] += polynomial2->coefficients[i];
    }

    return result;
}

// Функция для выполнения операции вычитания полиномов
Polynomial* subtractPolynomials(Polynomial* polynomial1, Polynomial* polynomial2) {
    int maxDegree = (polynomial1->degree > polynomial2->degree) ? polynomial1->degree : polynomial2->degree;
    Polynomial* result = createPolynomial(maxDegree);

    // Вычитание коэффициентов полиномов
    for (int i = 0; i <= polynomial1->degree; i++) {
        result->coefficients[result->degree - (polynomial1->degree - i)] += polynomial1->coefficients[i];
    }
    for (int i = 0; i <= polynomial2->degree; i++) {
        result->coefficients[result->degree - (polynomial2->degree - i)] -= polynomial2->coefficients[i];
    }

    return result;
}

// Функция для выполнения операции умножения полиномов
Polynomial* multiplyPolynomials(Polynomial* polynomial1, Polynomial* polynomial2) {
    int resultDegree = polynomial1->degree + polynomial2->degree;
    Polynomial* result = createPolynomial(resultDegree);

    // Умножение полиномов
    for (int i = 0; i <= polynomial1->degree; i++) {
        for (int j = 0; j <= polynomial2->degree; j++) {
            result->coefficients[result->degree - (resultDegree - (i + j))] += polynomial1->coefficients[i] * polynomial2->coefficients[j];
        }
    }

    return result;
}

// Функция для выполнения операции деления полиномов
Polynomial* dividePolynomials(Polynomial* dividend, Polynomial* divisor, Polynomial** remainder) {
    if (divisor->degree == 0 && divisor->coefficients[0] == 0) {
        printf("Ошибка: деление на ноль\n");
        return NULL;
    }

    int quotientDegree = dividend->degree - divisor->degree;
    Polynomial* quotient = createPolynomial(quotientDegree);
    *remainder = createPolynomial(dividend->degree);

    Polynomial* tempDividend = createPolynomial(dividend->degree);
    memcpy(tempDividend->coefficients, dividend->coefficients, (dividend->degree + 1) * sizeof(int));

    while (tempDividend->degree >= divisor->degree) {
        int currentDegree = tempDividend->degree - divisor->degree;
        int currentCoefficient = tempDividend->coefficients[tempDividend->degree] / divisor->coefficients[divisor->degree];

        quotient->coefficients[currentDegree] = currentCoefficient;

        for (int i = 0; i <= divisor->degree; i++) {
            tempDividend->coefficients[tempDividend->degree - (divisor->degree - i)] -= currentCoefficient * divisor->coefficients[i];
        }

        tempDividend->degree--;
    }

    memcpy((*remainder)->coefficients, tempDividend->coefficients, (tempDividend->degree + 1) * sizeof(int));

    freePolynomial(tempDividend);

    return quotient;
}

// Функция для парсинга полинома из строки
Polynomial* parsePolynomial(const char* expression) {
    int length = strlen(expression);
    int degree = 0;
    int index = 0;
    int sign = 1;

    // Поиск степени полинома
    while (index < length && expression[index] != 'x') {
        if (isdigit(expression[index])) {
            degree = degree * 10 + (expression[index] - '0');
        }
        index++;
    }

    while (index < length && !isdigit(expression[index])) {
        if (expression[index] == '-') {
            sign = -1;
        }
        index++;
    }

    Polynomial* polynomial = createPolynomial(degree);

    // Парсинг коэффициентов полинома
    while (index < length) {
        int coefficient = 0;
        int factor = 1;
        int factorIndex = 0;

        if (expression[index] == '-') {
            sign = -1;
            index++;
        }

        while (index < length && isdigit(expression[index])) {
            coefficient = coefficient * 10 + (expression[index] - '0');
            index++;
        }

        while (index < length && !isdigit(expression[index])) {
            if (expression[index] == 'x') {
                break;
            }

            if (expression[index] == '^') {
                index++;
                while (index < length && isdigit(expression[index])) {
                    factorIndex = factorIndex * 10 + (expression[index] - '0');
                    index++;
                }
                break;
            }

            if (expression[index] == '+') {
                sign = 1;
            }

            index++;
        }

        if (coefficient == 0) {
            continue;
        }

        if (factorIndex == 0) {
            factor = 1;
        } else if (factorIndex == 1) {
            factor = 10;
        } else {
            factor = pow(10, factorIndex);
        }

        polynomial->coefficients[degree] = sign * (coefficient * factor);
    }

    return polynomial;
}

void printPolynomial(Polynomial* polynomial) {
    for (int i = 0; i <= polynomial->degree; i++) {
        if (polynomial->coefficients[i] == 0) {
            continue;
        }

        if (i < polynomial->degree && polynomial->coefficients[i] > 0) {
            printf("+");
        }

        if (i == polynomial->degree) {
            printf("%d", polynomial->coefficients[i]);
        } else {
            printf("%dx^%d", polynomial->coefficients[i], polynomial->degree - i);
        }
    }
    printf("\n");
}

// Функция для выполнения операции с полиномами
Polynomial* performOperation(const char* expression) {
    int length = strlen(expression);
    int index = 0;
    Polynomial* result;

    // Парсинг первого полинома
    while (index < length && isalnum(expression[index])) {
        index++;
    }
    Polynomial* polynomial1 = parsePolynomial(expression);

    // Парсинг операции
    char operation = expression[index];
    index++;

    // Парсинг второго полинома
    while (index < length && isalnum(expression[index])) {
        index++;
    }
    Polynomial* polynomial2 = parsePolynomial(expression + index);

    // Выполнение операции в зависимости от символа
    switch (operation) {
        case '+':
            result = addPolynomials(polynomial1, polynomial2);
            break;
        case '-':
            result = subtractPolynomials(polynomial1, polynomial2);
            break;
        case '*':
            result = multiplyPolynomials(polynomial1, polynomial2);
            break;
        case '/':
            result = dividePolynomials(polynomial1, polynomial2, &remainder);
            break;
        default:
            printf("Ошибка: неверная операция\n");
            result = NULL;
            break;
    }

    // Освобождение памяти, занятой полиномами
    freePolynomial(polynomial1);
    freePolynomial(polynomial2);

    return result;
}


int main() {
    char expression[100];
    printf("Введите выражение: ");
    fgets(expression, 100, stdin);

    // Удаление символа новой строки из строки ввода
    if (strlen(expression) > 0 && expression[strlen(expression) - 1] == '\n')
        expression[strlen(expression) - 1] = '\0';

    // Выполнение операции и печать результата
    Polynomial* result = performOperation(expression);
    if (result != NULL) {
        printf("Результат: ");
        printPolynomial(result);
        freePolynomial(result);
    }

    return 0;
}
