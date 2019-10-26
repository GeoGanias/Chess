#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int Castle[4] = {1,1,1,1};

void PrintChessboard(struct Piece** board){
    int i,j;
    /*printf("   1  2  3  4  5  6  7  8\n");
    printf("  ________________________\n");
    printf("A |BR|BA|BB|BK|BQ|BB|BA|BR|\n");
    printf("  |=======================|\n");
    printf("B |BP|BP|BP|BP|BP|BP|BP|BP|\n");
    printf("  |=======================|\n");
    printf("C |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("D |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("E |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("F |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("G |WP|WP|WP|WP|WP|WP|WP|WP|\n");
    printf("  |=======================|\n");
    printf("H |WR|WA|WB|WK|WQ|WB|WA|WR|\n");
*/
    system("clear");
    printf(BLUE);//normal - blue
    printf("   1  2  3  4  5  6  7  8\n");
    printf("  ________________________\n");
    for(i=0;i<8;i++){
        printf("%c |",i+65);//ASCII for A,B,C,...
        for(j=0;j<8;j++){
            printf(RED);//bold
            printf("%s",board[i][j].toPrint);
            printf(BLUE "|");//normal - blue

        }
        printf("\n  |=======================|\n");
    }
    return;
}

void PrintChessboardBig(struct Piece** board,int WhoPlays){
    int i,j;
    /*printf("   1  2  3  4  5  6  7  8\n");
    printf("  ________________________\n");
    printf("A |BR|BA|BB|BK|BQ|BB|BA|BR|\n");
    printf("  |=======================|\n");
    printf("B |BP|BP|BP|BP|BP|BP|BP|BP|\n");
    printf("  |=======================|\n");
    printf("C |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("D |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("E |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("F |  |  |  |  |  |  |  |  |\n");
    printf("  |=======================|\n");
    printf("G |WP|WP|WP|WP|WP|WP|WP|WP|\n");
    printf("  |=======================|\n");
    printf("H |WR|WA|WB|WK|WQ|WB|WA|WR|\n");
*/
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
                    printf(" %s  ",board[i][j].toPrint);
            }
            else if(board[i][j].possible == '1'){
                printf(GREEN);
                if(board[i][j].team == 'X')
                    printf("  X  ");
                else if(board[i][j].team == 'W' || board[i][j].team == 'B')
                    printf(" %s  ",board[i][j].toPrint);
            }
            else{
                if(board[i][j].team == 'W'){
                    printf(WHITE);//bold
                    printf(" %s  ",board[i][j].toPrint);
                }
                else if(board[i][j].team == 'B'){
                    printf(BLACK);
                    printf(" %s  ",board[i][j].toPrint);
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


void InitBoard(struct Piece **board){
    int i,j;
    struct Piece brook,bbishop,balogo,bpawn,bking,bqueen;
    struct Piece wrook,wbishop,walogo,wpawn,wking,wqueen;
    struct Piece empty;
    strcpy(empty.type,"Empty");
    strcpy(empty.toPrint,"  ");
    empty.team = 'X';
    empty.selected = '0';
    empty.possible = '0';
    
    //BLACK TEAM PIECES

    //bzero(brook.type);
    strncpy(brook.type,"Rook\0",5);
    memset(brook.toPrint,'\0',10);
    strncpy(brook.toPrint,"BR\0",3);
    brook.team = 'B';
    brook.selected = '0';
    brook.possible = '0';

    strncpy(bbishop.type,"Bishop\0",7);
    strncpy(bbishop.toPrint,"BB\0",3);
    bbishop.team = 'B';
    bbishop.selected = '0';
    bbishop.possible = '0';

    strncpy(balogo.type,"Alogo\0",6);
    strncpy(balogo.toPrint,"BA\0",3);
    balogo.team = 'B';
    balogo.selected = '0';
    balogo.possible = '0';

    strncpy(bking.type,"King\0",5);
    strncpy(bking.toPrint,"BK\0",3);
    bking.team = 'B';
    bking.selected = '1';
    bking.possible = '0';

    strncpy(bqueen.type,"Queen\0",6);
    strncpy(bqueen.toPrint,"BQ\0",3);
    bqueen.team = 'B';
    bqueen.selected = '0';
    bqueen.possible = '0';

    strncpy(bpawn.type,"Pawn\0",5);
    strncpy(bpawn.toPrint,"BP\0",3);
    bpawn.team = 'B';
    bpawn.selected = '0';
    bpawn.possible = '0';

    //WHITE TEAM PIECES

    strncpy(wrook.type,"Rook\0",5);
    strncpy(wrook.toPrint,"WR\0",3);
    wrook.team = 'W';
    wrook.selected = '0';
    wrook.possible = '0';

    strncpy(wbishop.type,"Bishop\0",7);
    strncpy(wbishop.toPrint,"WB\0",3);
    wbishop.team = 'W';
    wbishop.selected = '0';
    wbishop.possible = '0';

    strncpy(walogo.type,"Alogo\0",6);
    strncpy(walogo.toPrint,"WA\0",3);
    walogo.team = 'W';
    walogo.selected = '0';
    walogo.possible = '0';

    strncpy(wking.type,"King\0",5);
    strncpy(wking.toPrint,"WK\0",3);
    wking.team = 'W';
    wking.selected = '0';
    wking.possible = '0';

    strncpy(wqueen.type,"Queen\0",6);
    strncpy(wqueen.toPrint,"WQ\0",3);
    wqueen.team = 'W';
    wqueen.selected = '0';
    wqueen.possible = '0';

    strncpy(wpawn.type,"Pawn\0",5);
    strncpy(wpawn.toPrint,"WP\0",3);
    wpawn.team = 'W';
    wpawn.selected = '0';
    wpawn.possible = '0';

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
    board[0][1] = board[0][6] = balogo;
    board[0][2] = board[0][5] = bbishop;
    board[0][3] = bqueen;
    board[0][4] = bking;
    
    board[7][0] = board[7][7] = wrook;
    board[7][1] = board[7][6] = walogo;
    board[7][2] = board[7][5] = wbishop;
    board[7][3] = wqueen;
    board[7][4] = wking;

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
    strncpy(empty.type,"Empty\0",6);
    empty.team = 'X';
    empty.selected = '0';
    empty.possible = '0';
    strncpy(empty.toPrint,"  \0",3);

    int castled = 0;
    //Check if Move is Castle
    if(WhoPlays == 0){

        if(index_x == 7 && index_y == 4){
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
    if(WhoPlays == 1){
        if(index_x == 0 && index_y == 4){
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

    //Move the Piece
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

    //Check if move Ruins Castle

    //Erase possible Moves
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board[i][j].possible = '0';
}

void ArrowKeyInput(struct Piece **board,int* x,int* y,int WhoPlays){
    char c;
    char start[3];
    char finish[3];
    int startflag=1,endflag=1;
    while(1){
        PrintChessboardBig(board,WhoPlays);
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
                if((WhoPlays == 0 && board[*x][*y].team == 'W') || (WhoPlays == 1 && board[*x][*y].team == 'B')){
                    start[0] = *x+65;
                    start[1] = *y+48+1; //Ascii from number to character (+1 cause we dont have 0)
                    start[2] = '-';
                    if(PossibleMoves(board,*x,*y,WhoPlays)){ //if there is no Possible Moves, repeat Start
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
                    ExecuteMove(board,start,finish,WhoPlays);
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
    return; //no need.. can be void
}

//return 0 if there are NO moves - 1 if there is
int PossibleMoves(struct Piece ** board,int x,int y,int WhoPlays){
    printf("x:%d y:%d %c %s\n",x,y,board[x][y].team,board[x][y].type);
    int moves_exist=0;
    if(!strncmp(board[x][y].type,"Pawn",4)){//if its a pawn

        //2 forward
        if(x==1 && board[x][y].team == 'B'){
            if(board[x+2][y].team == 'X'){
                board[x+2][y].possible = '1';
                moves_exist = 1;
            }
        }
        else if(x==6 && board[x][y].team == 'W'){
            if(board[x-2][y].team == 'X'){
                board[x-2][y].possible = '1';
                moves_exist = 1;
            }
        }

        //kill diagwnia + 1 forward
        if(WhoPlays == 0 && x>0){//White
            if(y>0){
                if(board[x-1][y-1].team == 'B'){
                    board[x-1][y-1].possible = '1';
                    moves_exist = 1;
                }
            }
            if(y<7){
                if(board[x-1][y+1].team == 'B'){
                    board[x-1][y+1].possible = '1';
                    moves_exist = 1;
                }
            }
            if(board[x-1][y].team == 'X'){
                board[x-1][y].possible = '1';
                moves_exist = 1;
            }
        }
        else if(WhoPlays == 1 && x<7){//Black
            if(y>0){
                if(board[x+1][y-1].team == 'W'){
                    board[x+1][y-1].possible = '1';
                    moves_exist = 1;
                }
            }
            if(y<7){
                if(board[x+1][y+1].team == 'W'){
                    board[x+1][y+1].possible = '1';
                    moves_exist = 1;
                }
            }
            if(board[x+1][y].team == 'X'){
                board[x+1][y].possible = '1';
                moves_exist = 1;
            }
        }
        
    }
    if((!strncmp(board[x][y].type,"Rook",4)) || (!strncmp(board[x][y].type,"Queen",5)) ){
        int counter=x;
        while(counter-1>=0){
            if(board[counter-1][y].team == 'X'){
                board[counter-1][y].possible = '1';
                moves_exist = 1;
                counter--;
            }
            else if(board[counter-1][y].team == 'B' && WhoPlays==0){
                board[counter-1][y].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[counter-1][y].team == 'W' && WhoPlays==1){
                board[counter-1][y].possible = '1';
                moves_exist = 1;
                break;
            }
            else{
                break;
            }
        }
        counter=x;
        while(counter+1<=7){
            if(board[counter+1][y].team == 'X'){
                board[counter+1][y].possible = '1';
                moves_exist = 1;
                counter++;
            }
            else if(board[counter+1][y].team == 'B' && WhoPlays==0){
                board[counter+1][y].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[counter+1][y].team == 'W' && WhoPlays==1){
                board[counter+1][y].possible = '1';
                moves_exist = 1;
                break;
            }
            else{
                break;
            }
        }
        counter=y;
        while(counter-1>=0){
            if(board[x][counter-1].team == 'X'){
                board[x][counter-1].possible = '1';
                moves_exist = 1;
                counter--;
            }
            else if(board[x][counter-1].team == 'B' && WhoPlays==0){
                board[x][counter-1].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[x][counter-1].team == 'W' && WhoPlays==1){
                board[x][counter-1].possible = '1';
                moves_exist = 1;
                break;
            }
            else{
                break;
            }
        }
        counter=y;
        while(counter+1<=7){
            if(board[x][counter+1].team == 'X'){
                board[x][counter+1].possible = '1';
                moves_exist = 1;
                counter++;
            }
            else if(board[x][counter+1].team == 'B' && WhoPlays==0){
                board[x][counter+1].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[x][counter+1].team == 'W' && WhoPlays==1){
                board[x][counter+1].possible = '1';
                moves_exist = 1;
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
                board[counter_x-1][counter_y-1].possible = '1';
                moves_exist = 1;
                counter_x--;
                counter_y--;
            }
            else if(board[counter_x-1][counter_y-1].team == 'B' && WhoPlays==0){
                board[counter_x-1][counter_y-1].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[counter_x-1][counter_y-1].team == 'W' && WhoPlays==1){
                board[counter_x-1][counter_y-1].possible = '1';
                moves_exist = 1;
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
                board[counter_x-1][counter_y+1].possible = '1';
                moves_exist = 1;
                counter_x--;
                counter_y++;
            }
            else if(board[counter_x-1][counter_y+1].team == 'B' && WhoPlays==0){
                board[counter_x-1][counter_y+1].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[counter_x-1][counter_y+1].team == 'W' && WhoPlays==1){
                board[counter_x-1][counter_y+1].possible = '1';
                moves_exist = 1;
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
                board[counter_x+1][counter_y-1].possible = '1';
                moves_exist = 1;
                counter_x++;
                counter_y--;
            }
            else if(board[counter_x+1][counter_y-1].team == 'B' && WhoPlays==0){
                board[counter_x+1][counter_y-1].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[counter_x+1][counter_y-1].team == 'W' && WhoPlays==1){
                board[counter_x+1][counter_y-1].possible = '1';
                moves_exist = 1;
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
                board[counter_x+1][counter_y+1].possible = '1';
                moves_exist = 1;
                counter_x++;
                counter_y++;
            }
            else if(board[counter_x+1][counter_y+1].team == 'B' && WhoPlays==0){
                board[counter_x+1][counter_y+1].possible = '1';
                moves_exist = 1;
                break;
            }
            else if(board[counter_x+1][counter_y+1].team == 'W' && WhoPlays==1){
                board[counter_x+1][counter_y+1].possible = '1';
                moves_exist = 1;
                break;
            }
            else{
                break;
            }
        }
    }
    if(!strncmp(board[x][y].type,"Alogo",5)){
        //1.paei mono se Γ 
        if(x-2>=0){
            if(y-1>=0 && ((board[x-2][y-1].team == 'W' && WhoPlays == 1 ) || (board[x-2][y-1].team == 'B' && WhoPlays == 0) || board[x-2][y-1].team == 'X' )){
                board[x-2][y-1].possible = '1';
                moves_exist = 1;
            }
            if(y+1<=7 && ((board[x-2][y+1].team == 'W' && WhoPlays == 1 ) || (board[x-2][y+1].team == 'B' && WhoPlays == 0) || board[x-2][y+1].team == 'X' )){
                board[x-2][y+1].possible = '1';
                moves_exist = 1;
            }
        }
        if(x-1>=0){
            if(y-2>=0 && ((board[x-1][y-2].team == 'W' && WhoPlays == 1 ) || (board[x-1][y-2].team == 'B' && WhoPlays == 0) || board[x-1][y-2].team == 'X' )){
                board[x-1][y-2].possible = '1';
                moves_exist = 1;
            }
            if(y+2<=7 && ((board[x-1][y+2].team == 'W' && WhoPlays == 1 ) || (board[x-1][y+2].team == 'B' && WhoPlays == 0) || board[x-1][y+2].team == 'X' )){
                board[x-1][y+2].possible = '1';
                moves_exist = 1;
            }
        }
        if(x+2<=7){
            if(y-1>=0 && ((board[x+2][y-1].team == 'W' && WhoPlays == 1 ) || (board[x+2][y-1].team == 'B' && WhoPlays == 0) || board[x+2][y-1].team == 'X' )){
                board[x+2][y-1].possible = '1';
                moves_exist = 1;
            }
            if(y+1<=7 && ((board[x+2][y+1].team == 'W' && WhoPlays == 1 ) || (board[x+2][y+1].team == 'B' && WhoPlays == 0) || board[x+2][y+1].team == 'X' )){
                board[x+2][y+1].possible = '1';
                moves_exist = 1;
            }
        }
        if(x+1<=7){
            if(y-2>=0 && ((board[x+1][y-2].team == 'W' && WhoPlays == 1 ) || (board[x+1][y-2].team == 'B' && WhoPlays == 0) || board[x+1][y-2].team == 'X' )){
                board[x+1][y-2].possible = '1';
                moves_exist = 1;
            }
            if(y+2<=7 && ((board[x+1][y+2].team == 'W' && WhoPlays == 1 ) || (board[x+1][y+2].team == 'B' && WhoPlays == 0) || board[x+1][y+2].team == 'X' )){
                board[x+1][y+2].possible = '1';
                moves_exist = 1;
            }

        }
    }
    if(!strncmp(board[x][y].type,"King",4)){
        if(x-1>=0){
            //TOP
            if((WhoPlays == 0 && board[x-1][y].team == 'B') || (WhoPlays == 1 && board[x-1][y].team == 'W') || (board[x-1][y].team == 'X')){
                board[x-1][y].possible = '1';
                moves_exist = 1;

            }
            //TOP-LEFT
            if(y-1>=0 && ((WhoPlays == 0 && board[x-1][y-1].team == 'B') || (WhoPlays == 1 && board[x-1][y-1].team == 'W') || (board[x-1][y-1].team == 'X'))){
                board[x-1][y-1].possible = '1';
                moves_exist = 1;
            }
            //TOP-RIGHT
            if(y+1<=7 && ((WhoPlays == 0 && board[x-1][y+1].team == 'B') || (WhoPlays == 1 && board[x-1][y+1].team == 'W') || (board[x-1][y+1].team == 'X'))){
                board[x-1][y+1].possible = '1';
                moves_exist = 1;
            }
        }
        if(x+1<=7){
            //BOTTOM
            if((WhoPlays == 0 && board[x+1][y].team == 'B') || (WhoPlays == 1 && board[x+1][y].team == 'W') || (board[x+1][y].team == 'X')){
                board[x+1][y].possible = '1';
                moves_exist = 1;

            }
            //BOTTOM-LEFT
            if(y-1>=0 && ((WhoPlays == 0 && board[x+1][y-1].team == 'B') || (WhoPlays == 1 && board[x+1][y-1].team == 'W') || (board[x+1][y-1].team == 'X'))){
                board[x+1][y-1].possible = '1';
                moves_exist = 1;
            }
            //BOTTOM-RIGHT
            if(y+1<=7 && ((WhoPlays == 0 && board[x+1][y+1].team == 'B') || (WhoPlays == 1 && board[x+1][y+1].team == 'W') || (board[x+1][y+1].team == 'X'))){
                board[x+1][y+1].possible = '1';
                moves_exist = 1;
            }
        }
        //LEFT
        if((WhoPlays == 0 && board[x][y-1].team == 'B') || (WhoPlays == 1 && board[x][y-1].team == 'W') || (board[x][y-1].team == 'X')){
            board[x][y-1].possible = '1';
            moves_exist = 1;

        }
        //RIGHT
        if((WhoPlays == 0 && board[x][y+1].team == 'B') || (WhoPlays == 1 && board[x][y+1].team == 'W') || (board[x][y+1].team == 'X')){
            board[x][y+1].possible = '1';
            moves_exist = 1;

        }
        //Castle[0]: WBig (left)
        //Castle[1]: WSmall (right)
        //[2]: BBig (left)
        //[3]: BSmall (right)
        for(int i=0;i<4;i++){
            printf("cas:%d\n",Castle[i]);
        }
        if(WhoPlays == 0 && Castle[0] == 1){
            if(board[7][1].team == 'X' && board[7][2].team == 'X' && board[7][3].team == 'X'){
                board[7][0].possible = '1';
                moves_exist = 1;
            }
        }
        if(WhoPlays == 0 && Castle[1] == 1){
            if(board[7][5].team == 'X' && board[7][6].team == 'X'){
                board[7][7].possible = '1';
                moves_exist = 1;
            }
        }
        if(WhoPlays == 1 && Castle[2] == 1){
            if(board[0][1].team == 'X' && board[0][2].team == 'X' && board[0][3].team == 'X'){
                board[0][0].possible = '1';
                moves_exist = 1;
            }
        }
        if(WhoPlays == 1 && Castle[3] == 1){
            if(board[0][5].team == 'X' && board[0][6].team == 'X'){
                board[0][7].possible = '1';
                moves_exist = 1;
            }
        }
        //1.paei mono 1 square makrua
        //2.Castle
    }

    return moves_exist;
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
