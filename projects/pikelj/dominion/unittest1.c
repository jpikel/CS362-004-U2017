/****************************************
 * Filename: unittest1.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Description: this is a unit test file to test the 
 * functionality of the initializeGame function in
 * the dominion.c code
 * *************************************/

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS "TEST SUCCESSFUL"
#define FAIL "TEST FAILED"

/*define some function prototypes*/
void fail();
void success();
void validate(int, int);
void printStars();


/****************************************
 * Function: main()
 * Parameters: none
 * Preconditions: none
 * Postconditions: test initializeGame
 * Description: This function tests the functionality of the initializeGame
 * function in dominion.c
 * *************************************/

int main(void ){

    struct gameState *testGame;
    int randSeed = 1;
    int i, j, r;
    int result;
    int tempCard;
    int expectedCurse;
    int expectedVictory;
    int expectedKingdomVictory;
    int expectedSupply = 10;
    int expectedCopper;
    int expectedSilver = 40;
    int expectedGold = 30;
    int expectedDeck;
    int expectedHand;
    int playerCopper;
    int playerEstate;

    int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

    printStars();
    printf("This unit test, tests the initializeGame()\n");

    printf("Testing for number of players initialized into game\n");
    printf("Max players allowed in the game %d\n", MAX_PLAYERS);

    /*short test to test a range of players from a negative number*/
    /*to a number that is larger than MAXPLAYERS */
    for(i = -1; i < MAX_PLAYERS +3; i++){
        /*create a new game to hopefully flush the contents*/
        testGame = newGame();
        printf("Testing initialize game with %d players...", i);
        result = initializeGame(i, k, randSeed, testGame);
        /*per the rules we are allowed 2, 3 or 4 players in a game*/
        if ( i >= 2 && i <= MAX_PLAYERS) {
            printf("Expected result = 0...");
            validate(result, 0);
            printf("Expecting %d players, game has %d...", i, testGame->numPlayers);
            validate(testGame->numPlayers, i );
        } else {
            printf("Expected result = -1....");
            validate(result, -1);
            printf("Expecting 0 players, game has %d...", testGame->numPlayers);
            validate(0, testGame->numPlayers);
        }
        free(testGame);
    }

    testGame = newGame();

    puts("\nTesting the for loop that checks if there are no duplicate cards in deck");
    /*testing the for loop that checks the different kingdom cards for each position*/
    for(i = 0; i < 9; i++){
        tempCard = k[i];
        k[i] = k[i+1];
        result = initializeGame(2, k, randSeed, testGame);
        printf("Card number %d is duplicate...expecting -1...", i);
        validate(result, -1);
        k[i] = tempCard;
    }

    puts("\nChecking game state supply for 2, 3 and 4 players");
    /*testing the entire initializeGame with a series of valid player numbers*/
    for(i = 2; i < 5; i++){
        printf("Initializing with %d players\n", i);
        result = initializeGame(i, k, randSeed, testGame);
        /*set our expected card inventory based on the number of players per the rules*/
        if(i == 2){
            expectedCurse = 10;
            expectedVictory = 8;
            expectedKingdomVictory = 8;
        } else if (i == 3) {
            expectedCurse = 20;
            expectedVictory = 12;
            expectedKingdomVictory = 12;
        } else {
            expectedCurse = 30;
            expectedVictory = 12;
            expectedKingdomVictory = 12;
        }
        /*for each of the different types of cards check how many found vs expected*/
        printf("Curse cards expected %d, found %d...", expectedCurse, 
                testGame->supplyCount[curse]);
        validate(testGame->supplyCount[curse], expectedCurse);
        printf("Estate cards expected %d, found %d...", expectedVictory, 
                testGame->supplyCount[estate]);
        validate(testGame->supplyCount[estate] , expectedVictory);
        printf("Duchy cards expected %d, found %d...", expectedVictory, 
                testGame->supplyCount[duchy]);
        validate(testGame->supplyCount[duchy] , expectedVictory);
        printf("Province cards expected %d, found %d...", expectedVictory, 
                testGame->supplyCount[province]);
        validate(testGame->supplyCount[province] , expectedVictory);
        /*check that the number of treasure cards are set correctly*/
        expectedCopper = 60 - (7*i);
        printf("Expected copper %d, found %d...", expectedCopper,
                testGame->supplyCount[copper]);
        validate(expectedCopper , testGame->supplyCount[copper]);
        printf("Expected silver %d, found %d...", expectedSilver,
                testGame->supplyCount[silver]);
        validate(expectedSilver, testGame->supplyCount[silver]);
        printf("Expected gold %d, found %d...", expectedGold,
                testGame->supplyCount[gold]);
        validate(expectedGold, testGame->supplyCount[gold]);

        /*iterate through the entire treasure map and those cards that are in play*/
        /*make sure that we have the correct number of Kingdom Victory cards and other*/
        /*supply cards*/
        for(r = adventurer; r <= treasure_map; r++){
            for(j = 0; j < 10; j++){
                if(k[j] == r){
                    if(k[j] == great_hall || k[j] == gardens){
                        printf("Kingdom victory cards expected %d, found %d...", 
                                expectedKingdomVictory, testGame->supplyCount[r]);
                        validate(expectedKingdomVictory, testGame->supplyCount[r]);
                    } else {
                        printf("Supply cards expected %d, found %d...",
                                expectedSupply, testGame->supplyCount[r]);
                        validate(expectedSupply, testGame->supplyCount[r]);
                    }
                }
            }
        }
        /* only the first player gets to draw so player 0 should have 5 in hand
         * and 5 in deck, all other players should have 0 in hand and 10 in deck
         * validate this here. also in this for loop we calidate that the players
         * each have been given 3 estate cards and 7 copper cards*/
        for(j = 0; j < i; j++){
            if(j == 0) {
                expectedHand = 5;
                expectedDeck = 5;
            } else {
                expectedHand = 0;
                expectedDeck = 10;
            }
            printf("Player %d has %d cards in deck. Expected %d...", j,
                    testGame->deckCount[j], expectedDeck);
            validate(expectedDeck, testGame->deckCount[j]);
            printf("Player %d had %d cards in Hand. Expected %d...", j,
                    testGame->handCount[j], expectedHand);
            validate(expectedHand, testGame->handCount[j]);
            printf("Player should have 3 estate and 7 copper to start\n");
            playerCopper = 0;
            playerEstate = 0;
            for(r = 0; r < testGame->handCount[j]; r++){
                if(testGame->hand[j][r] == copper){
                    playerCopper += 1;
                } else if(testGame->hand[j][r] == estate){
                    playerEstate += 1;
                }
            }
            for(r= 0; r < testGame->deckCount[j]; r++){
                if(testGame->deck[j][r] == copper){
                    playerCopper += 1;
                } else if(testGame->deck[j][r] == estate){
                    playerEstate += 1;
                }
            }
            printf("Copper found %d...", playerCopper);
            validate(playerCopper, 7);
            printf("Estate found %d...", playerEstate);
            validate(playerEstate, 3);

        }
        /* validateing the standard game state variables to be correct per the rules */

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
        printf("Player 0 turn...");
        validate(testGame->whoseTurn, 0);

    }

    free(testGame);

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
