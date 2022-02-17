

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

//Heart is 3
//Diamonds is 4
//Clubs is 5
//Spades is 6

typedef struct card_s {
    char suit;
    int face;
    struct card_s* next;
} card;

void buildDeck(card* deck);
void shuffleDeck(card* deck);
void dealHand(card** deck, card** hand);
void winnings(card* hand, int* bet, int* winRS, int* winTP, int* winTK, int* winFK, int* winST, int* winFL, int* winFH, int* winFST, int calculateBet);
void Discard(card** deck, card** hand, int N);
void print(card* deck, card* hand, int type);
int countNodes(card* deck);
card* MemLocofNthNode(card* HN, int N);
card* RemoveNthNode(card** HN, int N);
void InsertNthNode(card** HN, card* Node, int N);
void freeCards(card** deck, card** hand, int type);
void gameStats(char name[], int roundNum, int winRound, int winRS, int winTP, int winTK, int winFK, int winST, int winFL, int winFH, int winFST);
void cardGraphics(char suit[], char face[]);
void printFace(int face);

int main(void) {

    card* deck, * hand, * discard;
    int coins = 100, bet, N = 0, cardPos, cardRemove, i, winRound = 0, roundNum = 0, calculateBet;
    int winRS = 0, winTP = 0, winTK = 0, winFK = 0, winST = 0, winFL = 0, winFH = 0, winFST = 0; //keeps track of the amount of times each hand has been played
    char name[50], recommendation;

    printf(" ______ _____ ______   __ ______ _____\n");
    printf("|  ____/ ____|  ____| /_ |____  | ____|\n");
    printf("| |__ | |    | |__     | |   / /| |__\n");
    printf("|  __|| |    |  __|    | |  / / |___ \\ \n");
    printf("| |___| |____| |____   | | / /   ___) |\n");
    printf("|______\\_____|______|  |_|/_/   |____/      ____       _   _\n");
    printf("|__ \\  |  __ \\	  (_)                      |  _  \\    | | | |\n");
    printf("   ) | | |__) |_ _ _ _ __ ___	___  _ __  | |_) | ___| |_| |_ ___ _ __\n");
    printf("  / /  |  ___/ _` | | '__/ __|  / _ \\| '__||  _ < / _ \\ __| __/ _ \\ '__|\n");
    printf(" / /_  | |  | (_| | | |  \\__ \\ | (_) | |   | |_) |  __/ |_| ||  __/ |\n");
    printf("|____| |_|   \\__,_|_|_|  |___/  \\___/|_|   |____/ \\___|\\__|\\__\\___|_|\n\n\n");


    deck = (card*)calloc(sizeof(card), 1);
    hand = (card*)calloc(sizeof(card), 1);

    buildDeck(deck);

    shuffleDeck(deck);

    dealHand(&deck, &hand);

    printf("What is your name? ");
    fgets(name, 50, stdin);
    if (name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = 0;

    printf("\nLet's play Two Pairs or Better!\n\n");
    printf("$$$$$ Rank of Winning $$$$$\n\n");

    printf("Royal Flush\t10%c J%c Q%c K%c A%c\t250 * bet\n", 6, 6, 6, 6, 6);
    printf("Straight Flush\t2%c 3%c 4%c 5%c 6%c\t100 * bet\n", 5, 5, 5, 5, 5);
    printf("Four of a Kind\t9%c 9%c 9%c 9%c %c\t50 * bet\n", 6, 5, 4, 3, 254);
    printf("Full House\t9%c 9%c 9%c 3%c 3%c\t25 * bet\n", 3, 4, 6, 5, 3);
    printf("Flush\t\t%c%c %c%c %c%c %c%c %c%c\t10 * bet\n", 5, 254, 5, 254, 5, 254, 5, 254, 5, 254);
    printf("Straight\t4%c 5%c 6%c 7%c 8%c\t5 * bet\n", 254, 254, 254, 254, 254);
    printf("Three of a Kind\t9%c 9%c 9%c %c %c \t4 * bet\n", 6, 5, 4, 254, 254);
    printf("Two Pair\tK%c K%c 6%c 6%c %c\t2 * bet\n\n", 6, 5, 4, 3, 254);

    do {
        printf("Your hand\n");
        print(deck, hand, 0);
        do {
            printf("%s, you have %d coins. How many would you like to bet? (-1 to Quit)\n", name, coins);
            scanf("%d", &bet);
            if (bet == -1)
                break;
            else if (bet<1 || bet>coins)
                printf("Invalid entry\n");
        } while (bet < 1 || bet > coins);
        if (bet == -1)
            break;
        else {
            coins -= bet;

            printf("Would you like a recommendation, %s? (y for yes) ", name);
            scanf(" %c", &recommendation);
            if (recommendation == 'y')
                winnings(hand, &bet, &winRS, &winTP, &winTK, &winFK, &winST, &winFL, &winFH, &winFST, 0); //when 0 is passed in, the winnings function is only used to calculate the type of hand. 

            do {
                printf("\nHow many cards would you like to remove?\n");
                scanf("%d", &cardRemove);
            } while (cardRemove < 0 || cardRemove > 5);

            for (i = 0; i < cardRemove; i++) {
                do {
                    printf("What is the position of the card you wish to remove (1-5)?\n");
                    scanf("%d", &cardPos);
                } while (cardPos < 1 || cardPos >5);

                Discard(&deck, &hand, cardPos); //removes card at said position
            }
            printf("Your hand\n");
            print(deck, hand, 0);

            winnings(hand, &bet, &winRS, &winTP, &winTK, &winFK, &winST, &winFL, &winFH, &winFST, 1); //calls the winning function again, but this time in order to calculate the bet return amount
            if (bet > 0)
                winRound++; //used for gamestats

            coins += bet;
            printf("Current coin balance: %d\n", coins);

            for (i = 1; i <= 5; i++) {
                Discard(&deck, &hand, i); //discards hand from this round and replace with another
            }
            if (countNodes(deck) < 20) {
                freeCards(&deck, &hand, 1); //1 signifies to free the deck
                deck = (card*)calloc(sizeof(card), 1);
                buildDeck(deck);

                shuffleDeck(deck);
            }
            roundNum++;
        }
    } while (coins > 0);
    gameStats(name, roundNum, winRound, winRS, winTP, winTK, winFK, winST, winFL, winFH, winFST); //when the round ends, write certain stats to a file. 

    if (coins == 0)
        printf("Out of coins. ");

    freeCards(&deck, &hand, 2); //2 means to free the deck cards and the hand cards

    printf("Goodbye %s!", name);

    return 0;
}

void buildDeck(card* deck) {
    char suit[4] = { 3,4,5,6 };
    int face[13] = { 2,3,4,5,6,7,8,9,10,11,12,13,14 }; //11 is jack, 12 is queen, 13 is king, 14 is ace
    card* temp = deck, * hn = deck;
    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 13; j++)
        {
            temp->suit = suit[i];
            temp->face = face[j];
            temp->next = (card*)calloc(sizeof(card), 1);
            temp = temp->next; //builds a deck with sequential cards from 2 to ace of each suit
        }
    }
}

