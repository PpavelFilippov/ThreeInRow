//
// Created by roucr on 26.07.2023.
//
#include "Header.h"

Bonus::Bonus(double positionX, double positionY, int x, int y)
{
    displayX = positionX;
    displayY = positionY;
}

void Bonus::ProcessImg() {
    img.createMaskFromColor(img.getPixel(0, 0));
    tex.loadFromImage(img);
    BonusTex.setTexture(tex, true);
}

void Bonus::bombCreator(int x, int y)
{
    img.loadFromFile("resourses/textures/bomb.png");
    do{
        mat_x = x + 2 * (rand() % 3 + 1) - 3;
        mat_y = y + 2 * (rand() % 3 + 1) - 3; //rand item in radius of 3 cells
    } while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);
    score = 0;
}

void Bonus::colorchangeCreator(int x, int y)
{
    img.loadFromFile("resourses/textures/brush.png");
    do{
        mat_x = x + 2 * (rand() % 3 + 1) - 3;
        mat_y = y + 2 * (rand() % 3 + 1) - 3; //rand item in radius of 3 cells
    } while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);
    score = 0;
}

void Bonus::extrapointsCreator(int x, int y)
{
    img.loadFromFile("resourses/textures/extra_points.png");
    mat_x = x - 1;
    mat_y = y; //one up
    score = 200;
}

void Bonus::DrawBonus(sf::RenderWindow *window)
{
    if (this != nullptr)
    {
        BonusTex.setPosition((float)(displayY - bonussize / 2), (float)(displayX - bonussize / 2));

        if (displayX - bonussize / 2 >= 0)
        {
            window->draw(this->BonusTex);
        }
    }

    }

void Bonus::BonusMove(int startX, int startY)
{
    double edgeX, edgeY;
    double speedSign;
    double multiplyer = 1.0;


    edgeX = (double)(bonussize / 2 + bonussize * (mat_x)+startX);
    edgeY = (double)(bonussize / 2 + bonussize * (mat_y)+startY);

    if( (displayY - edgeY )!=0 && (displayX - edgeX) != 0)
        multiplyer = fabs((displayX - edgeX) / (displayY - edgeY));


    if(multiplyer<1.0 && speedX == 0)
    {
        if (displayX - edgeX < 0)
        {
            speedX =  BONUS_VELOCITY * 2.0;
        }
        else
        {
            speedX = -BONUS_VELOCITY * 2.0;
        }

        speedX *= 1 / multiplyer;
    }
    else if (speedX == 0)
    {
        if (displayX - edgeX < 0)
        {
            speedX = BONUS_VELOCITY * 2.0;
        }
        else
        {
            speedX = -BONUS_VELOCITY * 2.0;
        }
    }


    if (multiplyer > 1.0 && speedY == 0)
    {

        if (displayY - edgeY < 0)
        {
            speedY = BONUS_VELOCITY * 2.0;
        }
        else
        {
            speedY = -BONUS_VELOCITY * 2.0;
        }
        speedY *= multiplyer;
    }
    else if (speedY == 0)
    {
        if (displayY - edgeY < 0)
        {
            speedY = BONUS_VELOCITY * 2.0;
        }
        else
        {
            speedY = -BONUS_VELOCITY * 2.0;
        }
    }

    BonusTex.rotate( (float)3.14 / FPS);

    displayY += (speedY * 1.0 / (double)FPS);

    displayX += (speedX * 1.0 / (double)FPS);

    speedSign = speedY / fabs(speedY);

    if (fabs(displayY - edgeY) < bonussize / 16 || speedSign * displayY > speedSign*edgeY)
    {
        displayY = edgeY;
        speedY = 0.0;
    }

    speedSign = speedX / fabs(speedX);

    if (fabs(displayX - edgeX) < bonussize / 16 || speedSign * displayX > speedSign*edgeX)
    {
        displayX = edgeX;
        speedX = 0.0;
    }

}


int Bonus::GetScore() const
{
    return score;
}

