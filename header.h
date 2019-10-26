struct Piece{
    char type[10];
    char team;
    char toPrint[10];
    char selected;
    char possible;
};

void PrintChessboard(struct Piece**);
void PrintChessboardBig(struct Piece**,int);
void ArrowKeyInput(struct Piece**,int*,int*,int);
int PossibleMoves(struct Piece **,int,int,int);
void InitBoard(struct Piece**);
void ExecuteMove(struct Piece**,char*,char*,int);
int isNotValid(char *,char *,int,struct Piece**);

/*getch Implementation*/
char getch(void);
char getch_(int);
void initTermios(int);
void resetTermios(void);

/*Global*/
extern int Castle[4];
