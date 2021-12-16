// Vorontsova Marina
#include "TXLib.h"
#include "Map_Marina.cpp"

class Ghost
{
    private:
    int x,             // координата x в окне
        y,             // координата y в окне
        w = 46,        // ширина картинки
        h = 48,        // высота картинки
        st = 3;        // шаг персонажа
    HDC image;         // spritesheet
    int xpic = 0,      // координата х на изображении
        ypic = 0;      // координата у на изображении
    char a;            // направление персонажа
    int dir_x = 1,     // направление изменения поз персонажа на spritesheet-е
        line = 2,      // длина "строки"
        s_of_line,     // начало "строки" с костюмами для ходьбы в ту или иную сторону
        e_of_line;     // конец "строки" с костюмами для ходьбы в ту или иную сторону
    Map* map;          // карта
    int start[3][2] = {{w_tile + 28, 3 * h_tile + 28}, {w_tile + 28, 5 * h_tile + 28}, {5 * w_tile + 28, h_tile + 28}};

    void turn_up();    // направление персонажа (вверх)
    void turn_down();  // направление персонажа (вниз)
    void turn_right(); // направление персонажа (вправо)
    void turn_left();  // направление персонажа (влево)
    void go();         // ходьба персонажа

    public:
    Ghost(HDC, Map*);
    void draw();       // отрисовка персонажа
    void check();      // проверка клавиш
};

Ghost::Ghost(HDC image_gh, Map* map1)
{
    x = start[level][0];
    y = start[level][1];
    image = image_gh;
    map = map1;
}

void Ghost::draw()
{
    txTransparentBlt (txDC(), x, y, w, h, image, xpic, ypic, TX_WHITE);
}

void Ghost::turn_up()
{
    if(a != 'u')
    {
        xpic = w * 3;
        ypic = h * 0;
        a = 'u';
        s_of_line = xpic;
        e_of_line = s_of_line + line * w;
    }
}

void Ghost::turn_down()
{
    if(a != 'd')
    {
        xpic = w * 0;
        ypic = h * 0;
        a = 'd';
        s_of_line = xpic;
        e_of_line = s_of_line + line * w;
    }
}

void Ghost::turn_right()
{
    if(a != 'r')
    {
        xpic = w * 6;
        ypic = h * 1;
        a = 'r';
        s_of_line = xpic;
        e_of_line = s_of_line + line * w;
    }
}

void Ghost::turn_left()
{
    if(a != 'l')
    {
        xpic = w * 6;
        ypic = h * 0;
        a = 'l';
        s_of_line = xpic;
        e_of_line = s_of_line + line * w;
    }
}

void Ghost::go()
{
    int coord_h = h / 3,   // всопомогательное значение для вычисления границы "ног" персонажа по у
        coord_w = w / 7,   // всопомогательное значение для вычисления границы "ног" персонажа по х
        state = 1;

    if(a == 'u'      && (state = map->collision(x + coord_w + st, y + coord_h, x + w - st, y + coord_h)) != 0)
        y -= st;
    else if(a == 'r' && (state = map->collision(x + w, y + coord_h + st, x + w, y + h - st)) != 0)
            x += st;
    else if(a == 'd' && (state = map->collision(x + coord_w + st, y + h, x + w - st, y + h)) != 0)
            y += st;
    else if(a == 'l' && (state = map->collision(x + coord_w, y + coord_h + st, x + coord_w, y + h - st)) != 0)
            x -= st;

    if(state == map_finish)
    {
        x = start[level][0];
        y = start[level][1];
    }

    if(xpic >= e_of_line)
        dir_x = -1;
    if(xpic <= s_of_line)
        dir_x = 1;
    xpic += dir_x * w;
}

void Ghost::check()
{
    if(GetAsyncKeyState(VK_UP))
    {
        turn_up();
        go();
    }
    if(GetAsyncKeyState(VK_DOWN))
    {
        turn_down();
        go();
    }
    if(GetAsyncKeyState(VK_RIGHT))
    {
        turn_right();
        go();
    }
    if(GetAsyncKeyState(VK_LEFT))
    {
        turn_left();
        go();
    }
}

int main()
{
    txCreateWindow(size_x * w_tile, size_y * h_tile);
    HDC map_image = txLoadImage("map.bmp");

    HDC main_image = txLoadImage("ghost.bmp");

    Map map1(map_image);
    Ghost gh(main_image, &map1);

    while(!GetAsyncKeyState(VK_ESCAPE) && level < 3)
    {
        txBegin();
        txClear();
        map1.render();
        gh.check();
        gh.draw();
        txEnd();
        txSleep(40);
    }

    txDeleteDC (map_image);
    txDeleteDC (main_image);

    if(level == 3)
    {
        txSelectFont ("Comic Sans MS", 60);
        txTextOut (175, 200, "Congratulations!");
        txTextOut (240, 300, "You win!");
    }
}
