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
  return newHand;
}

CardNode* createNode(Card* thisCard) {
  CardNode* node = (CardNode*)malloc(sizeof(node));
  node->nextCard = NULL;
  node->prevCard = NULL;
  node->thisCard = thisCard;
  return node;
}
/*
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
void destroyHand(Hand* thisHand) {
  free(thisHand->firstCard->nextCard);
  free(thisHand->firstCard);
  free(thisHand);
}

void addCardToHand(Card* thisCard, Hand* thisHand) {
  CardNode* thisCardNode = createNode(thisCard);
  if(thisHand->firstCard != NULL) {
    thisCardNode->nextCard = thisHand->firstCard;
    thisHand->firstCard->prevCard = thisCardNode;
  }
  thisHand->firstCard = thisCardNode;
  thisHand->num_cards_in_hand++;
}


