#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <termios.h> //Change terminal i/o settings
#include "header.h"

#define BOLD "\x1B[1m"
#define BLACK "\x1B[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

static struct termios old, current;

/*0  1  2  3  4  5  6  7
0|BR|BA|BB|BK|BQ|BB|BA|BR|\n");
 |=======================|\n");
1|BP|BP|BP|BP|BP|BP|BP|BP|\n");
 |=======================|\n");
2|  |  |  |  |  |  |  |  |\n");
 |=======================|\n");
3|  |  |  |  |  |  |  |  |\n");
 |=======================|\n");
4|  |  |  |  |  |  |  |  |\n");
 |=======================|\n");
5|  |  |  |  |  |  |  |  |\n");
 |=======================|\n");
6|WP|WP|WP|WP|WP|WP|WP|WP|\n");
 |=======================|\n");
7|WR|WA|WB|WK|WQ|WB|WA|WR|\n");
*/
int Castle[4] = {1,1,1,1};

void PrintChessboardBig(struct Piece** board,int WhoPlays){
    int i,j;
    system("clear");
    printf(BOLD BLUE);//normal - blue
    printf("   ");
    for(i=1;i<9;i++){
        printf("  %d   ",i);
    }
    printf("\n  _________________________________________________\n");
    for(i=0;i<8;i++){
        printf("  |");
        for(j=0;j<8;j++){
            printf("     |"); //5 spaces for square
        }
        printf("\n%c |",i+65);//ASCII for A,B,C,...
        for(j=0;j<8;j++){
            if(board[i][j].selected == '1'){
                printf(RED);
                if(board[i][j].team == 'X')
                    printf("  X  ");
                else if(board[i][j].team == 'W' || board[i][j].team == 'B')
                    printf("  %s ",board[i][j].toPrint);
            }
            else if(board[i][j].possible == '1'){
                printf(GREEN);
                if(board[i][j].team == 'X')
                    printf("  X  ");
                else if(board[i][j].team == 'W' || board[i][j].team == 'B')
                    printf("  %s ",board[i][j].toPrint);
            }
            else{
                if(board[i][j].team == 'W'){
                    printf(WHITE);//bold
                    printf("  %s ",board[i][j].toPrint);
                }
                else if(board[i][j].team == 'B'){
                    printf(BLACK);
                    printf("  %s ",board[i][j].toPrint);
                }
                else if(board[i][j].team == 'X')
                    printf("     ");

            }
            printf(BLUE "|");//normal - blue

        }
        printf("\n  |");
        for(j=0;j<8;j++){
            printf("     |"); //5 spaces for square
        }
        printf("\n  |===============================================|\n");
    }
    printf("\n                 ^\n");
    printf("Use Arrow Keys <   > to Navigate\n");
    printf("                 v\n");
    printf("SpaceBar to Select\n\n");
    printf("X to Cancel Selection\n\n");

    if(WhoPlays==0){
        printf("It's ");
        printf(WHITE "White");
        printf(BLUE "'s Turn!\n");
    }
    else if(WhoPlays==1){
        printf("It's ");
        printf(BLACK "Black");
        printf(BLUE "'s Turn!\n");
    }

    return;
}

void InitPiece(struct Piece *piece,char *type,char team){
    bzero(piece->type,7);
    bzero(piece->toPrint,3);
    strncpy(piece->type,type,strlen(type));
    if(!strncmp("Knight",type,5))
        strncpy(piece->toPrint,type,2);
    else if(!strncmp("Empty",type,5))
        strncpy(piece->toPrint," ",1);
    else{
        strncpy(piece->toPrint,type,1);
        piece->toPrint[1] = ' ';

    }
    piece->team = team;
    piece->selected = '0';
    piece->possible = '0';

}

void InitBoard(struct Piece **board){
    int i,j;
    struct Piece brook,bbishop,bknight,bpawn,bking,bqueen;
    struct Piece wrook,wbishop,wknight,wpawn,wking,wqueen;
    struct Piece empty;

    //EMPTY BLOCK
    InitPiece(&empty,"Empty\0",'X');
    
    //BLACK TEAM PIECES
    InitPiece(&brook,"Rook\0",'B');
    InitPiece(&bbishop,"Bishop\0",'B');
    InitPiece(&bknight,"Knight\0",'B');
    InitPiece(&bpawn,"Pawn\0",'B');
    InitPiece(&bking,"King\0",'B');
    InitPiece(&bqueen,"Queen\0",'B');

    //WHITE TEAM PIECES
    InitPiece(&wrook,"Rook\0",'W');
    InitPiece(&wbishop,"Bishop\0",'W');
    InitPiece(&wknight,"Knight\0",'W');
    InitPiece(&wpawn,"Pawn\0",'W');
    InitPiece(&wking,"King\0",'W');
    InitPiece(&wqueen,"Queen\0",'W');

    //PLACE PIECES ON BOARD
    
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            if(i==1)
                board[i][j] = bpawn;
            else if(i==6)
                board[i][j] = wpawn;
            else
                board[i][j] = empty;
        }
    }
    board[0][0] = board[0][7] = brook;
    board[0][1] = board[0][6] = bknight;
    board[0][2] = board[0][5] = bbishop;
    board[0][3] = bqueen;
    board[0][4] = bking;
    
    board[7][0] = board[7][7] = wrook;
    board[7][1] = board[7][6] = wknight;
    board[7][2] = board[7][5] = wbishop;
    board[7][3] = wqueen;
    board[7][4] = wking;

    board[4][4].selected = '1'; //start on 4,4 the reds

}

