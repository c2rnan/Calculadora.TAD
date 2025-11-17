#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "expressao.h"

// -------------------------------------------------------
// ----- PILHA DE FLOATS (para avaliação numérica)
// -------------------------------------------------------
typedef struct NumNo {
    float valor;
    struct NumNo *prox;
} NumNo;

void pushNum(NumNo **topo, float v) {
    NumNo *n = (NumNo*)malloc(sizeof(NumNo));
    n->valor = v;
    n->prox = *topo;
    *topo = n;
}

float popNum(NumNo **topo, int *erro) {
    if (*topo == NULL) {
        *erro = 1;
        return 0;
    }

    NumNo *tmp = *topo;
    float v = tmp->valor;
    *topo = tmp->prox;
    free(tmp);
    return v;
}

// -------------------------------------------------------
// ----- PILHA DE STRINGS (para conversão de notação)
// -------------------------------------------------------
typedef struct StrNo {
    char exp[512];
    int prioridade;
    struct StrNo *prox;
} StrNo;

void pushStr(StrNo **topo, const char *txt, int prio) {
    StrNo *n = (StrNo*)malloc(sizeof(StrNo));
    strcpy(n->exp, txt);
    n->prioridade = prio;
    n->prox = *topo;
    *topo = n;
}

StrNo popStrNode(StrNo **topo, int *erro) {
    StrNo ret;
    ret.exp[0] = '\0';
    ret.prioridade = 0;

    if (*topo == NULL) {
        *erro = 1;
        return ret;
    }

    StrNo *tmp = *topo;
    strcpy(ret.exp, tmp->exp);
    ret.prioridade = tmp->prioridade;

    *topo = tmp->prox;
    free(tmp);

    return ret;
}

// -------------------------------------------------------
// ----- FUNÇÃO getValorPosFixa()
// -------------------------------------------------------
float getValorPosFixa(char *Str) {
    NumNo *pilha = NULL;
    int erro = 0;

    char copy[512];
    strcpy(copy, Str);

    char *tok = strtok(copy, " ");

    while (tok != NULL) {
        erro = 0;

        // operador binário
        if (strcmp(tok, "+") == 0 ||
            strcmp(tok, "-") == 0 ||
            strcmp(tok, "*") == 0 ||
            strcmp(tok, "/") == 0 ||
            strcmp(tok, "%") == 0 ||
            strcmp(tok, "^") == 0) {

            float b = popNum(&pilha, &erro);
            float a = popNum(&pilha, &erro);
            if (erro) return NAN;

            if (strcmp(tok, "+") == 0) pushNum(&pilha, a + b);
            else if (strcmp(tok, "-") == 0) pushNum(&pilha, a - b);
            else if (strcmp(tok, "*") == 0) pushNum(&pilha, a * b);
            else if (strcmp(tok, "/") == 0) pushNum(&pilha, a / b);
            else if (strcmp(tok, "%") == 0) pushNum(&pilha, fmod(a, b));
            else if (strcmp(tok, "^") == 0) pushNum(&pilha, pow(a, b));
        }

        // funções unárias
        else if (strcmp(tok, "raiz") == 0 ||
                 strcmp(tok, "sen") == 0  ||
                 strcmp(tok, "cos") == 0  ||
                 strcmp(tok, "tg") == 0   ||
                 strcmp(tok, "log") == 0) {

            float a = popNum(&pilha, &erro);
            if (erro) return NAN;

            if (strcmp(tok, "raiz") == 0) pushNum(&pilha, sqrt(a));
            else if (strcmp(tok, "sen") == 0) pushNum(&pilha, sin(a * M_PI / 180.0));
            else if (strcmp(tok, "cos") == 0) pushNum(&pilha, cos(a * M_PI / 180.0));
            else if (strcmp(tok, "tg") == 0) pushNum(&pilha, tan(a * M_PI / 180.0));
            else if (strcmp(tok, "log") == 0) pushNum(&pilha, log10(a));
        }

        // número
        else {
            pushNum(&pilha, atof(tok));
        }

        tok = strtok(NULL, " ");
    }

    float r = popNum(&pilha, &erro);
    return r;
}

// -------------------------------------------------------
// ----- PRIORIDADE para infixa
// -------------------------------------------------------
int prioridadeOp(const char *op) {
    if (strcmp(op, "sen") == 0) return 4;
    if (strcmp(op, "cos") == 0) return 4;
    if (strcmp(op, "tg") == 0)  return 4;
    if (strcmp(op, "log") == 0) return 4;
    if (strcmp(op, "raiz") == 0)return 4;

    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0) return 2;
    if (strcmp(op, "/") == 0) return 2;
    if (strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "+") == 0) return 1;
    if (strcmp(op, "-") == 0) return 1;

    return 0;
}

// -------------------------------------------------------
// ----- getFormaInFixa()
// -------------------------------------------------------
char *getFormaInFixa(char *Str) {
    StrNo *pilha = NULL;
    int erro = 0;

    char copy[512];
    strcpy(copy, Str);

    char *tok = strtok(copy, " ");

    while (tok != NULL) {

        // operador binário
        if (strcmp(tok, "+") == 0 || strcmp(tok, "-") == 0 ||
            strcmp(tok, "*") == 0 || strcmp(tok, "/") == 0 ||
            strcmp(tok, "%") == 0 || strcmp(tok, "^") == 0) {

            StrNo b = popStrNode(&pilha, &erro);
            StrNo a = popStrNode(&pilha, &erro);
            if (erro) return NULL;

            int prioOp = prioridadeOp(tok);

            char A[512], B[512];

            if (a.prioridade < prioOp) sprintf(A, "(%s)", a.exp);
            else strcpy(A, a.exp);

            if (b.prioridade < prioOp) sprintf(B, "(%s)", b.exp);
            else strcpy(B, b.exp);

            char novo[512];
            sprintf(novo, "%s%s%s", A, tok, B);

            pushStr(&pilha, novo, prioOp);
        }

        // funções unárias
        else if (strcmp(tok, "sen") == 0 ||
                 strcmp(tok, "cos") == 0 ||
                 strcmp(tok, "tg")  == 0 ||
                 strcmp(tok, "log") == 0 ||
                 strcmp(tok, "raiz")== 0) {

            StrNo a = popStrNode(&pilha, &erro);
            if (erro) return NULL;

            char novo[512];
            sprintf(novo, "%s(%s)", tok, a.exp);

            pushStr(&pilha, novo, prioridadeOp(tok));
        }

        // número
        else {
            pushStr(&pilha, tok, 5);
        }

        tok = strtok(NULL, " ");
    }

    StrNo resultado = popStrNode(&pilha, &erro);
    if (erro) return NULL;

    char *saida = (char*)malloc(strlen(resultado.exp) + 1);
    strcpy(saida, resultado.exp);

    return saida;
}
