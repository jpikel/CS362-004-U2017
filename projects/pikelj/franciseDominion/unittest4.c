/****************************************
 * Filename: unittest4.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Assignment: #3
 * Description: this is a unit test file for the isGameOver function in dominion.c
 * *************************************/

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define SUCCESS "TEST SUCCESSFUL"
#define FAIL "TEST FAILED"
#define RETSUCCESS 0
#define RETFAIL -1
#define GAMEOVER 1
#define SUPPLY 5

/*define some function prototypes*/
void fail();
void success();
void validate(int, int);
void printStars();

/* constants used to get string name of enum of cards */
const char* cardNames[] = {"Curse", "Estate", "Duchy", "Province", "Copper", "Silver",
                           "Gold", "Adventurer", "Council Room", "Feast", "Gardens", 
                           "Mine", "Remodel", "Smithy", "Village", "Baron", "Great Hall",
                           "Minion", "Steward", "Tribute", "Ambassador", "Cutpurse",
                           "Embargo", "Outpost", "Salvager", "Sea Hag"};


/****************************************
 * Function: main()
 * Parameters: none
 * Preconditions: none
 * Postconditions: unit test the isGameOver function
 * Description: game over is set by a few things, if all provinces are gone,
 * if three of the supply cards are at 0.
 * this is a pretty straight forward test and the printf statements
 * explain what is going to be tested here.
 * *************************************/

int main(void){
    struct gameState *testGame;
    int randSeed = 1;
    int result;
    int i;

    int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

    printStars();
    printf("This unit test, tests the isGameOver()\n");

    testGame = newGame();

    result = initializeGame(2, k, randSeed, testGame);
    assert(result == 0);

    printf("Testing base game after initialization...game should not be over...");
    validate(isGameOver(testGame), RETSUCCESS);

    printf("Ending one turn... game should not be over...");
    endTurn(testGame);
    validate(isGameOver(testGame), RETSUCCESS);

    printf("Testing first if provinces are all gone, game is over...");
    testGame->supplyCount[province] = 0;
    validate(isGameOver(testGame), GAMEOVER);
    testGame->supplyCount[province] = 0;

    printf("Going through the supply pile and set 2 cards to 0 every other card\n");
    printf("Game should never be over, setting all supply piles to 5\n");

    for(i = 0; i < 25; i++){
        testGame->supplyCount[i] = SUPPLY;
        printf("%d ", testGame->supplyCount[i]);
    }
    printf("\n");

    /* I had add an exception here because the province card is also part of the
     * supply and so this will cause the game to be over.*/
    for(i = 0; i < 24; i += 1){
        testGame->supplyCount[i] = 0;
        testGame->supplyCount[i+2] = 0;
        printf("Set supply card %s and %s to 0...", cardNames[i], cardNames[i+2]);
        if(strcmp(cardNames[i], "Province") == 0 
                || strcmp(cardNames[i+2],"Province")==0){
            validate(isGameOver(testGame), GAMEOVER);
        } else {
            validate(isGameOver(testGame), RETSUCCESS);
        }
        testGame->supplyCount[i] = SUPPLY;
        testGame->supplyCount[i+2] = SUPPLY;
    }

    printf("Now setting sets of 3 cards to 0, every other card\n");
    printf("Game should be over each time\n");

    for(i = 0; i < 22; i += 1){
        testGame->supplyCount[i] = 0;
        testGame->supplyCount[i+2] = 0;
        testGame->supplyCount[i+4] = 0;
        printf("Set supply card %s, %s and %s to 0...", cardNames[i], cardNames[i+2],
                cardNames[i+4]);
        validate(isGameOver(testGame), GAMEOVER);
        testGame->supplyCount[i] = SUPPLY;
        testGame->supplyCount[i+2] = SUPPLY;
        testGame->supplyCount[i+4] = SUPPLY;
    }

    printf("Now setting sets of 3 cards to -1, every other card\n");
    printf("Game should be over each time\n");

    for(i = 0; i < 22; i += 1){
        testGame->supplyCount[i] = -1;
        testGame->supplyCount[i+2] = -1;
        testGame->supplyCount[i+4] = -1;
        printf("Set supply card %s, %s and %s to -1...", cardNames[i], cardNames[i+2],
                cardNames[i+4]);
        validate(isGameOver(testGame), GAMEOVER);
        testGame->supplyCount[i] = SUPPLY;
        testGame->supplyCount[i+2] = SUPPLY;
        testGame->supplyCount[i+4] = SUPPLY;
    }

    printf("Now setting sets of 3 cards to 1, every other card\n");
    printf("Game should not be over each time\n");

    for(i = 0; i < 22; i += 1){
        testGame->supplyCount[i] = 1;
        testGame->supplyCount[i+2] = 1;
        testGame->supplyCount[i+4] = 1;
        printf("Set supply card %s, %s and %s to 1...", cardNames[i], cardNames[i+2],
                cardNames[i+4]);
        validate(isGameOver(testGame), RETSUCCESS);
        testGame->supplyCount[i] = SUPPLY;
        testGame->supplyCount[i+2] = SUPPLY;
        testGame->supplyCount[i+4] = SUPPLY;
    }

    printf("Now setting sets of 4 cards to 0, every other card\n");
    printf("Game should be over each time\n");

    for(i = 0; i < 20; i += 1){
        testGame->supplyCount[i] = 0;
        testGame->supplyCount[i+2] = 0;
        testGame->supplyCount[i+4] = 0;
        testGame->supplyCount[i+6] = 0;
        printf("Set supply card %s, %s, %s and %s to 0...", cardNames[i], cardNames[i+2],
                cardNames[i+4], cardNames[i+6]);
        validate(isGameOver(testGame), GAMEOVER);
        testGame->supplyCount[i] = SUPPLY;
        testGame->supplyCount[i+2] = SUPPLY;
        testGame->supplyCount[i+4] = SUPPLY;
        testGame->supplyCount[i+6] = SUPPLY;
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
