#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 12

void userTurn(char ourBoard[SIZE][SIZE], char enemyBoard[SIZE][SIZE], 
			  char userView[SIZE][SIZE], char enemyView[SIZE][SIZE]);

void readPositions(char path[28], char board[SIZE][SIZE]){
	FILE *f;
	char line[24];
	int i, j = 0;
	f = fopen(path, "r");
	if(f){
		while(fgets(line, sizeof(line), f)){
			for(i = 0; i < SIZE; i++){
				board[j][i] = line[i];
			}j++;
		}
	}
}

void writePositions(char path[28], char board[SIZE][SIZE]){
	FILE *f = fopen(path, "w");
	int i, j;
	for(i = 0; i < SIZE; i++){
		fprintf(f, "X");
	}fprintf(f, "\nX");
	for(i = 1; i < SIZE-1; i++){
		for(j = 1; j < SIZE-1; j++){
				fprintf(f, "%c", board[i][j]);
		}fprintf(f, "X\nX");
	}
	for(i = 0; i < SIZE-1; i++){
		fprintf(f, "X");
	}fclose(f);
}

void writeShip(char path[28], char board[SIZE][SIZE], int size, int horitzontal, int x, int y){
	int i;
	if(horitzontal){
		for(i = 0; i < size; i++){
			board[y+1][x+1+i] = '$';
		}
	}else{
		for(i = 0; i < size; i++){
			board[y+1+i][x+1] = '$';
		}
	}	
	writePositions(path, board);
}

int getRand(int max){
	return rand() % max;
}

int itsValid(int dir, int size, int xo, int yo, char board[SIZE][SIZE]){
	int i;
	xo++;
	yo++;
	if(dir == 0){	//VERTICAL
		if(yo > SIZE-2-size){
			return 0;
		}else{
			for(i = -1; i < size+1; i++){
				if(board[yo+i][xo] == '$' || board[yo+i][xo-1] == '$' ||  board[yo+i][xo+1] == '$'){
					return 0;
				}
			}	
			return 1;
		}
	}else{			//HORIZONTAL
		if(xo > SIZE-2-size){
			return 0;
		}else{
			for(i = -1; i < size+1; i++){
				if(board[yo][xo+i] == '$' || board[yo+1][xo+i] == '$' ||  board[yo-1][xo+i] == '$'){
					return 0;
				}
			}
			return 1;
		}
	}
}
void generateShip(char path[28], char board[SIZE][SIZE], int size){
	int dir = getRand(2), xo, yo;
	if(dir == 0){
		xo = getRand(SIZE-2);
		yo = getRand(SIZE-2-size);
	}else{
		xo = getRand(SIZE-2-size);
		yo = getRand(SIZE-2);
	}
	
	if(itsValid(dir, size, xo, yo, board) == 0){
		generateShip(path, board, size);
	}else{
		writeShip(path, board, size, dir, xo, yo);
	}
		
}

void restartPositions(char path[28], char board[SIZE][SIZE]){
	int i, j;
	for(i = 1; i < SIZE-1; i++){
		for(j = 1; j < SIZE-1; j++){
			board[i][j] = '-';
		}
	}
	if(path != ""){
		writePositions(path, board);
	}
}

void printBoard(char board[SIZE][SIZE]){
	int i, j;
	printf("\n");
	for(i = 1; i < SIZE-1; i++){
		for(j = 1; j < SIZE-1; j++){
			printf("%c ", board[i][j]);
		}printf("%i\n", i-1);
	}printf("0 1 2 3 4 5 6 7 8 9\n");
}

void printBoards(char board[SIZE][SIZE], char board2[SIZE][SIZE]){
	int i, j;
	printf("\nUser shots                Enemy shots\n");
	for(i = 1; i < SIZE-1; i++){
		for(j = 1; j < SIZE-1; j++){
			printf("%c ", board[i][j]);
		}printf("%i  ", i-1);
		for(j = 1; j < SIZE-1; j++){
			printf("%c ", board2[i][j]);
		}printf("%i\n", i-1);
	}printf("0 1 2 3 4 5 6 7 8 9    0 1 2 3 4 5 6 7 8 9\n");
}
	
void fillBoard(char path[28], char board[SIZE][SIZE]){
	int i, x, y, dir;
	for(i = 2; i < 6; i++){
		printBoard(board);
		printf("\nInput the %i-size ship coordenates(x y): ", i);
		scanf("%i", &x);
		scanf("%i", &y);
		printf("Vertical or horitzonal (0/1): ");
		scanf("%i", &dir);
		if(itsValid(dir, i, x, y, board) == 0){
			printf("\nWrong position think it better...\n");
			i--;
		}else{
			writeShip(path, board, i, dir, x, y);
		}
	}printBoard(board);
}

