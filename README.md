*feat:* implementar avaliador de expressões numéricas com conversão entre infixa e posfixa

Este commit adiciona a implementação completa do avaliador de expressões numéricas conforme os requisitos da atividade prática da disciplina de Estrutura de Dados.

Principais funcionalidades adicionadas:

Conversão de expressões pós-fixa → infixa usando pilhas de strings

Avaliação de expressões pós-fixas usando pilhas numéricas

Suporte aos operadores aritméticos binários: +, -, *, /, %, ^

Implementação de funções unárias: raiz, sen, cos, tg, log

Tratamento de ângulos em graus para funções trigonométricas

Retorno seguro em caso de erro na conversão (retorna NULL)

Alocação dinâmica para a saída da expressão infixa

Organização em arquivos: expressao.c, expressao.h e main.c

Objetivo do projeto:

Criar um mini-interpretador matemático capaz de avaliar e converter expressões, utilizando pilhas como estrutura central, reforçando conceitos de TAD, notação polonesa reversa e manipulação de strings em C.
