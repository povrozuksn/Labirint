#include "TXLib.h"

struct Personage
{
    int x;
    int y;
    HDC image;
    int w;
    int h;
    int vx;
    int vy;

    void draw()
    {
        txTransparentBlt(txDC(), x, y, w, h, image, 0, 0, TX_WHITE);
    }
};

struct Barrier
{
    int x;
    int y;
    int w;
    int h;
    COLORREF color;

    void draw()
    {
        txSetColor(color);
        txSetFillColor(color);
        txRectangle(x, y, x+w, y+h);
    }
};


int main()
{
txCreateWindow (800, 800);

    HDC fon = txLoadImage("Pictures/labirint.bmp");

    Personage hero = {80, 5, txLoadImage("Pictures/герой.bmp"), 40, 50, 10, 10};
    int x_hero_old;
    int y_hero_old;

    Personage enemy1 = {743, 743, txLoadImage("Pictures/враг.bmp"), 45, 50, 10, 0};
    Personage enemy2 = {15, 320, enemy1.image, 45, 50, 0, 10};
    Personage enemy3 = {743, 250, enemy1.image, 45, 50, 5, 0};

    Barrier bar1 = {300, 300, 200, 200, TX_BLACK};

    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        txSetColor (TX_WHITE);
        txSetFillColor (TX_WHITE);
        txClear();

        txBegin();
        txBitBlt(txDC(), 0, 0, 800, 800, fon);

        hero.draw();
        enemy1.draw();
        enemy2.draw();
        enemy3.draw();
        bar1.draw();

        x_hero_old = hero.x;
        y_hero_old = hero.y;

        if(GetAsyncKeyState(VK_RIGHT))
        {
            hero.x += hero.vx;
        }
        if(GetAsyncKeyState(VK_LEFT))
        {
            hero.x -= hero.vx;
        }
        if(GetAsyncKeyState(VK_UP))
        {
            hero.y -= hero.vy;
        }
        if(GetAsyncKeyState(VK_DOWN))
        {
            hero.y += hero.vy;
        }

        //Условие непроходимости препядствия по координатам
        if( hero.x+hero.w > bar1.x && hero.x < bar1.x+bar1.w &&
            hero.y+hero.h > bar1.y && hero.y < bar1.y+bar1.h)
        {
            hero.x = x_hero_old;
            hero.y = y_hero_old;
        }


        //Условие непроходимости препядствия по цвету
        for(int x=hero.x; x<hero.x+hero.w; x+=10)
        {
            for(int y=hero.y; y<hero.y+hero.h; y+=10)
            {
                if(txGetPixel(x, y) == RGB(255, 0, 0))
                {
                    hero.x = x_hero_old;
                    hero.y = y_hero_old;
                }
            }
        }

        char str[100];
        sprintf(str, "x= %d y= %d", hero.x, hero.y);
        txSetColor(TX_BLACK);
        txTextOut(650, 20, str);

        enemy1.x += enemy1.vx;
        if(enemy1.x<0 || enemy1.x>800) enemy1.vx = - enemy1.vx;
        enemy3.x += enemy3.vx;
        if(enemy3.x<0 || enemy3.x>800) enemy3.vx = - enemy3.vx;
        enemy2.y += enemy2.vy;
        if(enemy2.y<0 || enemy2.y>800) enemy2.vy = - enemy2.vy;

        txEnd();
        txSleep(10);

    }

    txDeleteDC(fon);
    txDeleteDC(enemy1.image);

txTextCursor (false);
txDisableAutoPause();
return 0;
}

