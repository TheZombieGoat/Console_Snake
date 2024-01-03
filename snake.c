#define TB_IMPL
#include "termbox2.h"
#define BD 4 //border distance
#define BD_HEIGHT tb_height() 
#define BD_WIDTH tb_width()
#define SNAKE_COLOR TB_GREEN
#define BORDER_COLOR TB_WHITE

enum dir {up,down,left,right};
int y = 0;
int x = 0;

typedef struct Snake {
  int sx;
  int sy;
  struct Snake *next;
  struct Snake *prev;
} snake;

void print_list(snake *s){
    while(s != NULL){
        tb_printf(s->sx, s->sy, SNAKE_COLOR, 0, "██");
        s = s->next;
    }
}

void print_border(){
    if(BD > BD_HEIGHT || BD > BD_WIDTH){
        tb_printf(20, 20, BORDER_COLOR, 0, "BORDER SIZE ERROR: BORDERS TOO SMALLz");
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

void print_cord(snake *s){
    while(s != NULL){
        tb_printf(++x, ++y, SNAKE_COLOR, 0, "%d   %d", s->sx, s->sy);
        s = s->next;
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
    int curDir;
    struct tb_event ev;
    snake *head;
    head = snake_init(head,50,50);
    snake *tail = head;
    tail = add_end(tail,52,50);
    int ax, ay;

    tb_init();
    tb_poll_event(&ev);
    tb_printf(50, 20, 0, 0, "%d  %d", tail->sx,tail->sy);
    tb_printf(60, 20, 0, 0, "%d  %d", head->sx,head->sy);
    tb_present();
    while(ev.ch != 'q'){
        tb_peek_event(&ev,200);
        switch(ev.key){
            case TB_KEY_ARROW_UP:
                if(curDir == up || curDir == down)
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
            case TB_KEY_HOME:
                tail = add_end(tail,30,30);
                break;
            default:
                break;;
        }
        tb_clear();
        head = switch_front(head,tail,curDir);
        tail = tail->prev;
        tail->next = NULL;
        print_border();
        print_list(head);
        tb_present();      
    }
    tb_printf(10, 10, TB_GREEN, 200, "LIES");
    tb_present();
    tb_poll_event(&ev);
    tb_shutdown();

    return 0;
}
