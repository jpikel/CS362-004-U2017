/****************************************
 * Filename: randtesthelper.c
 * Author: Johanes Pikel
 * ONID: pikelj
 * Date: 2017.07.11
 * Class: CS362-400
 * Assignment: #3
 * Description: implementation of helper functions
 * **************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "testhelper.h"

/* constants used to get string name of enum of cards */
const char* cardNames[] = {"Curse", "Estate", "Duchy", "Province", "Copper", "Silver",
    "Gold", "Adventurer", "Council Room", "Feast", "Gardens", 
    "Mine", "Remodel", "Smithy", "Village", "Baron", "Great Hall",
    "Minion", "Steward", "Tribute", "Ambassador", "Cutpurse",
    "Embargo", "Outpost", "Salvager", "Sea Hag"};

/****************************************
 * Function: printCardsInHAnd()
 * Parameters: struct gameState, int
 * Preconditions: given valid gamestate and int of a player
 * Postconditions: prints the cards in the player's hand to stdout
 * Description:
 * **************************************/

void printCardsInHand(struct gameState * testGame, int player){
    int i;
    printf("Player %d current cards in hand %d\n", player,testGame->handCount[player]);
    for(i = 0; i < testGame->handCount[player]; i ++){
        printf("%s ", cardNames[testGame->hand[player][i]]);
    }
    printf("\n");
}

/****************************************
 * Function: printCardsInDeck()
 * Parameters: struct gameState, int
 * Preconditions: given valid gamestate and int of a player
 * Postconditions: prints the cards in the player's deck to stdout
 * Description:
 * **************************************/

void printCardsInDeck(struct gameState * testGame, int player){
    int i;
    printf("Player %d current cards in deck %d\n", player,testGame->deckCount[player]);
    for(i = 0; i < testGame->deckCount[player]; i ++){
        printf("%s ", cardNames[testGame->deck[player][i]]);
    }
    printf("\n");
}

/****************************************
 * Function: addACard
 * Parameters: struct gameState, int, int
 * Preconditions: passed valid gamestate, int of a card that exists, and player
 * Postconditions: adds the integer representation of the card to the players hand
 * Description:
 * **************************************/

void addACard(struct gameState* testGame, int card, int player){
    testGame->hand[player][testGame->handCount[player]] = card;
    testGame->handCount[player] += 1;
    printf("%s added\n", cardNames[card]);

}

/****************************************
 * Function: validateBaseHandIsSame 
 * Parameters: struct gameState, struct gameState   
 * Preconditions: passed a changed gameState and a previous gameState   
 * Postconditions: prints test success or fail
 * Description: iterates through the base hand that should be 5 and checks if
 * the two hands are the same
 * **************************************/

void validateBaseHandIsSame(struct gameState *testGame,struct gameState* preGame, \
        int player){
    int isCorrect = 1, i;
    printf("Player %d, first 5 cards should still be the same? ...", player);
    for(i = 0; i < 5; i++){
        if(preGame->hand[player][i] != testGame->hand[player][i]){
            isCorrect = -1;
            printf("\nCard # %d should be %s and is %s\n", i+1, 
            cardNames[preGame->hand[player][i]], cardNames[testGame->hand[player][i]]);
        }
    }
    validate(isCorrect, ISCORRECT);
}


/****************************************
 * Function: validateHandCountChange    
 * Parameters: struct gameState, struct gameState, int, int
 * Preconditions: two gameStates, int for player and int for amount to modify
 * Postconditions: prints test success or fail
 * Description: with the modified checks if the new handCount matches the odl
 * plus the modifier
 * **************************************/
void validateHandCountChange(struct gameState *testGame,struct gameState* preGame, \
        int player, int modifier){
    printf("Player %d has %d cards in hand and should have %d...",player,
            testGame->handCount[player], preGame->handCount[player]+modifier);
    validate(testGame->handCount[player], preGame->handCount[player]+modifier);
}

