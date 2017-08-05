/****************************************
 * Filename: unittest3.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Assignment: #3
 * Description: this is a unit test file for the endTurn function in dominion.c
 * *************************************/

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SUCCESS "TEST SUCCESSFUL"
#define FAIL "TEST FAILED"
#define RETSUCCESS 0
#define RETFAIL -1

/*define some function prototypes*/
void fail();
void success();
void validate(int, int);
void printStars();

/****************************************
 * Function: main()
 * Parameters: none
 * Preconditions: none
 * Postconditions: unit test the endTurn function
 * Description:
 * *************************************/

int main(void){
    struct gameState *testGame;
    int randSeed = 1;
    int result;
    int i, j, m;
    int nextPlayer, prevPlayer;
    int expectedCoins = 0;

    int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

    printStars();
    printf("This unit test, tests the endTurn()\n");
    /*for a valid number of players namely 2 - 4, initialize the game
     * then for at least one complete revolution aroudn the players so we are
     * back at the first player ie player 0, we'll use a for loop to iterate through
     * we will check that the game endTurn actually works and we are at the player
     * we expected which should be +1 of the previous player until we reach the last
     * player and then it should be 0*/
    testGame = newGame();
    for(i = 2; i < 5; i++){
        result = initializeGame(i, k, randSeed, testGame);
        assert(result == 0);
        printf("Initialized with %d players\n",i);
        for(j = 0; j < i+1; j++){
            printf("It is player %d turn...ending turn...", testGame->whoseTurn);
            if(testGame->whoseTurn == i - 1){
                nextPlayer = 0;
            } else {
                nextPlayer = testGame->whoseTurn + 1;
            }
            /*in the end turn function the previous player completely discards
             * their hand check that here, and check that the new player has
             * a hand count of 5*/
            prevPlayer = testGame->whoseTurn;
            result = endTurn(testGame);
            printf("now player's %d turn...", testGame->whoseTurn);
            validate(nextPlayer, testGame->whoseTurn);
            printf("Validate endTurn returned 0...");
            validate(result, 0);
            printf("New player's hand count = 5?...");
            validate(5, testGame->handCount[nextPlayer]);
            printf("Previous player's hand count = 0?... ");
            validate(0, testGame->handCount[prevPlayer]);
            /*validate that the player has the correct number of coins set per
             * the cards in hand*/
            printf("Validating new players coins...\n");
            expectedCoins = 0;
            for(m = 0; m < testGame->handCount[testGame->whoseTurn]; m++){
                if(testGame->hand[nextPlayer][m] == copper){
                    expectedCoins = expectedCoins + 1;
                } else if(testGame->hand[nextPlayer][m] == silver){
                    expectedCoins = expectedCoins + 2;
                } else if(testGame->hand[nextPlayer][m] == gold){
                    expectedCoins = expectedCoins + 3;
                }
            }
            printf("I count %d treasure, game has %d...", expectedCoins, 
                    testGame->coins);
            validate(expectedCoins, testGame->coins);
            /* validate that the state variables have been reset per the rules */
            printf("Validating corrected game state settings\n");
            printf("Outpost played = 0...");
            validate(testGame->outpostPlayed, 0);
            printf("Phase = 0...");
            validate(testGame->phase, 0);
            printf("Number of Actions = 1...");
            validate(testGame->numActions, 1);
            printf("Buys = 1...");
            validate(testGame->numBuys, 1);
            printf("Played cards = 0...");
            validate(testGame->playedCardCount, 0);
            printf("Player %d turn...", testGame->whoseTurn);
            validate(testGame->whoseTurn, nextPlayer);
        }
    }

    return 0;
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