void ExecuteMove(struct Piece **board,char* start,char* finish,int WhoPlays){
    int index_x,index_y,fin_index_x,fin_index_y;
    struct Piece piece;

    index_x = start[0] - 65; //ascii A->0, B->1 etc
    index_y = atoi(&(start[1]));
    index_y = index_y - 1;

    piece = board[index_x][index_y];
    piece.selected = '0';

    fin_index_x = finish[0] - 65;//ascii A->0,B->1 etc
    fin_index_y = atoi(&(finish[1]));
    fin_index_y = fin_index_y -1;
    board[fin_index_x][fin_index_y].selected = '0';

    struct Piece empty;
    InitPiece(&empty,"Empty\0",'X');

    int castled = 0;
    //Check if Move is Castle
    if(!WhoPlays){ //White's move
        if(index_x == 7 && index_y == 4){
            //Big Castle
            if(fin_index_x == 7 && fin_index_y == 0){
                if((!strncmp(board[index_x][index_y].type,"King",4)) && (!strncmp(board[fin_index_x][fin_index_y].type,"Rook",4)) && Castle[0]==1){
                    board[7][1] = board[index_x][index_y];
                    board[7][2] = board[fin_index_x][fin_index_y];
                    board[index_x][index_y] = empty;
                    board[fin_index_x][fin_index_y] = empty;
                    Castle[0] = 0;
                    Castle[1] = 0;
                    castled = 1;
                    
                }
            }
            //Small Castle
            else if(fin_index_x == 7 && fin_index_y == 7){
                if((!strncmp(board[index_x][index_y].type,"King",4)) && (!strncmp(board[fin_index_x][fin_index_y].type,"Rook",4)) && Castle[1]==1){
                    board[7][6] = board[index_x][index_y];
                    board[7][5] = board[fin_index_x][fin_index_y];
                    board[index_x][index_y] = empty;
                    board[fin_index_x][fin_index_y] = empty;
                    Castle[0] = 0;
                    Castle[1] = 0;
                    castled = 1;
                }

            }
        }
        else if(index_x == 7 && index_y == 0){
            Castle[0] = 0;
        }
        else if(index_x == 7 && index_y == 7){
            Castle[1] = 0;
        }
    }
    else if(WhoPlays){//Black Plays
        if(index_x == 0 && index_y == 4){
            //Big Castle
            if(fin_index_x == 0 && fin_index_y == 0){
                if((!strncmp(board[index_x][index_y].type,"King",4)) && (!strncmp(board[fin_index_x][fin_index_y].type,"Rook",4)) && Castle[2]==1){
                    board[0][1] = board[index_x][index_y];
                    board[0][2] = board[fin_index_x][fin_index_y];
                    board[index_x][index_y] = empty;
                    board[fin_index_x][fin_index_y] = empty;
                    Castle[2] = 0;
                    Castle[3] = 0;
                    castled = 1;
                    
                }
            }
            //Small Castle
            else if(fin_index_x == 0 && fin_index_y == 7){
                if((!strncmp(board[index_x][index_y].type,"King",4)) && (!strncmp(board[fin_index_x][fin_index_y].type,"Rook",4)) && Castle[3]==1){
                    board[0][6] = board[index_x][index_y];
                    board[0][5] = board[fin_index_x][fin_index_y];
                    board[index_x][index_y] = empty;
                    board[fin_index_x][fin_index_y] = empty;
                    Castle[2] = 0;
                    Castle[3] = 0;
                    castled = 1;
                }

            }
        }
        else if(index_x == 0 && index_y == 0){
            Castle[2] = 0;
        }
        else if(index_x == 0 && index_y == 7){
            Castle[3] = 0;
        }
    }

    //Move the Piece (if the move is not Castle)
    if(castled == 0){
        board[fin_index_x][fin_index_y] = piece;
        board[index_x][index_y] = empty;
        if(!strncmp(piece.type,"King",4)){
            if(WhoPlays == 0){
                Castle[0] = 0;
                Castle[1] = 0;
            }
            else{
                Castle[2] = 0;
                Castle[3] = 0;
            }

        }
    }

    //Erase possible Moves
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board[i][j].possible = '0';
}

