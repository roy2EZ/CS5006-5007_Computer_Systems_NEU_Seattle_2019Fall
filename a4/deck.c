#include <stdio.h>
#include <stdlib.h>

#include "a4.h"

#define PRINT_DEBUG 1

// Put your deck functions in here

Deck* createDeck() {
  //Allocate space on the heap
  Deck* newDeck = (Deck*)malloc(sizeof(Deck));
  //Initialize
  newDeck->topCard = -1;
  //Return pointer to new deck
  return newDeck;
}

void destroyDeck(Deck* deck) {
  free(deck);
}

Deck* pushCardToDeck(Card* newCard, Deck* cardDeck) {
  //Update the head index
  cardDeck->topCard++;

  //Put the newCard into the cardDeck
  cardDeck->cards[cardDeck->topCard] = newCard;

  return cardDeck;
}


