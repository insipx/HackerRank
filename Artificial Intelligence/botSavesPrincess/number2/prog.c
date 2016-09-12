#include <stdio.h>
#include <string.h>
#include <math.h>


void nextMove(int n, int r, int c, char grid[101][101]){
  // r = row pos c = col pos of bot
  // n = size of board
  //Task
  //Complete the function nextMove which takes in 4 parameters - an integer N, integers r and c indicating 
  //the row & column position of the bot and the character array grid - 
  //and outputs the next move the bot makes to rescue the princess. 

  int p[2];
  int b[2];

  b[0] = r;
  b[1] = c;
  
 //find p  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(grid[i][j] == 'p'){
        p[0] = i;
        p[1] = j;
      }
    }
  } 
  
    
    if(b[0] == p[0] && b[1] < p[1]){
      printf("%s\n", "RIGHT");
    }else if(b[0] == p[0] && b[1] > p[1]){
      printf("%s\n", "LEFT");
    }
    
    else if(b[0] < p[0] && b[1] == p[1]){
      printf("%s\n", "DOWN");
    }else if(b[0] > p[0] && b[1] == p[1]){
      printf("%s\n", "UP");
    }
    
    else if(b[0] < p[0] && b[1] < p[1]){
       printf("%s\n", "DOWN");
  //     printf("%s\n", "RIGHT");
    }else if(b[0] > p[0] && b[1] > p[1]){
       printf("%s\n", "UP");
 //      printf("%s\n", "LEFT");
    }
    
    else if(b[0] < p[0] && b[1] > p[1]){
      printf("%s\n", "DOWN");
 //     printf("%s\n", "LEFT");
    }else if(b[0] > p[0] && b[1] < p[1]){
      printf("%s\n", "UP");
  //    printf("%s\n", "RIGHT");
    }else{
      //found it
    }

}

int main(void){
  
  int n, r, c;

  scanf("%d", &n);
  scanf("%d", &r);
  scanf("%d", &c);


  char grid[101][101];

  for(int i = 0; i<n; i++){
    scanf("%s[^\n]%*c", grid[i]);
  }

  nextMove(n, r, c, grid);
  return 0;

}
