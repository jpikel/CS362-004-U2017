/****************************************
 * Filename: testhelper.h
 * Author: Johannes pikel
 * ONID: pikelj
 * Date: 2017.07.11
 * Class: CS362-400
 * Assignment: #3
 * Description: a set of helper functions for the cardtests and unittests
 * **************************************/


#ifndef TESTHELPER_H
#define TESTHELPER_H

#define SUCCESS "TEST SUCCESSFUL"
#define FAIL "TEST FAILED"
#define RETSUCCESS 0
#define RETFAIL -1
#define ISCORRECT 1


/*function prototypes that check different states in the game and for players*/
void printCardsInHand(struct gameState*, int player);

void printCardsInDeck(struct gameState*, int player);

void addACard(struct gameState*, int card, int player);

void validateBaseHandIsSame(struct gameState *,struct gameState*, int player);

void validateHandCountChange(struct gameState *testGame,struct gameState* preGame, \
        int player, int modifier);

void validateDeckCountChange(struct gameState *testGame,struct gameState* preGame, \
        int player, int modifier);

void printCardsInPlayed(struct gameState * testGame);

void validatePlayedCount(struct gameState* testGame, int cardsPlayed);

void validateOtherPlayersNotChanged(struct gameState* testGame, \
        struct gameState* preGame);

void validateTreasureMap(struct gameState*, struct gameState*);

void validateEmbargoTokens(struct gameState*, struct gameState*);

void validateStaticVariables(struct gameState* testGame, struct gameState*preGame, \
        int outPMod, int outPTMod, int turnMod, int phaseMod, int actMod, int coinMod, \
        int buyMod);

void fail();
void success();
void validate(int, int);
void printStars();



#endif
