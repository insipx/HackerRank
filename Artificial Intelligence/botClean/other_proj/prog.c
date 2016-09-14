#include<stdio.h>
#include<stdlib.h>

void next_move(int posx, int posy, int dimx, int dimy, char board[50][50]) 
{
   int ansx=-1000,ansy=-1000;
    int i,j;

    //iterate through the board
    for(i=0;i<dimx;i++)
        for(j=0;j<dimy;j++)         //manhattan distance in one line
            if(board[i][j]=='d' && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
            {
                ansx = i;
                ansy = j;
            }
    if(ansx>posx)
        printf("DOWN");
    else if(ansx<posx)
        printf("UP");
    else if(ansy>posy)
        printf("RIGHT");
    else if(ansy<posy)
        printf("LEFT");
    else printf("CLEAN");
}
/* Tail starts here */
int main(void) {
    int pos[2], dim[2], i;
    char board[50][50];
    scanf("%d", &pos[0]);
    scanf("%d", &pos[1]);
    scanf("%d", &dim[0]);
    scanf("%d", &dim[1]);
    for(i=0; i<dim[0]; i++) {
        scanf("%s[^\\n]%*c", board[i]);
    }
    next_move(pos[0], pos[1],dim[0],dim[1], board);
    return 0;
}
