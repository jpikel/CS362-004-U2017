/****************************************
 * Filename: randomtestcard.c
 * Author: Johannes Pikel
 * ONID: pikelj
 * Date: 2017.07.18
 * Class: CS362-400
 * Assignment #4 random testing refactored cards
 * Description: This random tester tests the adventurer refactored function
 * Cite: the initialization of the gameState is taken from the 
 * example given in the betterTestDrawCard.c
 * **************************************/


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

#define NOISY 0

/****************************************
 *  Function: 
 *  Parameters:
 *  Preconditions:
 *  Postconditions:
 *  Description:
 *  ***************************************/

int checkAdventurerCard(int p, struct gameState *post, int iteration){
    struct gameState pre;
    int handpos, count, deckCounter, adjust = 1, drawntreasure = 0, tmpcard, z = 0;
    int tmphand[MAX_HAND];

    handpos = floor(Random() * MAX_HAND);
    memcpy(&pre, post, sizeof(struct gameState));
    if (playAdventurer(p, post, handpos) != 0){
        printf("playAdventurer failed \n");
    }
    /* adjust for the drawCard, if our deckCount is larger than 0 then we can
     * go ahead and proceed with drawing a card from it per usual
     * otherwise we need to deal with the shuffle function so we copy the memory
     * from the post deck into our copied deck and similarly for the discard 
     * memory locations.  then we set the pre hand to be that of the changed games
     * new card.  and complete the rest of the discardCard with setting the 
     * handCount += 1, deckCount -=1 and discardCount = 0*/
    while(drawntreasure < 2){
        if(pre.deckCount[p] > 0){
            count = pre.handCount[p];
            deckCounter = pre.deckCount[p];
            pre.hand[p][count] = pre.deck[p][deckCounter-1];
            pre.handCount[p] += 1;
            pre.deckCount[p] -= 1;
            tmpcard = pre.hand[p][pre.handCount[p]-1];
            if(tmpcard == copper || tmpcard == silver || tmpcard == gold){
                drawntreasure++;
            } else {
                tmphand[z]=tmpcard;
                pre.handCount[p]--;
                z++;
            }
        } else {
            memcpy(pre.deck[p], post->deck[p], sizeof(int) * post->deckCount[p]);
            memcpy(pre.discard[p], post->discard[p], sizeof(int)*post->discardCount[p]);
            memcpy(pre.hand[p], post->hand[p], sizeof(int)*post->handCount[p]);
            memcpy(pre.playedCards,post->playedCards
                    ,sizeof(int)*post->playedCardCount);
            pre.handCount[p] = post->handCount[p];
            pre.deckCount[p] = post->deckCount[p];
            pre.discardCount[p] = post->discardCount[p];
            pre.playedCardCount = post->playedCardCount;
            adjust = 0;
            break;
        }
    }

    if(adjust == 1){
        while(z-1 >= 0){
            pre.discard[p][pre.discardCount[p]++] = tmphand[z-1];
            z = z - 1;
        }
    } else {
        /* when shuffles occur I was unable to determine where the two states differed
         * in this case for playAdventurer, I removed this because it is not a good
         * method*/
        /*memcpy(&pre, post, sizeof(struct gameState));*/
    }

    /* after setting our pre copied stats to what should be adventurer correctly
     * we'll check the memory to see if it is the same, we'll only print a message
     * if an iteration fails, otherwise we're silent.
     * I tested this initially by removing the bug from village that I introduced and
     * it worked just fine, of course after reintroducing the bug, all of our tests
     * failed as we should expect.*/
    if (memcmp(&pre, post, sizeof(struct gameState)) != 0){
        if(NOISY){
            printf ("PRE: p %d HC %d DeC %d DiC %d plCC:%d plC:%d hnd:%d lst:%d \n",
                    p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p],
                    pre.playedCardCount, pre.playedCards[pre.playedCardCount],
                    pre.hand[p][handpos], pre.hand[p][pre.handCount[p]-1]);
            printf ("POST: p %d HC %d DeC %d DiC %d plCC:%d plC:%d hnd:%d lst:%d\n",
                    p, post->handCount[p], post->deckCount[p], post->discardCount[p],
                    post->playedCardCount, post->playedCards[post->playedCardCount],
                    post->hand[p][handpos], post->hand[p][pre.handCount[p]-1]);
        }
        printf("Iteration:%d memcmp failed.\n", iteration);
    }
    return 0;

}

