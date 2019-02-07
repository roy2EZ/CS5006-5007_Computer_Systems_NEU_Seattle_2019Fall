#include <stdio.h>
#include <stdlib.h>

#include "a4.h"

#define PRINT_DEBUG 1

// Put your deck functions in here

Deck* createDeck() {
  // Allocate space on the heap
  Deck* newDeck = (Deck*)malloc(sizeof(Deck));
  // Initialize
  newDeck->topCard = -1;
  // Return pointer to new deck
  return newDeck;
}

void destroyDeck(Deck* deck) {
  if (!isDeckEmpty(deck)) {
    for (int i = 0; i < NUM_CARDS_IN_DECK; i++) {
      free(deck->cards[i]);
    }
  }
  free(deck);
}

Deck* pushCardToDeck(Card* newCard, Deck* cardDeck) {
  // Update the head index
  cardDeck->topCard++;

  // Put the newCard into the cardDeck
  cardDeck->cards[cardDeck->topCard] = newCard;

  return cardDeck;
}

Card* peekAtTopCard(Deck* cardDeck) {
  // Get the top card from the cardDeck
  return cardDeck->cards[NUM_CARDS_IN_DECK];
}

Card* popCardFromDeck(Deck* cardDeck) {
  if (cardDeck->topCard == -1) {
  return NULL;
  } else {
  return cardDeck->cards[cardDeck->topCard--];
  }
}

int isDeckEmpty(Deck* cardDeck) {
  if (cardDeck->topCard == -1) {
  return 1;
  } else {
  return 0;
  }
}


