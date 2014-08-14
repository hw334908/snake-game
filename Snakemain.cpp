
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <list>
#include <time.h>
#include "Snake.h"

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;
const int FPS = 15;
enum KEYS{UP,DOWN,LEFT,RIGHT,A};

void DrawBoard();
void InitSnake(list<Snake>& snake);
void DrawSnake(list<Snake>& snake);
void MoveUp(list<Snake>& snake);
void MoveDown(list<Snake>& snake);
void MoveLeft(list<Snake>& snake);
void MoveRight(list<Snake>& snake);
void MoveRest(list<Snake>& snake);
void AddSnake(list<Snake>& snake);
void InitItem(Item& item);
int Randomx();
int Randomy();
void DrawItem(Item& item);
void CollideItem(Item& item, list<Snake>& snake);
void CollideSnake(list<Snake>& snake);
 
int main(int argc, char **argv)
{
   list<Snake> snake;
   Item item;
   bool redraw = true;
   bool isGameOver= false;
   bool done = false; 
   bool keys[5] = {false, false, false, false,false};
     
   
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_FONT *font18 = NULL;
   
   //Snake snake[MAX_SNAKES];
 
   if(!al_init()) 
      return -1;
   display = al_create_display(WIDTH,HEIGHT);
   if(!display) 
      return -1;
      
   al_install_keyboard();
   al_init_primitives_addon();
   al_init_font_addon();
   al_init_ttf_addon();
   
   
   srand(time(NULL));
   InitSnake(snake);
   InitItem(item);
	srand(time(NULL));
   font18 = al_load_font("arial.ttf",18,0);
   event_queue = al_create_event_queue();
   timer = al_create_timer(1.0/FPS);
   
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   int n = 1;
   
   al_start_timer(timer);
   while(!done)
   {
     
     
     ALLEGRO_EVENT ev;
     al_wait_for_event(event_queue, &ev);
     if(ev.type == ALLEGRO_EVENT_TIMER)
     {
       redraw = true;
       if(keys[UP] == true)
       {
         if(snake.front().direction != DOWN)
             snake.front().direction = UP;
         
       }
       else if(keys[DOWN] == true)
       { 
         if(snake.front().direction != UP)
           snake.front().direction = DOWN;

       }
       else if(keys[LEFT] == true)
       {
         if(snake.front().direction != RIGHT)
           snake.front().direction = LEFT;
      
       }       
       else if(keys[RIGHT] == true)
       {
         if(snake.front().direction != LEFT)
           snake.front().direction = RIGHT;
         
       }
       if(!isGameOver)
       {

           if(snake.front().direction == UP)
           {
             MoveRest(snake);
             MoveUp(snake);    
           }
           else if(snake.front().direction == DOWN)
           { 
             MoveRest(snake);         
             MoveDown(snake);
    
           }
           else if(snake.front().direction == LEFT)
           {
             MoveRest(snake);
             MoveLeft(snake);
          
           }       
           else if(snake.front().direction == RIGHT)
           {
             MoveRest(snake);        
             MoveRight(snake);
             
           }
           CollideSnake(snake);
           if(snake.front().dead)
           {
             isGameOver = true;
             
           }
           CollideItem(item,snake);
       }
       
    
     }
     else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
     {
          done = true;
     }
     else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
     {
       switch(ev.keyboard.keycode)
       {
         case ALLEGRO_KEY_ESCAPE:
         done = true;
         break;
         
         case ALLEGRO_KEY_UP:
         keys[UP] = true;
         break;
         
         case ALLEGRO_KEY_DOWN:
         keys[DOWN] = true;
         break;   
         
         case ALLEGRO_KEY_LEFT:
         keys[LEFT] = true;
         break; 
         
         case ALLEGRO_KEY_RIGHT:
         keys[RIGHT] = true;
         break; 
         
         case ALLEGRO_KEY_A:
         keys[A] = true;
         break;
       }
     }
     else if(ev.type == ALLEGRO_EVENT_KEY_UP)
     {
       switch(ev.keyboard.keycode)
       {
         case ALLEGRO_KEY_UP:
         keys[UP] = false;
         break;
         
         case ALLEGRO_KEY_DOWN:
         keys[DOWN] = false;
         break;   
         
         case ALLEGRO_KEY_LEFT:
         keys[LEFT] = false;
         break; 
         
         case ALLEGRO_KEY_RIGHT:
         keys[RIGHT] = false;
         break; 
         
         case ALLEGRO_KEY_A:
         keys[A] = false;
         break;
       }
     }
     if(redraw && al_is_event_queue_empty(event_queue))
     {
       redraw = false;
       if(isGameOver)
       {
         al_draw_textf(font18, al_map_rgb(255,255,255),WIDTH/2,HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "Game is over. Score is %i", snake.front().score);
       }
       else
       {
           if(!item.live)
           {
             item.x = Randomx();
             item.y = Randomy();
             item.live = true;
           }
           DrawItem(item);
           DrawBoard();             
           DrawSnake(snake);
       }
       al_flip_display();   
       al_clear_to_color(al_map_rgb(0,0,0));
     }     
   } 
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   al_destroy_timer(timer);
 
   return 0;
}