/****************************************
 * Function: validateDeckCountChange    
 * Parameters: struct gameState, struct gameState, int, int
 * Preconditions: two gameStates, int for player and int for amount to modify
 * Postconditions: prints test success or fail
 * Description: with the modified checks if the new deckCount matches the odl
 * plus the modifier
 * **************************************/
void validateDeckCountChange(struct gameState *testGame,struct gameState* preGame, \
        int player, int modifier){
    printf("Player %d deck count should be %d, deck count now %d...",player, 
            preGame->deckCount[player]-modifier, testGame->deckCount[player]);
    validate(preGame->deckCount[player]-modifier, testGame->deckCount[player]);
}
/****************************************
 * Function: validateOtherPlayersNotChanged 
 * Parameters: struct gameState, struct gameState   
 * Preconditions: passed two valid gameState    
 * Postconditions: prints test success or fail
 * Description: compares the previous gamestate for all players states other than 0 
 * to their current state, including handCount, deckCount, hands and decks
 * **************************************/
void validateOtherPlayersNotChanged(struct gameState* testGame, \
        struct gameState* preGame){
    int c,j, isCorrect;
    printf("Testing other players hands and handcounts.\n");
    for(j = 1; j < preGame->numPlayers; j++){
        printf("Player %d handCount was %d, now is %d...", j, preGame->handCount[j],
                testGame->handCount[j]);
        validate(preGame->handCount[j], testGame->handCount[j]);
        printf("Player %d deckCount was %d, now is %d...", j, preGame->deckCount[j],
                testGame->deckCount[j]);
        validate(preGame->deckCount[j], testGame->deckCount[j]);
        printf("Validating hands cards are the same for player %d...", j);
        isCorrect = 1;
        for(c = 0; c < preGame->handCount[j]; c++){
            if(preGame->hand[j][c] != testGame->hand[j][c]){
                isCorrect = -1;
            }
        }
        validate(isCorrect, ISCORRECT);
        printf("Validating deck cards are the same for player %d...", j);
        isCorrect = 1;
        for(c = 0; c < preGame->handCount[j]; c++){
            if(preGame->deck[j][c] != testGame->hand[j][c]){
                isCorrect = -1;
            }
        }
        validate(isCorrect, ISCORRECT);
        printf("Player %d discardCound was %d, now is %d...", j, 
                preGame->discardCount[j], testGame->discardCount[j]);
        validate(preGame->discardCount[j], testGame->discardCount[j]);
    }
}

/****************************************
 * Function: printCardsInPlayed 
 * Parameters: struct gameState 
 * Preconditions: valid gameState   
 * Postconditions: prints all the cards in the played  pile to stout
 * Description:
 * **************************************/

void printCardsInPlayed(struct gameState * testGame){
    int i;
    printf("Played pile contains: ");
    for(i = 0; i < testGame->playedCardCount; i++){
        printf("%s ", cardNames[testGame->playedCards[i]]);
    }
    printf("\n");
}
/****************************************
 *  Function: validatePlayedCount   
 *  Parameters: gameState, int
 *  Preconditions: valid gameState  , and int
 *  Postconditions: prints test success or fail
 *  Description: checks the game state value of playedCardCount to the passed in 
 *  integer and validates if the two are the same
 *  ***************************************/
void validatePlayedCount(struct gameState*testGame, int cardsPlayed){
    printf("Played count in current game should be %d, is %d...", cardsPlayed,
            testGame->playedCardCount);
    validate(cardsPlayed, testGame->playedCardCount);
}
/****************************************
 * Function: validateTreasureMap    
 * Parameters: gameState, gameState 
 * Preconditions: passed two valid gameStates
 * Postconditions: prints test success or fail
 * Description: iterates through the enture supplyCount and validates the two
 * are the same for the two gameStates
 * **************************************/
void validateTreasureMap(struct gameState *testGame, struct gameState *preGame){
    int i, isCorrect = 1;

    printf("Validating treasure map has not changed...");
    for(i = 0; i < treasure_map +1; i++){
        if(testGame->supplyCount[i] != preGame->supplyCount[i]){
            isCorrect = -1;
        }
    }
    validate(ISCORRECT, isCorrect);

}

