#include<stdio.h>
#include<stdlib.h>

void next_move(int posx, int posy, char board[5][5]) 
{
   int ansx=-1000,ansy=-1000;
    int i,j;
    
    //iterate through the board
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)         //manhattan distance in one line
            if(board[i][j]=='d' && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
            {
                ansx = i;
                ansy = j;
            }

    if(ansx>posx) printf("DOWN");
    else if(ansx<posx) printf("UP");
    else if(ansy>posy) printf("RIGHT");
    else if(ansy<posy) printf("LEFT");
    else printf("CLEAN");
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

