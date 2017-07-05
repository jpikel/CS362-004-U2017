/* Filename: cards.h
 * Author: Johannes Pikel
 * Date: 2017.06.28
 * ONID: pikelj
 * Class: CS362-400
 * Assignment: #2
 * *************************************/


#ifndef CARDS_H
#define CARDS_H

/* All functions return -1 on failure, and DO NOT CHANGE GAME STATE;
   unless specified for other return, return 0 on success */

/* plays the adventurer card
 * adventurer turns over cards from deck until 2 treasure cards are revealed 
 * remaining cards overturned are discard.*/
int playAdventurer();


#endif
