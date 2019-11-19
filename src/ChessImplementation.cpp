#include <iostream>
#include <string>
#include <termios.h> //Change terminal i/o settings
#include "../include/ChessInterface.hpp"
#include "../include/config.h"

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

void ChessBoard::Display(){
    int i,j;
    //system("clear");
    cout << BOLD BLUE;//normal - blue
    cout << "   ";
    for(i=1;i<9;i++)
        cout << "  " << i << "   " ; 
    
    printf("\n  _________________________________________________\n");
    for(i=0;i<8;i++){
        cout << "  |";
        for(j=0;j<8;j++)
            cout << "     |"; //5 spaces for square

        cout << endl << char(i+65) << " |";
        for(j=0;j<8;j++){
            if(ChessPiece[i][j].selected == '1'){
                cout << RED;
                if(ChessPiece[i][j].team == 'X')
                    cout << "  X  ";
                else if(ChessPiece[i][j].team == 'W' || ChessPiece[i][j].team == 'B')
                    cout << "  " << ChessPiece[i][j].toPrint << " ";
            }
            else if(ChessPiece[i][j].possible == '1'){
                cout << GREEN;
                if(ChessPiece[i][j].team == 'X')
                    cout << "  X  ";
                else if(ChessPiece[i][j].team == 'W' || ChessPiece[i][j].team == 'B')
                    cout << "  " << ChessPiece[i][j].toPrint << " ";
            }
            else{
                if(ChessPiece[i][j].team == 'W'){
                    cout << WHITE;//bold
                    cout << "  " << ChessPiece[i][j].toPrint << " ";
                }
                else if(ChessPiece[i][j].team == 'B'){
                    cout << BLACK;
                    cout << "  " << ChessPiece[i][j].toPrint << " ";
                }
                else if(ChessPiece[i][j].team == 'X')
                    cout << "     ";

            }
            cout << BLUE
                 << "|";

        }
        cout << endl << "  |";
        for(j=0;j<8;j++){
            cout << "     |";
        }
        cout << "\n  |===============================================|\n";
    }
    cout << "\n                 ^\n";
    cout << "Use Arrow Keys <   > to Navigate\n";
    cout << "                 v\n";
    cout << "SpaceBar to Select\n\n";
    cout << "X to Cancel Selection\n\n";

    if(!WhoPlays){
        cout << "It's ";
        cout << WHITE << "White";
        cout << BLUE << "'s Turn!\n";
    }
    else if(WhoPlays){
        cout << "It's ";
        cout << BLACK << "Black";
        cout << BLUE << "'s Turn!\n";
    }

    return;
}

ChessBoard::ChessBoard(){
    WhoPlays = 0;
    for(int i=0;i<4;i++)
        Castle[i] = true;
    Piece bpawn("Pawn",'B');
    Piece brook("Rook",'B');
    Piece bknight("Knight",'B');
    Piece bbishop("Bishop",'B');
    Piece bking("King",'B');
    Piece bqueen("Queen",'B');
    Piece wpawn("Pawn",'W');
    Piece wrook("Rook",'W');
    Piece wknight("Knight",'W');
    Piece wbishop("Bishop",'W');
    Piece wking("King",'W');
    Piece wqueen("Queen",'W');
    ChessPiece = new Piece*[8];
    for(int i=0;i<8;i++){
        ChessPiece[i] = new Piece[8];
        for(int j=0;j<8;j++){
            if(i==1)
                ChessPiece[i][j] = bpawn;
            else if(i==6)
                ChessPiece[i][j] = wpawn;
        }
    }
    ChessPiece[0][0] = ChessPiece[0][7] = brook;
    ChessPiece[0][1] = ChessPiece[0][6] = bknight;
    ChessPiece[0][2] = ChessPiece[0][5] = bbishop;
    ChessPiece[0][3] = bqueen;
    ChessPiece[0][4] = bking;
    
    ChessPiece[7][0] = ChessPiece[7][7] = wrook;
    ChessPiece[7][1] = ChessPiece[7][6] = wknight;
    ChessPiece[7][2] = ChessPiece[7][5] = wbishop;
    ChessPiece[7][3] = wqueen;
    ChessPiece[7][4] = wking;

    ChessPiece[4][4].selected = '1'; //start on 4,4 the reds
}