/****************************************
 *  Function: validateEmbargoTokens 
 * Parameters: gameState, gameState 
 * Preconditions: passed two valid gameStates
 * Postconditions: prints test success or fail
 *  Description: iterates throught all embargoTokens for both games and validates
 *  the values are the same
 *  ***************************************/
void validateEmbargoTokens(struct gameState*testGame, struct gameState*preGame){
    int i, isCorrect = 1;

    printf("Validating embargo tokens has not changed...");
    for(i = 0; i < treasure_map +1; i++){
        if(testGame->embargoTokens[i] != preGame->embargoTokens[i]){
            isCorrect = -1;
        }
    }
    validate(ISCORRECT, isCorrect);

}

/****************************************
 *  Function: validateStaticVariables   
 *  Parameters: gameState, gameState, 7x int
 *  Preconditions: passed two gameStates, and 7x integers
 *  Postconditions: prints test success or fail
 *  Description: compares the new gameState to the old gameState plus a modifier, 
 *  there is a modifier for each of the single variablee in game state, so the
 *  modifier can be passed in as a negative or positive
 *  ***************************************/
void validateStaticVariables(struct gameState* testGame, struct gameState*preGame, \
        int outPMod, int outPTMod, int turnMod, int phaseMod, int actMod, int coinMod, \
        int buyMod){
    printf("Outposts is %d, was %d, mod:%d... ",
            testGame->outpostPlayed, preGame->outpostPlayed, outPMod);
    validate(testGame->outpostPlayed, preGame->outpostPlayed+outPMod);
    printf("Outpost Turn is %d was %d, mod:%d...",
            testGame->outpostTurn, preGame->outpostTurn, outPTMod);
    validate(testGame->outpostTurn, preGame->outpostTurn+outPTMod);
    printf("Turn is %d was %d, mod:%d..",
            testGame->whoseTurn, preGame->whoseTurn, turnMod);
    validate(testGame->whoseTurn, preGame->whoseTurn+turnMod);
    printf("Phase is %d was %d, mod:%d...", testGame->phase, preGame->phase,phaseMod);
    validate(testGame->phase, preGame->phase+phaseMod);
    printf("NumActions is %d was %d, mod:%d..",
            testGame->numActions, preGame->numActions, actMod);
    validate(testGame->numActions, preGame->numActions+actMod);
    printf("Coins is %d was %d, mod:%d...",testGame->coins, preGame->coins, coinMod); 
    validate(testGame->coins, preGame->coins+coinMod);
    printf("Numbuys is %d was %d, mod%d...", testGame->numBuys, preGame->numBuys,buyMod);
    validate(testGame->numBuys, preGame->numBuys+buyMod);
}

/****************************************
 * Function: fail()
 * Parameters: none
 * Preconditions: none
 * Postconditions: none
 * Description: writes a fail message to STDOUT
 * *************************************/

void fail(){
    puts(FAIL);
}

/****************************************
 * Function: success()
 * Parameters: none
 * Preconditions: none
 * Postconditions: none
 * Description: writes a success message to STDOUT
 * *************************************/
void success(){
    puts(SUCCESS);
}

/****************************************
 * Function: validate
 * Parameters: int, int
 * Preconditions: passed 2 integers
 * Postconditions: success or fail message printed to STDOUT
 * Description: if the two integers are equal prints a success message
 * if they are not then a fail message is printed
 * *************************************/
void validate(int a, int b){
    if(a == b){
        success();
    } else {
        fail();
    }
}

/****************************************
 * Function: printStarts()
 * Parameters: na
 * Preconditions: na
 * Postconditions: 42 * to STDOUT
 * Description: na
 * *************************************/
void printStars(){
    int i, j;

    for(i = 0; i < 2; i++){
        for(j = 0; j < 42; j++){
            printf("*");
        }
        printf("\n");
    }
}