void shuffleDeck(card* deck) {
    card* hn = deck, * runner = deck, * indexer = deck; //indexer goes 1by1 after each swap is completed. Runner finds the specified index.
    card holder;
    int test[52];
    srand((int)time(0));
    for (int i = 0; i < 10000; i++)
    {
        for (int i = 0; i < 52; i++)
        {
            int index = (rand() % (52));
            test[index] = index;
            for (int i = 0; i < index; i++)
            {
                runner = runner->next;
            }
            holder = *indexer;
            indexer->suit = runner->suit; //flip the values of the two cards
            indexer->face = runner->face;
            runner->suit = holder.suit;
            runner->face = holder.face;
            indexer = indexer->next;
            runner = hn;
        }
        indexer = hn; //reset back to position 1
        runner = indexer;
    }
}

void dealHand(card** deck, card** hand) { //function removes the first five cards off of the deck and places them in player's hand
    card* hTemp = *hand;
    int i;
    for (i = 0; i < 5; i++) {
        hTemp = (card*)calloc(sizeof(card), 1);
        hTemp = RemoveNthNode(deck, 0);
        InsertNthNode(hand, hTemp, (i + 1));
        hTemp = hTemp->next;
    }
    hTemp = NULL;

}

void Discard(card** deck, card** hand, int N) {
    card* hTemp = *hand, * dTemp = deck;
    hTemp = RemoveNthNode(hand, N);
    dTemp = RemoveNthNode(deck, 1);
    InsertNthNode(hand, dTemp, N);
    free(hTemp);
}