ChessBoard::~ChessBoard(){
    for(int i=0;i<8;i++)
        delete[] ChessPiece[i];
    delete[] ChessPiece;
}
/*
void InitPiece(struct Piece *piece,char *type,char team){
    piece->team = team;
    piece->selected = '0';
    piece->possible = '0';

}
*/
void ChessBoard::ExecuteMove(string start,string finish){
    int index_x,index_y,fin_index_x,fin_index_y;
    Piece piece;

    index_x = start[0] - 65; //ascii A->0, B->1 etc
    index_y = atoi(&(start[1]));
    index_y = index_y - 1;

    piece = ChessPiece[index_x][index_y];
    piece.selected = '0';

    fin_index_x = finish[0] - 65;//ascii A->0,B->1 etc
    fin_index_y = atoi(&(finish[1]));
    fin_index_y = fin_index_y -1;
    ChessPiece[fin_index_x][fin_index_y].selected = '0';
    cout << "move: " << start << finish << endl;

    Piece empty;

    bool castled = false;
    //Check if Move is Castle
    if(!WhoPlays){ //White's move
        if(index_x == 7 && index_y == 4){
            //Big Castle
            if(fin_index_x == 7 && fin_index_y == 0){
                if(ChessPiece[index_x][index_y].type == "King" && ChessPiece[fin_index_x][fin_index_y].type == "Rook" && Castle[0]==true){
                    ChessPiece[7][1] = ChessPiece[index_x][index_y];
                    ChessPiece[7][2] = ChessPiece[fin_index_x][fin_index_y];
                    ChessPiece[index_x][index_y] = empty;
                    ChessPiece[fin_index_x][fin_index_y] = empty;
                    Castle[0] = false;
                    Castle[1] = false;
                    castled = true;
                    
                }
            }
            //Small Castle
            else if(fin_index_x == 7 && fin_index_y == 7){
                if(ChessPiece[index_x][index_y].type == "King" && ChessPiece[fin_index_x][fin_index_y].type == "Rook" && Castle[1]==true){
                    ChessPiece[7][6] = ChessPiece[index_x][index_y];
                    ChessPiece[7][5] = ChessPiece[fin_index_x][fin_index_y];
                    ChessPiece[index_x][index_y] = empty;
                    ChessPiece[fin_index_x][fin_index_y] = empty;
                    Castle[0] = false;
                    Castle[1] = false;
                    castled = true;
                }

            }
        }
        else if(index_x == 7 && index_y == 0){
            Castle[0] = false;
        }
        else if(index_x == 7 && index_y == 7){
            Castle[1] = false;
        }
    }
    else if(WhoPlays){//Black Plays
        if(index_x == 0 && index_y == 4){
            //Big Castle
            if(fin_index_x == 0 && fin_index_y == 0){
                if(ChessPiece[index_x][index_y].type == "King" && ChessPiece[fin_index_x][fin_index_y].type == "Rook" && Castle[2]==true){
                    ChessPiece[0][1] = ChessPiece[index_x][index_y];
                    ChessPiece[0][2] = ChessPiece[fin_index_x][fin_index_y];
                    ChessPiece[index_x][index_y] = empty;
                    ChessPiece[fin_index_x][fin_index_y] = empty;
                    Castle[2] = false;
                    Castle[3] = false;
                    castled = true;
                    
                }
            }
            //Small Castle
            else if(fin_index_x == 0 && fin_index_y == 7){
                if(ChessPiece[index_x][index_y].type == "King" && ChessPiece[fin_index_x][fin_index_y].type == "Rook" && Castle[3]==true){
                    ChessPiece[0][6] = ChessPiece[index_x][index_y];
                    ChessPiece[0][5] = ChessPiece[fin_index_x][fin_index_y];
                    ChessPiece[index_x][index_y] = empty;
                    ChessPiece[fin_index_x][fin_index_y] = empty;
                    Castle[2] = false;
                    Castle[3] = false;
                    castled = true;
                }

            }
        }
        else if(index_x == 0 && index_y == 0){
            Castle[2] = false;
        }
        else if(index_x == 0 && index_y == 7){
            Castle[3] = false;
        }
    }

    //Move the Piece (if the move is not Castle)
    if(!castled){
        ChessPiece[fin_index_x][fin_index_y] = piece;
        ChessPiece[index_x][index_y] = empty;
        if(piece.type == "King"){
            if(!WhoPlays){
                Castle[0] = false;
                Castle[1] = false;
            }
            else{
                Castle[2] = false;
                Castle[3] = false;
            }

        }
    }

    //Erase possible Moves
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            ChessPiece[i][j].possible = '0';
}

