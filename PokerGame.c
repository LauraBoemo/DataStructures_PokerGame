#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define HAND_SIZE 5
#define NUM_HANDS 5

typedef struct {
    int value;
    char suit;
} Card;

typedef struct CardNode {
    Card card;
    struct CardNode *next;
} CardNode;

typedef struct {
    CardNode *head;
    int size;
} PokerHand;

CardNode* createNode(Card card);
void appendCard(PokerHand *hand, Card card);
void shuffleDeck(CardNode **head, int deckSize);
void dealHands(CardNode **deck, PokerHand *hands, int numHands);
void sortHand(PokerHand *hand);
void printHand(PokerHand hand);
void freeHand(PokerHand *hand);

int evaluateHand(PokerHand hand);
bool isRoyalStraightFlush(PokerHand hand);
bool isStraightFlush(PokerHand hand);
bool isFourOfAKind(PokerHand hand);
bool isFullHouse(PokerHand hand);
bool isFlush(PokerHand hand);
bool isStraight(PokerHand hand);
bool isThreeOfAKind(PokerHand hand);
bool isTwoPairs(PokerHand hand);
bool isPair(PokerHand hand);
int getHighestCard(PokerHand hand);

int countCards(CardNode *head); 
void freeDeck(CardNode **head); 


// Esta função cria um novo nó (CardNode) para uma lista encadeada de cartas. 
// Primeiro, ela aloca memória para o novo nó. Se a alocação de memória falhar, ela exibe uma mensagem de erro e encerra o programa. 
// O novo nó é inicializado com a carta fornecida (card) e o próximo nó é definido como NULL. Por fim, a função retorna o novo nó.
CardNode* createNode(Card card) {
    CardNode *newNode = (CardNode *)malloc(sizeof(CardNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->card = card;
    newNode->next = NULL;
    return newNode;
}

// Esta função adiciona uma nova carta ao final de uma mão de pôquer (PokerHand). 
// Ela usa a função createNode para criar um novo nó para a carta. 
// Se a mão está vazia (ou seja, hand->head é NULL), a nova carta é adicionada como o primeiro nó. 
// Se a mão já contém cartas, a função itera através dos nós existentes até encontrar o último e então adiciona a nova carta após este. 
// A contagem de cartas na mão (hand->size) é incrementada.
void appendCard(PokerHand *hand, Card card) {
    CardNode *newNode = createNode(card);
    if (hand->head == NULL) {
        hand->head = newNode;
    } else {
        CardNode *current = hand->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    hand->size++;
}

// Esta função embaralha o baralho de cartas. Ela retorna imediatamente se o tamanho do baralho é menor que 2, pois não há o que embaralhar. 
// A função cria um array temporário de ponteiros para os nós do baralho e então usa o algoritmo de embaralhamento Fisher-Yates. 
// Para cada posição no array, a função troca o nó na posição atual com um nó em uma posição aleatória no array. 
// Após embaralhar, a função reconstrói a lista encadeada de cartas e libera a memória alocada para o array.
void shuffleDeck(CardNode **head, int deckSize) {
    if (deckSize < 2) {
        return;
    }

    CardNode **array = (CardNode **)malloc(deckSize * sizeof(CardNode *));
    if (array == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    CardNode *current = *head;
    for (int i = 0; i < deckSize && current != NULL; i++) {
        array[i] = current;
        current = current->next;
    }

    srand((unsigned int)time(NULL));
    for (int i = deckSize - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        CardNode *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    for (int i = 0; i < deckSize - 1; i++) {
        array[i]->next = array[i + 1];
    }
    array[deckSize - 1]->next = NULL;
    *head = array[0];

    free(array);
}

// Esta função distribui as cartas para um número especificado de mãos (numHands). 
// Para cada mão, a função inicializa a mão como vazia e então anexa cartas do baralho a ela usando a função appendCard. 
// Após adicionar uma carta a uma mão, a função remove essa carta do baralho (*deck) e libera a memória do nó correspondente. 
// Este processo se repete até que todas as mãos tenham o número de cartas definido por HAND_SIZE.
void dealHands(CardNode **deck, PokerHand *hands, int numHands) {
    for (int i = 0; i < numHands; i++) {
        hands[i].head = NULL; 
        hands[i].size = 0;   
        for (int j = 0; j < HAND_SIZE; j++) {
            if (*deck != NULL) {
                appendCard(&hands[i], (*deck)->card);
                CardNode *temp = *deck;
                *deck = (*deck)->next; 
                free(temp);
            }
        }
    }
}

// Esta função classifica uma mão de pôquer (PokerHand) em ordem crescente com base nos valores das cartas. 
// Ela utiliza um algoritmo de classificação conhecido como "bubble sort" (ordenação por bolha), que é adequado para pequenos conjuntos de dados. 
// A função começa com a variável swapped definida como false, o que indica que nenhuma troca ocorreu inicialmente.
// Em seguida, entra em um loop do-while que continua enquanto pelo menos uma troca foi feita durante a iteração atual. 
// Dentro do loop, a função itera pela lista encadeada de cartas e compara o valor da carta atual com o valor da próxima carta. 
// Se a carta atual tiver um valor maior do que a próxima carta, as duas cartas são trocadas. 
// Isso é repetido até que todas as cartas estejam em ordem crescente.
//  A variável swapped é usada para rastrear se alguma troca ocorreu durante a iteração atual, e o loop continua até que nenhuma troca seja feita.
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

// Esta função imprime as cartas em uma mão de pôquer (PokerHand). 
// Ela começa com um ponteiro para o nó da cabeça da mão e itera pela lista encadeada de cartas. 
// Para cada carta, a função imprime o valor da carta (current->card.value) seguido pelo naipe da carta (current->card.suit). 
// Isso é feito para todas as cartas na mão, resultando na impressão de todas as cartas em uma única linha. 
// Após imprimir todas as cartas, a função insere uma quebra de linha (\n) para separar as mãos quando várias são impressas.
void printHand(PokerHand hand) {
    CardNode *current = hand.head;
    while (current != NULL) {
        printf("%d%c ", current->card.value, current->card.suit);
        current = current->next;
    }
    printf("\n");
}

// Esta função libera a memória alocada para os nós que representam as cartas em uma mão de pôquer (PokerHand). 
// Ela começa com um ponteiro para o nó da cabeça da mão e itera pela lista encadeada de cartas. 
// Para cada nó, a função libera a memória alocada para o nó atual e avança para o próximo nó. 
// Isso é feito para todos os nós na lista, garantindo que toda a memória alocada para as cartas da mão seja liberada. 
// Após liberar a memória, a função define o ponteiro da cabeça da mão como NULL e redefine o tamanho da mão (hand->size) como 0, indicando que a mão está vazia.
void freeHand(PokerHand *hand) {
    CardNode *current = hand->head;
    while (current != NULL) {
        CardNode *next = current->next;
        free(current);
        current = next;
    }
    hand->head = NULL;
    hand->size = 0;
}

// Esta função avalia a pontuação de uma mão de pôquer (PokerHand) com base em sua classificação. 
// Ela começa com uma série de verificações condicionais que determinam a classificação da mão, da mais alta (Royal Straight Flush) para a mais baixa (High Card). 
// Se a mão for uma Royal Straight Flush, ela retorna 1000. Se for um Straight Flush, retorna 750. Se for Four of a Kind, retorna 500, e assim por diante. 
// Se a mão não se encaixar em nenhuma dessas categorias, a função retorna a pontuação com base na carta mais alta na mão, obtida pela função getHighestCard.
int evaluateHand(PokerHand hand) {
    if (isRoyalStraightFlush(hand)) return 1000;
    if (isStraightFlush(hand)) return 750;
    if (isFourOfAKind(hand)) return 500;
    if (isFullHouse(hand)) return 300;
    if (isFlush(hand)) return 200;
    if (isStraight(hand)) return 150;
    if (isThreeOfAKind(hand)) return 100;
    if (isTwoPairs(hand)) return 50;
    if (isPair(hand)) return 25;
    return getHighestCard(hand);
}

// Esta função verifica se uma mão de pôquer (PokerHand) é um Royal Straight Flush. 
// Um Royal Straight Flush é a mão mais alta no pôquer, consistindo em um Straight Flush com as cartas de 10 a Ás (10, J, Q, K, A) do mesmo naipe.
// A função verifica se a mão é um Straight Flush (todas as cartas do mesmo naipe em ordem consecutiva) e se a carta mais alta é um 10.
bool isRoyalStraightFlush(PokerHand hand) {
    return isStraightFlush(hand) && hand.head->card.value == 10;
}

// Esta função verifica se uma mão de pôquer (PokerHand) é um Straight Flush.
// Um Straight Flush é uma sequência de cinco cartas do mesmo naipe em ordem consecutiva. 
// A função verifica se a mão é um Flush (todas as cartas do mesmo naipe) e se é uma Straight (todas as cartas em ordem consecutiva).
bool isStraightFlush(PokerHand hand) {
    return isFlush(hand) && isStraight(hand);
}

// Esta função verifica se uma mão de pôquer (PokerHand) contém Quatro de um Tipo (Four of a Kind). 
// Isso ocorre quando quatro cartas na mão têm o mesmo valor. 
// A função verifica se o primeiro valor de carta é igual ao valor da terceira carta ou se o último valor de carta é igual ao valor da segunda carta.
bool isFourOfAKind(PokerHand hand) {
    int firstCardValue = hand.head->card.value;
    int lastCardValue = hand.head->next->next->next->next->card.value;
    return (firstCardValue == hand.head->next->next->next->card.value) ||
           (lastCardValue == hand.head->next->card.value);
}

// Esta função verifica se uma mão de pôquer (PokerHand) é uma Full House. 
// Um Full House ocorre quando há três cartas de um tipo (Three of a Kind) e duas cartas de outro tipo (One Pair). 
// A função verifica várias combinações para determinar se a mão se encaixa em qualquer uma dessas combinações de Full House e retorna true se for o caso.
bool isFullHouse(PokerHand hand) {
    bool threeFirst = hand.head->card.value == hand.head->next->next->card.value;
    bool twoLast = hand.head->next->next->next->card.value == hand.head->next->next->next->next->card.value;
    bool twoFirst = hand.head->card.value == hand.head->next->card.value;
    bool threeLast = hand.head->next->next->card.value == hand.head->next->next->next->next->card.value;

    return (threeFirst && twoLast) || (twoFirst && threeLast);
}

// Esta função verifica se uma mão de pôquer (PokerHand) é um Flush. 
// Um Flush ocorre quando todas as cartas da mão têm o mesmo naipe. 
// A função começa obtendo o naipe da primeira carta na mão e, em seguida, verifica se todas as cartas subsequentes têm o mesmo naipe. 
// Se todas as cartas tiverem o mesmo naipe, a função retorna true; caso contrário, retorna false.
bool isFlush(PokerHand hand) {
    char suit = hand.head->card.suit;
    for (CardNode* node = hand.head->next; node != NULL; node = node->next) {
        if (node->card.suit != suit) return false;
    }
    return true;
}

// Esta função verifica se uma mão de pôquer (PokerHand) é uma Straight. 
// Uma Straight ocorre quando todas as cartas da mão estão em ordem consecutiva. 
// A função começa obtendo o valor da primeira carta na mão e, em seguida, verifica se todas as cartas subsequentes têm valores consecutivos, aumentando de 1. 
// A função também lida com o caso especial onde a mão pode ser A, 2, 3, 4, 5 (conhecida como "Straight de Roda"). 
// Se a mão se encaixar em qualquer uma dessas condições, a função retorna true; caso contrário, retorna false.
bool isStraight(PokerHand hand) {
    CardNode* node = hand.head;
    int value = node->card.value;
    node = node->next;
    while (node != NULL) {
        if (node->card.value != ++value) {
            if (hand.head->card.value == 2 && value == 5 && node->card.value == 14) { 
                return true;
            }
            return false;
        }
        node = node->next;
    }
    return true;
}

// Esta função verifica se uma mão de pôquer (PokerHand) contém Três de um Tipo (Three of a Kind). 
// Isso ocorre quando três cartas na mão têm o mesmo valor. 
// A função verifica se há três cartas consecutivas na mão com o mesmo valor e retorna true se for o caso.
bool isThreeOfAKind(PokerHand hand) {
    CardNode *node = hand.head;
    while (node->next != NULL && node->next->next != NULL) {
        if (node->card.value == node->next->card.value &&
            node->card.value == node->next->next->card.value) {
            return true;
        }
        node = node->next;
    }
    return false;
}

// Esta função verifica se uma mão de pôquer (PokerHand) contém Dois Pares (Two Pairs). 
// Isso ocorre quando há duas combinações de duas cartas com o mesmo valor na mão. 
// A função conta o número de pares encontrados na mão e retorna true se houver dois pares; caso contrário, retorna false.
bool isTwoPairs(PokerHand hand) {
    CardNode *node = hand.head;
    int pairs = 0;
    while (node != NULL && node->next != NULL) {
        if (node->card.value == node->next->card.value) {
            pairs++;
            node = node->next;
        }
        if (pairs == 2) return true;
        node = node->next;
    }
    return false;
}

// Esta função verifica se uma mão de pôquer (PokerHand) contém Um Par (One Pair). 
// Isso ocorre quando há duas cartas na mão com o mesmo valor.
//  A função verifica se há pelo menos uma combinação de duas cartas com o mesmo valor na mão e retorna true se for o caso; caso contrário, retorna false.
bool isPair(PokerHand hand) {
    CardNode *node = hand.head;
    while (node != NULL && node->next != NULL) {
        if (node->card.value == node->next->card.value) {
            return true;
        }
        node = node->next;
    }
    return false;
}

// Esta função recebe uma mão de pôquer (PokerHand) como entrada e retorna o valor da carta mais alta presente na mão. 
// Ela percorre a lista de cartas na mão até encontrar a última carta (a mais alta) e retorna o valor dessa carta.
int getHighestCard(PokerHand hand) {
    CardNode *node = hand.head;
    while (node->next != NULL) {
        node = node->next;
    }
    return node->card.value;
}

// Esta função recebe um ponteiro para o início de uma lista ligada de cartas (head) como entrada e retorna o número de cartas presentes na lista. 
// Ela percorre a lista contando cada carta ao iterar através dos nós da lista e incrementando um contador. 
// O valor do contador é então retornado como o número total de cartas na lista.
int countCards(CardNode *head) {
    int count = 0;
    CardNode *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Esta função é responsável por liberar a memória alocada para uma lista ligada de cartas, incluindo todas as cartas presentes na lista. 
// Ela recebe um ponteiro para o ponteiro da cabeça da lista (**head) como entrada. 
// A função percorre a lista, liberando a memória de cada nó (cada carta) à medida que avança. 
// Após a conclusão da função, a cabeça da lista é definida como NULL para indicar que a lista está vazia e a memória foi completamente liberada.
void freeDeck(CardNode **head) {
    CardNode *current = *head;
    while (current != NULL) {
        CardNode *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

int main() {
    srand((unsigned int)time(NULL)); // Inicializa o gerador de números aleatórios com a hora atual como semente.

    CardNode *deck = NULL; // Cria um ponteiro para o topo do baralho, inicialmente nulo (vazio).
    int totalScore = 0; // Inicializa a variável que irá acumular a pontuação total das mãos.

    // Loop para preencher o baralho com cartas. Existem 4 naipes (suit) e 13 valores (value) para cada naipe.
    for (int suit = 0; suit < 4; suit++) { // Itera sobre os 4 naipes (0 a 3).
        for (int value = 1; value <= 13; value++) { // Itera sobre os valores das cartas (1 a 13).
            Card card; // Cria uma variável temporária para armazenar uma carta.
            card.value = value; // Atribui o valor atual do loop ao valor da carta.
            card.suit = "HDCS"[suit]; // Atribui o naipe correspondente à carta (Hearts, Diamonds, Clubs, Spades -- Corações, Diamantes, Copas, Espadas).
            appendCard(&deck, card); // Adiciona a carta criada ao topo do baralho.
        }
    }

    shuffleDeck(&deck, countCards(deck)); // Embaralha o baralho usando a função shuffleDeck.

    PokerHand hands[NUM_HANDS] = {0}; // Inicializa um array de mãos de pôquer com NUM_HANDS elementos.

    dealHands(&deck, hands, NUM_HANDS); // Distribui as cartas do baralho entre as mãos.

    // Loop para processar cada mão de pôquer.
    for (int i = 0; i < NUM_HANDS; i++) {
        sortHand(&hands[i]); // Ordena as cartas da mão atual.
        printf("Mão %d\n", i + 1); // Imprime o número da mão atual.
        printHand(hands[i]); // Imprime as cartas da mão atual.
        int handScore = evaluateHand(hands[i]); // Avalia a pontuação da mão atual.
        printf("Pontuação - %d\n\n", handScore); // Imprime a pontuação da mão atual.
        totalScore += handScore; // Adiciona a pontuação da mão ao total.
        freeHand(&hands[i]); // Libera a memória usada pela mão atual.
    }

    // Imprime a pontuação total após processar todas as mãos.
    printf("Total de pontos acumulados: %d\n", totalScore);

    freeDeck(&deck); // Libera a memória usada pelo baralho.

    return 0; // Retorna 0 indicando que o programa foi executado com sucesso.
}
