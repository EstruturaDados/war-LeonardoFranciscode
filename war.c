// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <string.h> // para strcson()
#include <stdlib.h>
#include <locale.h>
#include <time.h>
// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 60
#define MAX_MISSOES 3
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

struct Territorio *war;

struct Mission{
    char tropa[TAM_COR];
    char missao[TAM_MISSAO];
    int totalVitorias;
};

struct Mission *missao;

// --- Protótipos das Funções ---
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha. 
void exibirMenu();
void limparBufferEntrada();
void pausa();
void cadastrarTropas();
void exibirMissao(struct Mission *missao);
void faseDeAtaque();
void simularAtaque(int at, int df, int *totalVitorias);
void alocarMapa();
void alocarMissao();
void exibirMapa();
void atribuirMissao(struct Mission *missao, char* destino, char* missoes[], int exercito);
void liberarMemoria(struct Territorio *war, struct Mission *missao);
int verificarMissao(char* missoes[], struct Territorio *war, int *totalVitorias);

// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    
    alocarMapa();

    cadastrarTropas();

    alocarMissao();
    
    int exercito = rand() % MAX_TERRITORIOS;
    char * missoes[MAX_MISSOES] = { "Conquistar 1 territorio",
                        "Eliminar todas a tropas do exército %s",
                        "Não perder durante 4 batalhas"
    };
    
    atribuirMissao(missao, war[exercito].nome, missoes, exercito);

    int opc;
    do {
        exibirMapa(war);
        exibirMissao(missao);
        exibirMenu();
        scanf("%d", &opc);
        limparBufferEntrada();

        switch(opc) {
            case 1: //ATACAR
                faseDeAtaque();
                break;

            case 2: //VERIFICAR MISSAO
                if (verificarMissao(missoes, war, &missao[0].totalVitorias)) {
                    printf("\nPARABÉNS!! VOCÊ CONCLUIU SUA MISSÃO!!\n");
                    break;
                } else {
                    printf("\nVocê ainda não concluiu sua missão! Volte para a batalha!\n\n");
                }

            case 0: //SAIR

        }
    } while (opc != 0);
    
    printf("Jogo encerrado e memoria liberada. Até a próxima!\n");

    liberarMemoria(war, missao);

    return 0;
}


void exibirMenu() {
    printf("\n--- MENU DE ACOES ---\n");
    printf("[1] ATACAR\n");
    printf("[2] VERIFICAR MISSAO\n");
    printf("[0] SAIR\n>>>> ");
}

// --- Implementação das Funções ---
void alocarMapa(){
    war = (struct Territorio*) calloc (MAX_TERRITORIOS, sizeof(struct Territorio));
    //Verifica se a alocação foi bem-sucedida.
    if (war == NULL) {
        printf("Erro: Falha ao alocar memória");
    };
}

void alocarMissao(){
    missao = (struct Mission*) calloc (1, sizeof(struct Mission));
}
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const struct Territorio *war){
    printf("=============================================\n");
    printf("        MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("=============================================\n");

    for (int i = 0; i < MAX_TERRITORIOS; i++){
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas);
    }
    printf("\n");
}

