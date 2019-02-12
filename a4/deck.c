#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "a4.h"

#define PRINT_DEBUG 1
// Name: Rongyi Chen
// Date: FEB 11, 2019

// Put your deck functions in here
//----------------------------------------
// Deck functions
//----------------------------------------
Deck* createDeck() {
  // Allocate space on the heap
  Deck* newDeck = (Deck*)malloc(sizeof(Deck));
  // Initialize
  newDeck->topCard = -1;
  // Return pointer to new deck
  return newDeck;
}

void destroyDeck(Deck* deck) {
  if (isDeckEmpty(deck) == 0) {
    for (int i = 0; i < NUM_CARDS_IN_DECK; i++) {
      free(deck->cards[i]);
    }
  }
  free(deck);
}

Deck* pushCardToDeck(Card* newCard, Deck* cardDeck) {
  if (cardDeck->topCard >= NUM_CARDS_IN_DECK - 1) {
    return NULL;
  } else {
    // Update the head index
    cardDeck->topCard++;
    // Put the newCard into the cardDeck
    cardDeck->cards[cardDeck->topCard] = newCard;
    return cardDeck;
  }
}

Card* peekAtTopCard(Deck* cardDeck) {
  if (isDeckEmpty(cardDeck) == 1) {
    return NULL;
  } else {
    // Get the top card from the cardDeck
    return cardDeck->cards[cardDeck->topCard];
  }
}

Card* popCardFromDeck(Deck* cardDeck) {
  if (isDeckEmpty(cardDeck) == 1) {
    return NULL;
  } else {
    // Get the top card from the cardDeck
    // and remove the card from deck
    // by minus the topCard with 1 which is topCard--
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

//----------------------------------------
// Card functions
//----------------------------------------

Card* createCard(Suit suit, Name name) {
  Card* newCard = (Card*)malloc(sizeof(Card));
  newCard->suit = suit;
  newCard->name = name;
  newCard->value = -1;
  return newCard;
}

void destroyCard(Card* card) {
  free(card);
}

//----------------------------------------
// Game functions
//----------------------------------------

// Create a Deck for this game, and add any
// needed cards to the deck.
// Return a pointer to the deck to be used for the game
Deck* populateDeck() {
  Deck* deck = createDeck();
  int i, j;
  for (i = HEARTS; i <= DIAMONDS; i++) {
    for (j = NINE; j <= ACE; j++) {
      Card *card = createCard(i, j);
      pushCardToDeck(card, deck);
    }
  }
}

// Shuffle the deck.
// Put them in a random order.
// My reference:
// https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
void shuffle(Deck *thisDeck) {
  srand(time(0));
  int seed;
  for (int i = NUM_CARDS_IN_DECK-1; i > 0; i--) {
    int j = rand() % (i + 1);
    Card* tempCard = thisDeck->cards[i];
    thisDeck->cards[i] = thisDeck->cards[j];
    thisDeck->cards[j] = tempCard;
  }
}

// Given a deck (assume that it is already shuffled),
// take the top card from the deck and alternately give
// it to player 1 and player 2, until they both have
// NUM_DECKS_IN_HAND.
void deal(Deck *aDeck, Hand *p1hand, Hand *p2hand) {
  while (p1hand->num_cards_in_hand < 5 || p2hand->num_cards_in_hand < 5) {
    if (p1hand->num_cards_in_hand < 5) {
      addCardToHand(popCardFromDeck(aDeck), p1hand);
    }
    if (p2hand->num_cards_in_hand < 5) {
      addCardToHand(popCardFromDeck(aDeck), p2hand);
    }
  }
}