void DrawBoard()
{
     al_draw_filled_rectangle(5,5,WIDTH-15,15,al_map_rgb(255,255,255));
     al_draw_filled_rectangle(5,5,15,HEIGHT-15,al_map_rgb(255,255,255));
     al_draw_filled_rectangle(5,HEIGHT-5,WIDTH-15,HEIGHT-15,al_map_rgb(255,255,255)); 
     al_draw_filled_rectangle(WIDTH-5,5,WIDTH-15,HEIGHT-5,al_map_rgb(255,255,255));
}    
void InitSnake(list<Snake>& snake)
{
     Snake s;
     s.score = 0;
     s.length = 1;
     s.speed = 2;
     s.startx = 320;
     s.starty = 240;
     s.direction = LEFT;
     s.dead = false;
     snake.push_front(s);
}
void DrawSnake(list<Snake>& snake)
{
     list<Snake>::iterator it;
     it = snake.begin();
     while(it != snake.end())
     {
       al_draw_filled_rectangle(it->startx,it->starty,it->startx+15,it->starty+15,al_map_rgb(255,255,255));

       ++it;
     }

}
     

void MoveUp(list<Snake>& snake)
{

    // snake.front().starty -= snake.front().speed;
     
     snake.front().starty -= 16;
     if(snake.front().starty < 5)
     {
       snake.front().dead = true;
     }
}
void MoveDown(list<Snake>& snake)
{
 
     snake.front().starty += 16;
     if(snake.front().starty >= HEIGHT-17)
     {
       snake.front().dead = true;
     }
}
void MoveLeft(list<Snake>& snake)
{
     
     snake.front().startx -= 16;
     if(snake.front().startx < 11)
     {
       snake.front().dead = true;
     }
}
void MoveRight(list<Snake>& snake)
{

     snake.front().startx += 16;
     if(snake.front().startx >= WIDTH-19)
     {
       snake.front().dead = true;
     }
}
void MoveRest(list<Snake>& snake)
{
     int oldx = snake.front().startx;
     int oldy = snake.front().starty;
     int newy,newx;
     int direction = snake.front().direction;
     list<Snake>::iterator it;
     it = snake.begin();
     ++it;
     while(it != snake.end())
     {
       if(direction == UP)
       {
         

         newx = it->startx;
         newy = it->starty;
         it->starty = oldy;
         it->startx = oldx;
         oldx=newx;
         oldy=newy;
         direction = it->direction;
         it->direction = UP;
       }
       else if(direction == DOWN)
       {
        
         newx = it->startx;
         newy = it->starty;
         it->starty = oldy;
         it->startx = oldx;
         oldx= newx;
         oldy=newy;
         direction = it->direction;
         it->direction = DOWN;
       }
        else if(direction == LEFT)
       {

        newx = it->startx;
         newy = it->starty;
         it->starty = oldy;
         it->startx = oldx;
         oldx= newx;
         oldy=newy;
         direction = it->direction;
         it->direction = LEFT;
       }
       else if(direction == RIGHT)
       {                 
         newx = it->startx;
         newy = it->starty;
         it->starty = oldy;
         it->startx = oldx;
         oldx= newx;
         oldy=newy;
         direction = it->direction;
         it->direction = RIGHT;
       }
       ++it;
     }
}
void AddSnake(list<Snake>& snake)
{
     snake.front().score++;

     Snake s;
     if(snake.back().direction == UP)
     {
       s.direction = UP;
       s.startx = snake.back().startx;
       s.starty = snake.back().starty +16;
       snake.push_back(s);
     }
     else if(snake.back().direction == DOWN)
     {
       s.direction = DOWN;
       s.startx = snake.back().startx;
       s.starty = snake.back().starty -16;
       snake.push_back(s);
     }   
     else if(snake.back().direction == LEFT)
     {
       s.direction = LEFT;
       s.startx = snake.back().startx+16;
       s.starty = snake.back().starty ;
       snake.push_back(s);
     }  
     else if(snake.back().direction == RIGHT)
     {
       s.direction = RIGHT;
       s.startx = snake.back().startx-16;
       s.starty = snake.back().starty ;
       snake.push_back(s);
     }
}
int Randomx()
{
    int x=1;
   /* do{
      x = (rand()%601) +20;
      }while(x%5 != 0);*/
    while(x%16 != 0)
    {
      x = rand()%641;
      if(x <20 || x > WIDTH -20)
        x=1;
    }
    return(x);
}
int Randomy()
{
    int x=1;/*
    do{
      x = (rand()%441) +20;
      }while(x%5 != 0);*/
    while(x%16 != 0)
    {
      x = rand()%481;
      if(x <20 || x > HEIGHT -20)
        x=1;
    }
    return(x);
}
void DrawItem(Item& item)
{
      al_draw_filled_rectangle(item.x,item.y,item.x +15,item.y+15,al_map_rgb(255,255,255));
}
void InitItem(Item& item)
{
      item.x = Randomx();
       item.y = Randomy();
       item.live = true;

}  
void CollideItem(Item& item, list<Snake>& snake)
{
     if(snake.front().startx == item.x && snake.front().starty == item.y)
     {
       item.live = false;
       AddSnake(snake);
     }
}
void CollideSnake(list<Snake>& snake)
{
     list<Snake>::iterator it;
     it= snake.begin();
     int x,y;
     x = snake.front().startx;
     y = snake.front().starty;
     ++it;
     while(it != snake.end() && snake.front().dead == false)
     {
       if(x == it->startx && y == it->starty)
       {
            snake.front().dead = true;
       }
       ++it;
     }
}
       
        
