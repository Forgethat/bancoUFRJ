#ifndef CONTA_H
#define CONTA_H

#ifdef _WIN32
    #include <windows.h>
    #define sleep(x) Sleep(1000 * (x))
#else
    #include <unistd.h>
#endif

#include <stdio.h>
#include <time.h>

#define MAX_TRANS 100

typedef enum { DEP = 1, SAQ, APLI, RESG, PIX, CSAL, EXT, SAIR, REND, PIXR} TipoTransacao;

typedef struct {
    TipoTransacao tipo;
    long long valor;
	long long saldo_corrente_apos;
	long long saldo_poupanca_apos;
    char destino[128];
    char quando[20];
} Transacao;

typedef struct {
    long long saldo_corrente;
    long long saldo_poupanca;
    Transacao log[MAX_TRANS]; //Ex: conta.log[2].valor acessar o valor da transação 2
    int nlog; //Armazena o total de transações registradas
} Conta;

extern Conta conta1;

enum {
    OK = 0,
    ERRO_VALOR_INVALIDO = 1,
    ERRO_SALDO_INSUFICIENTE = 2,
    ERRO_CAPACIDADE_LOG = 3
};

void conta_init(void); // inicializar a conta logo no início da aplicação. 

int  depositar(long long valor); // depositar na conta
int  sacar(long long valor); // sacar da conta
int  aplicar_poupanca(long long valor); // passar dinheiro da conta para a poupança
int  resgatar_poupanca(long long valor); // passar dinheiro da poupança para a conta
int  fazer_pix(const char destino[], long long valor); // tirar dinheiro da conta e transferir para a chave pix
void render_poupanca(double rendimento_mensal); // colocar na conta o rendimento mensal da poupança
int  receber_pix(long long valor); // creditar um pix recebido na conta

long long saldo_corrente(void); // retorna o valor do saldo
long long saldo_poupanca(void); // retorna o valor da poupança

void extrato_imprimir(void); // imprime as transações realizadas e o saldo final.

void calcular_tempo(char *quando, int tamanho);


#endif // CONTA_H
