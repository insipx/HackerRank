#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0


void next_move(int posx, int posy, char board[5][5]) 
{
  char mapped_board[5][5];
  
  FILE *fp;
  fp = fopen("file.txt", "ab+");
  fread(mapped_board, sizeof(char), 25, fp);
  if(feof(fp) != 0){
    clearerr(fp);
    fseek(fp, SEEK_SET, 0);
  }
  fclose(fp);

  int ansx=-1000,ansy=-1000;
  int i,j, contains_d;
  
  contains_d = FALSE;

  for(i=0; i< 5;i++){
    for(j = 0; j<5;j++){
      if(board[i][j] == 'd'){
        contains_d = TRUE; 
        mapped_board[i][j] = 'd';
      }
      else if (board[i][j] == '-' && mapped_board[i][j] != '-') mapped_board[i][j] = '-';
      else if (board[i][j] == 'o' && (mapped_board[i][j] != '-' && mapped_board[i][j] != 'd'))
      {
        mapped_board[i][j] = 'o';
      }
    }
  } 
  
  fp = fopen("file.txt", "w");
  fseek(fp, SEEK_SET, 0);
  fwrite(mapped_board, sizeof(char), 25, fp);
  if(feof(fp) != 0){
    //fuck 
  }

  //iterate through the board
  for(i=0;i<5;i++){
      for(j=0;j<5;j++){        //manhattan distance in one line
        if(contains_d == TRUE){
           if(board[i][j] == 'd' && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
          {
              ansx = i;
              ansy = j;
          }

        } else {
           if(mapped_board[i][j] == 'd' && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
           { 
             ansx = i;
             ansy = j;
           }
           else if(mapped_board[i][j] == 'o' && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
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
}


/* Tail starts here */
int main(void) {
    int pos[2], i;
    char board[5][5];
    scanf("%d", &pos[0]);
    scanf("%d", &pos[1]);
    for(i=0; i<5; i++) {
        scanf("%s[^\\n]%*c", board[i]);
    }
    next_move(pos[0], pos[1], board);
    return 0;
}

