#include<stdio.h>
#include<stdlib.h>
//LL to hold where we've already moved
struct moves_node {
  int p[2];
  struct moves_node *next;
};

typedef struct moves_node Node;

void append_dist(int posxy, int posy);
void append(Node *newNode);

Node *head = NULL;

void next_move(int posx, int posy, char board[5][5]) 
{
    Node *temp = head;

    int ansx=-1000,ansy=-1000;
    int i,j, contains_d;
    contains_d = 0; // 0 is false 1 is true
     
    char go = 'd';

    for(i=0; i< 5;i++){
      for(j = 0; j<5;j++){
        if(board[i][j] == go) contains_d = 1;
      }
    } 
    if(contains_d == 0){
      while(temp != NULL){

      }
    
    }

    //iterate through the board
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)         //manhattan distance in one line
            if(board[i][j]==go && (abs(i-posx)+abs(j-posy))<= (abs(ansx-posx)+abs(ansy-posy)))
            {
                ansx = i;
                ansy = j;
            }
    
    if(ansx>posx) {
      printf("DOWN");
      posx++;
      append_dist(posx,posy);
    }
    else if(ansx<posx){
      printf("UP");
      posx--;
      append_dist(posx,posy);
    }
    else if(ansy>posy){
      printf("RIGHT");
      posy++;
      append_dist(posx,posy);
    }
    else if(ansy<posy) {
      printf("LEFT");
      posy--; 
      append_dist(posx,posy);
    }
    else{
      printf("CLEAN");
      append_dist(posx, posy);
    }
}

void append_dist(int posx, int posy){
  Node *aNode = NULL;
  aNode = (Node *) malloc(sizeof(Node));
  aNode->p[0] = posx;
  aNode->p[1] = posy;
  append(aNode);
}


void append(Node *newNode) {
  Node *temp = head; 

  if (head == NULL){
    head = newNode;
    return;
  }
  while (temp->next != NULL) temp = temp->next;
  temp -> next = newNode;
  newNode->next = NULL;
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

