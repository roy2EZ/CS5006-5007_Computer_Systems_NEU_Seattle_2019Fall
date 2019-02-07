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
    printHand(thisHand);
  }
  thisHand->firstCard = thisCardNode;
  (thisHand->num_cards_in_hand)++;
  printHand(thisHand);
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
/*
Card* removeCardFromHand(Card* aCard, Hand* aHand) {
  CardNode* tempNode = aHand->firstCard;
  while(tempNode != NULL) {
    // to find which node's card is as same as the aCard 
    if (tempNode->thisCard->name != aCard->name || tempNode->thisCard->suit != aCard->suit) {
    tempNode = tempNode->nextCard;
    } else {
      // start to remove
      //case 1: the will-remove node is not the first of hand
      if (tempNode->prevCard != NULL) {
        tempNode->prevCard->nextCard = tempNode->nextCard;
        tempNode->nextCard->prevCard = tempNode->prevCard;
        return tempNode->thisCard;
      } 
      //case 2: the will-remove node is the first of hand
      else {
        aHand->firstCard = tempNode->nextCard;
        aHand->firstCard->prevCard = NULL;
        tempNode->nextCard=NULL;
        return tempNode->thisCard;
      }
    }
}*/