void print(card* deck, card* hand, int type) { //prints out either the hand or the deck based off of the int type that is passed in. If 0 is passed, the func. will call the cardGraphics function to print out the hand.
    card* hTemp, * dTemp;
    int i;
    char face[5];
    char suit[5];
    hTemp = hand;
    dTemp = deck;
    if (type == 0) {
        for (i = 0; i < 5; i++) {
            suit[i] = hTemp->suit;
            if (hTemp->face == 14)
                face[i] = 'A';
            else if (hTemp->face == 11)
                face[i] = 'J';
            else if (hTemp->face == 12)
                face[i] = 'Q';
            else if (hTemp->face == 13)
                face[i] = 'K';
            else if (hTemp->face == 10)
                face[i] = 'T';
            else
                face[i] = hTemp->face + 48;
            hTemp = hTemp->next;

        }
        cardGraphics(suit, face);
    }
    else if (type == 1) {
        for (i = 0; i < ((countNodes(deck)) - 1); i++) {
            if (dTemp->face == 14) {
                printf("A%c ", dTemp->suit);
            }
            else if (dTemp->face == 11) {
                printf("J%c ", dTemp->suit);
            }
            else if (dTemp->face == 12) {
                printf("Q%c ", dTemp->suit);
            }
            else if (dTemp->face == 13) {
                printf("K%c ", dTemp->suit);
            }
            else {
                printf("%d%c ", dTemp->face, dTemp->suit);
            }
            if ((i + 1) % 13 == 0) {
                printf("\n");
            }
            dTemp = dTemp->next;
        }

    }
    printf("\n");
}

int countNodes(card* deck) {
    int count = 0;
    card* tmp;
    tmp = deck;
    while (deck != NULL) {
        count++;
        deck = deck->next;
    }
    return(count);
}

card* RemoveNthNode(card** HN, int N) {
    card* tmp, * tmp2;

    if (N > 1) {
        tmp = MemLocofNthNode(*HN, N - 1);   //Assume N > 1. tmp is the node before
        tmp2 = tmp->next;   //tmp2 is the node being extracted
        tmp->next = tmp2->next;
    }
    else {
        tmp2 = *HN;
        *HN = tmp2->next;
    }
    tmp2->next = NULL;	//creates a linked list of length 1  

    return(tmp2);
}

card* MemLocofNthNode(card* HN, int N) {

    for (int i = 2; i <= N; i++) {
        HN = HN->next;
    }
    return(HN);

}

void InsertNthNode(card** HN, card* Node, int N) {
    card* tmp, * tmp2;
    if (N > 1) {
        tmp = MemLocofNthNode(*HN, N - 1);
        Node->next = tmp->next;   //now they both point to the next one down
        tmp->next = Node;
    }
    else {
        Node->next = *HN;
        *HN = Node;
    }
    return;
}