//Returns 0 if the game ends!
bool ChessBoard::ArrowKeyInput(int* x,int* y){
    char c,team;
    string start,finish;
    bool startflag=true,endflag=true;
    if(!WhoPlays)
        team = 'W';
    else
        team = 'B';
    while(1){
        Display();
        if((c = getch()) == '\033') { // if the first value is esc
            getch(); // skip the [
            switch(getch()) { // the real value
                case 'A': //arrow UP ^
                    if(*x>0){
                        ChessPiece[*x][*y].selected = '0';
                        ChessPiece[*x-1][*y].selected = '1';
                        *x = *x - 1;
                    }
                    break;
                case 'B': //arrow DOWN v
                    if(*x<7){
                        ChessPiece[*x][*y].selected = '0';
                        ChessPiece[*x+1][*y].selected = '1';
                        *x = *x + 1;
                    }
                    break;
                case 'C': //arrow RIGHT >
                    if(*y<7){
                        ChessPiece[*x][*y].selected = '0';
                        ChessPiece[*x][*y+1].selected = '1';
                        *y = *y + 1;
                    }
                    break;
                case 'D': //arrow LEFT <
                    if(*y>0){
                        ChessPiece[*x][*y].selected = '0';
                        ChessPiece[*x][*y-1].selected = '1';
                        *y = *y - 1;
                    }
                    break;
            }
        }
        else if(c == ' '){//SpaceBar
            if(startflag){
                if(ChessPiece[*x][*y].team == team){
                    if(PossibleMoves(*x,*y)){ //if there is no Possible Moves, repeat Start
                        start.append(1,char(*x+65));
                        start.append(1,char(*y+48+1));
                        start.append("-");
                        startflag = false;
                    }//make the squares the piece can go green
                }
            }
            else if(endflag){
                if(ChessPiece[*x][*y].possible == '1'){
                    finish.append(1,char(*x+65));
                    finish.append(1,char(*y+48+1));
                    endflag = 0;
                    ExecuteMove(start,finish);
                    
                    if(!WhoPlays)
                        WhoPlays=true;
                    else
                        WhoPlays=false;
                    
                    char team;
                    if(!WhoPlays)
                        team = 'W';
                    else
                        team = 'B';
                        
                    //CheckVictory();
                    bool moves_left=false;
                    for(int i=0;i<8;i++){
                        for(int j=0;j<8;j++){
                            if(ChessPiece[i][j].team == team){
                                moves_left=PossibleMoves(i,j);
                                if(moves_left)
                                    break;
                            }
                        }
                        if(moves_left)
                            break;
                    }
                    if(!moves_left){ // 'team' has no more moves -> Lost
                        Display();
                        if(team == 'W'){
                            cout << MAGENTA << "Checkmate!! ";
                            cout << BLACK << "BLACK ";
                            cout << MAGENTA << "Wins!" << endl;
                        }
                        else{
                            cout << MAGENTA << "Checkmate!! ";
                            cout << WHITE << "WHITE ";
                            cout << MAGENTA << "Wins!" << endl;
                        }
                        return false;

                    }
                    for(int i=0;i<8;i++)
                        for(int j=0;j<8;j++)
                            ChessPiece[i][j].possible = '0';

                    break;
                }
            }
        }
        else if(c == 'x' || c == 'X'){
            startflag = 1;
            start = "";
            for(int i=0;i<8;i++)
                for(int j=0;j<8;j++)
                    ChessPiece[i][j].possible = '0';
        }
    }
    return true; //no need.. can be void
}

