#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define NUM_CELLS 49
#define ROWS 7
#define COLS 7
#define FILE_n "data.dat"
//need to rework this code for mazefinder
void next_move(int posx, int posy, int pid, char board[5][5]) 
{
  int i, j;
  char mapped_board[ROWS][COLS];
  
  FILE *fp;
  fp = fopen(FILE_n, "ab+");
  fread(mapped_board, sizeof(char), NUM_CELLS, fp);
  if(feof(fp) != 0){
    clearerr(fp);
    fseek(fp, SEEK_SET, 0);
  }
  if(fp){
    //use pointers to iterate through board
    //
      char *newBoard = board;
      char *oldBoard = mapped_board;
    for(i = 0; i<ROWS; i++){
      if((*newBoard) == 'o' && (*oldBoard) != 'o') 
        *newBoard = *oldBoard;
      oldBoard++;
      newBoard++; 
    }
  }
  fclose(fp);

  int ansx=-1000,ansy=-1000;
  int contains_e;
  
  
  //iterate through the board
  for(i=0;i<5;i++){
      for(j=0;j<5;j++){        //manhattan distance in one line
        if(contains_e == TRUE){
           if(board[i][j] == 'd' && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
          {
              ansx = i;
              ansy = j;
          }
        } else {
            if(mapped_board[i][j] == 'd'  && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
           { 
             ansx = i;
             ansy = j;
           }
            else if(mapped_board[i][j] == 'o'&& (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
           { 
              ansx = i;
              ansy = j;
           }
        }
      }
  }
  
  if(ansx>posx)
    printf("DOWN");
  else if(ansx<posx)
    printf("UP");
  else if(ansy>posy)
    printf("RIGHT");
  else if(ansy<posy)
    printf("LEFT");
  else
    printf("CLEAN");
 
  fp = fopen("file.txt", "w");
  fseek(fp, SEEK_SET, 0);
  fwrite(mapped_board, sizeof(char), NUM_CELLS, fp);
  fclose(fp);

}
int *move()
//characters included:
//walls -> #
//empty cells -> -
//door -> e
/* Tail starts here */
int main(void) {
    int pos[2], i, pid;
    char board[3][3];
    scanf("%d", &pid);
 
    for(i=0; i<3; i++) {
        scanf("%s[^\\n]%*c", board[i]);
    }
    for(int i = 0; i < 3; i ++)
      for(int j = 0; j < 3; j++)
        if(board[i][j] == 'b'){
          pos[0] = i;
          pos[1] = j;
        }
   
    next_move(pos[0], pos[1], pid, board);

    return 0;
}

