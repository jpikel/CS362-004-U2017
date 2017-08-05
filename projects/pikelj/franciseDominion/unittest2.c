/****************************************
 * Filename: unittest2.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Assignment: #3
 * Description: this is a unit test file for the shuffle function in dominion.c
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
 * Postconditions: unit test the shuffle function
 * Description:
 * *************************************/

int main(void){

    struct gameState *testGame;
    int randSeed = 1;
    int result;
    int i, j, m;
    int originalDeck[4][10];
    int origDeckSize;
    int outOfSequence = -1;
    int handCount;
    int deckCounter;

    int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

    printStars();
    printf("This unit test, tests the shuffle()");
    /*intiialize the game to the valid number of player
     * for each player in the current game we'll record the current deck and the
     * sequence of cards then we'll shuffle that players cards.
     * Validate by checking that the new deck is not in the same order as the old
     * deck*/
    for(i = 2; i < 5; i++){
        testGame = newGame();
        result = initializeGame(i, k, randSeed, testGame);
        assert(result ==  0);
        printf("\nGame initialized with %d players\n",i);
        for(m = 0; m < i; m++){
            printf("Recording current deck for player %d\n",m);
            origDeckSize = testGame->deckCount[m];
            for(j = 0; j < testGame->deckCount[m]; j++){
                originalDeck[m][j] = testGame->deck[m][j];
                printf("%d ", testGame->deck[m][j]);
            }

            printf("\nAttemping to shuffle deck for player %d...",m);
            printf("expecting success\n");
            printf("Current deck size %d...", testGame->deckCount[m]);

            result = shuffle(m,testGame);
            printf("shuffle returned %d...",result);
            validate(RETSUCCESS, result);

            printf("Checking that returned deck is not in the same sequence...\n");
            for(j = 0; j < testGame->deckCount[m]; j++){
                if(originalDeck[m][j] != testGame->deck[m][j]){
                outOfSequence = 0;
                }
                printf("%d ",testGame->deck[m][j]);
            }   
            validate(RETSUCCESS, outOfSequence);

            printf("Checking that original and current deck size are the same...");
            validate(origDeckSize, testGame->deckCount[m]);
            /*we will also draw cards out of the deck for each player until that deck
             * is less than 1 card.  This should result in shuffle returning a -1
             * because the deck is empty, and cannot be shuffled*/
            printf("For player %d drawing cards until deck count < 1\n", m);
            handCount = testGame->handCount[m] + 1;
            deckCounter = testGame->deckCount[m];
            for(j = 0; j < deckCounter; j++){
                testGame->hand[m][handCount+j] = testGame->deck[m][j];
                testGame->deckCount[m]--;
                testGame->handCount[m]++;
            }
            printf("Player %d deck count %d, shuffling, ", m, testGame->deckCount[m]);
            result = shuffle(m, testGame);
            validate(RETFAIL, result);

        }

        free(testGame);
    }


    return(0);

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
