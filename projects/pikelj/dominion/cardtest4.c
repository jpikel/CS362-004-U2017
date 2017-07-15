/****************************************
 * Filename: cardtest4.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Assignment: #3
 * Description: this is a unit test file for the playMinion function in dominion.c
 * *************************************/

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testhelper.h"
#include <assert.h>

/*special function for minion to check if players should get new cards*/
int validatePlayersHand(struct gameState* testGame, struct gameState* preGame, \
        int player, int choice2);


/****************************************
 * Function: main()
 * Parameters: none
 * Preconditions: none
 * Postconditions: unit test the playMinion function
 * Description: minion should give the player +1 action
 * and then have a choice between +2 coins or be able to redraw their cards and
 * any players that have more than 4 cards also redraw their hand.
 * *************************************/
int main(void){
    struct gameState *testGame, *preGame;
    int randSeed = 1;
    int numPlayers = 3;
    int i, m;
    int result;
    int choice1;
    int choice2;
    int coinBonus;
    int expectedBonus;
    int expectedDifferentHand;
    int deckChange;

    int k[10] = {adventurer, council_room, ambassador, gardens, mine,
        remodel, smithy, village, baron, minion};


    for(m = 0; m < 4; m++){
        printStars();
        printf("Unit test for the playMinion function\n");
        printf("We have multiple ways to play Minion\n");
        printf("Minion gives us 2 choices so we have 0,0;1,0;0,1;1,1\n");

        testGame = newGame();
        preGame = newGame();
        result = initializeGame(numPlayers, k, randSeed, testGame);
        validate(result, 0);
        result = initializeGame(numPlayers, k, randSeed, preGame);
        memcpy(preGame, testGame, sizeof(struct gameState));
        validate(result, 0);
    
        printCardsInHand(testGame, 0);
        printf("First we'll give ourselves the Minion card\n");
        addACard(testGame, minion, 0);
        printCardsInHand(testGame, 0);

        printf("We're also going to give the other players some cards\n");
        testGame->handCount[1] = 0;
        preGame->handCount[1] = 0;
        for( i = 0; i < 4; i ++){
            testGame->hand[1][i] = silver;
            testGame->handCount[1] += 1;
            preGame->hand[1][i] = silver;
            preGame->handCount[1] += 1;
        }
        testGame->handCount[2] = 0;
        preGame->handCount[2] = 0;
        for( i = 0; i < 5; i++){
            testGame->hand[2][i] = gold;
            testGame->handCount[2] += 1;
            preGame->hand[2][i] = gold;
            preGame->handCount[2] += 1;
        }
        printCardsInHand(testGame, 1);
        printCardsInHand(testGame, 2);

        /*setup choices and set coinBonus to 0
         * so with two choices we have 4 possible combinations
         * thus we need to check against all the possible combinations of
         * choices when the minion card is played*/
        coinBonus = 0;
        if(m == 0){
            choice1 = 0;
            choice2 = 0;
        } else if(m == 1){
            choice1 = 1;
            choice2 = 0;
        } else if (m == 2){
            choice1 = 0;
            choice2 = 1;
        } else {
            choice1 = 1;
            choice2 = 1;
        }
        printf("Playing minion, choice1 = %d, choice2 = %d, return = 0...",
                choice1, choice2);
        validate(playMinion(0, testGame, 5, choice1, choice2, &coinBonus), RETSUCCESS);
        testGame->coins += coinBonus;
        validatePlayedCount(testGame, 1);
        printCardsInPlayed(testGame);
        /* the action check should always happen regardless of choices */
        printf("Check that we received +1 action,\n");
        /* if choice1 is 0 we should not received any bonus coins */
        printf("Check number of bonus coins received\n");
        if(choice1 == 0){
            expectedBonus = 0;
        } else {
            expectedBonus = 2;
        }
        printf("Choice1 = %d bonus coins = %d expected = %d.\n", choice1, 
                coinBonus, expectedBonus);
        validateStaticVariables(testGame, preGame, 0,0,0,0,1,expectedBonus,0);
        /* because the card potentially iterates through other players hands
         * we gave the two other players 4 cards and 5 cards, because that is the
         * boundary of the if statement in the minion card.
         * so here we check that if choice2 was selected then player 0 should
         * have a new hand, and any other player with a hander greater than 4 cards
         * should also have received a new hand*/

        for(i = 0; i < numPlayers; i ++){
            result = validatePlayersHand(testGame, preGame, i, choice2);
            if((choice2 == 1 && preGame->handCount[i] > 4) || (choice2 == 1 && i == 0)){
                printf(" expect different hand...");
                expectedDifferentHand = 1;
                deckChange = 4;
            } else {
                printf(" expect same hand...");
                expectedDifferentHand = 0;
                deckChange = 0;
            }
            validate(result, expectedDifferentHand);
            validateDeckCountChange(testGame, preGame, i,deckChange);
            printf("If we have bonus coins other players should not have new hands\n");
            printf("Coin bonus %d found, player %d has new hand %d...", coinBonus, i,
                    result);
            if(coinBonus && result){
                fail();
            } else {
                success();
            }
        }

        validateTreasureMap(testGame, preGame);
        validateEmbargoTokens(testGame, preGame);

        free(testGame);
        free(preGame);
    }

    return 0;
}

/***************************************
 * Function: validatePlayersHand()
 * Parameters: struct gameState*, int, int[][MAX_HAND], int []
 * Preconditions: passed initialized game state, valid player number, 
 * a 2d integer array, and another integer array with at least elements = number of 
 * players
 * Postconditions: returns int
 * Description: checks how many cards player had and how many player now has
 * also if the current hand of cards is different than the previous then return 1
 * else return 0
 * *************************************/

int validatePlayersHand(struct gameState* testGame, struct gameState* preGame, \
        int player, int choice2){

    int expectedHandCount = 4;
    int i;
    int handDifferent = 0;

    if(choice2 == 0){
        expectedHandCount = preGame->handCount[player];
    }
    printf("Player %d had %d cards, now has %d cards, expect %d...", player,
            preGame->handCount[player], testGame->handCount[player], expectedHandCount);
    validate(expectedHandCount, testGame->handCount[player]);
    /* iterate through the players hand and if any card is not the same
     * then we know we have a different hand*/
    printf("Check if hand is different than before,");
    for (i = 0; i < testGame->handCount[player]; i++){
        if(preGame->hand[player][i] != testGame->hand[player][i]){
            handDifferent = 1;
        }
    }
    return handDifferent;
}


