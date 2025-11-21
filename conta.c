#include "conta.h"

Conta conta1;

void conta_init(void)
{
    conta1.saldo_corrente = 0;
    conta1.saldo_poupanca = 0;
    conta1.nlog = 0;
}

int depositar(long long valor)
{
    if (valor < 0) {
        printf("Valor inválido! Encerrando a aplicação...\n\n");
        sleep(3);
        return ERRO_VALOR_INVALIDO;
    }

    Transacao A;
    A.tipo = DEP;
    A.valor = valor;
    A.saldo_corrente_apos = conta1.saldo_corrente + valor;
    A.saldo_poupanca_apos = conta1.saldo_poupanca; // para evitar lixo de memoria

    calcular_tempo(A.quando, sizeof(A.quando));

    conta1.log[conta1.nlog] = A;
    conta1.saldo_corrente = A.saldo_corrente_apos;
    conta1.nlog++;

    printf("Depósito de R$%.02f realizado com sucesso às %s\n", (float) valor / 100, A.quando);
    sleep(3);

    return OK;
}

int sacar(long long valor)
{
    if (valor > conta1.saldo_corrente)
    {
        printf("Saldo insuficiente para o saque! Encerrando a aplicação...\n\n");
        sleep(2);
        return ERRO_SALDO_INSUFICIENTE;
    } 

    Transacao A;
    A.tipo = SAQ;
    A.valor = valor;
    A.saldo_corrente_apos = conta1.saldo_corrente - valor;
    A.saldo_poupanca_apos = conta1.saldo_poupanca; // para evitar lixo de memoria

    calcular_tempo(A.quando, sizeof(A.quando));

    conta1.log[conta1.nlog] = A;
    conta1.saldo_corrente = A.saldo_corrente_apos;
    conta1.nlog++;
    printf("Saque de R$%.02f realizado com sucesso às %s\n\n", (float) valor / 100, A.quando);
    sleep(3);

    return OK;
}


int aplicar_poupanca(long long valor)
{
    if (valor > conta1.saldo_corrente)
    {
        printf("Saldo insuficiente! Encerrando a aplicação...\n\n");
        sleep(2);
        return ERRO_SALDO_INSUFICIENTE;
    }
    if (valor < 0) 
    {
        printf("Valor inválido! Encerrando a aplicação...\n\n");
        sleep(2);
        return ERRO_VALOR_INVALIDO;
    }

    Transacao A;
    A.tipo = APLI;
    A.valor = valor;
    A.saldo_corrente_apos = conta1.saldo_corrente - valor;
    A.saldo_poupanca_apos = conta1.saldo_poupanca + valor;

    calcular_tempo(A.quando, sizeof(A.quando));

    conta1.log[conta1.nlog] = A;
    conta1.saldo_corrente = A.saldo_corrente_apos;
    conta1.saldo_poupanca = A.saldo_poupanca_apos;
    conta1.nlog++;
    printf("Aplicação de R$%.02f realizada com sucesso às %s\n\n", (float) valor / 100, A.quando);
    sleep(3);

    return OK;
}

int resgatar_poupanca(long long valor)
{
    printf("SALDO: %lld\n\n", valor);
    if (valor > conta1.saldo_poupanca)
    {
        printf("Saldo insuficiente! Encerrando a aplicação...\n\n");
        sleep(2);
        return ERRO_SALDO_INSUFICIENTE;
    }
    if (valor < 0) 
    {
        printf("Valor inválido! Encerrando a aplicação...\n\n");
        sleep(2);
        return ERRO_VALOR_INVALIDO;
    }

    Transacao A;
    A.tipo = RESG;
    A.valor = valor;
    A.saldo_corrente_apos = conta1.saldo_corrente + valor;
    A.saldo_poupanca_apos = conta1.saldo_poupanca - valor;

    calcular_tempo(A.quando, sizeof(A.quando));

    conta1.log[conta1.nlog] = A;
    conta1.saldo_corrente = A.saldo_corrente_apos;
    conta1.saldo_poupanca = A.saldo_poupanca_apos;
    conta1.nlog++;
    printf("Resgate de R$%.02f realizada com sucesso às %s\n\n", (float) valor / 100, A.quando);
    sleep(3);

    return OK;
}


long long saldo_corrente(void)
{
    return conta1.saldo_corrente;
}

long long saldo_poupanca(void)
{
    return conta1.saldo_poupanca;
}

void extrato_imprimir(void)
{
    for (int i = conta1.nlog - 1; i > - 1; i--)
    {
        printf("Extrato Transação %i:\n\n", i + 1);

        switch (conta1.log[i].tipo)
        {
            case DEP:  printf("Tipo da transação: %s\n", "Depósito"); break;
            case SAQ:  printf("Tipo da transação: %s\n", "Saque"); break;
            case APLI: printf("Tipo da transação: %s\n", "Aplicação"); break;
            case RESG: printf("Tipo da transação: %s\n", "Resgate"); break;
            case PIX:  printf("Tipo da transação: %s\n", "PIX Realizado"); break;
            case REND: printf("Tipo da transação: %s\n", "Rendimento"); break;
            case PIXR: printf("Tipo da transação: %s\n", "PIX Recebido"); break;
            default:   printf("Tipo da transação: %s\n", "Desconhecido"); break;
        }
        printf("Extrato do valor da transação: R$%.02f\n", (float) conta1.log[i].valor / 100);
        printf("Extrato do saldo após a transação: R$%.02f\n", (float) conta1.log[i].saldo_corrente_apos / 100);
        printf("Extrato da poupança após a transação: R$%.02f\n", (float) conta1.log[i].saldo_poupanca_apos / 100);
        if (conta1.log[i].tipo == PIX)
        {
            printf("Destino da transação: %s\n", conta1.log[i].destino);
        }
        printf("Horário da transação: %s\n\n", conta1.log[i].quando);
        sleep(2);
    }
}

void calcular_tempo(char *quando, int tamanho)
{
    time_t tempo_atual = time(NULL);
    struct tm *data_hora = localtime(&tempo_atual);

    snprintf(quando, tamanho, "%d-%02d-%02d %02d:%02d:%02d",
        data_hora->tm_year + 1900,
        data_hora->tm_mon + 1,
        data_hora->tm_mday,
        data_hora->tm_hour,
        data_hora->tm_min,
        data_hora->tm_sec
    );
}