//return 0 if there are NO moves - 1 if there is
bool ChessBoard::PossibleMoves(int x,int y){
    bool moves_exist=false;
    char enemy;
    if(!WhoPlays)
        enemy = 'B';
    else
        enemy = 'W';

    if(ChessPiece[x][y].type=="Pawn"){//if its a pawn

        //2 forward
        if(x==1 && ChessPiece[x][y].team == 'B'){
            if(ChessPiece[x+2][y].team == 'X' && ChessPiece[x+1][y].team == 'X'){
                if(!Pinned(x,y,x+2,y)){
                    ChessPiece[x+2][y].possible = '1';
                    moves_exist = true;
                }
            }
        }
        else if(x==6 && ChessPiece[x][y].team == 'W'){
            if(ChessPiece[x-2][y].team == 'X' && ChessPiece[x-1][y].team == 'X'){
                if(!Pinned(x,y,x-2,y)){
                    ChessPiece[x-2][y].possible = '1';
                    moves_exist = true;
                }
            }
        }

        //kill diagwnia + 1 forward
        if( (!WhoPlays) && x>0){//White
            if(y>0){
                if(ChessPiece[x-1][y-1].team == 'B'){
                    if(!Pinned(x,y,x-1,y-1)){
                        ChessPiece[x-1][y-1].possible = '1';
                        moves_exist = true;
                    }
                }
            }
            if(y<7){
                if(ChessPiece[x-1][y+1].team == 'B'){
                    if(!Pinned(x,y,x-1,y+1)){
                        ChessPiece[x-1][y+1].possible = '1';
                        moves_exist = true;
                    }
                }
            }
            if(ChessPiece[x-1][y].team == 'X'){
                if(!Pinned(x,y,x-1,y)){
                    ChessPiece[x-1][y].possible = '1';
                    moves_exist = true;
                }
            }
        }
        else if(WhoPlays && x<7){//Black
            if(y>0){
                if(ChessPiece[x+1][y-1].team == 'W'){
                    if(!Pinned(x,y,x+1,y-1)){
                        ChessPiece[x+1][y-1].possible = '1';
                        moves_exist = true;
                    }
                }
            }
            if(y<7){
                if(ChessPiece[x+1][y+1].team == 'W'){
                    if(!Pinned(x,y,x+1,y+1)){
                        ChessPiece[x+1][y+1].possible = '1';
                        moves_exist = true;
                    }
                }
            }
            if(ChessPiece[x+1][y].team == 'X'){
                if(!Pinned(x,y,x+1,y)){
                    ChessPiece[x+1][y].possible = '1';
                    moves_exist = true;
                }
            }
        }
        
    }
    if(ChessPiece[x][y].type == "Rook" || ChessPiece[x][y].type == "Queen" ){
        int counter=x;
        while(counter-1>=0){
            if(ChessPiece[counter-1][y].team == 'X'){
                if(!Pinned(x,y,counter-1,y)){
                    ChessPiece[counter-1][y].possible = '1';
                    moves_exist = true;
                }
                counter--;
            }
            else if(ChessPiece[counter-1][y].team == enemy){
                if(!Pinned(x,y,counter-1,y)){
                    ChessPiece[counter-1][y].possible = '1';
                    moves_exist = true;
                }
                break;
            }
            else{
                break;
            }
        }
        counter=x;
        while(counter+1<=7){
            if(ChessPiece[counter+1][y].team == 'X'){
                if(!Pinned(x,y,counter+1,y)){
                    ChessPiece[counter+1][y].possible = '1';
                    moves_exist = true;
                }
                counter++;
            }
            else if(ChessPiece[counter+1][y].team == enemy){
                if(!Pinned(x,y,counter+1,y)){
                    ChessPiece[counter+1][y].possible = '1';
                    moves_exist = true;
                }
                break;
            }
            else{
                break;
            }
        }
        counter=y;
        while(counter-1>=0){
            if(ChessPiece[x][counter-1].team == 'X'){
                if(!Pinned(x,y,x,counter-1)){
                    ChessPiece[x][counter-1].possible = '1';
                    moves_exist = true;
                }
                counter--;
            }
            else if(ChessPiece[x][counter-1].team == enemy){
                if(!Pinned(x,y,x,counter-1)){
                    ChessPiece[x][counter-1].possible = '1';
                    moves_exist = true;
                }
                break;
            }
            else{
                break;
            }
        }
        counter=y;
        while(counter+1<=7){
            if(ChessPiece[x][counter+1].team == 'X'){
                if(!Pinned(x,y,x,counter+1)){
                    ChessPiece[x][counter+1].possible = '1';
                    moves_exist = true;
                }
                counter++;
            }
            else if(ChessPiece[x][counter+1].team == enemy){
                if(!Pinned(x,y,x,counter+1)){
                    ChessPiece[x][counter+1].possible = '1';
                    moves_exist = true;
                }
                break;
            }
            else{
                break;
            }
        }
    }
    if( ChessPiece[x][y].type == "Bishop" || ChessPiece[x][y].type == "Queen" ){
        int counter_x=x,counter_y=y;
        while(counter_x-1>=0 && counter_y-1>=0){
            if(ChessPiece[counter_x-1][counter_y-1].team == 'X'){
                if(!Pinned(x,y,counter_x-1,counter_y-1)){
                    ChessPiece[counter_x-1][counter_y-1].possible = '1';
                    moves_exist = true;
                }
                counter_x--;
                counter_y--;
            }
            else if(ChessPiece[counter_x-1][counter_y-1].team == enemy){
                if(!Pinned(x,y,counter_x-1,counter_y-1)){
                    ChessPiece[counter_x-1][counter_y-1].possible = '1';
                    moves_exist = true;
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
            if(ChessPiece[counter_x-1][counter_y+1].team == 'X'){
                if(!Pinned(x,y,counter_x-1,counter_y+1)){
                    ChessPiece[counter_x-1][counter_y+1].possible = '1';
                    moves_exist = true;
                }
                counter_x--;
                counter_y++;
            }
            else if(ChessPiece[counter_x-1][counter_y+1].team == enemy){
                if(!Pinned(x,y,counter_x-1,counter_y+1)){
                    ChessPiece[counter_x-1][counter_y+1].possible = '1';
                    moves_exist = true;
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
            if(ChessPiece[counter_x+1][counter_y-1].team == 'X'){
                if(!Pinned(x,y,counter_x+1,counter_y-1)){
                    ChessPiece[counter_x+1][counter_y-1].possible = '1';
                    moves_exist = true;
                }
                counter_x++;
                counter_y--;
            }
            else if(ChessPiece[counter_x+1][counter_y-1].team == enemy){
                if(!Pinned(x,y,counter_x+1,counter_y-1)){
                    ChessPiece[counter_x+1][counter_y-1].possible = '1';
                    moves_exist = true;
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
            if(ChessPiece[counter_x+1][counter_y+1].team == 'X'){
                if(!Pinned(x,y,counter_x+1,counter_y+1)){
                    ChessPiece[counter_x+1][counter_y+1].possible = '1';
                    moves_exist = true;
                }
                counter_x++;
                counter_y++;
            }
            else if(ChessPiece[counter_x+1][counter_y+1].team == enemy){
                if(!Pinned(x,y,counter_x+1,counter_y+1)){
                    ChessPiece[counter_x+1][counter_y+1].possible = '1';
                    moves_exist = true;
                }
                break;
            }
            else{
                break;
            }
        }
    }
    if( ChessPiece[x][y].type == "Knight"){
        //1.paei mono se Γ 
        if(x-2>=0){
            if(y-1>=0 && (ChessPiece[x-2][y-1].team == enemy || ChessPiece[x-2][y-1].team == 'X' )){
                if(!Pinned(x,y,x-2,y-1)){
                    ChessPiece[x-2][y-1].possible = '1';
                    moves_exist = true;
                }
            }
            if(y+1<=7 && (ChessPiece[x-2][y+1].team == enemy || ChessPiece[x-2][y+1].team == 'X' )){
                if(!Pinned(x,y,x-2,y+1)){
                    ChessPiece[x-2][y+1].possible = '1';
                    moves_exist = true;
                }
            }
        }
        if(x-1>=0){
            if(y-2>=0 && (ChessPiece[x-1][y-2].team == enemy || ChessPiece[x-1][y-2].team == 'X' )){
                if(!Pinned(x,y,x-1,y-2)){
                    ChessPiece[x-1][y-2].possible = '1';
                    moves_exist = true;
                }
            }
            if(y+2<=7 && (ChessPiece[x-1][y+2].team == enemy || ChessPiece[x-1][y+2].team == 'X' )){
                if(!Pinned(x,y,x-1,y+2)){
                    ChessPiece[x-1][y+2].possible = '1';
                    moves_exist = true;
                }
            }
        }
        if(x+2<=7){
            if(y-1>=0 && (ChessPiece[x+2][y-1].team == enemy || ChessPiece[x+2][y-1].team == 'X' )){
                if(!Pinned(x,y,x+2,y-1)){
                    ChessPiece[x+2][y-1].possible = '1';
                    moves_exist = true;
                }
            }
            if(y+1<=7 && (ChessPiece[x+2][y+1].team == enemy || ChessPiece[x+2][y+1].team == 'X' )){
                if(!Pinned(x,y,x+2,y+1)){
                    ChessPiece[x+2][y+1].possible = '1';
                    moves_exist = true;
                }
            }
        }
        if(x+1<=7){
            if(y-2>=0 && (ChessPiece[x+1][y-2].team == enemy || ChessPiece[x+1][y-2].team == 'X' )){
                if(!Pinned(x,y,x+1,y-2)){
                    ChessPiece[x+1][y-2].possible = '1';
                    moves_exist = true;
                }
            }
            if(y+2<=7 && (ChessPiece[x+1][y+2].team == enemy || ChessPiece[x+1][y+2].team == 'X' )){
                if(!Pinned(x,y,x+1,y+2)){
                    ChessPiece[x+1][y+2].possible = '1';
                    moves_exist = true;
                }
            }
        }
    }
    if( ChessPiece[x][y].type == "King"){
        if(x-1>=0){
            //TOP
            if(ChessPiece[x-1][y].team == enemy || ChessPiece[x-1][y].team == 'X'){
                if(!Pinned(x,y,x-1,y)){
                    ChessPiece[x-1][y].possible = '1';
                    moves_exist = true;
                }

            }
            //TOP-LEFT
            if(y-1>=0 && (ChessPiece[x-1][y-1].team == enemy || ChessPiece[x-1][y-1].team == 'X')){
                if(!Pinned(x,y,x-1,y-1)){
                    ChessPiece[x-1][y-1].possible = '1';
                    moves_exist = true;
                }
            }
            //TOP-RIGHT
            if(y+1<=7 && (ChessPiece[x-1][y+1].team == enemy || ChessPiece[x-1][y+1].team == 'X')){
                if(!Pinned(x,y,x-1,y+1)){
                    ChessPiece[x-1][y+1].possible = '1';
                    moves_exist = true;
                }
            }
        }
        if(x+1<=7){
            //BOTTOM
            if(ChessPiece[x+1][y].team == enemy || ChessPiece[x+1][y].team == 'X'){
                if(!Pinned(x,y,x+1,y)){
                    ChessPiece[x+1][y].possible = '1';
                    moves_exist = true;
                }

            }
            //BOTTOM-LEFT
            if(y-1>=0 && (ChessPiece[x+1][y-1].team == enemy || ChessPiece[x+1][y-1].team == 'X')){
                if(!Pinned(x,y,x+1,y-1)){
                    ChessPiece[x+1][y-1].possible = '1';
                    moves_exist = true;
                }
            }
            //BOTTOM-RIGHT
            if(y+1<=7 && (ChessPiece[x+1][y+1].team == enemy || ChessPiece[x+1][y+1].team == 'X')){
                if(!Pinned(x,y,x+1,y+1)){
                    ChessPiece[x+1][y+1].possible = '1';
                    moves_exist = true;
                }
            }
        }
        //LEFT
        if(ChessPiece[x][y-1].team == enemy || ChessPiece[x][y-1].team == 'X'){
            if(!Pinned(x,y,x,y-1)){
                ChessPiece[x][y-1].possible = '1';
                moves_exist = true;
            }
        }

        //RIGHT
        if(ChessPiece[x][y+1].team == enemy || ChessPiece[x][y+1].team == 'X'){
            if(!Pinned(x,y,x,y+1)){
                ChessPiece[x][y+1].possible = '1';
                moves_exist = true;
            }
        }

        //Castle[0]: WBig (left)
        //Castle[1]: WSmall (right)
        //[2]: BBig (left)
        //[3]: BSmall (right)
        if( (!WhoPlays) && Castle[0]){
            if(ChessPiece[7][1].team == 'X' && ChessPiece[7][2].team == 'X' && ChessPiece[7][3].team == 'X'){
                //if king is not in check now & after Castle
                if((!KingOnCheck()) && (!Pinned(7,4,7,1))){
                    ChessPiece[7][0].possible = '1';
                    moves_exist = true;
                }
            }
        }
        if( (!WhoPlays) && Castle[1]){
            if(ChessPiece[7][5].team == 'X' && ChessPiece[7][6].team == 'X'){
                if((!KingOnCheck()) && (!Pinned(7,4,7,6))){
                    ChessPiece[7][7].possible = '1';
                    moves_exist = true;
                }
            }
        }
        if( WhoPlays && Castle[2]){
            if(ChessPiece[0][1].team == 'X' && ChessPiece[0][2].team == 'X' && ChessPiece[0][3].team == 'X'){
                if((!KingOnCheck()) && (!Pinned(0,4,0,1))){
                    ChessPiece[0][0].possible = '1';
                    moves_exist = true;
                }
            }
        }
        if( WhoPlays && Castle[3]){
            if(ChessPiece[0][5].team == 'X' && ChessPiece[0][6].team == 'X'){
                if((!KingOnCheck()) && (!Pinned(0,4,0,6))){
                    ChessPiece[0][7].possible = '1';
                    moves_exist = true;
                }
            }
        }
    }

    return moves_exist;
}

//Returns 0 if it's not Pinned - 1 if it is
bool ChessBoard::Pinned(int start_x,int start_y,int fin_x,int fin_y){
    bool pinned = false;
    struct Piece tmp_piece,empty;

    //1.Do the Move
    tmp_piece = ChessPiece[fin_x][fin_y];
    ChessPiece[fin_x][fin_y] = ChessPiece[start_x][start_y];
    ChessPiece[start_x][start_y] = empty;

    //2.Check if King is on check after move (1:king is on check / 0:king is safe)
    if(KingOnCheck())
        pinned=true;

    //3.Undo the move
    ChessPiece[start_x][start_y] = ChessPiece[fin_x][fin_y];
    ChessPiece[fin_x][fin_y] = tmp_piece;

    //4.return 1/0;
    return pinned;

}

bool ChessBoard::KingOnCheck(){
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
            if(ChessPiece[i][j].team == team && ChessPiece[i][j].type == "King"){
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
                if(ChessPiece[x-1][y-1].team == 'B' && ChessPiece[x-1][y-1].type=="Pawn")
                    return true;
            }
            if(y+1<=7){
                if(ChessPiece[x-1][y+1].team == 'B' && ChessPiece[x-1][y+1].type=="Pawn")
                    return true;
            }
        }
    }
    if(team == 'B'){
        //Pawn
        if(x+1>=0){
            if(y-1>=0){
                if(ChessPiece[x+1][y-1].team == 'W' && ChessPiece[x+1][y-1].type=="Pawn")
                    return true;
            }
            if(y+1<=7){
                if(ChessPiece[x+1][y+1].team == 'W' && ChessPiece[x+1][y+1].type=="Pawn")
                    return true;
            }
        }
    }

    //printf("No Its rook-queen\n");
    //Rook - Queen
    int counter=x;
    while(counter-1>=0){
        if(ChessPiece[counter-1][y].team == enemy && (ChessPiece[counter-1][y].type=="Rook" || ChessPiece[counter-1][y].type=="Queen"))
            return true;
        else if(ChessPiece[counter-1][y].team != 'X')
            break;
        counter--;
    }
    counter=x;
    while(counter+1<=7){
        if(ChessPiece[counter+1][y].team == enemy && (ChessPiece[counter+1][y].type=="Rook" || ChessPiece[counter+1][y].type=="Queen"))
            return true;
        else if(ChessPiece[counter+1][y].team != 'X')
            break;
        counter++;
    }
    counter = y;
    while(counter-1>=0){
        if(ChessPiece[x][counter-1].team == enemy && (ChessPiece[x][counter-1].type=="Rook" || ChessPiece[x][counter-1].type=="Queen"))
            return true;
        else if(ChessPiece[x][counter-1].team != 'X')
            break;
        counter--;
    }
    counter = y;
    while(counter+1<=7){
        if(ChessPiece[x][counter+1].team == enemy && (ChessPiece[x][counter+1].type=="Rook" || ChessPiece[x][counter+1].type=="Queen"))
            return true;
        else if(ChessPiece[x][counter+1].team != 'X')
            break;
        counter++;
    }

    //printf("no Its bishop - queen\n");
    //Bishop - Queen
    int counter_x=x;
    int counter_y=y;
    while(counter_x-1>=0 && counter_y-1>=0){
        if(ChessPiece[counter_x-1][counter_y-1].team == enemy && (ChessPiece[counter_x-1][counter_y-1].type=="Bishop" || ChessPiece[counter_x-1][counter_y-1].type=="Queen"))
            return true;
        else if(ChessPiece[counter_x-1][counter_y-1].team != 'X')
            break;
        counter_x--;
        counter_y--;
    }
    counter_x=x;
    counter_y=y;
    while(counter_x-1>=0 && counter_y+1<=7){
        if(ChessPiece[counter_x-1][counter_y+1].team == enemy && (ChessPiece[counter_x-1][counter_y+1].type=="Bishop" || ChessPiece[counter_x-1][counter_y+1].type=="Queen"))
            return true;
        else if(ChessPiece[counter_x-1][counter_y+1].team != 'X')
            break;
        counter_x--;
        counter_y++;
    }
    counter_x=x;
    counter_y=y;
    while(counter_x+1<=7 && counter_y-1>=0){
        if(ChessPiece[counter_x+1][counter_y-1].team == enemy && (ChessPiece[counter_x+1][counter_y-1].type=="Bishop" || ChessPiece[counter_x+1][counter_y-1].type=="Queen"))
            return true;
        else if(ChessPiece[counter_x+1][counter_y-1].team != 'X')
            break;
        counter_x++;
        counter_y--;
    }
    counter_x=x;
    counter_y=y;
    while(counter_x+1<=7 && counter_y+1<=7){
        if(ChessPiece[counter_x+1][counter_y+1].team == enemy && (ChessPiece[counter_x+1][counter_y+1].type=="Bishop" || ChessPiece[counter_x+1][counter_y+1].type=="Queen"))
            return true;
        else if(ChessPiece[counter_x+1][counter_y+1].team != 'X')
            break;
        counter_x++;
        counter_y++;
    }


    //Knight

    if(x-2>=0){
        if(y-1>=0){
            if(ChessPiece[x-2][y-1].team == enemy && ChessPiece[x-2][y-1].type=="Knight")
                return true;
        }
        if(y+1<=7){
            if(ChessPiece[x-2][y+1].team == enemy && ChessPiece[x-2][y+1].type=="Knight")
                return true;
        }
    }
    if(x-1>=0){
        if(y-2>=0){
            if(ChessPiece[x-1][y-2].team == enemy && ChessPiece[x-1][y-2].type=="Knight")
                return true;
        }
        if(y+2<=7){
            if(ChessPiece[x-1][y+2].team == enemy && ChessPiece[x-1][y+2].type=="Knight")
                return true;
        }
    }
    if(x+2<=7){
        if(y-1>=0){
            if(ChessPiece[x+2][y-1].team == enemy && ChessPiece[x+2][y-1].type=="Knight")
                return true;
        }
        if(y+1<=7){
            if(ChessPiece[x+2][y+1].team == enemy && ChessPiece[x+2][y+1].type=="Knight")
                return true;
        }
    }
    if(x+1<=7){
        if(y-2>=0){
            if(ChessPiece[x+1][y-2].team == enemy && ChessPiece[x+1][y-2].type=="Knight")
                return true;
        }
        if(y+2<=7){
            if(ChessPiece[x+1][y+2].team == enemy && ChessPiece[x+1][y+2].type=="Knight")
                return true;
        }

    }

    //printf("no Its king\n");
    //King
    if(x-1>=0){
        //TOP
        if(ChessPiece[x-1][y].team == enemy && ChessPiece[x-1][y].type=="King")
            return true;
        //TOP-LEFT
        if(y-1>=0){
            if(ChessPiece[x-1][y-1].team == enemy && ChessPiece[x-1][y-1].type=="King")
                return true;
        }
        //TOP-RIGHT
        if(y+1<=7){
            if(ChessPiece[x-1][y+1].team == enemy && ChessPiece[x-1][y+1].type=="King")
                return true;
        }
    }
    if(x+1<=7){
        //BOTTOM
        if(ChessPiece[x+1][y].team == enemy && ChessPiece[x+1][y].type=="King")
            return true;
        //BOTTOM-LEFT
        if(y-1>=0){
            if(ChessPiece[x+1][y-1].team == enemy && ChessPiece[x+1][y-1].type=="King")
                return true;
        }
        //BOTTOM-RIGHT
        if(y+1<=7){
            if(ChessPiece[x+1][y+1].team == enemy && ChessPiece[x+1][y+1].type=="King")
                return true;
        }
    }
    //LEFT
    if(y-1>=0){
        if(ChessPiece[x][y-1].team == enemy && ChessPiece[x][y-1].type=="King")
            return true;
    }
    //RIGHT
    if(y+1<=7){
        if(ChessPiece[x][y+1].team == enemy && ChessPiece[x][y+1].type=="King")
            return true;
    }


    return false;



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