int getShips(char board[SIZE][SIZE]){
	int i, j, c = 0;
	for(i = 1; i < SIZE-1; i++){
		for(j = 1; j < SIZE-1; j++){
			if(board[i][j] == '$'){
				c++;
			}
		}
	}return c;
}


void systemTurn(char ourBoard[SIZE][SIZE],char enemyBoard[SIZE][SIZE], 
			    char systemView[SIZE][SIZE], char enemyView[SIZE][SIZE]){
	if(getShips(systemView) == 14){
		printf("\nGame finished, system won!\n");
		restartPositions("userBoard.txt", ourBoard);
		restartPositions("systemBoard.txt", enemyBoard);
		restartPositions("userView.txt", enemyView);
		restartPositions("enemyView.txt", systemView);
	}else{
		int x = rand() % 10;
		int y = rand() % 10;
		if(enemyBoard[y+1][x+1] == '$'){
			systemView[y+1][x+1] = '$';
			writePositions("enemyView.txt", systemView);
			userTurn(enemyBoard, ourBoard, enemyView, systemView);
		}else{
			systemView[y+1][x+1] = 'O';
			writePositions("enemyView.txt", systemView);
			userTurn(enemyBoard, ourBoard, enemyView, systemView);
		}
	}
}

void userTurn(char ourBoard[SIZE][SIZE], char enemyBoard[SIZE][SIZE], 
			  char userView[SIZE][SIZE], char enemyView[SIZE][SIZE]){
	printBoards(userView, enemyView);
	int x, y;
	if(getShips(userView) == 14){
		printf("\nGame finished, you won!\n");
		restartPositions("userBoard.txt", ourBoard);
		restartPositions("systemBoard.txt", enemyBoard);
		restartPositions("userView.txt", userView);
		restartPositions("enemyView.txt", enemyView);
	}else{
		printf("Pause (-1 0), Finish(-2 0)\n");
		printf("Misil coordenates (x y): ");
		scanf("%i", &x);scanf("%i", &y);
		if(x == -1){
			printf("\nBye\n");
		}else if(x == -2){
			restartPositions("userBoard.txt", ourBoard);
			restartPositions("systemBoard.txt", enemyBoard);
			restartPositions("userView.txt", userView);
			restartPositions("enemyView.txt", enemyView);
			printf("\nBye\n");
		}else if(x > 9 || x < 0 || y > 9 || y < 0){
			printf("\nwrong coordenates...\n");
			userTurn(ourBoard, enemyBoard, enemyView, userView);
		}else{
			if(enemyBoard[y+1][x+1] == '$'){
				printf("\nHiit!\n");
				userView[y+1][x+1] = '$';
				writePositions("userView.txt", userView);
				systemTurn(enemyBoard, ourBoard, enemyView, userView);	
			}else{
				printf("\nMiss!\n");
				userView[y+1][x+1] = 'O';
				writePositions("userView.txt", userView);
				systemTurn(enemyBoard, ourBoard, enemyView, userView);
			}
		}
	}
}

void setup(char userBoard[SIZE][SIZE], char systemBoard[SIZE][SIZE], 
		   char userView[SIZE][SIZE], char enemyView[SIZE][SIZE]){
			   
	int n;
	readPositions("systemBoard.txt", systemBoard);
	readPositions("userBoard.txt", userBoard);
	readPositions("userView.txt", userView);
	readPositions("enemyView.txt", enemyView);

	if(getShips(userView) == 0){
		restartPositions("systemBoard.txt", systemBoard);
		generateShip("systemBoard.txt", systemBoard, 2);
		generateShip("systemBoard.txt", systemBoard, 3);
		generateShip("systemBoard.txt", systemBoard, 4);
		generateShip("systemBoard.txt", systemBoard, 5);
		
		printf("Create positions manually or randomly (0 or 1): ");
		scanf("%i", &n);
		
		restartPositions("userBoard.txt", userBoard);
		restartPositions("systemBoard.txt", systemBoard);
		restartPositions("userView.txt", userView);
		restartPositions("enemyView.txt", enemyView);
		
		if(n != 0){ //random users ship selection
			generateShip("userBoard.txt", userBoard, 2);
			generateShip("userBoard.txt", userBoard, 3);
			generateShip("userBoard.txt", userBoard, 4);
			generateShip("userBoard.txt", userBoard, 5);
			printBoard(userBoard);
		}else{
			fillBoard("userBoard.txt", userBoard);
		}
	}else{
		printf("------------\n");
		printf("CONTINUE GAME\n");
		printf("------------\n");
	}
}

int main(int argc, char **argv){
	char userBoard[SIZE][SIZE], systemBoard[SIZE][SIZE], userView[SIZE][SIZE], enemyView[SIZE][SIZE];
	time_t t;
	srand((unsigned) time(&t));
	
	setup(userBoard, systemBoard, userView, enemyView);
	
	userTurn(userBoard, systemBoard, userView, enemyView);	
		
	return 0;
}
