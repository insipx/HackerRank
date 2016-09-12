#include <stdio.h>
#include <string.h>
#include <math.h>

void displayPathtoPrincess(int n, char grid[101][101]){
  //n-1 moves to get from center to corner
  int p[2];
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(grid[i][j] == 'p') {
        p[0] = i; 
        p[1] = j;
      }
    }
  }

  //i is down, j is across
  //top right 
  if(p[0] == 0 && p[1] > 0){
    for(int i = 0; i < (n-1)/2; i++){
    printf("%s\n", "RIGHT");
    printf("%s\n", "UP");
    }
    //bottom left
  }else if(p[0] > 0 && p[1] == 0){

    for(int i = 0; i < (n-1)/2; i++){
    printf("%s\n", "LEFT");
    printf("%s\n", "DOWN");
    }

   //bottom right 
  }else if(p[0] > 0 && p[1] > 0){

    for(int i = 0; i < (n-1)/2; i++){
    printf("%s\n", "RIGHT");
    printf("%s\n", "DOWN");
    }

   //top left 
  }else{

    for(int i = 0; i < (n-1)/2; i++){
    printf("%s\n", "LEFT");
    printf("%s\n", "UP");
    }
  }
}

int main (void) {
  
  int m;

  scanf("%d", &m);
  char grid[101][101]={};
  char line [101];


  for(int i = 0; i<m; i++) {
    scanf("%s", line);
    strcpy(grid[i], line);
  }

  displayPathtoPrincess(m,grid);

  return 0;

}
