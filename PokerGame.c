#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DECK_SIZE 52
#define HAND_SIZE 5
#define NUM_HANDS 5


typedef struct {
    int value;
    char suit;
} Card;

typedef struct {
    Card cards[HAND_SIZE];
} PokerHand;

// Protótipos das funções principais
void createDeck(Card *deck);
void shuffleDeck(Card *deck);
void dealHands(Card *deck, PokerHand *hands);
int evaluateHand(PokerHand hand);
void sortHand(PokerHand *hand);

// Protótipos das funções auxiliares
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

// ==========================
//     FUNÇÕES PRINCIPAIS 
// ==========================

// Cria um baralho de cartas
void createDeck(Card *deck) {
    char suits[] = {'H', 'D', 'C', 'S'}; // Corações, Diamantes, Copas, Espadas
    int i, suit, rank;

    for (i = 0; i < DECK_SIZE; i++) {
        suit = i / 13;
        rank = i % 13 + 1; // Os valores das cartas começam em 1 (Ás) e vão até 13 (Rei)
        deck[i].value = rank;
        deck[i].suit = suits[suit];
    }
}

// Embaralha o baralho de cartas
void shuffleDeck(Card *deck) {
    int i, j;
    Card temp;

    srand(time(NULL)); // Semente para a função rand()

    for (i = DECK_SIZE - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Distribui as cartas em mãos de pôquer
void dealHands(Card *deck, PokerHand *hands) {
    int cardIndex = 0;
    for (int i = 0; i < NUM_HANDS; i++) {
        for (int j = 0; j < HAND_SIZE; j++) {
            hands[i].cards[j] = deck[cardIndex++];
        }
    }
}

// Ordena a mão de cartas por valor
void sortHand(PokerHand *hand) {
    int i, j;
    Card temp;

    for (i = 0; i < HAND_SIZE - 1; i++) {
        for (j = 0; j < HAND_SIZE - i - 1; j++) {
            if (hand->cards[j].value > hand->cards[j + 1].value) {
                temp = hand->cards[j];
                hand->cards[j] = hand->cards[j + 1];
                hand->cards[j + 1] = temp;
            }
        }
    }
}

// Função para imprimir a mão
void printHandAndScore(PokerHand hand, int score) {
    const char *valueStrings[14] = {"", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    for (int i = 0; i < HAND_SIZE; i++) {
        printf("%s%c ", valueStrings[hand.cards[i].value], hand.cards[i].suit);
    }
    printf("- Pontuação: %d\n", score);
}

// Função de avaliação
int evaluateHand(PokerHand hand) {
    if (isRoyalStraightFlush(hand)) {
        return 1000;
    } else if (isStraightFlush(hand)) {
        return 750;
    } else if (isFourOfAKind(hand)) {
        return 500;
    } else if (isFullHouse(hand)) {
        return 300;
    } else if (isFlush(hand)) {
        return 200;
    } else if (isStraight(hand)) {
        return 150;
    } else if (isThreeOfAKind(hand)) {
        return 100;
    } else if (isTwoPairs(hand)) {
        return 50;
    } else if (isPair(hand)) {
        return 25;
    } else {
        // Se não formou nenhum jogo, retorna a carta mais alta
        return getHighestCard(hand);
    }
}

// ==========================
//     FUNÇÕES AUXILIARES
// ==========================

bool isRoyalStraightFlush(PokerHand hand) {
    return isStraightFlush(hand) && hand.cards[0].value == 1 && hand.cards[4].value == 13;
}

bool isStraightFlush(PokerHand hand) {
    return isFlush(hand) && isStraight(hand);
}

bool isFourOfAKind(PokerHand hand) {
    // Verifica se os quatro primeiros ou os quatro últimos cartões são iguais
    return (hand.cards[0].value == hand.cards[3].value ||
            hand.cards[1].value == hand.cards[4].value);
}

bool isFullHouse(PokerHand hand) {
    // Verifica se a mão tem um Three of a Kind e um Par
    return (hand.cards[0].value == hand.cards[2].value && hand.cards[3].value == hand.cards[4].value) ||
           (hand.cards[0].value == hand.cards[1].value && hand.cards[2].value == hand.cards[4].value);
}

bool isFlush(PokerHand hand) {
    // Verifica se todas as cartas são do mesmo naipe
    for (int i = 1; i < HAND_SIZE; i++) {
        if (hand.cards[i].suit != hand.cards[0].suit) {
            return false;
        }
    }
    return true;
}

bool isStraight(PokerHand hand) {
    // Verifica se todas as cartas formam uma sequência
    for (int i = 0; i < HAND_SIZE - 1; i++) {
        if (hand.cards[i].value != hand.cards[i + 1].value - 1) {
            // Trata o caso especial do Ás no início (Ás, 2, 3, 4, 5)
            if (i == 0 && hand.cards[0].value == 1 && hand.cards[1].value == 10) {
                continue;
            }
            return false;
        }
    }
    return true;
}

bool isThreeOfAKind(PokerHand hand) {
    // Verifica se há três cartas de mesmo valor
    for (int i = 0; i < HAND_SIZE - 2; i++) {
        if (hand.cards[i].value == hand.cards[i + 1].value &&
            hand.cards[i].value == hand.cards[i + 2].value) {
            return true;
        }
    }
    return false;
}

bool isTwoPairs(PokerHand hand) {
    // Verifica se há dois pares na mão
    int pairCount = 0;
    for (int i = 0; i < HAND_SIZE - 1; i++) {
        if (hand.cards[i].value == hand.cards[i + 1].value) {
            pairCount++;
            i++; // Pula a próxima carta, pois já faz parte do par
        }
    }
    return pairCount == 2;
}

bool isPair(PokerHand hand) {
    // Verifica se há um par na mão
    for (int i = 0; i < HAND_SIZE - 1; i++) {
        if (hand.cards[i].value == hand.cards[i + 1].value) {
            return true;
        }
    }
    return false;
}

int getHighestCard(PokerHand hand) {
    // Retorna o valor da carta mais alta, tratando o Ás como 14
    int highest = hand.cards[0].value == 1 ? 14 : hand.cards[0].value; // Ás é a carta mais alta
    for (int i = 1; i < HAND_SIZE; i++) {
        if (hand.cards[i].value > highest) {
            highest = hand.cards[i].value;
        }
    }
    return highest;
}

int main() {
    Card deck[DECK_SIZE];
    PokerHand hands[NUM_HANDS];

    int scores[NUM_HANDS];
    int totalScore = 0;

    // Inicializa o baralho e embaralha
    createDeck(deck);
    shuffleDeck(deck);

    // Distribui as cartas para as mãos
    dealHands(deck, hands);

    // Avalia cada mão e calcula a pontuação total
    for (int i = 0; i < NUM_HANDS; i++) {
        sortHand(&hands[i]);
        scores[i] = evaluateHand(hands[i]);
        totalScore += scores[i];
        printHandAndScore(hands[i], scores[i]); // Imprime a mão e a pontuação
    }

    // Imprime a pontuação total
    printf("Total Score: %d\n", totalScore);

    return 0;
}
