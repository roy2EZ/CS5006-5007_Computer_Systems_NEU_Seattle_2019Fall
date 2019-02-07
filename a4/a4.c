#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "a4.h"

#define PRINT_DEBUG 1

// Implement the Hand and other functions in here

Hand* createHand() {
  Hand* newHand = (Hand*)malloc(sizeof(newHand));
  newHand->num_cards_in_hand = 0;
  newHand->firstCard = NULL;
  return newHand;
}

void destroyHand(Hand* thisHand) {
  free(thisHand->firstCard->nextCard);
  free(thisHand->firstCard);
  free(thisHand);
}

void addCardToHand(Card* card, Hand* thisHand) {
  CardNode* thisCardNode = (CardNode*)malloc(sizeof(thisCardNode));
  thisCardNode->thisCard = card;
  thisCardNode->prevCard = NULL;
  thisCardNode->nextCard = thisHand->firstCard;
  if(thisHand->firstCard != NULL) {
    thisHand->firstCard->prevCard = thisCardNode;
  }
  thisHand->firstCard = thisCardNode;
  thisHand->num_cards_in_hand++;
}
// Removes a card from the hand. Return a pointer to the card that's been removed from the hand. Consider if need to remove the CardNode from the heap.
/*
struct card {
  Name name;
  Suit suit;
  int value;
};
struct card_node {
  CardNode* nextCard;
  CardNode* prevCard;
  Card* thisCard;
};
typedef struct card_node CardNode;

struct hand {
  int num_cards_in_hand;
  CardNode* firstCard;
};*/

Card* removeCardFromHand(Card *aCard, Hand *aHand) {
  CardNode* tempNode = aHand->firstCard;
  Card* topCard = aHand->firstCard->thisCard;
  // to search the nodes in hand using while loop
  // while the firstCard in the hand is not NULL, which means there are cards in hand
  while(tempNode != NULL) {
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
      return tempCard;
    }
    tempNode = tempNode->nextCard;
  // out of the while loop, which means no cardnode in hand
  // then nothing need to be removed, return NULL.
  }
  return NULL;
}
