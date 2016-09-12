#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int *closest_block(int posr, int posc, int dimh, int dimw, char board[3][3]);
int *dist(int posr, int posc, int i, int j);
int man_dist(int posr, int posc, int i, int j);
void findExit();
void next_move(int pid, char board[3][3]){
  //bot will always be at [1][1] 
  //bot faces directon in which it chooses to make its next move
  
  int posr = 1;
  int posc = 1;
  
  int *dist = closest_block(posr, posc, 3, 3, board);
  if(dist != NULL){
    findExit(*dist);
  } 
  if(board[1][0] != '#'){
    printf("LEFT\n");
  }
  else if (board[1][0] == '#'){
    printf("DOWN\n");
  }
  else if(board[1][0] =='-' && board[0][1] && board[1][2] =='-' && board[2][1] == '-'){
    printf("LEFT\n");
  }else if (board[0][1] == '-'){
    printf("LEFT\n");
  }else if (board[0][2] == '#' && board[0][1] == '-'){
    printf("UP\n");
  }else if(board[0][1] == '#' && board[1][0] == '-'){
    printf("LEFT\n");
  }


    

}
void findExit(int *dist){
  int posc = 1;
  int posr = 1;
  
  if(*(dist+0) < 0 && *(dist+0) != 0){
    printf("DOWN\n");
    return;
  }else if( *(dist+0) > 0 && *(dist+0) != 0){
    printf("UP\n");
    return;
  }else if (*(dist+0) ==0){
    //nothing
  }

  if(*(dist+1)>0){
    printf("LEFT\n");
    return;
  }else if (*(dist+1)< 0){
    printf("RIGHT\n");
    return;
  }else if (*(dist+1) == 0){
    //nothing
  }
  if(posc < 4){
    printf("RIGHT\n");
    return;
  }else{
    printf("UP\n"); 
    return;
  }
  if (posr < 4){
    printf("DOWN\n");
    return;
  }else{
    printf("UP\n");
    return; 
  }
 
}

int* closest_block(int posr, int posc, int dimh, int dimw, char board[3][3]){
  
  int man_dist1 = 9999;
  int a = 9999;
  int *clos;

  for(int i = 0; i < dimh; i++){
    for(int j = 0; j < dimw; j++){
      if(board[i][j] == 'e'){
        a = man_dist(posr, posc, i, j);
        if(a < man_dist1){
          man_dist1 = man_dist(posr, posc, i, j);
          clos = dist(posr, posc, i, j);
        }
      }
    }
  }
 return clos;
}
int* dist(int posr, int posc, int i, int j){
  static int coord[2];
  coord[0] = posr-i;
  coord[1] = posc-j;
  return coord;
}

int man_dist(int posr, int posc, int i, int j){
  int one, two;
  one = abs(posr-i);
  two = abs(posc-j);
  return one + two;

}



int main() {
    int pid;
    char board[3][3];
    scanf("%d", &pid);
    for(int i =0; i < 4; i++){
      scanf("%s[^\\n]%*c", board[i]);
    }   
    next_move(pid, board);
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */    
    return 0;
}

