# Mãos de Pôquer

## Estruturas Dinâmicas

O programa utiliza <b>Listas Encadeadas</b> para representar o <b>baralho de cartas</b> e as <b>mãos dos jogadores</b>. 

Cada carta é um nó em uma lista encadeada, `CardNode`, que contém um valor e um naipe. 

```c
typedef struct CardNode {
    Card card;
    struct CardNode *next;
} CardNode;
```

As mãos dos jogadores são listas encadeadas, `PokerHand`, que contêm nós de cartas e são avaliadas para encontrar a pontuação da mão.

```c
typedef struct {
    CardNode *head;
    int size;
} PokerHand;
```

### Bônus: Também sou sua aluna em Pesquisa e Ordenação de Dados!

As mãos dos jogadores são ordenadas antes da avaliação para facilitar a verificação das combinações de pôquer.
A ordenação é realizada usando o algoritmo `Bubble Sort`, que é simples e eficaz para pequenos conjuntos de dados como uma mão de pôquer.

```c
void sortHand(PokerHand *hand) {
    bool swapped;
    do {
        swapped = false;
        CardNode *current = hand->head;
        while (current != NULL && current->next != NULL) {
            if (current->card.value > current->next->card.value) {
                Card temp = current->card;
                current->card = current->next->card;
                current->next->card = temp;
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}
```

## Estratégia de Jogo

O código funciona da seguinte maneira:

### 1. Gera-se o baralho

A criação inicial do baralho envolve a criação de nós individuais para cada carta e a montagem desses nós em uma lista encadeada.
As principais funções envolvidas nesse processo são:

1. `createNode`: Cria-se um novo nó, que representa uma carta no baralho. Ela aloca memória para um novo `CardNode` e inicializa o nó com a carta fornecida e o define para apontar para NULL, indicando que não há outra carta a seguir. A função retorna o endereço do novo nó criado.

2. `appendCard`: Após a criação de um novo nó com a função `createNode`, esta função é chamada para adicionar o nó ao final do baralho. Se o baralho estiver vazio (ou seja, se o ponteiro para a cabeça do baralho for NULL), o novo nó se torna o primeiro nó do baralho. Se já houver cartas no baralho, ela percorre a lista até o último nó e adiciona o novo nó depois dele. A função também incrementa o contador de cartas no baralho.

### 2. Embaralha-se o baralho

Essas funções, trabalhando em conjunto, permitem que o baralho seja embaralhado de forma eficiente e aleatória, preparando as cartas para serem distribuídas para os jogadores de uma maneira que simula o embaralhamento real de um baralho de cartas em um jogo de pôquer.
As principais funções envolvidas nesse processo são:

1. `countCards`: Antes de embaralhar o baralho gerado pelo `appendCards`, é necessário saber o número total de cartas no baralho. Portanto, esta função percorre o baralho começando pela cabeça, contando cada nó até chegar ao final da lista (quando o ponteiro next é NULL). O total de cartas é retornado e usado como argumento para a função `shuffleDeck`.

2. `shuffleDeck`: Esta função embaralha as cartas do baralho. Primeiramente, verifica se o baralho tem mais de uma carta (não pode embaralhar um único elemento). A função então cria um array temporário de ponteiros para `CardNodes`, copiando os endereços dos nós da lista encadeada para esse array. Utiliza o algoritmo de `Fisher-Yates` para embaralhar este array. Após a conclusão do embaralhamento, os ponteiros next dos `CardNodes` são atualizados para refletir a nova ordem embaralhada e o ponteiro para a cabeça do baralho é atualizado para apontar para o novo primeiro nó.

### 3. Distribui-se as cartas às mãos (5 sequências, 5 vezes)

Essas funções trabalham em conjunto para gerenciar a distribuição das cartas do baralho para as mãos dos jogadores.
As principais funções envolvidas nesse processo são:

1. `dealHands`: Esta é a função principal para distribuir as cartas do baralho para as várias mãos no jogo. Ela executa os seguintes passos:

- Inicia um loop que irá percorrer o número de mãos a serem distribuídas (`numHands`).
- Dentro desse loop, outro loop é iniciado para adicionar cartas a cada mão (`HAND_SIZE` vezes, que é o número de cartas por mão).
- Para cada iteração do loop, a função `appendCard` é chamada para adicionar a carta atual à mão correspondente.
- Após uma carta ser adicionada a uma mão, o nó representando essa carta é removido do baralho (a cabeça do baralho é atualizada para o próximo nó), e a memória alocada para o nó removido é liberada.

2. `free`: Embora não explicitamente chamada na função `dealHands`, a função free é usada para liberar a memória do nó que foi removido do baralho após a carta ser adicionada a uma mão.

### 4. Determina-se a pontuação de cada mão

As mãos são avaliadas com base nas combinações clássicas do pôquer, como Royal Straight Flush, Straight Flush, Four of a Kind, Full House, Flush, Straight, Three of a Kind, Two Pairs, One Pair e a carta mais alta.
As principais funções envolvidas nesse processo são:

1. `evaluateHand`: Esta é a função principal que determina a pontuação de uma mão específica. Ela chama outras funções que verificam as diferentes combinações de pôquer e retorna um valor numérico correspondente à combinação mais forte encontrada na mão.