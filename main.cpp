#include<iostream>
using namespace std;
 
const char player = 'X', opponent = 'O', empty = '_';

struct Cell
{
    size_t _row, _col;
};
 
bool isCellsLeft(const char board[3][3]){
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]==empty)
                return true;
    return false;
}
 
int evaluate(const char b[3][3]){
    for (size_t row = 0; row<3; row++){
        if (b[row][0]==b[row][1] && b[row][1]==b[row][2]){
            if (b[row][0]==player)
                return +10;
            else if (b[row][0]==opponent)
                return -10;
        }
    }

    for (size_t col = 0; col<3; col++){
        if (b[0][col]==b[1][col] && b[1][col]==b[2][col]){
            if (b[0][col]==player)
                return +10;
            else if (b[0][col]==opponent)
                return -10;
        }
    }

    if (b[0][0]==b[1][1] && b[1][1]==b[2][2]){
        if (b[0][0]==player)
            return +10;
        else if (b[0][0]==opponent)
            return -10;
    }
 
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0]){
        if (b[0][2]==player)
            return +10;
        else if (b[0][2]==opponent)
            return -10;
    }

    return 0;
}

int minimax(char board[3][3], int depth, bool isMax){
    int score = evaluate(board);
    if (score != 0)
        return score;
    if (isCellsLeft(board)==false)
        return 0;

	int best = (isMax)? -1000:+1000;
	for (size_t row = 0; row<3; row++){
            for (size_t col = 0; col<3; col++){
                if (board[row][col]==empty){
                    board[row][col] = (isMax)?player:opponent;
					if(isMax) best = max(best, minimax(board, depth+1, !isMax) );
					else best = min(best, minimax(board, depth+1, !isMax) );
                    board[row][col] = empty;
                }
            }
        }
	if(isMax) best -= depth;
	return best;
}
 
Cell findBestCell(const char board[3][3]){
	char board_copy[3][3] = {{board[0][0], board[0][1], board[0][2]}, {board[1][0], board[1][1], board[1][2]}, {board[2][0], board[2][1], board[2][2]}}; 
    int bestVal = -1000;
    Cell bestCell;
    bestCell._row = -1;
    bestCell._col = -1;
 
   for (size_t row = 0; row<3; row++){
        for (size_t col = 0; col<3; col++){
            if (board[row][col]==empty){
                board_copy[row][col] = player;
                int CellVal = minimax(board_copy, 0, false);
                board_copy[row][col] = empty;
				
                if (CellVal > bestVal){
                    bestCell._row = row;
                    bestCell._col = col;
                    bestVal = CellVal;
                }
            }
        }
    }
    return bestCell;
}

void printBoard(const char board[3][3]){
	for(size_t row = 0; row<3; row++){
		for(size_t col = 0; col<3; col++){
			cout<<board[row][col]<<' ';
		}
		cout<<endl;
	}
	cout<<endl;
}

void makeMove(char board[3][3], const Cell move, char side){
	if(board[move._row][move._col]!=empty)
		throw "Move is already made";
	board[move._row][move._col] = side;
}

void makeBestMove(char board[3][3]){
	if(!isCellsLeft(board))
		throw "There is nowhere to move";
	Cell m = findBestCell(board);
	if(board[m._row][m._col]!=empty)
		throw "Best Move is already made";
	makeMove(board, m, player);
}
 
int main(){
	cout<<"This is an unbeatable tic-tac-toe algorithm MINIMAX"<<endl;
	cout<<"The idea is to calculate all the possible outcomes and find one which leads to faster victory"<<endl;
	cout<<"Here are the indexes:\n0 1 2\n3 4 5\n6 7 8\n\n"<<endl;
    char board[3][3] = {{empty, empty, empty }, {empty, empty, empty }, {empty, empty, empty }};

	while(isCellsLeft(board) && evaluate(board)==0){		
		size_t move;
		while(true){
			cout<<"Make your move: ";
			cin>>move;
			if(move<0||9<move){
				cout<<"There is no such cell. Try again"<<endl;
				continue;
			}
			try{
				Cell cell = {move/3, move%3};
				makeMove(board, cell, opponent);
				printBoard(board);
				break;
			} catch (char* err){
				cout<<"That cell is already taken. Try again"<<endl;
			}
		}
		if(evaluate(board)<0) cout<<"YOU WON!\n ARE YOU SURE YOU ARE NOT ALSO AN UNBEATABLE ALGORITHM? TRY VOIGHT-KAMPF TEST, WILL YOU?"<<endl;
		if(!isCellsLeft(board)) cout<<"DRAW"<<endl<<endl;
		else if(isCellsLeft(board) && !evaluate(board)){
			makeBestMove(board);
			printBoard(board);
			if(evaluate(board)>0) cout<<"YOU LOST"<<endl<<endl;
			else{
				if(!isCellsLeft(board)) cout<<"DRAW"<<endl<<endl;
			}
		}
	}
    return 0;
}