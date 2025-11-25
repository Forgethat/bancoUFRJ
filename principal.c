#include "conta.h"

int main()
{
    conta_init(); // inicializar conta
    int resposta = 0;

    while (conta1.nlog < 100) // limitar a 100 transações
    {
        printf("Escolha uma opção digitando o número correspondente:\n\n");

        printf("1. Depositar (conta corrente)\n");
        printf("2. Sacar (sem cheque especial)\n");
        printf("3. Aplicar na poupança\n");
        printf("4. Resgatar da poupança\n");
        printf("5. Consultar saldos\n");
        printf("6. Extrato (listar transações com data/hora)\n");
        printf("7. Sair\n\n");

        if (scanf(" %i", &resposta) == 0) // checando se usuário digitou um número de fato
        {
            char buffer[100];
            scanf("%s", buffer); // limpando o stdin
        }

        switch (resposta)
        {
            case DEP:
                long long valor_DEP = 0;
                printf("O valor do seu saldo atual é: R$%.02f\n", conta1.saldo_corrente / 100.0);
                printf("Digite o valor que deseja depositar em centavos: ");
                if (scanf("%lld", &valor_DEP) == 0)
                {
                    char buffer[100];
                    scanf("%s", buffer); 
                }
                depositar(valor_DEP);
                break;

            case SAQ:
                long long saque = 0;
                printf("O valor do seu saldo atual é: R$%.02f.\n", conta1.saldo_corrente / 100.0);
                printf("Digite o valor que deseja sacar em centavos: ");
                if (scanf("%lld", &saque) == 0)
                {
                    char buffer[100];
                    scanf("%s", buffer); 
                }
                sacar(saque);
                break;

            case APLI:
                long long valor_APLI = 0;
                printf("O valor do seu saldo atual é: R$%.02f\n", conta1.saldo_corrente / 100.0);
                printf("Digite o valor que deseja aplicar na poupança em centavos: ");
                if (scanf("%lld", &valor_APLI) == 0)
                {
                    char buffer[100];
                    scanf("%s", buffer);
                }
                aplicar_poupanca(valor_APLI);
                break;

            case RESG:
                long long valor_RESG = 0;
                printf("O valor do seu saldo da poupança atual é: R$%.02f\n", conta1.saldo_poupanca / 100.0);
                printf("Digite o valor que deseja resgatar da poupança em centavos: ");
                if (scanf("%lld", &valor_RESG) == 0)
                {
                    char buffer[100];
                    scanf("%s", buffer);
                }
                resgatar_poupanca(valor_RESG);
                break;

            case CSAL:
                printf("Saldo corrente: R$%.02f\n", saldo_corrente() / 100.0);
                printf("Saldo poupança: R$%.02f\n\n", saldo_poupanca() / 100.0);
                sleep(2);
                break;

            case EXT:
                extrato_imprimir();
                break;

            case SAIR:
                printf("Saindo da aplicação.\n");
                return OK;

            default:
                printf("Opção inválida.\n\n");
                sleep(2);
                break;
        }
    }
    printf("Limite de transações alcançado! Encerrando a aplicação por medidas de segurança.\n");
    return ERRO_CAPACIDADE_LOG;
}