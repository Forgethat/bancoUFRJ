#include <stdio.h>
#include <time.h>

#define MAX_TRANS 100

typedef enum { DEP = 1, SAQ, APLI, RESG, PIX, REND, PIXR, SAIR} TipoTransacao;

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





Conta conta1;

int main()
{
    conta_init(); // inicializar conta
    int resposta = 0;

    while (conta1.nlog < 2)
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
            case DEP:  // depositar
                long long valor;
                printf("o valor do seu saldo é: %d\n", conta1.saldo_corrente);
                printf("Digite o valor que deseja depositar: ");
                scanf("%lld", &valor);
                depositar(valor);
                break;

            case SAQ:  // sacar
                long long saque;
                printf("o valor do seu saldo é: %d\n", conta1.saldo_corrente);
                printf("Digite o valor que deseja sacar: ");
                scanf("%lld", &saque);
                sacar(saque);
                break;

            case APLI:
                conta1.nlog++;
                break;

            case RESG:
                conta1.nlog++;
                break;

            case PIX:
                conta1.nlog++;
                break;

            case REND:
                break;

            case PIXR:
                break;

            case SAIR:
                printf("Saindo da aplicação.\n");
                return OK;
        }
    }
    printf("Limite de transações alcançado! Encerrando a aplicação por medidas de segurança.\n");
    return ERRO_CAPACIDADE_LOG;
}


void conta_init(void)
{
    conta1.saldo_corrente = 100;
    conta1.saldo_poupanca = 0;
    conta1.nlog = 0;
}

int depositar(long long valor)
{
    //inicializar tempo
    time_t tempo_atual;
    tempo_atual = time(NULL);
    struct tm *data_hora;
    data_hora = localtime(&tempo_atual);

    if (valor < 0)
    {
        printf("Valor inválido.\n");
        return ERRO_VALOR_INVALIDO;
    }
        Transacao A;
        A.tipo = DEP;
        A.valor = valor;
        A.saldo_corrente_apos = conta1.saldo_corrente + valor;
        A.quando[20];

        //snprintf transforma o horário para string e joga pro array
        snprintf(A.quando, sizeof(A.quando), "%d-%02d-%02d %02d:%02d:%02d", 
        data_hora->tm_year + 1900, 
        data_hora->tm_mon + 1, 
        data_hora->tm_mday, 
        data_hora->tm_hour, 
        data_hora->tm_min, 
        data_hora->tm_sec);

        conta1.log[conta1.nlog] = A;
        conta1.saldo_corrente = A.saldo_corrente_apos;
        conta1.nlog++;

        printf("Transação ocorrida em: %s\n", A.quando);
        sleep(2);
        return OK;
}

int sacar(long long valor)
{
    //inicializar tempo
    time_t tempo_atual;
    tempo_atual = time(NULL);
    struct tm *data_hora;
    data_hora = localtime(&tempo_atual);
    
    if (valor > conta1.saldo_corrente)
        {
            printf("Saldo insuficiente para o saque! Encerrando a aplicação...\n");
            sleep(2); //para brevemente o programa para poder ler o terminal
            return ERRO_SALDO_INSUFICIENTE;
        }
            Transacao A;
            A.tipo = SAQ;
            A.valor = valor;
            A.saldo_corrente_apos = conta1.saldo_corrente - valor;
            A.quando[20];

            //snprintf transforma o horário para string e joga pro array
            snprintf(A.quando, sizeof(A.quando), "%d-%02d-%02d %02d:%02d:%02d", 
            data_hora->tm_year + 1900, 
            data_hora->tm_mon + 1, 
            data_hora->tm_mday, 
            data_hora->tm_hour, 
            data_hora->tm_min, 
            data_hora->tm_sec);

            conta1.log[conta1.nlog] = A;
            conta1.saldo_corrente = A.saldo_corrente_apos;
            conta1.nlog++;
            printf("Transação ocorrida em: %s\n", A.quando);
            sleep(2);

            return OK;
}