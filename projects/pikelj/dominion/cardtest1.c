/****************************************
 * Filename: cardtest1.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Assignment: #3
 * Description: this is a unit test file for the playSmithy function in dominion.c
 * *************************************/

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "testhelper.h"


/****************************************
 * Function: main()
 * Parameters: none
 * Preconditions: none
 * Postconditions: unit test the playSmithy function
 * Description: smithy should add +3 cards to the players hand and should discard only
 * the smithy card.  So we'll arbitrarily add the smithy card to the first player's 
 * hand, play the card and then check against some expected variables.
 *
 * *************************************/
int main(void){
    struct gameState *testGame, *preGame;
    int randSeed = 1;
    int result;
    int p, c;
    int isCorrect = 1;

    int k[10] = {adventurer, council_room, ambassador, gardens, mine,
        remodel, smithy, village, baron, minion};


    for ( p = 2; p < 5; p++){
        printStars();
        testGame = newGame();
        preGame = newGame();
        result = initializeGame(p, k, randSeed, testGame);
        validate(result, 0);
        result = initializeGame(p, k, randSeed, preGame);
        memcpy(preGame, testGame, sizeof(struct gameState));
        validate(result, 0);
        printf("Unit test for the playSmithy function, with %d players\n", p);
        
        printCardsInHand(testGame, 0);
        printCardsInDeck(testGame, 0);
        printf("First we'll give ourselves the smithy card\n");
        addACard(testGame, smithy, 0);
        printCardsInHand(testGame, 0);
        printf("Playing smithy, return = 0...");
        validate(playSmithy(0, testGame, 5), RETSUCCESS);
        /*validate that no funny business happened here with the first 5 cards we 
         * had in our hand.*/
        validateBaseHandIsSame(testGame, preGame, 0);
        printCardsInHand(testGame, 0);
        /*we need to check that the handCount is +3 since the smithy card gets discarded
         * */
        validateHandCountChange(testGame, preGame, 0, 3);
        /* since the smithy card gets discarded to the played pile we'll also check that
         * here*/
        printf("Should only be 1 card in playedCard pile 'smithy'\n");
        printf("Played pile has %d...", testGame->playedCardCount);
        validate(1, testGame->playedCardCount);
        printCardsInPlayed(testGame);
       /*since our deck count was 5 starting the game and we draw 3 cards it should
         * be 2 now*/
        validateDeckCountChange(testGame, preGame, 0, 3);

        validateOtherPlayersNotChanged(testGame, preGame);
        printf("Played cards should be different between the two games...");
        isCorrect = -1;
        for(c = 0; c < MAX_DECK; c++){
            if(preGame->playedCards[c] != testGame->playedCards[c]){
                isCorrect = 1;
            }
            preGame->playedCards[c] = 0;
            testGame->playedCards[c] = 0;
        }
        validate(isCorrect, 1);
        validateStaticVariables(testGame, preGame,0,0,0,0,0,0,0);
        validateTreasureMap(testGame, preGame);
        validateEmbargoTokens(testGame, preGame);

        free(testGame);
        free(preGame);
    }
    return 0;
}