void winnings(card* hand, int* bet, int* winRS, int* winTP, int* winTK, int* winFK, int* winST, int* winFL, int* winFH, int* winFST, int calculateBet)
{
    /*
    This function uses a multitude of if statements and for loops to calculate the type of hand that the player has. If the value of calculateBet
    is 1, the function will also calculate the bet return based on the bet amount that was passed in. If calculateBet=0, then function is instructed
    to instead give a recommendation for which cards to keep, based on the cards held. ints winTP, winTK, win FK, etc. are used for game stats.
    */

    char handType = 'n';
    char handSuit[5];
    int handFace[5], repeat[2] = { 0,0 }, num = 0, countOne = 0, countTwo = 0, temp, straight = 0, flush = 0, winnings;
    for (int i = 0; i < 5; i++) //puts the hand into an array for easier viewing
    {
        handSuit[i] = hand->suit;
        handFace[i] = hand->face;
        hand = hand->next;
    }
    /*
    These if statements build an array with the faces of the cards that repeat. If none the array is populated with {0,0}. Otherwise
    array is populated with the faces of the repeated cards (up to two).
    */
    if (handFace[0] == handFace[1] || handFace[0] == handFace[2] || handFace[0] == handFace[3] || handFace[0] == handFace[4]) {
        repeat[num] = handFace[0];
        num++;
    }
    if (handFace[1] == handFace[2] || handFace[1] == handFace[3] || handFace[1] == handFace[4]) {
        repeat[num] = handFace[1];
        num++;
    }
    if (handFace[2] == handFace[3] || handFace[2] == handFace[4] && num <= 1) {
        repeat[num] = handFace[2];
        num++;
    }
    if (handFace[3] == handFace[4] && num <= 1)
    {
        repeat[num] = handFace[3];
    }
    for (int i = 0; i < 5; i++)
    {
        if (handFace[i] == repeat[0]) //checks for the amount of times that each face repeats.
            countOne++;
    }
    for (int j = 0; j < 5; j++)
    {
        if (handFace[j] == repeat[1])
            countTwo++;
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (handFace[j + 1] < handFace[j]) //orders the array based on the face number (2-14)
            {
                temp = handFace[j + 1];
                handFace[j + 1] = handFace[j];
                handFace[j] = temp;
            }
        }

    }
    temp = 0;
    for (int i = 0; i < 4; i++)
    {
        if ((handFace[i]) + 1 == handFace[i + 1]) //checks to see if the hand is a straight. int straight is assigned 1 if true.
            temp++;
    }
    if (temp == 4)
        straight = 1;
    temp = 0;
    for (int j = 0; j < 4; j++)
    {
        if (handSuit[j] == handSuit[j + 1]) //checks to see if the hand is a flush. int flush is assigned 1 if true. 
            temp++;
    }
    if (temp == 4)
        flush = 1;
    if (calculateBet) {
        if (flush && straight && handSuit[0] == 14 && handFace[0] == 10) {
            winnings = (*bet * 250);
            *bet = (*bet * 250) + *bet;
            printf("Congratulations, you have a royal flush! You win %d coins.\n", winnings);
            (*winRS)++;
        }
        else if (flush && straight) {
            winnings = (*bet * 100);
            *bet = (*bet * 100) + *bet;
            printf("Congratulations, you have a straight flush! You win %d coins.\n", winnings);
            (*winFST)++;
        }
        else if (countOne == 4 || countTwo == 4) {
            winnings = (*bet * 50);
            *bet = (*bet * 50) + *bet;
            printf("Congratulations, you have four of a kind! You win %d coins.\n", winnings);
            (*winFK)++;
        }
        else if ((countOne == 3 && countTwo == 2) || (countOne == 2 && countTwo == 3)) {
            winnings = (*bet * 50);
            *bet = (*bet * 25) + *bet;
            printf("Congratulations, you have a full house! You win %d coins.\n", winnings);
            (*winFH)++;
        }
        else if (flush) {
            winnings = (*bet * 10);
            *bet = (*bet * 10) + *bet;
            printf("Congratulations, you have a flush! You win %d coins.\n", winnings);
            (*winFL)++;
        }
        else if (straight) {
            winnings = (*bet * 5);
            *bet = (*bet * 5) + *bet;
            printf("Congratulations, you have a straight! You win %d coins.\n", winnings);
            (*winST)++;
        }
        else if (countOne == 3 || countTwo == 3) {
            winnings = (*bet * 4);
            *bet = (*bet * 4) + *bet;
            printf("Congratulations, you have three of a kind! You win %d coins.\n", winnings);
            (*winTK)++;
        }
        else if (countOne == 2 && countTwo == 2) {
            winnings = (*bet * 2);
            *bet = (*bet * 2) + *bet;
            printf("Congratulations, you have two pair! You win %d coins.\n", winnings);
            (*winTP)++;
        }
        else {
            printf("You have nothing. You lose %d coins.\n", *bet);
            *bet *= 0;
        }
    }
    else { //recommendation algorithm
        if (flush || straight || (countOne == 3 && countTwo == 2) || (countOne == 2 && countTwo == 3))
            printf("You should keep all of the cards in your hand.");
        else {
            if (countOne == 4) {
                printf("If you keep all cards that have a face of ");
                printFace(repeat[0]);
                printf(" you will have a four of a kind");
            }
            else if (countTwo == 4) {
                printf("If you keep all cards that have a face of ");
                printFace(repeat[1]);
                printf(" you will have a four of a kind");
            }
            else if (countOne == 3) {
                printf("If you keep all cards that have a face of ");
                printFace(repeat[0]);
                printf(" you will have a three of a kind");
            }
            else if (countTwo == 3) {
                printf("If you keep all cards that have a face of ");
                printFace(repeat[1]);
                printf(" you will have a three of a kind");
            }
            else if (countOne == 2 && countTwo == 2) {
                printf("If you keep all cards that have a face of ");
                printFace(repeat[0]);
                printf(" and ");
                printFace(repeat[1]);
                printf(" you will have two pair");
            }
            else if (countOne == 2) {
                printf("You should keep all cards that have a face of ");
                printFace(repeat[0]);
                printf(" for the best chance of a winning hand");
            }
            else if (countTwo == 2) {
                printf("You should keep all cards that have a face of ");
                printFace(repeat[1]);
                printf(" for the best chance of a winning hand");
            }
            else
                printf("The computer cannot make a recommendation");
        }
        printf("\n");
    }
}

