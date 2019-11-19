#include <iostream>
#include <string>

using namespace std;

struct Piece{
    string type;
    char team;
    string toPrint;
    char selected;
    char possible;
    Piece(string arg_type,char arg_team):
    type(arg_type),
    team(arg_team)
    {
        if(type == "Knight")
            toPrint = "Kn";
        else{
            toPrint.append(type,0,1);//1st letter
            toPrint.append(" ");
        }
        possible = 0;
        selected = 0;
    };
    Piece(){
        type = "Empty";
        toPrint = " ";
        team = 'X';
        selected = 0;
        possible = 0;
    }
};

class ChessBoard{
    private:
        bool WhoPlays;
        bool Castle[4];
        Piece **ChessPiece;
    public:
        ChessBoard();
        ~ChessBoard();
        void Display();
        bool ArrowKeyInput(int*,int*);
        bool PossibleMoves(int,int);
        bool Pinned(int,int,int,int);
        bool KingOnCheck();
        void ExecuteMove(string,string);

};


/*getch Implementation*/
char getch(void);
char getch_(int);
void initTermios(int);
void resetTermios(void);

// 0 - 1 - 2 - 3
// WBig - WSmall - BBig - BSmall 
//int Castle[4] = {1};//1 = Can Castle , 0 = Can't Castle
