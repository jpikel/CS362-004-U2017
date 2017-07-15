/****************************************
 * Filename: cardtest2.c
 * Author: Johannes Pikel
 * Date: 2017.07.06
 * Class: CS362-400
 * ONID: pikelj
 * Assignment: #3
 * Description: this is a unit test file for the playAdventuer function in dominion.c
 * Cite: segmentation fault handling from this source
 * https://stackoverflow.com/questions/22052340/how-do-i-handle-and-move-past-a-segfault
 * *************************************/

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <setjmp.h>

#define SUCCESS "TEST SUCCESSFUL"
#define FAIL "TEST FAILED"
#define RETSUCCESS 0
#define RETFAIL -1
#define ISCORRECT 1

jmp_buf restore_point;

/*define some function prototypes*/
void fail();
void success();
void validate(int, int);
void printStars();
void addCardToHand(struct gameState* testGame, int cardToAdd);
void printCardsInHand(struct gameState* testGame, int player, 
        int currentHand[][MAX_HAND],int origHandCount[], int*);
void printCardsInDeck(struct gameState* testGame, int player);
void sighandler(int sig);

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
 * Postconditions: unit test the playAdventurer function
 * Description:
 * *************************************/
int main(void){
    struct gameState *testGame;
    int randSeed = 1;
    int currentHand[1][MAX_HAND];
    int origHandCount[1];
    int currentTreasureCards = 0;
    int newTreasureCards = 0;
    int newHand[1][MAX_HAND];
    int newHandCount[1];
    int result;
    int player = 0;
    int fault_code;
    int i;

    int k[10] = {adventurer, council_room, ambassador, gardens, mine,
        remodel, smithy, village, baron, minion};

    testGame = newGame();
    result = initializeGame(2, k, randSeed, testGame);
    assert(result == 0);

    printStars();
    printf("Unit test for the playAdventurer function\n");

    /*This one was a bit tricky to test, because I added the seg fault possiblilty
     * I had to add some code here, to catch that and not cause the entire thing
     * to exit out.  Which would cause gcov not to finish.  I found a nice bit
     * of code on stack overflow.  So we know on the first initialization we should
     * be safe because the randseed is always the same and there will be two
     * treasure cards on top*/
    printf("\nFirst we'll give ourselves the Adventurer card\n");
    printCardsInHand(testGame, player, currentHand, origHandCount,
            &currentTreasureCards);
    addCardToHand(testGame, adventurer);
    printCardsInHand(testGame, player, currentHand, origHandCount,
            &currentTreasureCards);
    printCardsInDeck(testGame, player);
    printf("Playing adventurer, return = 0...");
    validate(playAdventurer(player, testGame, 5), RETSUCCESS);
    printCardsInHand(testGame, player, newHand, newHandCount, &newTreasureCards);
    /*adventurer gives the player 2 additional treasure cards into their hand
     * cards are drawn from the deck until 2 treasure cards are found, all the 
     * remaining cards are put into the discard pile*/
    printf("Should have +2 treasure cards\n");
    printf("Had %d treasure cards, found %d ....", currentTreasureCards, 
            newTreasureCards);
    validate(currentTreasureCards+2, newTreasureCards);

    printf("Had %d handCount, now have %d handCount, expect +2"
            " (adventurer is not discarded in the code)\n", 
            origHandCount[player], newHandCount[player]);
    validate(origHandCount[player]+1, newHandCount[player]);
    printCardsInDeck(testGame, player);

    /*we also want to test for what happenes when the player deck is empty so
     * we'll use a for loop here to each time end two turns so we are back at
     * out original player and carry on aribitrarily adding adventurer and playing
     * it.  We validate the number of treasure cards expected to have and 
     * that our hand count has only increase by +2 for the new treausre cards*/

    signal(SIGSEGV, sighandler);
    for( i = 0; i < 10; i++){
        if (fault_code == 0){
            printf("\nEnding turn twice to get back to player 0.");
            endTurn(testGame);
            endTurn(testGame);
            printf("\nFirst we'll give ourselves the Adventurer card\n");
            printCardsInHand(testGame, player, currentHand, origHandCount,
                    &currentTreasureCards);
            addCardToHand(testGame, adventurer);
            printCardsInHand(testGame, player, currentHand, origHandCount,
                    &currentTreasureCards);
            printCardsInDeck(testGame, player);
            printf("Playing adventurer, return = 0...");
            fault_code = setjmp(restore_point);
                validate(playAdventurer(player, testGame, 5), RETSUCCESS);
            
            printCardsInDeck(testGame, player);
            printCardsInHand(testGame, player, currentHand, origHandCount,
                    &currentTreasureCards);
            printf("Should have +2 treasure cards\n");
            printf("Had %d treasure cards, found %d ....", currentTreasureCards, 
                    newTreasureCards);
            validate(currentTreasureCards+2, newTreasureCards);

            printf("Had %d handCount, now have %d handCount, expect +2"
                    " (adventurer is not discarded in the code)\n", 
                    origHandCount[player], newHandCount[player]);
            validate(origHandCount[player]+1, newHandCount[player]);
            printCardsInDeck(testGame, player);
        } else {
            printf("SEGFAULT caught.\n");
        }
    }

    free(testGame);
    return 0;
}