void freeCards(card** deck, card** hand, int type) {
    card* hTemp = *hand, * dTemp = *deck, * temp;

    if (type == 0) {
        while (hTemp != NULL)
        {
            temp = hTemp;
            hTemp = hTemp->next;
            free(temp);
        }
    }
    else if (type == 1) {
        while (dTemp != NULL)
        {
            temp = dTemp;
            dTemp = dTemp->next;
            free(temp);
        }
    }

    else if (type == 2) {
        while (hTemp != NULL)
        {
            temp = hTemp;
            hTemp = hTemp->next;
            free(temp);
        }
        while (dTemp != NULL)
        {
            temp = dTemp;
            dTemp = dTemp->next;
            free(temp);
        }
    }
    return;
}

void gameStats(char name[], int roundNum, int winRound, int winRS, int winTP, int winTK, int winFK, int winST, int winFL, int winFH, int winFST) {
    FILE* out;
    out = fopen("FinalProjectGameStats.txt", "a"); //writes game statistics to a file at the end of each round.
    fprintf(out, "Name: %s\nRounds played: %d Rounds won: %d\nHands played:\nTwo pair: %d\nThree of a kind: %d\nStraight: %d\nFlush: %d\nFull house: %d\nFour of a kind: %d\nStraight flush: %d\nRoyal flush: %d\n\n", name, roundNum, winRound, winTP, winTK, winST, winFL, winFH, winFK, winFST, winRS);

    fclose(out);
}

void cardGraphics(char suit[], char face[]) {
    printf(" -------  -------  -------  -------  -------\n");
    printf(" |%c%c   |  |%c%c   |  |%c%c   |  |%c%c   |  |%c%c   |\n", face[0], suit[0], face[1], suit[1], face[2], suit[2], face[3], suit[3], face[4], suit[4]);
    printf(" |     |  |     |  |     |  |     |  |     |\n");
    printf(" |   %c%c|  |   %c%c|  |   %c%c|  |   %c%c|  |   %c%c|\n", face[0], suit[0], face[1], suit[1], face[2], suit[2], face[3], suit[3], face[4], suit[4]);
    printf(" -------  -------  -------  -------  -------\n");
}

void printFace(int face) {
    //used in the recommendation section of the winnings function
    if (face == 14)
        printf("A");
    else if (face == 13)
        printf("K");
    else if (face == 12)
        printf("Q");
    else if (face == 11)
        printf("J");
    else
        printf("%d", face);
}