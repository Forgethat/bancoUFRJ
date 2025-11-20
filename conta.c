#include <stdio.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define sleep(x) Sleep(1000 * (x))
#else
    #include <unistd.h>
#endif

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


Conta conta1;

int main()
{
    conta_init(); // inicializar conta
    int resposta = 0;

    while (conta1.nlog < 3) // mudar pra 100 depois
    {
        printf("Escolha uma opção digitando o número correspondente:\n\n");

        printf("1. Depositar (conta corrente)\n");
        printf("2. Sacar (sem cheque especial)\n");
        printf("3. Aplicar na poupança\n");
        printf("4. Resgatar da poupança\n");
        printf("5. Fazer PIX (registrar conta/chave de destino)\n");
        printf("6. Consultar saldos\n");
        printf("7. Extrato (listar transações com data/hora)\n");
        printf("8. Sair\n\n");

        scanf(" %i", &resposta);

        switch (resposta)
        {
            case DEP:
                long long valor_DEP;
                printf("O valor do seu saldo atual é: R$%.02f\n", (float) conta1.saldo_corrente / 100);
                printf("Digite o valor que deseja depositar em centavos: ");
                scanf("%lld", &valor_DEP);
                depositar(valor_DEP);
                break;

            case SAQ:
                long long saque;
                printf("O valor do seu saldo atual é: R$%.02f.\n", (float) conta1.saldo_corrente / 100);
                printf("Digite o valor que deseja sacar em centavos: ");
                scanf("%lld", &saque);
                sacar(saque);
                break;

            case APLI:
                long long valor_APLI;
                printf("O valor do seu saldo atual é: R$%.02f\n", (float) conta1.saldo_corrente / 100);
                printf("Digite o valor que deseja aplicar na poupança em centavos: ");
                scanf("%lld", &valor_APLI);
                aplicar_poupanca(valor_APLI);
                break;

            case RESG:
                long long valor_RESG;
                printf("O valor do seu saldo da poupança atual é: R$%.02f\n", (float) conta1.saldo_poupanca / 100);
                printf("Digite o valor que deseja resgatar da poupança em centavos: ");
                scanf("%lld", &valor_RESG);
                resgatar_poupanca(valor_RESG);
                break;

            case PIX:
                conta1.nlog++;
                break;

            case CSAL:
                printf("Saldo corrente: R$%.02f\n", (float) saldo_corrente() / 100);
                printf("Saldo poupança: R$%.02f\n\n", (float) saldo_poupanca() / 100);
                sleep(3);
                break;

            case EXT:
                extrato_imprimir();
                break;

            case SAIR:
                printf("Saindo da aplicação.\n");
                return OK;

            default:
                printf("Opção inválida.\n\n");
                break;

        }
    }
    printf("Limite de transações alcançado! Encerrando a aplicação por medidas de segurança.\n");
    return ERRO_CAPACIDADE_LOG;
}


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
