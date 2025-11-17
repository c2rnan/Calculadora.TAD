#include <stdio.h>
#include <stdlib.h>

#include "expressao.h"

int main() {
    char expr[512];

    printf("Digite expressao posfixa: ");
    fgets(expr, 512, stdin);

    // retirar \n
    expr[strcspn(expr, "\n")] = 0;

    char *infixa = getFormaInFixa(expr);
    float valor = getValorPosFixa(expr);

    printf("Infixa: %s\n", infixa);
    printf("Valor: %.4f\n", valor);

    free(infixa);
    return 0;
}
