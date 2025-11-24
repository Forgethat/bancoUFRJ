#include <stdio.h>
#include <time.h>
#include "conta.h"

int main()
{
    conta_init(); // inicializar conta
    int resposta = 0;

    while (conta1.nlog < 3) // mudar pra 100 depois
    {
        printf("Escolha uma opção digitando o número correspondente:\n\n");

        printf("1. Depositar (conta corrente)\n");
        printf("2. Sacar (sem cheque especial)\n");
        printf("3. Aplicar na poupança\n"); //deletar
        printf("4. Resgatar da poupança\n");
        //printf("5. Fazer PIX (registrar conta/chave de destino)\n"); //deletar
        printf("5. Consultar saldos\n");
        printf("6. Extrato (listar transações com data/hora)\n");
        printf("7. Sair\n\n");

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