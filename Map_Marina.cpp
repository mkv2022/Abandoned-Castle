// Vorontsova Marina
#include "TXLib.h"

#define size_x 7           // длина карты по х
#define size_y 7           // длина карты по у

int level = 0,
    h_tile = 96,           // высота одной плитки карты (тайла)
    w_tile = 96;           // ширина одной плитки карты (тайла)

int map_ground1 = 5,       // земля вариант 1 (пока не используется)
    map_ground2 = 6,       // земля вариант 2
    map_button = 9,        // кнопка
    map_wall = 0,          // стена
    map_open_door = 8,     // открытая "дверь"
    map_closed_door = 7,   // закрытая "дверь"
    map_water = 2,         // вода
    map_bridge = 4,        // мост
    map_start = 1,         // начало уровня
    map_finish = 3;        // конец уровня

class Lock
{
    private:
    int x_but,
        y_but,
        door[2];

    public:
    Lock(){x_but = -1; y_but = -1;}
    void create(int, int, int, int);
    int check(int, int);
    int* coord_door();
};

void Lock::create(int x1, int y1, int x2, int y2)
{
    x_but = x1;
    y_but = y1;
    door[0] = x2;
    door[1] = y2;
}

int Lock::check(int x, int y)
{
    if(x == x_but && y == y_but)
    {
        return 1;
    }
    else
      return 0;
}
int* Lock::coord_door()
{
   return  door;
}

class Map
{
    private:
    int scheme[size_y][size_x];
    HDC mapimage;
    int locate(int, int);
    Lock doors[11];

    int levels[4][size_y][size_x] = { {
                       {map_wall, map_wall,    map_wall,    map_wall,        map_wall,    map_wall,    map_wall},
                       {map_wall, map_ground2, map_ground2, map_ground2,     map_wall,    map_ground2, map_wall},
                       {map_wall, map_ground2, map_wall,    map_ground2,     map_ground2, map_button,  map_wall},
                       {map_wall, map_start,   map_wall,    map_closed_door, map_wall,    map_wall,    map_wall},
                       {map_wall, map_wall,    map_wall,    map_ground2,     map_wall,    map_ground2, map_wall},
                       {map_wall, map_finish,  map_ground2, map_ground2,     map_ground2, map_ground2, map_wall},
                       {map_wall, map_wall,    map_wall,    map_wall,        map_wall,    map_wall,    map_wall}},

                      {{map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall},
                       {map_wall, map_closed_door, map_button, map_closed_door, map_wall,   map_finish,      map_wall},
                       {map_wall, map_button,      map_wall,   map_button,      map_wall,   map_ground2,     map_wall},
                       {map_wall, map_closed_door, map_wall,   map_closed_door, map_wall,   map_closed_door, map_wall},
                       {map_wall, map_button,      map_wall,   map_button,      map_wall,   map_button,      map_wall},
                       {map_wall, map_start,       map_wall,   map_closed_door, map_button, map_closed_door, map_wall},
                       {map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall}},

                      {{map_wall, map_wall,        map_wall,    map_wall,        map_wall,    map_wall,        map_wall},
                       {map_wall, map_finish,      map_wall,    map_wall,        map_button,  map_start,       map_wall},
                       {map_wall, map_closed_door, map_wall,    map_wall,        map_wall,    map_ground2,     map_wall},
                       {map_wall, map_closed_door, map_ground2, map_closed_door, map_ground2, map_ground2,     map_wall},
                       {map_wall, map_ground2,     map_ground2, map_wall,        map_wall,    map_ground2,     map_wall},
                       {map_wall, map_button,      map_wall,    map_button,      map_button,  map_closed_door, map_wall},
                       {map_wall, map_wall,        map_wall,    map_wall,        map_wall,    map_wall,        map_wall}},

                      {{map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall},
                       {map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall},
                       {map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall},
                       {map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall},
                       {map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall},
                       {map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall},
                       {map_wall, map_wall,        map_wall,   map_wall,        map_wall,   map_wall,        map_wall}}};

    int locks1[5] = {5, 2, 3, 3, -1};
    int locks2[29] = {1, 4, 1, 3,  1, 2, 1, 1,  2, 1, 3, 1,  3, 2, 3, 3,  3, 4, 3, 5,  4, 5, 5, 5,  5, 4, 5, 3,  -1};
    int locks3[17] = {4, 1, 5, 5,  4, 5, 3, 3,  3, 5, 1, 2,  1, 5, 1, 3,  -1};
    int* locks[3] = {locks1, locks2, locks3};

    public:
    Map(HDC);
    void render();
    int collision(int, int, int, int);
    void create_doors();
    void unlock(int, int);
    void load();
};

Map::Map(HDC map_image)
{
    mapimage = map_image;
    load();
}

void Map::load()
{
    for(int i = 0; i < size_x; i++)
        for(int j = 0; j < size_y; j++)
        {
            scheme[j][i] = levels[level][j][i];
        }
    if(level < 3)
        create_doors();
    render();
}

void Map::create_doors()
{
    int i = 0,
        j = 0;
    while(locks[level][i] != -1)
    {
        doors[j].create(locks[level][i], locks[level][i + 1], locks[level][i + 2], locks[level][i + 3]);
        i += 4;
        j += 1;
    }
}

void Map::unlock(int x, int y)
{
    int x_b = x / w_tile;
    int y_b = y / h_tile;
    for(int i = 0; i < 11; i++)
    {
           if (doors[i].check(x_b, y_b))
           {
               int* door = doors[i].coord_door();
               scheme[door[1] ][ door[0] ]  = map_open_door;
           }
    }
}

void Map::render()
{
    for(int i = 0; i < size_x; i++)
        for(int j = 0; j < size_y; j++)
        {
            txTransparentBlt(txDC(), i * w_tile, j * h_tile, w_tile, h_tile, mapimage, scheme[j][i] * w_tile, 0, TX_WHITE);
        }
}

int Map::locate(int x, int y)
{
    return scheme[y / h_tile][x / w_tile];
}

int Map::collision(int x1, int y1, int x2, int y2)
{
    int a = locate(x1, y1);
    int b = locate(x2, y2);
    if(a != map_wall && b != map_wall && a != map_closed_door && b != map_closed_door)
    {
        if(a == map_button)
            unlock(x1, y1);
        else if(b == map_button)
            unlock(x2, y2);
        else if(a == map_finish || b == map_finish)
            {
                level += 1;
                load();
                if(level == 3)
                    return 0;
                return map_finish;
            }
        return 1;
    }
    return 0;
}

