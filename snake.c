#define TB_IMPL
#include <stdlib.h> 
#include "termbox2.h"
#define BD 4 //border distance
#define BD_HEIGHT tb_height() 
#define BD_WIDTH tb_width()
#define SNAKE_COLOR TB_GREEN
#define BORDER_COLOR TB_WHITE

enum dir {up,down,left,right};

typedef struct Snake {
  int sx;
  int sy;
  struct Snake *next;
  struct Snake *prev;
} snake;

void print_list(snake *s){ //prints the snake
    while(s != NULL){
        tb_printf(s->sx, s->sy, SNAKE_COLOR, 0, "██");
        s = s->next;
    }
}


void rand_apple(int *x, int *y){
        *x = rand() % (BD_WIDTH - 4 - BD) + BD + 2;
        *y = rand() % (BD_HEIGHT - 4 - BD) + BD + 2;
}

//rand() % (max_number + 1 - minimum_number) + minimum_number
void print_apple(int *x, int *y, snake *s){
	snake *p = s;
	rand_apple(x,y);
	while(p != NULL){
	  if(p->sx == *x && p->sy == *y){
	      rand_apple(x,y);
	      p = s;  
	      if(p->sx == *x && p->sy == *y)
		 rand_apple(x,y); 
	  }  	  
	  p = p->next;
	}
        tb_printf(*x, *y, TB_RED, 0, "██");
}

void print_border(){
    if(BD > BD_HEIGHT || BD > BD_WIDTH){
        tb_printf(20, 20, BORDER_COLOR, 0, "BORDER SIZE ERROR: BORDERS TOO SMALL");
        return;
    }
    int height = BD_HEIGHT - BD;
    int width = BD_WIDTH - BD;
    for(int i = BD; i <= width; i+=2){   //printing the horizontal border
        tb_printf(i, BD, BORDER_COLOR, 0, "██");
        tb_printf(i, height, BORDER_COLOR, 0, "██");
    }
    for(int i = BD; i <= height; i++){
        tb_printf(BD, i, BORDER_COLOR, 0, "██");
        tb_printf(width, i, BORDER_COLOR, 0, "██");
    }
}

snake *snake_init(snake *s, int sx, int sy){
    s = (snake*)malloc(sizeof(snake));
    s->sx = sx;
    s->sy = sy;
    s->next = NULL;
    s->prev = NULL;
    return s;
}

//testing purposes
snake *switch_front(snake *head, snake *tail,int dir){  
    switch(dir){
        case up:
            tail->sx = head->sx;
            tail->sy = head->sy - 1;
            break; 
        case down:
            tail->sx = head->sx;
            tail->sy = head->sy + 1;
            break;
        case left:
            tail->sx = head->sx - 2;
            tail->sy = head->sy;
            break;
        case right:
            tail->sx = head->sx + 2;
            tail->sy = head->sy;
            break;   
    }

    if(tail->prev == NULL){
        return head;
    }else{
        snake *newhead = snake_init(newhead,tail->sx,tail->sy);
        newhead->next = head;
        head->prev = newhead;
        return newhead;
    }
}

snake *add_end(snake *tail, int sx, int sy){ 
    snake *newhead = snake_init(newhead,sx,sy);
    tail->next = newhead;
    newhead->prev = tail;
    return newhead;
}

int main(int argc, char **argv) {
    int ax, ay, curDir;
    int over = 0;
    struct tb_event ev;
    snake *head;
    head = snake_init(head,BD + 20,BD + 20);
    snake *tail = head;
    tail = add_end(tail,BD + 22,BD + 20);
    snake *p = head;
    tb_init();
    tb_poll_event(&ev);
    print_apple(&ax,&ay, head);
    int height = BD_HEIGHT - BD;
    int width = BD_WIDTH - BD;

    while(ev.ch != 'q'){
        tb_peek_event(&ev,200);
        switch(ev.key){ //changes direction based on input
            case TB_KEY_ARROW_UP:
                if(curDir == up || curDir == down) //can't go to opposite direction so down inp is nullified
                    break;
                curDir = up;
                break;
            case TB_KEY_ARROW_DOWN:
                if(curDir == up || curDir == down)
                    break;
                curDir = down;
                break; 
            case TB_KEY_ARROW_LEFT:
                if(curDir == left || curDir == right)
                    break;
                curDir = left;
                break;
            case TB_KEY_ARROW_RIGHT:
                if(curDir == left || curDir == right)
                    break;
                curDir = right;
                break;
            default:
                break;
        }
        if((head->sx == ax || head->sx == ax+1) && head->sy == ay){ //interaction with apple
            print_apple(&ax,&ay,head);
	    tail = add_end(tail,head->sx+2,head->sy);
	}
	
	if(head->sx >= width || head->sx <= BD || head->sy >= height || head->sy <= BD)
		break;

	p = head->next;
	while(p != NULL){
	    if(p->sx == head->sx && p->sy == head->sy){
		    over = 1;
		    break;
	    }
	    p = p->next;
	}
	if(over == 1)
	    break;
        tb_clear();
        head = switch_front(head,tail,curDir);
        tail = tail->prev;
        tail->next = NULL;
        tb_printf(ax, ay, TB_RED, 0, "██");
        tb_printf(2, 2, TB_RED, 0, "Apple cords: %d %d", ax,ay);
	tb_printf(36, 2, TB_BLUE, 0, "Snake Cords: %d %d", head->sx,head->sy);
        print_border();
        print_list(head);
        tb_present();      
    }
    tb_printf(10, 10, TB_GREEN, 200, "GAME OVER");
    tb_present();
    tb_poll_event(&ev);
    tb_shutdown();

    return 0;
}
