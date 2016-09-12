#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int *closest_block(int posr, int posc, int dimh, int dimw, char board[5][5]);
int *dist(int posr, int posc, int i, int j);
int man_dist(int posr, int posc, int i, int j);

void next_move(int posr, int posc, char board[5][5]){
   
  if(board[posr][posc] == 'd'){
    printf("CLEAN\n");
    return;
  }

  int *dist = closest_block(posr, posc, 5, 5, board);

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

int* closest_block(int posr, int posc, int dimh, int dimw, char board[5][5]){
  
  int man_dist1 = 9999;
  int a = 9999;
  int *clos;

  for(int i = 0; i < dimh; i++){
    for(int j = 0; j < dimw; j++){
      if(board[i][j] == 'd'){
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