void cadastrarTropas() {
    int totalTropas = 0;
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    printf("\n========================================================\n\n");
    printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo.\n\n");

    do {
        printf("\n--- Cadastrando Território %d ---\n", totalTropas + 1);
        printf("Nome do Território: ");
        fgets(war[totalTropas].nome, TAM_NOME, stdin);

        printf("Cor do Exercito (ex: Azul, Verde): ");
        fgets(war[totalTropas].cor, TAM_COR, stdin);

        // Remove o '\n' que o fgets adiciona no final da string
        war[totalTropas].nome[strcspn(war[totalTropas].nome, "\n")] = '\0';
        war[totalTropas].cor[strcspn(war[totalTropas].cor, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &war[totalTropas].tropas);
        limparBufferEntrada();

        totalTropas++;

    } while (totalTropas < MAX_TERRITORIOS);

    printf("\nCadastro inicial concluído com sucesso!\n\n");
}

void atribuirMissao(struct Mission *missao, char* destino, char* missoes[], int exercito) {
        int missaoIndex = rand() % MAX_MISSOES; //Para sortear a missão.
        int cor;
        do {
            cor = rand() % MAX_TERRITORIOS; //Para sortear a tropa em caso da missão[1].
        } while (cor == exercito);

        switch (missaoIndex) {
            case 0:
                sprintf(missao[0].missao, "%s", missoes[0]);
                break;
            case 1:
                sprintf(missao[0].missao,missoes[1], war[cor].cor); // Atribui a missao ao jogador
                break;      
            case 2:
                sprintf(missao[0].missao, "%s", missoes[2]);
                break;
        }
        sprintf(missao[0].tropa, "%s", war[exercito].cor);
}

void exibirMissao(struct Mission *missao){
    printf("--- SUA MISSAO (Exercito %s) ---\n", missao[0].tropa);
    printf("%s\n", missao[0].missao);
}

int verificarMissao(char* missoes[], struct Territorio* war, int *totalVitorias) {
    int tipo = -1;

    if (strstr(missao[0].missao, "Conquistar 1 territorio"))
        tipo = 0;
    else if (strstr(missao[0].missao, "Eliminar todas a tropas"))
        tipo = 1;
    else if (strstr(missao[0].missao, "Não perder durante 4 batalhas"))
        tipo = 2;

    switch (tipo) {
        case 0: {
            int conta = 0;
            for (int c = 0; c < MAX_TERRITORIOS; c++) {
                if (strcmp(missao[0].tropa, war[c].cor) == 0)
                    conta++;
            }
            return conta > 1; // conquistou mais de 1 território
        }

        case 1: {
            // pegar a última palavra (cor alvo)
            char *ultima = strrchr(missao[0].missao, ' ');
            if (!ultima) return 0;
            ultima++; // pular o espaço

            // remover \n ou \r no final (caso exista)
            ultima[strcspn(ultima, "\r\n")] = '\0';

            int existe = 0;
            for (int c = 0; c < MAX_TERRITORIOS; c++) {
                if (strcmp(ultima, war[c].cor) == 0) {
                    existe = 1;
                    break;
                }
            }

            return !existe; // missão cumprida se o exército alvo foi eliminado
        }

        case 2:
            return *totalVitorias >= 4;

        default:
            return 0;
    }
}


// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(){
    int atacante;
    int defensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    int go = 0;
    do {
        printf("Escolha o territorio atacante (1 a 5)\n");
        scanf("%d", &atacante);
        limparBufferEntrada();
        if (atacante < 1 || atacante > 6){
            printf("Erro! Digite uma opção válida!\n");
            go = 1;
        } else {
            go = 0;
        }
    } while (go);

    do {
        printf("Escolha o territorio defensor (1 a 5): ");
        scanf("%d", &defensor);
        limparBufferEntrada();
        if (defensor < 1 || defensor > 6){
            printf("Erro! Digite uma opção válida!\n");
            go = 1;
        } else {
            go = 0;
        }

    } while (go);

    simularAtaque(atacante - 1, defensor - 1, &missao[0].totalVitorias);

}


// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(int at, int df, int *totalVitorias){
    
    int dado1 = (rand() % 6) + 1;
    printf("Ataque (%s): %d ", war[at].nome, dado1);

    int dado2 = (rand() % 6) + 1;
    printf("| Defesa (%s): %d\n", war[df].nome, dado2);

    if (dado1 > dado2) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        war[df].tropas--;
        (*totalVitorias)++;

        if (war[df].tropas == 0) {
            printf("CONQUISTA! O território %s foi dominado pelo Exército %s\n", war[df].nome, war[at].cor);
            strcpy(war[df].cor, war[at].cor);
            war[df].tropas++;
            if (war[at].tropas > 1){
                war[at].tropas--;
            }           
        }

    }

    else if (dado1 < dado2) {
        printf("ATAQUE NEUTRALIZADO! O defensor venceu essa batalha!");
        war[at].tropas--;
        *totalVitorias = 0;
    }

    else {
        printf("EMPATE! Os exercitos não se confrontaram!");
    }
    pausa();
}

// Funções de setup e gerenciamento de memória:
void limparBufferEntrada(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// Funções de interface com o usuário:
void pausa() {
    printf("\nPressione ENTER para continuar...\n\n");
    getchar();
}

// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(struct Territorio *war, struct Mission *missao) {
    free(war);
    free(missao);
}

