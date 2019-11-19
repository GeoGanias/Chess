#include "../include/ChessInterface.hpp"
#include "../config.h"

int main(int argc, const char *argv[]){
    ChessBoard* board = new ChessBoard();
    int select_x=4,select_y=4;

    board->Display();


    while(board->ArrowKeyInput(&select_x,&select_y)){}

    delete board;
    
    return 0;
}
