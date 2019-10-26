#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// 0 - 1 - 2 - 3
// WBig - WSmall - BBig - BSmall 
//int Castle[4] = {1};//1 = Can Castle , 0 = Can't Castle

int main(int argc, const char *argv[]){
    int i;
    int WhoPlays=0;
    struct Piece** Board=NULL; 
    int select_x=0,select_y=4;
    Board = malloc(8*sizeof(struct Piece*));
    for(i=0;i<8;i++){
        Board[i] = malloc(8*sizeof(struct Piece));
    }
    InitBoard(Board);
    PrintChessboardBig(Board,WhoPlays);


    while(1){
        if(WhoPlays==0)
            printf("It's White's Turn!\n");
        else
            printf("It's Black's Turn!\n");

        //0 = white
        //1 = black
        printf("Write Your Move: (e.g.: G5-E5)\n");
        ArrowKeyInput(Board,&select_x,&select_y,WhoPlays);
        
        if(WhoPlays==0)
            WhoPlays=1;
        else
            WhoPlays=0;

        printf("\n");
    }
    return 0;
}