/***************************************
 * Function: sighandler()
 * Parameters:  int
 * Preconditions: passed in the signal integer
 * Postconditions: jump to restore point
 * Description: signal handler prints a segementation fault message and
 * jumps back to the restore point
 * *************************************/

void sighandler(int sig){
    printf("CAUGHT A SEGMENTATION\n");
    signal(SIGSEGV, &sighandler);
    longjmp(restore_point, SIGSEGV);
}

/***************************************
 * Function: addCardToHand()
 * Parameters: struct gameState*, int
 * Preconditions: passed initialized game and a valid card integer
 * Postconditions: card added to player 0 hand
 * Description: arbitrarily adds a card to player 0 hand and updates
 * the hand count
 * *************************************/

void addCardToHand(struct gameState* testGame, int cardToAdd){
    testGame->hand[0][testGame->handCount[0]] = cardToAdd;
    testGame->handCount[0] += 1;
    printf("%s added\n", cardNames[cardToAdd]);
}



/***************************************
 * Function: printCardsInDeck()
 * Parameters: struct gameState*, int
 * Preconditions: passed initialized game, and player 
 * Postconditions: Cards in deck printed to STDOUT
 * Description: print passed player's deck
 * *************************************/

void printCardsInDeck(struct gameState* testGame, int player){
    int i;

    printf("Player %d current deck count is %d containing\n", player,
            testGame->deckCount[player]);

    for(i = 0; i < testGame->deckCount[player]; i ++){
        printf("%s ", cardNames[testGame->deck[player][i]]);
    }
    printf("\n");
}



/***************************************
 * Function: printCardsInHand()
 * Parameters: struct gameState*, int, int [][], int[]
 * Preconditions: passed initialized game, and player, 2d array sized to number
 * of player, 1d array sized to number of players
 * Postconditions: Cards in hand printed to STDOUT
 * Description: loop through players, print their cards
 * also stores their current hand in the variable currentHand
 * *************************************/

void printCardsInHand(struct gameState* testGame, int player, 
        int currentHand[][MAX_HAND], int origHandCount[], int* treasureCards){
    int i;

    *treasureCards = 0;
    printf("Current cards in player %d hand %d\n", player, testGame->handCount[player]);
    origHandCount[player] = testGame->handCount[player];
    for(i = 0; i < testGame->handCount[player]; i ++){
        printf("%s ", cardNames[testGame->hand[player][i]]);
        currentHand[player][i] = testGame->hand[player][i];
        if(testGame->hand[player][i] == copper ||
                testGame->hand[player][i] == silver ||
                testGame->hand[player][i] == gold  ){
            *treasureCards += 1;
        }
    }

    printf("\n");
    printf("Current treasure cards in player %d hand %d\n", player, *treasureCards);
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
