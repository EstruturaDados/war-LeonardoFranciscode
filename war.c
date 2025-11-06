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
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

struct Territorio *war;
// --- Protótipos das Funções ---
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha. 

void limparBufferEntrada();
void simularAtaque(int at, int df);
void pausa();
void simularAtaque(int at, int df);



void alocarMapa(){
    war = (struct Territorio*) calloc (MAX_TERRITORIOS, sizeof(struct Territorio));
    //Verifica se a alocação foi bem-sucedida.
    if (war == NULL) {
        printf("Erro: Falha ao alocar memória");
    };
}
    
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(){
    printf("=============================================\n");
    printf("        MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("=============================================\n");

    for (int i = 0; i < MAX_TERRITORIOS; i++){
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas);
    }
    printf("\n");
}

// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(){
    int atacante;
    int defensor;
    do {
        exibirMapa();
        printf("\n--- FASE DE ATAQUE ---\n");
        int go = 0;
        do {
            printf("Escolha o territorio atacante (1 a 5, ou 0 para sair): ");
            scanf("%d", &atacante);
            limparBufferEntrada();
            if (atacante == 0){
                return;
            }
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

        simularAtaque(atacante - 1, defensor - 1);
    } while (atacante != 0);
}


// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(int at, int df){
    int dado1 = (rand() % 6) + 1;
    printf("O atacante %s rolou o dado e tirou: %d\n", war[at].nome, dado1);

    int dado2 = (rand() % 6) + 1;
    printf("O defensor %s rolou o dado e tirou: %d\n", war[df].nome, dado2);

    if (dado1 > dado2) {
        printf("VITORIA DO ATAQUE! O defensor perdeu uma tropa.\n");
        war[df].tropas--;

        if (war[df].tropas == 0){
            printf("CONQUISTA! O territorio %s foi dominado pelo Exercito %s\n", war[df].nome, war[at].cor);
            strcpy(war[df].cor, war[at].cor);
            war[df].tropas++;
            war[at].tropas--;
        } 
    }

    else if (dado1 < dado2) {
        printf("ATAQUE NEUTRALIZADO! O defensor venceu essa batalha!");
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

// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    
    alocarMapa();

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

    } while (totalTropas < 5);

    printf("\nCadastro inicial concluído com sucesso!\n\n");

    pausa();

    faseDeAtaque();

    // - Define a cor do jogador e sorteia sua missão secreta.
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // exibirMenuPrincipal():
    // Imprime na tela o menu de ações disponíveis para o jogador.
    
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    free(war);
    printf("Jogo encerrado e memoria liberada. Até a próxima!\n");

    return 0;
}

// --- Implementação das Funções ---



// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.




// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.





// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.