/****************************************
 *  Function: 
 *  Parameters:
 *  Preconditions:
 *  Postconditions:
 *  Description:
 *  ***************************************/
void addTreasure(struct gameState* rndGame, int player){
    int rndpos;


    if (rndGame->deckCount[player] < 3){
        do {
            rndpos = floor(Random() * rndGame->discardCount[player]);
        } while(rndpos < 0 || rndpos > rndGame->discardCount[player]);
        /*fprintf(stdout, "C: %d ", rndpos);*/
        rndGame->discard[player][rndpos] = copper;
        do {
            rndpos = floor(Random() * rndGame->discardCount[player]);
        } while((rndpos < 0 || rndpos > rndGame->discardCount[player])
                && (rndGame->discard[player][rndpos] == copper || 
                    rndGame->discard[player][rndpos] == gold));
        rndGame->discard[player][rndpos] = silver;
        /*fprintf(stdout, "S: %d ", rndpos);*/
        do {
            rndpos = floor(Random() * rndGame->discardCount[player]);
        }while((rndpos < 0 || rndpos > rndGame->discardCount[player])
                && (rndGame->discard[player][rndpos] == silver ||
                    rndGame->discard[player][rndpos] == copper));
        rndGame->discard[player][rndpos] = gold;
    }
    do {
        rndpos = floor(Random() * rndGame->deckCount[player]);
    } while(rndpos < 0 || rndpos > rndGame->deckCount[player]);
    /*fprintf(stdout, "C: %d ", rndpos);*/
    rndGame->deck[player][rndpos] = copper;
    do {
        rndpos = floor(Random() * rndGame->deckCount[player]);
    } while((rndpos < 0 || rndpos > rndGame->deckCount[player])
            && (rndGame->deck[player][rndpos] == copper || 
                rndGame->deck[player][rndpos] == gold));
    rndGame->deck[player][rndpos] = silver;
    /*fprintf(stdout, "S: %d ", rndpos);*/
    do {
        rndpos = floor(Random() * rndGame->deckCount[player]);
    }while((rndpos < 0 || rndpos > rndGame->deckCount[player])
            && (rndGame->deck[player][rndpos] == silver ||
                rndGame->deck[player][rndpos] == copper));
    rndGame->deck[player][rndpos] = gold;

}

/****************************************
 *  Function: 
 *  Parameters:
 *  Preconditions:
 *  Postconditions:
 *  Description:
 *  ***************************************/

int main(void){

    struct gameState rndGame;
    int n, i, player;

    fprintf(stdout, "Testing playAdventurer randomly\n");

    SelectStream(2);
    PutSeed(3);

    for ( n = 0; n < 2000; n++){
        for(i = 0; i < sizeof(struct gameState); i++){
            ((char*)&rndGame)[i] = floor(Random() * 256);
        }
        player = floor(Random() * 2);
        rndGame.deckCount[player] = floor(Random() * MAX_DECK/4);
        rndGame.discardCount[player] = floor(Random() * MAX_DECK/4);
        rndGame.handCount[player] = floor(Random() * MAX_HAND/4);
        rndGame.playedCardCount = floor(Random() * MAX_HAND);
        addTreasure(&rndGame, player);
        checkAdventurerCard(player, &rndGame, n);
    }

    fprintf(stdout, "Finished all tests.\n");
    return 0;
}