//Returns 0 if the game ends!
int ArrowKeyInput(struct Piece **board,int* x,int* y,int *WhoPlays){
    char c;
    char start[3];
    char finish[3];
    int startflag=1,endflag=1;
    while(1){
        PrintChessboardBig(board,*WhoPlays);
        if((c = getch()) == '\033') { // if the first value is esc
            getch(); // skip the [
            switch(getch()) { // the real value
                case 'A': //arrow UP ^
                    if(*x>0){
                        board[*x][*y].selected = '0';
                        board[*x-1][*y].selected = '1';
                        *x = *x - 1;
                    }
                    break;
                case 'B': //arrow DOWN v
                    if(*x<7){
                        board[*x][*y].selected = '0';
                        board[*x+1][*y].selected = '1';
                        *x = *x + 1;
                    }
                    break;
                case 'C': //arrow RIGHT >
                    if(*y<7){
                        board[*x][*y].selected = '0';
                        board[*x][*y+1].selected = '1';
                        *y = *y + 1;
                    }
                    break;
                case 'D': //arrow LEFT <
                    if(*y>0){
                        board[*x][*y].selected = '0';
                        board[*x][*y-1].selected = '1';
                        *y = *y - 1;
                    }
                    break;
            }
        }
        else if(c == ' '){//SpaceBar
            if(startflag){
                if((*WhoPlays == 0 && board[*x][*y].team == 'W') || (*WhoPlays == 1 && board[*x][*y].team == 'B')){
                    start[0] = *x+65;
                    start[1] = *y+48+1; //Ascii from number to character (+1 cause we dont have 0)
                    start[2] = '-';
                    if(PossibleMoves(board,*x,*y,*WhoPlays)){ //if there is no Possible Moves, repeat Start
                        startflag = 0;
                    }//make the squares the piece can go green
                }
            }
            else if(endflag){
                if(board[*x][*y].possible == '1'){
                    finish[0] = *x+65;
                    finish[1] = *y+48+1;
                    finish[2] = '\0';
                    endflag = 0;
                    ExecuteMove(board,start,finish,*WhoPlays);
                    
                    if(!(*WhoPlays))
                        *WhoPlays=1;
                    else
                        *WhoPlays=0;
                    
                    char team;
                    if(*WhoPlays == 0)
                        team = 'W';
                    else
                        team = 'B';
                        
                    //CheckVictory();
                    int moves_left=0;
                    for(int i=0;i<8;i++){
                        for(int j=0;j<8;j++){
                            if(board[i][j].team == team){
                                moves_left+=PossibleMoves(board,i,j,*WhoPlays);
                            }
                        }
                    }
                    if(moves_left==0){ // 'team' has no more moves -> Lost
                        PrintChessboardBig(board,*WhoPlays);
                        if(team == 'W'){
                            printf(MAGENTA "Checkmate!! ");
                            printf(BLACK "BLACK ");
                            printf(MAGENTA "Wins!\n");
                        }
                        else{
                            printf(MAGENTA "Checkmate!! ");
                            printf(WHITE "WHITE ");
                            printf(MAGENTA "Wins!\n");
                        }
                        return 0;

                    }
                    for(int i=0;i<8;i++)
                        for(int j=0;j<8;j++)
                            board[i][j].possible = '0';

                    break;
                }
            }
        }
        else if(c == 'x' || c == 'X'){
            startflag = 1;
            for(int i=0;i<8;i++)
                for(int j=0;j<8;j++)
                    board[i][j].possible = '0';
        }
    }
    return 1; //no need.. can be void
}

