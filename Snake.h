using namespace std;
struct Snake
{
       int score;
       int length;
       int speed;
       int startx;
       int starty;
       int endx;
       int endy;
       bool dead;
       int direction;
};

struct Item
{
       int x;
       int y;
       int boundx;
       int boundy;
       bool live;
};

