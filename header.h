struct Piece{
    char type[7];
    char team;
    char toPrint[3];
    char selected;
    char possible;
};

void PrintChessboardBig(struct Piece**,int);
int ArrowKeyInput(struct Piece**,int*,int*,int*);
int PossibleMoves(struct Piece **,int,int,int);
void InitBoard(struct Piece**);
void InitPiece(struct Piece* ,char*,char);
void ExecuteMove(struct Piece**,char*,char*,int);
int Pinned(struct Piece **,int,int,int,int,int);
int KingOnCheck(struct Piece **,int);

/*getch Implementation*/
char getch(void);
char getch_(int);
void initTermios(int);
void resetTermios(void);

/*Global*/
extern int Castle[4];
// 0 - 1 - 2 - 3
// WBig - WSmall - BBig - BSmall 
//int Castle[4] = {1};//1 = Can Castle , 0 = Can't Castle
