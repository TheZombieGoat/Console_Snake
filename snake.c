#define TB_IMPL
#include "termbox2.h"
#define BD 20 //border distance
#define SNAKE_COLOR TB_BLUE
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
    int width = tb_width();
    int height = tb_height();
    int curDir;
    struct tb_event ev;
    snake *head;
    head = snake_init(head,50,50);
    snake *tail = head;
    tail = add_end(tail,52,50);
    tail = add_end(tail,54,50);
    tail = add_end(tail,56,50);

    tb_init();
    tb_poll_event(&ev);
    /*
    head = switch_front(head,tail,up);
    tail = tail->prev;
    tail->next = NULL;
    */
    tb_printf(50, 20, 0, 0, "%d  %d", tail->sx,tail->sy);
    tb_printf(60, 20, 0, 0, "%d  %d", head->sx,head->sy);
    tb_present();
    while(ev.ch != 'q'){
        tb_peek_event(&ev,200);
        switch(ev.key){
            case TB_KEY_ARROW_UP:
                curDir = up;
                tb_printf(20, 20, 0, 0, "pressed UP");
                break;
            case TB_KEY_ARROW_DOWN:
                tb_printf(20, 20, 0, 0, "pressed DOWN");
                curDir = down;
                break; 
            case TB_KEY_ARROW_LEFT:
                tb_printf(20, 20, 0, 0, "pressed LEFT");
                curDir = left;
                break;
            case TB_KEY_ARROW_RIGHT:
                curDir = right;
                tb_printf(20, 20, 0, 0, "pressed RIGHT");
                break;
            default:
                break;;
        }
        tb_clear();
        head = switch_front(head,tail,curDir);
        tail = tail->prev;
        tail->next = NULL;
        print_list(head);
        print_cord(head);
        tb_present();      
    }
    tb_printf(10, 10, TB_GREEN, 200, "LIES");
    tb_present();
    tb_poll_event(&ev);
    tb_shutdown();

    return 0;
}