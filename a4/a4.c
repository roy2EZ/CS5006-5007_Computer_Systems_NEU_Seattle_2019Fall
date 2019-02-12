#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "a4.h"

#define PRINT_DEBUG 1
// Name: Rongyi Chen
// Date: FEB 11, 2019

// Implement the Hand and other functions in here

//----------------------------------------
// Hand functions
//----------------------------------------
Hand* createHand() {
  Hand* newHand = (Hand*)malloc(sizeof(Hand));
  newHand->num_cards_in_hand = 0;
  newHand->firstCard = NULL;
  return newHand;
}

// Destroys the hand, freeing any memory allocated for it.
void destroyHand(Hand* thisHand) {
  if (!isHandEmpty(thisHand)) {
    for (int i = 0; i < thisHand->num_cards_in_hand; i++) {
      destroyCard(thisHand->firstCard->thisCard);
      thisHand->firstCard = thisHand->firstCard->nextCard;
    }
  }
  free(thisHand);
}

void addCardToHand(Card* card, Hand* thisHand) {
  CardNode* thisCardNode = (CardNode*)malloc(sizeof(CardNode));
  thisCardNode->thisCard = card;
  thisCardNode->prevCard = NULL;
  thisCardNode->nextCard = thisHand->firstCard;
  if (thisHand->firstCard != NULL) {
    thisHand->firstCard->prevCard = thisCardNode;
  }
  thisHand->firstCard = thisCardNode;
  thisHand->num_cards_in_hand++;
}
// Removes a card from the hand. Return a pointer to the card
// that's been removed from the hand.
// Consider if need to remove the CardNode from the heap.
Card* removeCardFromHand(Card *aCard, Hand *aHand) {
  CardNode* tempNode = aHand->firstCard;
  Card* topCard = aHand->firstCard->thisCard;
  // to search the nodes in hand using while loop
  // while the firstCard in the hand is not NULL,
  // which means there are cards in hand
  while (tempNode != NULL) {
    Card* tempCard = tempNode->thisCard;
    // if tempCard is as same as the to-remove card aCard
    if (tempCard->name == aCard->name && tempCard->suit == aCard->suit) {
      if (tempCard->name == topCard->name && tempCard->suit == topCard->suit) {
        aHand->firstCard = aHand->firstCard->nextCard;
      }
      aHand->num_cards_in_hand--;
      if (tempNode->nextCard != NULL) {
        tempNode->nextCard->prevCard = tempNode->prevCard;
      }
      if (tempNode->prevCard != NULL) {
        tempNode->prevCard->nextCard = tempNode->nextCard;
      }
      tempNode->nextCard = NULL;
      tempNode->prevCard = NULL;
      free(tempNode);
      return tempCard;
    }
    tempNode = tempNode->nextCard;
  // out of the while loop, which means no cardnode in hand
  // then nothing need to be removed, return NULL.
  }
  return NULL;
}

// Determines if there are any cards in the hand.
// Return 1 if the hand is empty; 0 otherwise.
int isHandEmpty(Hand* thisHand) {
  if (thisHand->num_cards_in_hand == 0) {
    return 1;
  }
  return 0;
}

// Given a lead card, a players hand, and the card the player wants
// to play, is it legal?
// If the player has a card of the same suit as the leadCard, they
// must play a card of the same suit.
// If the player does not have a card of the same suit, they can
// play any card.
int isLegalMove(Hand *hand, Card *leadCard, Card *playedCard) {
  Suit leadSuit = leadCard->suit;
  if (playedCard->suit == leadSuit) {
    return 1;
  }
  CardNode* tempNode = hand->firstCard;
  while (tempNode != NULL) {
    if (tempNode->thisCard->suit == leadSuit) {
      return 0;
    }
    tempNode = tempNode->nextCard;
  }
  return 1;
}

// Given two cards that are played in a hand, which one wins?
// If the suits are the same, the higher card value wins.
// If the suits are not the same, player 1 wins, unless player 2 played trump.
// Returns 1 if the person who led won, 0 if the person who followed won.
int whoWon(Card *leadCard, Card *followedCard, Suit trump) {
  if (leadCard->suit == followedCard->suit) {
    if (leadCard->name > followedCard->name) {
      return 1;
    } else {
      return 0;
    }
  } else {
    if (followedCard->suit != trump) {
      return 1;
    } else {
      return 0;
    }
  }
}

void returnHandToDeck(Hand *aHand, Deck *aDeck) {
  while(!isHandEmpty(aHand)) {
    pushCardToDeck(removeCardFromHand(aHand->firstCard->thisCard, aHand), aDeck);
  }
}
