/****************************************
 * Filename: testme.c
 * Student: Johannes Pikel
 * ONID: pikelj
 * Date: 2017.07.17
 * Class: CS362-400
 * Assignmet: Random Testing Quiz2
 * Description: Implement a random test for the function testme()
 * **************************************/


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <limits.h>

/****************************************
 *  Function: inputChar()
 *  Parameters: none
 *  Preconditions: none
 *  Postconditions: returns a random char in the range of 31 to CHAR_MAX
 *  Description: returns a random char in the range from 31 to 127 that results
 *  in a character from the ASCII table.  That are letters, numbers and symbols
 *  Cite: random char formula taken from this site
 *  http://c-faq.com/lib/randrange.html
 *  ***************************************/

char inputChar()
{
    // TODO: rewrite this function
    return (31 + rand() / (RAND_MAX / (CHAR_MAX - 31 + 1) + 1));
}

/****************************************
 *  Function: inputString()
 *  Parameters: none
 *  Preconditions: none
 *  Postconditions: returns a char*
 *  Description: assigns some memory on the heap, and sets the entire location to the
 *  null byte.  Then for the first 4 letters chooses at random a letter from the
 *  word "rest" and returns address of the start of the
 *  resulting string to the calling function.
 *  ***************************************/

char *inputString()
{
    // TODO: rewrite this function
    char source[4] = "rest";
    char *str;
    int i;

    str = malloc(sizeof(char)*6);
    memset(str, '\0', sizeof(char)*6);
    for (i = 0; i < 5; i++){
        str[i] = source[(rand() / (RAND_MAX / (3 - 0 + 1) + 1))];
    }
    
    return str;
}

/****************************************
 *  Function: testMe()
 *  Parameters: none
 *  Preconditions: prints error once the test cases are matched
 *  Postconditions: prints error to stdout and exits the program
 *  Description: continues to loop until all the test cases are met.
 *  the char that is taken from inputChar has to first equal in order
 *  [({ ax})], once we have found that series of chars we are at state 9
 *  at which point if the string from inputString evaluates to 'reset' with the
 *  6th byte as a null byte, then we have passed all the if statements and the
 *  program will print 'error' to stdout and exit.
 *  ***************************************/

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
    free(s);
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
