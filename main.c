#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, const char *argv[]){
    int i;
    int WhoPlays=0;
    struct Piece** Board=NULL; 
    int select_x=4,select_y=4;
    Board = malloc(8*sizeof(struct Piece*));
    for(i=0;i<8;i++)
        Board[i] = malloc(8*sizeof(struct Piece)); 

    InitBoard(Board);
    PrintChessboardBig(Board,WhoPlays);


    while(ArrowKeyInput(Board,&select_x,&select_y,&WhoPlays)){}

    for(i=0;i<8;i++)
        free(Board[i]);
    free(Board);
    
    return 0;
}
