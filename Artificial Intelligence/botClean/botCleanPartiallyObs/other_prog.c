#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUM_CELLS 25
#define ROWS 5
#define COLS 5
#define FILE_n "data.dat"

int *move(int posx, int posy, char target, char board[ROWS][COLS]);

void next_move(int posx, int posy, char board[ROWS][COLS]) 
{
  int i, *target_pos, ansx, ansy;
  
  char mapped_board[ROWS][COLS];
  
  FILE *fp;
  fp = fopen(FILE_n, "ab+");
  fread(mapped_board, sizeof(char), NUM_CELLS, fp);
  if(feof(fp) != 0){
    clearerr(fp);
    fseek(fp, SEEK_SET, 0);
  }
  else if(fp){
    char *newBoard = board;
    char *oldBoard = mapped_board;
    for(i = 0; i<25; i++){
      if((*newBoard) == 'o' && (*oldBoard) != 'o') *newBoard = *oldBoard;
      if( (*newBoard) == 'b') *newBoard = '-';
      oldBoard++;
      newBoard++; 
    }
  }
  fclose(fp);

  target_pos = move(posx, posy, 'd', board);
  if(*target_pos == -1000) target_pos = move(posx, posy, 'o', board);

  ansx = *target_pos;
  ansy = *(target_pos+1);


  if(ansx>posx) printf("DOWN");
  else if(ansx<posx) printf("UP");
  else if(ansy>posy) printf("RIGHT");
  else if(ansy<posy) printf("LEFT");
  else{ 
    printf("CLEAN");
    board[posx][posy] = '-';
  }

  fp = fopen(FILE_n, "w");
  fseek(fp, SEEK_SET, 0);
  fwrite(board, sizeof(char), NUM_CELLS, fp);
  fclose(fp);

}
int *move(int posx, int posy, char target, char board[ROWS][COLS] ){
  int static target_pos[2];
  int i, j, ansx=-1000, ansy=-1000;
  for(i=0;i<5;i++){
    for(j=0;j<5;j++){        //manhattan distance in one line
      if((target == (board[i][j])) && (abs(i-posx)+abs(j-posy)) <= (abs(ansx-posx) + abs(ansy-posy))){
        ansx = i;
        ansy = j;
      }
    }
  }
  target_pos[0] = ansx;
  target_pos[1] = ansy;
  return target_pos;
}

int main(void) {
  int pos[2];
  char board[5][5];
  scanf("%d", &pos[0]);
  scanf("%d", &pos[1]);

  for(int i = 0; i<5;i++)
    scanf("%s[^\\n]%*c", board[i]);
  next_move(pos[0],pos[1],board);
  return 0;
}