//return 0 if there are NO moves - 1 if there is
int PossibleMoves(struct Piece ** board,int x,int y,int WhoPlays){
    int moves_exist=0;
    if(!strncmp(board[x][y].type,"Pawn",4)){//if its a pawn

        //2 forward
        if(x==1 && board[x][y].team == 'B'){
            if(board[x+2][y].team == 'X' && board[x+1][y].team == 'X'){
                if(!Pinned(board,x,y,x+2,y,WhoPlays)){
                    board[x+2][y].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        else if(x==6 && board[x][y].team == 'W'){
            if(board[x-2][y].team == 'X' && board[x-1][y].team == 'X'){
                if(!Pinned(board,x,y,x-2,y,WhoPlays)){
                    board[x-2][y].possible = '1';
                    moves_exist = 1;
                }
            }
        }

        //kill diagwnia + 1 forward
        if(WhoPlays == 0 && x>0){//White
            if(y>0){
                if(board[x-1][y-1].team == 'B'){
                    if(!Pinned(board,x,y,x-1,y-1,WhoPlays)){
                        board[x-1][y-1].possible = '1';
                        moves_exist = 1;
                    }
                }
            }
            if(y<7){
                if(board[x-1][y+1].team == 'B'){
                    if(!Pinned(board,x,y,x-1,y+1,WhoPlays)){
                        board[x-1][y+1].possible = '1';
                        moves_exist = 1;
                    }
                }
            }
            if(board[x-1][y].team == 'X'){
                if(!Pinned(board,x,y,x-1,y,WhoPlays)){
                    board[x-1][y].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        else if(WhoPlays == 1 && x<7){//Black
            if(y>0){
                if(board[x+1][y-1].team == 'W'){
                    if(!Pinned(board,x,y,x+1,y-1,WhoPlays)){
                        board[x+1][y-1].possible = '1';
                        moves_exist = 1;
                    }
                }
            }
            if(y<7){
                if(board[x+1][y+1].team == 'W'){
                    if(!Pinned(board,x,y,x+1,y+1,WhoPlays)){
                        board[x+1][y+1].possible = '1';
                        moves_exist = 1;
                    }
                }
            }
            if(board[x+1][y].team == 'X'){
                if(!Pinned(board,x,y,x+1,y,WhoPlays)){
                    board[x+1][y].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        
    }
    if((!strncmp(board[x][y].type,"Rook",4)) || (!strncmp(board[x][y].type,"Queen",5)) ){
        int counter=x;
        while(counter-1>=0){
            if(board[counter-1][y].team == 'X'){
                if(!Pinned(board,x,y,counter-1,y,WhoPlays)){
                    board[counter-1][y].possible = '1';
                    moves_exist = 1;
                }
                counter--;
            }
            else if(board[counter-1][y].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,counter-1,y,WhoPlays)){
                    board[counter-1][y].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[counter-1][y].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,counter-1,y,WhoPlays)){
                    board[counter-1][y].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
        counter=x;
        while(counter+1<=7){
            if(board[counter+1][y].team == 'X'){
                if(!Pinned(board,x,y,counter+1,y,WhoPlays)){
                    board[counter+1][y].possible = '1';
                    moves_exist = 1;
                }
                counter++;
            }
            else if(board[counter+1][y].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,counter+1,y,WhoPlays)){
                    board[counter+1][y].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[counter+1][y].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,counter+1,y,WhoPlays)){
                    board[counter+1][y].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
        counter=y;
        while(counter-1>=0){
            if(board[x][counter-1].team == 'X'){
                if(!Pinned(board,x,y,x,counter-1,WhoPlays)){
                    board[x][counter-1].possible = '1';
                    moves_exist = 1;
                }
                counter--;
            }
            else if(board[x][counter-1].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,x,counter-1,WhoPlays)){
                    board[x][counter-1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[x][counter-1].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,x,counter-1,WhoPlays)){
                    board[x][counter-1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
        counter=y;
        while(counter+1<=7){
            if(board[x][counter+1].team == 'X'){
                if(!Pinned(board,x,y,x,counter+1,WhoPlays)){
                    board[x][counter+1].possible = '1';
                    moves_exist = 1;
                }
                counter++;
            }
            else if(board[x][counter+1].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,x,counter+1,WhoPlays)){
                    board[x][counter+1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[x][counter+1].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,x,counter+1,WhoPlays)){
                    board[x][counter+1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
    }
    if( (!strncmp(board[x][y].type,"Bishop",6)) || (!strncmp(board[x][y].type,"Queen",5)) ){
        int counter_x=x,counter_y=y;
        while(counter_x-1>=0 && counter_y-1>=0){
            if(board[counter_x-1][counter_y-1].team == 'X'){
                if(!Pinned(board,x,y,counter_x-1,counter_y-1,WhoPlays)){
                    board[counter_x-1][counter_y-1].possible = '1';
                    moves_exist = 1;
                }
                counter_x--;
                counter_y--;
            }
            else if(board[counter_x-1][counter_y-1].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,counter_x-1,counter_y-1,WhoPlays)){
                    board[counter_x-1][counter_y-1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[counter_x-1][counter_y-1].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,counter_x-1,counter_y-1,WhoPlays)){
                    board[counter_x-1][counter_y-1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
        counter_x=x;
        counter_y=y;
        while(counter_x-1>=0 && counter_y+1<=7){
            if(board[counter_x-1][counter_y+1].team == 'X'){
                if(!Pinned(board,x,y,counter_x-1,counter_y+1,WhoPlays)){
                    board[counter_x-1][counter_y+1].possible = '1';
                    moves_exist = 1;
                }
                counter_x--;
                counter_y++;
            }
            else if(board[counter_x-1][counter_y+1].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,counter_x-1,counter_y+1,WhoPlays)){
                    board[counter_x-1][counter_y+1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[counter_x-1][counter_y+1].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,counter_x-1,counter_y+1,WhoPlays)){
                    board[counter_x-1][counter_y+1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
        counter_x=x;
        counter_y=y;
        while(counter_x+1<=7 && counter_y-1>=0){
            if(board[counter_x+1][counter_y-1].team == 'X'){
                if(!Pinned(board,x,y,counter_x+1,counter_y-1,WhoPlays)){
                    board[counter_x+1][counter_y-1].possible = '1';
                    moves_exist = 1;
                }
                counter_x++;
                counter_y--;
            }
            else if(board[counter_x+1][counter_y-1].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,counter_x+1,counter_y-1,WhoPlays)){
                    board[counter_x+1][counter_y-1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[counter_x+1][counter_y-1].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,counter_x+1,counter_y-1,WhoPlays)){
                    board[counter_x+1][counter_y-1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
        counter_x=x;
        counter_y=y;
        while(counter_x+1<=7 && counter_y+1<=7){
            if(board[counter_x+1][counter_y+1].team == 'X'){
                if(!Pinned(board,x,y,counter_x+1,counter_y+1,WhoPlays)){
                    board[counter_x+1][counter_y+1].possible = '1';
                    moves_exist = 1;
                }
                counter_x++;
                counter_y++;
            }
            else if(board[counter_x+1][counter_y+1].team == 'B' && WhoPlays==0){
                if(!Pinned(board,x,y,counter_x+1,counter_y+1,WhoPlays)){
                    board[counter_x+1][counter_y+1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else if(board[counter_x+1][counter_y+1].team == 'W' && WhoPlays==1){
                if(!Pinned(board,x,y,counter_x+1,counter_y+1,WhoPlays)){
                    board[counter_x+1][counter_y+1].possible = '1';
                    moves_exist = 1;
                }
                break;
            }
            else{
                break;
            }
        }
    }
    if(!strncmp(board[x][y].type,"Knight",6)){
        //1.paei mono se Γ 
        if(x-2>=0){
            if(y-1>=0 && ((board[x-2][y-1].team == 'W' && WhoPlays == 1 ) || (board[x-2][y-1].team == 'B' && WhoPlays == 0) || board[x-2][y-1].team == 'X' )){
                if(!Pinned(board,x,y,x-2,y-1,WhoPlays)){
                    board[x-2][y-1].possible = '1';
                    moves_exist = 1;
                }
            }
            if(y+1<=7 && ((board[x-2][y+1].team == 'W' && WhoPlays == 1 ) || (board[x-2][y+1].team == 'B' && WhoPlays == 0) || board[x-2][y+1].team == 'X' )){
                if(!Pinned(board,x,y,x-2,y+1,WhoPlays)){
                    board[x-2][y+1].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        if(x-1>=0){
            if(y-2>=0 && ((board[x-1][y-2].team == 'W' && WhoPlays == 1 ) || (board[x-1][y-2].team == 'B' && WhoPlays == 0) || board[x-1][y-2].team == 'X' )){
                if(!Pinned(board,x,y,x-1,y-2,WhoPlays)){
                    board[x-1][y-2].possible = '1';
                    moves_exist = 1;
                }
            }
            if(y+2<=7 && ((board[x-1][y+2].team == 'W' && WhoPlays == 1 ) || (board[x-1][y+2].team == 'B' && WhoPlays == 0) || board[x-1][y+2].team == 'X' )){
                if(!Pinned(board,x,y,x-1,y+2,WhoPlays)){
                    board[x-1][y+2].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        if(x+2<=7){
            if(y-1>=0 && ((board[x+2][y-1].team == 'W' && WhoPlays == 1 ) || (board[x+2][y-1].team == 'B' && WhoPlays == 0) || board[x+2][y-1].team == 'X' )){
                if(!Pinned(board,x,y,x+2,y-1,WhoPlays)){
                    board[x+2][y-1].possible = '1';
                    moves_exist = 1;
                }
            }
            if(y+1<=7 && ((board[x+2][y+1].team == 'W' && WhoPlays == 1 ) || (board[x+2][y+1].team == 'B' && WhoPlays == 0) || board[x+2][y+1].team == 'X' )){
                if(!Pinned(board,x,y,x+2,y+1,WhoPlays)){
                    board[x+2][y+1].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        if(x+1<=7){
            if(y-2>=0 && ((board[x+1][y-2].team == 'W' && WhoPlays == 1 ) || (board[x+1][y-2].team == 'B' && WhoPlays == 0) || board[x+1][y-2].team == 'X' )){
                if(!Pinned(board,x,y,x+1,y-2,WhoPlays)){
                    board[x+1][y-2].possible = '1';
                    moves_exist = 1;
                }
            }
            if(y+2<=7 && ((board[x+1][y+2].team == 'W' && WhoPlays == 1 ) || (board[x+1][y+2].team == 'B' && WhoPlays == 0) || board[x+1][y+2].team == 'X' )){
                if(!Pinned(board,x,y,x+1,y+2,WhoPlays)){
                    board[x+1][y+2].possible = '1';
                    moves_exist = 1;
                }
            }

        }
    }
    if(!strncmp(board[x][y].type,"King",4)){
        if(x-1>=0){
            //TOP
            if((WhoPlays == 0 && board[x-1][y].team == 'B') || (WhoPlays == 1 && board[x-1][y].team == 'W') || (board[x-1][y].team == 'X')){
                if(!Pinned(board,x,y,x-1,y,WhoPlays)){
                    board[x-1][y].possible = '1';
                    moves_exist = 1;
                }

            }
            //TOP-LEFT
            if(y-1>=0 && ((WhoPlays == 0 && board[x-1][y-1].team == 'B') || (WhoPlays == 1 && board[x-1][y-1].team == 'W') || (board[x-1][y-1].team == 'X'))){
                if(!Pinned(board,x,y,x-1,y-1,WhoPlays)){
                    board[x-1][y-1].possible = '1';
                    moves_exist = 1;
                }
            }
            //TOP-RIGHT
            if(y+1<=7 && ((WhoPlays == 0 && board[x-1][y+1].team == 'B') || (WhoPlays == 1 && board[x-1][y+1].team == 'W') || (board[x-1][y+1].team == 'X'))){
                if(!Pinned(board,x,y,x-1,y+1,WhoPlays)){
                    board[x-1][y+1].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        if(x+1<=7){
            //BOTTOM
            if((WhoPlays == 0 && board[x+1][y].team == 'B') || (WhoPlays == 1 && board[x+1][y].team == 'W') || (board[x+1][y].team == 'X')){
                if(!Pinned(board,x,y,x+1,y,WhoPlays)){
                    board[x+1][y].possible = '1';
                    moves_exist = 1;
                }

            }
            //BOTTOM-LEFT
            if(y-1>=0 && ((WhoPlays == 0 && board[x+1][y-1].team == 'B') || (WhoPlays == 1 && board[x+1][y-1].team == 'W') || (board[x+1][y-1].team == 'X'))){
                if(!Pinned(board,x,y,x+1,y-1,WhoPlays)){
                    board[x+1][y-1].possible = '1';
                    moves_exist = 1;
                }
            }
            //BOTTOM-RIGHT
            if(y+1<=7 && ((WhoPlays == 0 && board[x+1][y+1].team == 'B') || (WhoPlays == 1 && board[x+1][y+1].team == 'W') || (board[x+1][y+1].team == 'X'))){
                if(!Pinned(board,x,y,x+1,y+1,WhoPlays)){
                    board[x+1][y+1].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        //LEFT
        if((WhoPlays == 0 && board[x][y-1].team == 'B') || (WhoPlays == 1 && board[x][y-1].team == 'W') || (board[x][y-1].team == 'X')){
            if(!Pinned(board,x,y,x,y-1,WhoPlays)){
                board[x][y-1].possible = '1';
                moves_exist = 1;
            }
        }

        //RIGHT
        if((WhoPlays == 0 && board[x][y+1].team == 'B') || (WhoPlays == 1 && board[x][y+1].team == 'W') || (board[x][y+1].team == 'X')){
            if(!Pinned(board,x,y,x,y+1,WhoPlays)){
                board[x][y+1].possible = '1';
                moves_exist = 1;
            }
        }

        //Castle[0]: WBig (left)
        //Castle[1]: WSmall (right)
        //[2]: BBig (left)
        //[3]: BSmall (right)
        if(WhoPlays == 0 && Castle[0] == 1){
            if(board[7][1].team == 'X' && board[7][2].team == 'X' && board[7][3].team == 'X'){
                //if king is not in check now & after Castle
                if((!KingOnCheck(board,WhoPlays)) && (!Pinned(board,7,4,7,1,WhoPlays))){
                    board[7][0].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        if(WhoPlays == 0 && Castle[1] == 1){
            if(board[7][5].team == 'X' && board[7][6].team == 'X'){
                if((!KingOnCheck(board,WhoPlays)) && (!Pinned(board,7,4,7,6,WhoPlays))){
                    board[7][7].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        if(WhoPlays == 1 && Castle[2] == 1){
            if(board[0][1].team == 'X' && board[0][2].team == 'X' && board[0][3].team == 'X'){
                if((!KingOnCheck(board,WhoPlays)) && (!Pinned(board,0,4,0,1,WhoPlays))){
                    board[0][0].possible = '1';
                    moves_exist = 1;
                }
            }
        }
        if(WhoPlays == 1 && Castle[3] == 1){
            if(board[0][5].team == 'X' && board[0][6].team == 'X'){
                if((!KingOnCheck(board,WhoPlays)) && (!Pinned(board,0,4,0,6,WhoPlays))){
                    board[0][7].possible = '1';
                    moves_exist = 1;
                }
            }
        }
    }

    return moves_exist;
}

//Returns 0 if it's not Pinned - 1 if it is
int Pinned(struct Piece **board,int start_x,int start_y,int fin_x,int fin_y,int WhoPlays){
    int pinned = 0;
    struct Piece tmp_piece,empty;
    InitPiece(&empty,"Empty",'X');

    //1.Do the Move
    tmp_piece = board[fin_x][fin_y];
    board[fin_x][fin_y] = board[start_x][start_y];
    board[start_x][start_y] = empty;

    //2.Check if King is on check after move (1:king is on check / 0:king is safe)
    if(KingOnCheck(board,WhoPlays))
        pinned=1;

    //3.Undo the move
    board[start_x][start_y] = board[fin_x][fin_y];
    board[fin_x][fin_y] = tmp_piece;

    //4.return 1/0;
    return pinned;

}

int KingOnCheck(struct Piece **board,int WhoPlays){
    char team,enemy;
    int x,y;
    if(!WhoPlays){
        team = 'W';
        enemy = 'B';
    }
    else{
        team = 'B';
        enemy = 'W';
    }
    //1.Search King
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board[i][j].team == team && (!strncmp(board[i][j].type,"King",4))){
                x=i;
                y=j;
            }
        }
    }

    //[x][y] = King

    //2.See if he is on Check

    //printf("Its pawn\n");
    if(team == 'W'){
        //Pawn
        if(x-1>=0){
            if(y-1>=0){
                if(board[x-1][y-1].team == 'B' && (!strncmp(board[x-1][y-1].type,"Pawn",4))){
                    return 1;
                }

            }
            if(y+1<=7){
                if(board[x-1][y+1].team == 'B' && (!strncmp(board[x-1][y+1].type,"Pawn",4))){
                    return 1;
                }
            }
        }
    }
    if(team == 'B'){
        //Pawn
        if(x+1>=0){
            if(y-1>=0){
                if(board[x+1][y-1].team == 'W' && (!strncmp(board[x+1][y-1].type,"Pawn",4))){
                    return 1;
                }

            }
            if(y+1<=7){
                if(board[x+1][y+1].team == 'W' && (!strncmp(board[x+1][y+1].type,"Pawn",4))){
                    return 1;
                }
            }
        }
    }

    //printf("No Its rook-queen\n");
    //Rook - Queen
    int counter=x;
    while(counter-1>=0){
        if(board[counter-1][y].team == enemy && ((!strncmp(board[counter-1][y].type,"Rook",4)) || (!strncmp(board[counter-1][y].type,"Queen",5)))){
            return 1;
        }
        else if(board[counter-1][y].team != 'X'){
            break;
        }
        counter--;
    }
    counter=x;
    while(counter+1<=7){
        if(board[counter+1][y].team == enemy && ((!strncmp(board[counter+1][y].type,"Rook",4)) || (!strncmp(board[counter+1][y].type,"Queen",5)))){
            return 1;
        }
        else if(board[counter+1][y].team != 'X'){
            break;
        }
        counter++;
    }
    counter = y;
    while(counter-1>=0){
        if(board[x][counter-1].team == enemy && ((!strncmp(board[x][counter-1].type,"Rook",4)) || (!strncmp(board[x][counter-1].type,"Queen",5)))){
            return 1;
        }
        else if(board[x][counter-1].team != 'X'){
            break;
        }
        counter--;
    }
    counter = y;
    while(counter+1<=7){
        if(board[x][counter+1].team == enemy && ((!strncmp(board[x][counter+1].type,"Rook",4)) || (!strncmp(board[x][counter+1].type,"Queen",5)))){
            return 1;
        }
        else if(board[x][counter+1].team != 'X'){
            break;
        }
        counter++;
    }

    //printf("no Its bishop - queen\n");
    //Bishop - Queen
    int counter_x=x;
    int counter_y=y;
    while(counter_x-1>=0 && counter_y-1>=0){
        if(board[counter_x-1][counter_y-1].team == enemy && ((!strncmp(board[counter_x-1][counter_y-1].type,"Bishop",6)) || (!strncmp(board[counter_x-1][counter_y-1].type,"Queen",5)))){
            return 1;
        }
        else if(board[counter_x-1][counter_y-1].team != 'X'){
            break;
        }
        counter_x--;
        counter_y--;
    }
    counter_x=x;
    counter_y=y;
    while(counter_x-1>=0 && counter_y+1<=7){
        if(board[counter_x-1][counter_y+1].team == enemy && ((!strncmp(board[counter_x-1][counter_y+1].type,"Bishop",6)) || (!strncmp(board[counter_x-1][counter_y+1].type,"Queen",5)))){
            return 1;
        }
        else if(board[counter_x-1][counter_y+1].team != 'X'){
            break;
        }
        counter_x--;
        counter_y++;
    }
    counter_x=x;
    counter_y=y;
    while(counter_x+1<=7 && counter_y-1>=0){
        if(board[counter_x+1][counter_y-1].team == enemy && ((!strncmp(board[counter_x+1][counter_y-1].type,"Bishop",6)) || (!strncmp(board[counter_x+1][counter_y-1].type,"Queen",5)))){
            return 1;
        }
        else if(board[counter_x+1][counter_y-1].team != 'X'){
            break;
        }
        counter_x++;
        counter_y--;
    }
    counter_x=x;
    counter_y=y;
    while(counter_x+1<=7 && counter_y+1<=7){
        if(board[counter_x+1][counter_y+1].team == enemy && ((!strncmp(board[counter_x+1][counter_y+1].type,"Bishop",6)) || (!strncmp(board[counter_x+1][counter_y+1].type,"Queen",5)))){
            return 1;
        }
        else if(board[counter_x+1][counter_y+1].team != 'X'){
            break;
        }
        counter_x++;
        counter_y++;
    }


    //printf("no Its knight\n");
    //Knight

    if(x-2>=0){
        if(y-1>=0){
            if(board[x-2][y-1].team == enemy && (!strncmp(board[x-2][y-1].type,"Knight",6))){
                return 1;
            }
        }
        if(y+1<=7){
            if(board[x-2][y+1].team == enemy && (!strncmp(board[x-2][y+1].type,"Knight",6))){
                return 1;
            }
        }
    }
    if(x-1>=0){
        if(y-2>=0){
            if(board[x-1][y-2].team == enemy && (!strncmp(board[x-1][y-2].type,"Knight",6))){
                return 1;
            }
        }
        if(y+2<=7){
            if(board[x-1][y+2].team == enemy && (!strncmp(board[x-1][y+2].type,"Knight",6))){
                return 1;
            }
        }
    }
    if(x+2<=7){
        if(y-1>=0){
            if(board[x+2][y-1].team == enemy && (!strncmp(board[x+2][y-1].type,"Knight",6))){
                return 1;
            }
        }
        if(y+1<=7){
            if(board[x+2][y+1].team == enemy && (!strncmp(board[x+2][y+1].type,"Knight",6))){
                return 1;
            }
        }
    }
    if(x+1<=7){
        if(y-2>=0){
            if(board[x+1][y-2].team == enemy && (!strncmp(board[x+1][y-2].type,"Knight",6))){
                return 1;
            }
        }
        if(y+2<=7){
            if(board[x+1][y+2].team == enemy && (!strncmp(board[x+1][y+2].type,"Knight",6))){
                return 1;
            }
        }

    }

    //printf("no Its king\n");
    //King
    if(x-1>=0){
        //TOP
        if(board[x-1][y].team == enemy && (!strncmp(board[x-1][y].type,"King",4))){
            return 1;
        }
        //TOP-LEFT
        if(y-1>=0){
            if(board[x-1][y-1].team == enemy && (!strncmp(board[x-1][y-1].type,"King",4))){
                return 1;
            }
        }
        //TOP-RIGHT
        if(y+1<=7){
            if(board[x-1][y+1].team == enemy && (!strncmp(board[x-1][y+1].type,"King",4))){
                return 1;
            }
        }
    }
    if(x+1<=7){
        //BOTTOM
        if(board[x+1][y].team == enemy && (!strncmp(board[x+1][y].type,"King",4))){
            return 1;
        }
        //BOTTOM-LEFT
        if(y-1>=0){
            if(board[x+1][y-1].team == enemy && (!strncmp(board[x+1][y-1].type,"King",4))){
                return 1;
            }
        }
        //BOTTOM-RIGHT
        if(y+1<=7){
            if(board[x+1][y+1].team == enemy && (!strncmp(board[x+1][y+1].type,"King",4))){
                return 1;
            }
        }
    }
    //LEFT
    if(y-1>=0){
        if(board[x][y-1].team == enemy && (!strncmp(board[x][y-1].type,"King",4))){
            return 1;
        }
    }
    //RIGHT
    if(y+1<=7){
        if(board[x][y+1].team == enemy && (!strncmp(board[x][y+1].type,"King",4))){
            return 1;
        }
    }

    printf("Nothing\n");

    return 0;



}

char getch(void){
    return getch_(0);
}

char getch_(int echo){
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    current = old; /* make new settings same as old settings */
    current.c_lflag &= ~ICANON; /* disable buffered i/o */
    if(echo){
        current.c_lflag |= ECHO; /* set echo mode */
    }
    else{
        current.c_lflag &= ~ECHO; /* set no echo mode */
    }
    tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void){
    tcsetattr(0, TCSANOW, &old);
}
