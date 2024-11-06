#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Fruta;

typedef struct No {
    Fruta *fruta;
    struct No *prox;
} No;

No *lista = NULL;

// Função para verificar se um código de fruta já existe
int codigoExiste(int codigo) {
    No *atual = lista;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

// Função para cadastrar uma nova fruta
void cadastrarFruta() {
    Fruta *novaFruta = (Fruta *)malloc(sizeof(Fruta));
    printf("Digite o código da fruta: ");
    scanf("%d", &novaFruta->codigo);
    if (codigoExiste(novaFruta->codigo)) {
        printf("Código já existe. Cadastro não permitido.\n");
        free(novaFruta);
        return;
    }
    printf("Digite o nome da fruta: ");
    scanf("%s", novaFruta->nome);
    printf("Digite a quantidade em estoque: ");
    scanf("%d", &novaFruta->quantidade);
    printf("Digite o preço da fruta: ");
    scanf("%f", &novaFruta->preco);

    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->fruta = novaFruta;
    novoNo->prox = lista;
    lista = novoNo;
    printf("Fruta cadastrada com sucesso!\n");
}

// Função para listar todas as frutas
void listarFrutas() {
    No *atual = lista;
    printf("\nFrutas cadastradas:\n");
    while (atual != NULL) {
        printf("Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f\n",
               atual->fruta->codigo, atual->fruta->nome, atual->fruta->quantidade, atual->fruta->preco);
        atual = atual->prox;
    }
}

// Função para buscar uma fruta pelo código
Fruta *buscarFruta(int codigo) {
    No *atual = lista;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) {
            return atual->fruta;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Função para alterar dados de uma fruta
void alterarFruta() {
    int codigo;
    printf("Digite o código da fruta a ser alterada: ");
    scanf("%d", &codigo);
    Fruta *fruta = buscarFruta(codigo);
    if (fruta == NULL) {
        printf("Fruta não encontrada.\n");
        return;
    }
    printf("Digite o novo nome da fruta: ");
    scanf("%s", fruta->nome);
    printf("Digite a nova quantidade: ");
    scanf("%d", &fruta->quantidade);
    printf("Digite o novo preço: ");
    scanf("%f", &fruta->preco);
    printf("Fruta alterada com sucesso!\n");
}

// Função para excluir uma fruta
void excluirFruta() {
    int codigo;
    printf("Digite o código da fruta a ser excluída: ");
    scanf("%d", &codigo);
    No *anterior = NULL;
    No *atual = lista;
    while (atual != NULL && atual->fruta->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) {
        printf("Fruta não encontrada.\n");
        return;
    }
    if (atual->fruta->quantidade > 0) {
        printf("Não é possível excluir uma fruta que ainda tem estoque.\n");
        return;
    }
    if (anterior == NULL) {
        lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    free(atual->fruta);
    free(atual);
    printf("Fruta excluída com sucesso!\n");
}

// Função para registrar a venda no arquivo
void registrarVenda(int codigo, int quantidade) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }
    fprintf(arquivo, "Código: %d, Quantidade vendida: %d\n", codigo, quantidade);
    fclose(arquivo);
}

// Função para vender uma fruta
void venderFruta() {
    int codigo, quantidade;
    printf("Digite o código da fruta a ser vendida: ");
    scanf("%d", &codigo);
    Fruta *fruta = buscarFruta(codigo);
    if (fruta == NULL) {
        printf("Fruta não encontrada.\n");
        return;
    }
    printf("Digite a quantidade a ser vendida: ");
    scanf("%d", &quantidade);
    if (quantidade > fruta->quantidade) {
        printf("Quantidade insuficiente em estoque.\n");
        return;
    }
    fruta->quantidade -= quantidade;
    registrarVenda(codigo, quantidade);
    printf("Venda realizada com sucesso!\n");
}

// Função para liberar a memória alocada
void liberarMemoria() {
    No *atual = lista;
    while (atual != NULL) {
        No *prox = atual->prox;
        free(atual->fruta);
        free(atual);
        atual = prox;
    }
}

// Função principal
int main() {
    int opcao;
    do {
        printf("\n--- Mercado de Frutas ---\n");
        printf("1. Cadastrar fruta\n");
        printf("2. Listar frutas\n");
        printf("3. Buscar fruta\n");
        printf("4. Alterar fruta\n");
        printf("5. Excluir fruta\n");
        printf("6. Vender fruta\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                cadastrarFruta();
                break;
            case 2:
                listarFrutas();
                break;
            case 3: {
                int codigo;
                printf("Digite o código da fruta para busca: ");
                scanf("%d", &codigo);
                Fruta *fruta = buscarFruta(codigo);
                if (fruta != NULL) {
                    printf("Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f\n",
                           fruta->codigo, fruta->nome, fruta->quantidade, fruta->preco);
                } else {
                    printf("Fruta não encontrada.\n");
                }
                break;
            }
            case 4:
                alterarFruta();
                break;
            case 5:
                excluirFruta();
                break;
            case 6:
                venderFruta();
                break;
            case 7:
                printf("Saindo...\n");
                liberarMemoria();
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 7);
    return 0;
}