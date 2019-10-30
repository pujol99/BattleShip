#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 12

void userTurn(char userBoard[SIZE][SIZE], char systemBoard[SIZE][SIZE], char userShots[SIZE][SIZE], char systemShots[SIZE][SIZE]);

void display_rules(){
	printf("\n\n\n\tRules\n");
	printf("\t1. No ships crossing\n");
	printf("\t2. A line of water minimum within ships\n");
	printf("\t3. Input coordenates like this: 2 3\n");
}

void clear(){
	system("@cls||clear");
}

void readPositions(char path[28], char board[SIZE][SIZE]){
	FILE *f = fopen(path, "r");
	char line[24];
	int i, j = 0;
	if(f){
		while(fgets(line, sizeof(line), f)){
			for(i = 0; i < SIZE; i++){
				board[j][i] = line[i];
			}j++;
		}
	}
}

void updatePositions(char path[28], char board[SIZE][SIZE]){
	FILE *f = fopen(path, "w");
	int i, j;
	if(f){
		for(i = 0; i < SIZE; i++){
			fprintf(f, "X");
		}
		fprintf(f, "\nX");
		for(i = 1; i < SIZE-1; i++){
			for(j = 1; j < SIZE-1; j++){
					fprintf(f, "%c", board[i][j]);
			}fprintf(f, "X\nX");
		}
		for(i = 0; i < SIZE-1; i++){
			fprintf(f, "X");
		}
	}fclose(f);
}

void writeShip(char path[28], char board[SIZE][SIZE], int size, int dir, int x, int y){
	int i;
	if(dir){ 	//horitzonal
		for(i = 0; i < size; i++){
			board[y+1][x+1+i] = '$';
		}
	}else{		//vertical
		for(i = 0; i < size; i++){
			board[y+1+i][x+1] = '$';
		}
	}	
	updatePositions(path, board);
}

int getRand(int max){
	return rand() % max;
}

