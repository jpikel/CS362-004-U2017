/****************************************
 * Filename: cardtest3.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Assignment: #3
 * Description: this is a unit test file for the playVillage function in dominion.c
 * *************************************/

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "testhelper.h"

/****************************************
 * Function: main()
 * Parameters: none
 * Preconditions: none
 * Postconditions: unit test the playVillage function
 * Description: village should give the current player +1 card and +2 actions
 * so arbitrarily give player 0 the village card and play it and the 
 * validate results
 * *************************************/
int main(void){
    struct gameState *testGame, *preGame;
    int randSeed = 1;
    int p;
    int result;

    int k[10] = {adventurer, council_room, ambassador, gardens, mine,
        remodel, smithy, village, baron, minion};

    for(p = 2; p< 5; p++){
        printStars();
        printf("Unit test for the playVillage function\n");
        testGame = newGame();
        preGame = newGame();
        result = initializeGame(p, k, randSeed, testGame);
        validate(result, 0);
        result = initializeGame(p, k, randSeed, preGame);
        memcpy(preGame, testGame, sizeof(struct gameState));

        printCardsInHand(testGame, 0);
        printCardsInDeck(testGame, 0);
        printf("First we'll give ourselves the Village card\n");
        /* print the current number of cards in my hand and the cards name
         * then after adding the village card do the same, so there is a visual
         * reference that it worked.  We also record the hand count for future
         * validation, we use the hand count with the village card added*/
        addACard(testGame, 0, village);
        printCardsInHand(testGame, 0);

        printf("Playing village, return = 0...");
        validate(playVillage(0, testGame, 5), RETSUCCESS);

        /*after playing the village card, we can check that the village card did
         * get discarded, then we check that we have +1 card in our current hadn
         * and +2 actions*/
        printf("Should only be 1 card in playedCard pile 'village'\n");
        printf("Played pile has %d...", testGame->playedCardCount);
        validate(1, testGame->playedCardCount);
        printCardsInPlayed(testGame);
        printCardsInHand(testGame, 0);
        validateBaseHandIsSame(testGame, preGame, 0);
        validateHandCountChange(testGame, preGame, 0, 0);
        validateDeckCountChange(testGame, preGame, 0, 1);
        validatePlayedCount(testGame, 1);
        validateOtherPlayersNotChanged(testGame, preGame);
        validateStaticVariables(testGame, preGame, 0,0,0,0,2,0,0);
        validateTreasureMap(testGame, preGame);
        validateEmbargoTokens(testGame, preGame);

        free(testGame);
        free(preGame);
    }

    return 0;
}
