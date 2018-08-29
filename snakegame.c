#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <malloc.h>

#define H 25	//hight of the map
#define W 80	//width of the map

int level = 2;		//level of difficulty 1 - 3, default 2(median)
COORD coord={0,0};	//use for function gotoxy
int score = 0;		//score of the game
int speed = 61;	//speed of the game/ moving

typedef struct snake{
	int x, y;    //location of the snake
	int dirc;	//direction of the snake movinng
}Snake;

typedef struct food{
	int x, y;	//location of the food
}Food;

void init();
void gotoxy(int x, int y);	//since cannot use gotoxy directly, take out the code of this function and declare it  
Food *createfood(); 			//randomly create food at any place in the board
void createsnake();
int move(char dir, int x, int y);		//dir is input direction, x and y use for location 
int eatfood();
void erasetail(int x, int y);
void restart();
int gameover();

Snake *head = NULL;
Snake body[166]; 		//max length of the snake, keep x-y index of all body parts
Food *food = NULL;
int len = 10;			//init len of snake

int main(int argc, char* argv[]){

	
	system("cls");

	init();
	createsnake();
    food = createfood();
	
	while(1){			
		
		Sleep(speed);

		char dirc;

		if(food == NULL){
			food = createfood();
		}
	
		if(kbhit()){	//if user input
			dirc = getch();		//get input direction
			if(move(dirc, head->x, head->y) == 0){
				
				if(!gameover()){
					continue;
				}
				break;
			}
		}else		
			if(move(head->dirc, head->x, head->y) == 0){
				
				if(!gameover()){
					continue;
				}
				break;
			}	
		
	
	}	

return 1;
}

void init(){
	
	int level = 2;
	
	system("color 0B");		//set background color and text color
	
	//introduce the rule
	printf("Welcome to the snake game!\n");
	printf("\nPlease use WSAD to control the snake\n");
	printf("-> W for up\n-> S for down\n-> A for left\n-> D for right\n");
	printf("\nPress any key to start the game");
	
	if(getch()){
		system("cls");
		gotoxy(10,20);
		printf("\nPlease select a level of difficulty: 1 for hardest, 2 for median, and 3 for easiest\nType number and press ENTER\n Enter : ");
		scanf("%d", &level);
		switch(level){
			case 1:
				speed = 21;
				break;
			
			case 3:
				speed = 200;
				break;
				
			//alreasy set level 2 for 250 in the global var. speed
		}
	}
	
	//Print boarder	
	system("cls");
	int i, j;
	for(i = 0; i < W; i++){
	
				gotoxy(i,5);
					printf("-");
				gotoxy(i,25);
					printf("-");			
		}

		for(j = 5; j <= H; j++){
			
				gotoxy(0,j);
					printf("|");
				gotoxy(80,j);
					printf("|");			
		}	
		
		//print the score
		gotoxy(83, 8);
		printf("Score: %d",score);
	
}

Food *createfood(){
	
	Food *food = malloc(sizeof(Food));
	int X = rand()%80;				//x-axis btw 0 - 79
	int Y = rand()%25;				//y axis

	if(Y >= 25){						//make sure it do not go outside of the board
		Y = Y-6;
	}
	if(Y <= 5){
		Y = Y+6;
	}
	if(X == 0){
		X = X + 1;
	}
	
	gotoxy(X,Y);
	printf("$");
	
	food->x = X;
	food->y = Y;
	
	return food;
}

void createsnake(){
	
	body[0].x = 15;								//initial location
	body[0].y = 15;
	body[0].dirc = 'w';							//default direction
	head = &body[0];
	
	gotoxy(15,15);
	printf("@");		//head
	
	move('w',15,15);

}

int move(char dir, int x, int y){
	
	tolower(dir);		
		
	int hx = head->x;		//location of current head
	int hy = head->y;
	
	int tx = body[len-1].x;		//location of currennt tail, might need to be removed
	int ty = body[len-1].y;			
				
	switch(dir){
		
		case 'w':	//move up
			if(head->dirc == 's'){
				break;
			}
			head->y = (head->y)-1;
			head->dirc = 'w';
		
			break;
			
		case 's':	//down
			
			if(head->dirc == 'w'){
				break;
			}
			head->y = (head->y) +1;
			head->dirc = 's';
					
			break;
			
		case 'a':	//left
			
			if(head->dirc == 'd'){
				break;
			}
			head->x = (head->x)-1;			
			head->dirc = 'a';
			
			break;
			
		case 'd':	//right
			
			if(head->dirc == 'a'){
				break;
			}
			head->x = (head->x)+1;
			head->dirc = 'd';
		
			break;
	}
	
			gotoxy(head->x,head->y);			//go print new head
			printf("@");
			
			if(head->x == 0 || head->x == 80){		//this two if check if snake touches the wall
				return 0;
			}
			if(head->y == 5 || head->y == 25){
				return 0;
			}
			
			eatfood(head->x,head->y);		//check if snake eats food 				
			erasetail(tx,ty);					//erase old tail
			
	
	int i, a , b;
	for(i = len-1; i > 0; i--){			//update movement, all go to location of previous block of body 
		if(head->x == body[i].x && head->y == body[i].y){		//check if snake eats its body
			return 0;
		}
		if(i == 1){
			a = body[1].x = hx;
			b = body[1].y = hy;
		}else{
			a = body[i].x = body[i-1].x;
			b = body[i].y = body[i-1].y;
		}	
		gotoxy(a,b);
		printf("*");					//body move 
	}

	return 1;
}

int eatfood(int X, int Y){
	
	if(food == NULL || head == NULL){
		return 0;
	}
	if(head->x == food->x && head->y == food->y){		//if head touches food				
				score++;								//update score
				gotoxy(83, 8);
				printf("Score: %d",score);
				
				free(food);
				
				len++;									//body length ++
				food = createfood();					//create new food
			
				return 1;
			}	
	
	return 0;		
}

void erasetail(int x,int y){			//x-y is the location of old tail
	
	gotoxy(x,y);
	printf(" ");
}

void restart(){
	
	food = NULL;		//reset initial value
	head = NULL;
	len = 10;
	score = 0;
	memset(body, 0 ,sizeof(body));
	
	system("cls");		//clear and reset screen
	init();
	createsnake();
    food = createfood();
}

int gameover(){
	
		system("cls");
		gotoxy(50,5);
		printf("GAME OVER!\n\n\n-> Your Score: %d\n-> Do you want to restart the game?\n-> Please press y or n to select\n", score);
				
		char start;
		start = getch();		//ask use if they want to restart the game
		if(start == 'y'){
			system("cls");
			restart();
			return 0;
		}
		
	return 1;
}

void gotoxy(int x,int y){				//go to x-y location 
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}