int itsValid(int dir, int size, int xo, int yo, char board[SIZE][SIZE]){
	int i;
	xo++;
	yo++;
	if(dir == 0){	//VERTICAL
		if(yo > SIZE-2-size+1){
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
		if(xo > SIZE-2-size+1){
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
		yo = getRand(SIZE-2-size+1);
	}else{
		xo = getRand(SIZE-2-size+1);
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
		updatePositions(path, board);
	}
}

void printBoard(char board[SIZE][SIZE]){
	int i, j;
	printf("\n ");
	for(i = 1; i < SIZE-1; i++){
		for(j = 1; j < SIZE-1; j++){
			printf("%c ", board[i][j]);
		}printf("%i\n ", i-1);
	}printf("0 1 2 3 4 5 6 7 8 9\n");
}

void printBoards(char board[SIZE][SIZE], char board2[SIZE][SIZE]){
	int i, j;
	printf("\n-----------------------------------------------");
	printf("\n User shots            |          System shots\n ");
	for(i = 1; i < SIZE-1; i++){
		for(j = 1; j < SIZE-1; j++){
			printf("%c ", board[i][j]);
		}printf("%i | ", i-1);
		for(j = 1; j < SIZE-1; j++){
			printf("%c ", board2[i][j]);
		}printf("%i\n ", i-1);
	}printf("0 1 2 3 4 5 6 7 8 9   | 0 1 2 3 4 5 6 7 8 9\n\n");
}
	
void fillBoard(char path[28], char board[SIZE][SIZE]){
	int i, x, y, dir;
	for(i = 2; i < 6; i++){
		printBoard(board);
		printf("\n Input the %i-size ship coordenates(x y): ", i);
		scanf("%i", &x);
		scanf("%i", &y);
		printf(" Vertical or Horitzonal (0 or 1): ");
		scanf("%i", &dir);
		if(itsValid(dir, i, x, y, board) == 0){
			printf("\n Wrong position think it better...\n");
			i--;
		}else{
			writeShip(path, board, i, dir, x, y);
		}
	}
	clear();
	printf("\n\tUser board:\n");
	printBoard(board);
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

void shoot(char userBoard[SIZE][SIZE], char systemShots[SIZE][SIZE], int i, int j){
	if(userBoard[j][i] == '$'){
		systemShots[j][i] = '$';
		updatePositions("systemShots.txt", systemShots);	
	}else{
		systemShots[j][i] = 'O';
		updatePositions("systemShots.txt", systemShots);
	}
}

void nextPosition(int *x, int *y){
	*y += 2;
	if(*y > 9){
		if(*x%2 == 0){
			*y = 1;
		}else{
			*y = 0;
		}*x += 1;
	}
}

void AIsystem(char userBoard[SIZE][SIZE],char systemBoard[SIZE][SIZE], 
				char userShots[SIZE][SIZE], char systemShots[SIZE][SIZE], int x, int y){
	int i = x + 1, j = y + 1;
	if(systemShots[j][i] == '-'){	//unexplored --> shoot
		shoot(userBoard, systemShots, i, j);
		userTurn(userBoard, systemBoard, userShots, systemShots);
	}
	else if(systemShots[j][i] == '$'){	//ship part --> try to find entire ship
	
		if(systemShots[j][i-1] == '$'){ //check behind
			
			if(systemShots[j][i+1] == '$'){ //check right
				
				if(systemShots[j][i+2] == '$'){ //check right
					
					if(systemShots[j][i+3] == '-'){ //check right
						shoot(userBoard, systemShots, i+3, j);
						userTurn(userBoard, systemBoard, userShots, systemShots);
					}else{
						nextPosition(&x, &y);	//next position
						AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
					}
				}else if(systemShots[j][i+2] == '-'){
					shoot(userBoard, systemShots, i+2, j);
					userTurn(userBoard, systemBoard, userShots, systemShots);
				}else{
					nextPosition(&x, &y);	//next position
					AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
				}
				
			}else if(systemShots[j][i+1] == '-'){
				shoot(userBoard, systemShots, i+1, j);
				userTurn(userBoard, systemBoard, userShots, systemShots);
			}else{
				nextPosition(&x, &y);	//next position
				AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
			}
		}else if(systemShots[j][i-1] == '-'){
			shoot(userBoard, systemShots, i-1, j);
			userTurn(userBoard, systemBoard, userShots, systemShots);
		}else{
			if(systemShots[j-1][i] == '$'){
				
				if(systemShots[j+1][i] == '$'){ //check down
					
					if(systemShots[j+2][i] == '$'){ //check down
						
						if(systemShots[j+3][i] == '-'){ //check down
							shoot(userBoard, systemShots, i, j+3);
							userTurn(userBoard, systemBoard, userShots, systemShots);
						}else{
							x = i-1;
							y = j+2;
							nextPosition(&x, &y);	//next position
							AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
						}
					}else if(systemShots[j+2][i] == '-'){
						shoot(userBoard, systemShots, i, j+2);
						userTurn(userBoard, systemBoard, userShots, systemShots);
					}else{
						x = i-1;
						y = j+1;
						nextPosition(&x, &y);	//next position
						AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
					}
					
				}else if(systemShots[j+1][i] == '-'){
					shoot(userBoard, systemShots, i, j+1);
					userTurn(userBoard, systemBoard, userShots, systemShots);
				}else{
					x = i-1;
					y = j;
					nextPosition(&x, &y);	//next position
					AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
				}
			}else if(systemShots[j-1][i] == '-'){
				shoot(userBoard, systemShots, i, j-1);
				userTurn(userBoard, systemBoard, userShots, systemShots);
			}else{
				if(systemShots[j][i+1] == '$'){ //check right
				
					if(systemShots[j][i+2] == '$'){ //check right
						
						if(systemShots[j][i+3] == '$'){ //check right
						
							if(systemShots[j][i+4] == '-'){ //check right
								shoot(userBoard, systemShots, i+4, j);
								userTurn(userBoard, systemBoard, userShots, systemShots);
							}else{
								nextPosition(&x, &y);	//next position
								AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
							}
						}else if(systemShots[j][i+3] == '-'){
							shoot(userBoard, systemShots, i+3, j);
							userTurn(userBoard, systemBoard, userShots, systemShots);
						}else{
							nextPosition(&x, &y);	//next position
							AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
						}
					}else if(systemShots[j][i+2] == '-'){
						shoot(userBoard, systemShots, i+2, j);
						userTurn(userBoard, systemBoard, userShots, systemShots);
					}else{
						nextPosition(&x, &y);	//next position
						AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
					}
				
				}else if(systemShots[j][i+1] == '-'){
					shoot(userBoard, systemShots, i+1, j);
					userTurn(userBoard, systemBoard, userShots, systemShots);
				}else{
					//
					if(systemShots[j+1][i] == '$'){ //check down
					
						if(systemShots[j+2][i] == '$'){ //check down
							
							if(systemShots[j+3][i] == '$'){ //check down
							
								if(systemShots[j+4][i] == '-'){ //check down
									shoot(userBoard, systemShots, i, j+4);
									userTurn(userBoard, systemBoard, userShots, systemShots);
								}else{
									x = i-1;
									y = j+3;
									nextPosition(&x, &y);	//next position
									AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
								}
							}else if(systemShots[j+3][i] == '-'){
								shoot(userBoard, systemShots, i, j+3);
								userTurn(userBoard, systemBoard, userShots, systemShots);
							}else{
								x = i-1;
								y = j+2;
								nextPosition(&x, &y);	//next position
								AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
							}
						}else if(systemShots[j+2][i] == '-'){
							shoot(userBoard, systemShots, i, j+2);
							userTurn(userBoard, systemBoard, userShots, systemShots);
						}else{
							x = i-1;
							y = j+1;
							nextPosition(&x, &y);	//next position
							AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
						}
						
					}else if(systemShots[j+1][i] == '-'){
						shoot(userBoard, systemShots, i, j+1);
						userTurn(userBoard, systemBoard, userShots, systemShots);
					}else{
						x = i-1;
						y = j;
						nextPosition(&x, &y);	//next position
						AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
					}
				}
			}
		}
	}else{
		nextPosition(&x, &y);	//next position
		AIsystem(userBoard, systemBoard, userShots, systemShots, x, y);
	}
	
}

void systemTurn(char userBoard[SIZE][SIZE],char systemBoard[SIZE][SIZE], 
				char userShots[SIZE][SIZE], char systemShots[SIZE][SIZE]){
	if(getShips(userShots) == 14){
		printf("\nGame finished, YOU won!\n");
		restartPositions("userBoard.txt", userBoard);
		restartPositions("systemBoard.txt", systemBoard);
		restartPositions("userShots.txt", systemShots);
		restartPositions("systemShots.txt", systemShots);
	}else{
		AIsystem(userBoard, systemBoard, userShots, systemShots, 0, 0);
	}
}

void userTurn(char userBoard[SIZE][SIZE], char systemBoard[SIZE][SIZE],
				char userShots[SIZE][SIZE], char systemShots[SIZE][SIZE]){
	printBoards(userShots, systemShots);
	int x, y;
	if(getShips(systemShots) == 14){
		printf("\nGame finished, SYSTEM won!\n");
		restartPositions("userBoard.txt", userBoard);
		restartPositions("systemBoard.txt", systemBoard);
		restartPositions("userShots.txt", userShots);
		restartPositions("systemShots.txt", systemShots);
	}else{
		printf("  Pause (-1 0), Finish(-2 0)\n");
		printf("  Misil coordenates (x y): ");
		scanf("%i", &x);scanf("%i", &y);
		if(x == -1){
			printf("\n\n\n");
		}else if(x == -2){
			restartPositions("userBoard.txt", userBoard);
			restartPositions("systemBoard.txt", systemBoard);
			restartPositions("userShots.txt", userShots);
			restartPositions("systemShots.txt", systemShots);
			printf("\nBye\n");
		}else if(x > 9 || x < 0 || y > 9 || y < 0){
			clear();
			printf("\n\tWrong coordenates...\n");
			userTurn(userBoard, systemBoard, userShots, systemShots);
		}else{
			if(systemBoard[y+1][x+1] == '$'){
				clear();
				printf("\n\tHIT!\n");
				userShots[y+1][x+1] = '$';
				updatePositions("userShots.txt", userShots);
				systemTurn(userBoard, systemBoard, userShots, systemShots);	
			}else{
				clear();
				printf("\n\tMISS!\n");
				userShots[y+1][x+1] = 'O';
				updatePositions("userShots.txt", userShots);
				systemTurn(userBoard, systemBoard, userShots, systemShots);
			}
		}
	}
}

void setup(char userBoard[SIZE][SIZE], char systemBoard[SIZE][SIZE], 
		   char userShots[SIZE][SIZE], char systemShots[SIZE][SIZE]){  
	int n;
	//PASS DATA FROM FILES TO VARIABLES
	readPositions("systemBoard.txt", systemBoard);
	readPositions("userBoard.txt", userBoard);
	readPositions("userShots.txt", userShots);
	readPositions("systemShots.txt", systemShots);

	if(getShips(userBoard) < 14){		//IF USER HASN'T ALL SHIPS ON BOARD RESTART POSITIONING
		clear();
		printf("  Create your positions manually or randomly (0 or 1): ");
		scanf("%i", &n);
		//RESTART POSITIONS
		restartPositions("systemBoard.txt", systemBoard);
		restartPositions("userBoard.txt", userBoard);
		restartPositions("userShots.txt", userShots);
		restartPositions("systemShots.txt", systemShots);
		//GENERATE system SHIPS RANDOMLY
		generateShip("systemBoard.txt", systemBoard, 2);
		generateShip("systemBoard.txt", systemBoard, 3);
		generateShip("systemBoard.txt", systemBoard, 4);
		generateShip("systemBoard.txt", systemBoard, 5);
		clear();
		if(n != 0){ 			//GENERATE USER SHIPS RANDOMLY
			generateShip("userBoard.txt", userBoard, 2);
			generateShip("userBoard.txt", userBoard, 3);
			generateShip("userBoard.txt", userBoard, 4);
			generateShip("userBoard.txt", userBoard, 5);
			printf("\n\tUser board:\n");
			printBoard(userBoard);
		}else{					//GENERATE USER SHIPS MANUALLY
			display_rules();
			fillBoard("userBoard.txt", userBoard);
		}
		
		printf("\n-----------------/GAME STARTED/----------------\n");
	}else{						//CONTINUE GAME AS IT WAS
		clear();
		printf("\n\n\n----------------/GAME RESTARTED/---------------\n");
	}
}

int main(int argc, char **argv){
	//GLOBLAL VARIABLES
	char userBoard[SIZE][SIZE], systemBoard[SIZE][SIZE], userShots[SIZE][SIZE], systemShots[SIZE][SIZE];
	time_t t;
	srand((unsigned) time(&t));
	
	//GAME
	setup(userBoard, systemBoard, userShots, systemShots);		//PREPARE GAME
	
	userTurn(userBoard, systemBoard, userShots, systemShots);	//START GAME
		
	return 0;
}
