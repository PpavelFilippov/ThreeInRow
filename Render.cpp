//
// Created by roucr on 26.07.2023.
//
#include "Header.h"

void Render::ToMatCoords(int disp_x, int disp_y, int* mat_x, int* mat_y)
{
    *mat_y = (disp_x - OBJECT_SIZE) / OBJECT_SIZE;
    *mat_x = (disp_y - OBJECT_SIZE) / OBJECT_SIZE;
}

void Render::FieldUpdate(Builder *Matrix)
{
    //ItemsUpdate
    for (int i = 0; i < FIELD_SIZE; i++)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            if (Matrix->getObject(i, j) != nullptr)
                Matrix->getObject(i, j)->Move(OBJECT_SIZE, OBJECT_SIZE);
        }
    }

    //BonusesUpdate
    for (int i = 0; i < FIELD_SIZE*FIELD_SIZE; i++)
    {
        if (Matrix->getBonus(i) != nullptr)
        {
            Matrix->getBonus(i)->BonusMove(OBJECT_SIZE, OBJECT_SIZE );

            if (Matrix->getBonus(i)->speedX == 0.0 && Matrix->getBonus(i)->speedY == 0.0)
            {
                Matrix->DeleteBonus(i, Matrix->getBonus(i));
                Matrix->setBonus(i, nullptr);
            }
        }
    }
}

void Render::PrintItems(Builder *Matrix, sf::RenderWindow *window)
{
    for (int i = FIELD_SIZE-1; i >=0; i--)
    {
        for (int j= FIELD_SIZE - 1; j >= 0; j--)
        {
            Matrix->getObject(i, j)->DrawItem(window);
        }
    }

}

void Render::PrintBonuses(Builder *Matrix, sf::RenderWindow *window)
{
    for (int i = FIELD_SIZE - 1; i >= 0; i--)
    {
        for (int j = FIELD_SIZE - 1; j >= 0; j--)
        {
            Matrix->getBonus(i*FIELD_SIZE + j)->DrawBonus(window);
        }
    }
}

void Render::Swap(Builder *Matrix, SelectedItems *selected)
{
    Object *temp, *First, *Second;

    First = Matrix->getObject(selected->FirstItemX, selected->FirstItemY);
    Second = Matrix->getObject(selected->SecondItemX, selected->SecondItemY);

    First->mat_x = selected->SecondItemX;
    First->mat_y = selected->SecondItemY;

    Second->mat_x = selected->FirstItemX;
    Second->mat_y = selected->FirstItemY;

    First->speedX = -(First->displayX - Second->displayX) / OBJECT_SIZE * FPS;
    First->speedY = -(First->displayY - Second->displayY) / OBJECT_SIZE * FPS; //TODO: kalibrowka

    Second->speedX = -First->speedX;
    Second->speedY = -First->speedY;					             	// Set speed

    temp = Matrix->getObject(selected->FirstItemX, selected->FirstItemY);
    Matrix->setObject(selected->FirstItemX, selected->FirstItemY, Matrix->getObject(selected->SecondItemX, selected->SecondItemY) );
    Matrix->setObject(selected->SecondItemX, selected->SecondItemY, temp);
}

bool Render::LineCheck(Builder *Matrix)
{
    //For Checking
    Object **mass = new Object *[FIELD_SIZE*FIELD_SIZE];
    int num = 0;

    ObjectType currentItem;

    int i=0;
    int count=0;
    bool ans = false;
    for (int x = 0; x < FIELD_SIZE; x++)
    {
        for (int y = 0; y < FIELD_SIZE; y++)
        {
            currentItem = Matrix->getObject(x,y)->getType();

            if (!CheckRepeat(mass, num, Matrix->getObject(x,y)))
            {
                continue;
            }

            //vertical
            i = 0;
            count = 0;
            while ((x + i) < FIELD_SIZE && Matrix->getObject(x + i, y)->getType() == currentItem)
            {
                count++;
                i++;
            }

            i = 1;
            while ((x - i) >= 0 && Matrix->getObject(x - i, y)->getType() == currentItem)
            {
                count++;
                i++;
            }

            if (count >= 3)
            {
                ans = true;
                i = 0;
                while ((x - i) >= 0 && Matrix->getObject(x - i, y)->getType() == currentItem)
                {
                    if (CheckRepeat(mass, num, Matrix->getObject(x - i, y)))
                    {
                        mass[num] = Matrix->getObject(x - i, y);
                        num++;
                    }
                    i++;
                }

                i = 1;
                while ((x + i) < FIELD_SIZE && Matrix->getObject(x + i, y)->getType() == currentItem)
                {
                    if (CheckRepeat(mass, num, Matrix->getObject(x + i, y)))
                    {
                        mass[num] = Matrix->getObject(x + i, y);
                        num++;
                    }
                    i++;
                }
            }

            //horizontal
            i = 0;
            count = 0;
            while ((y + i) < FIELD_SIZE && Matrix->getObject(x , y + i)->getType() == currentItem)
            {
                count++;
                i++;
            }

            i = 1;
            while ((y - i) >= 0 && Matrix->getObject(x, y- i)->getType() == currentItem)
            {
                count++;
                i++;
            }

            if (count >= 3)
            {
                ans = true;
                i = 0;
                while ((y - i) >= 0 && Matrix->getObject(x, y - i)->getType() == currentItem)
                {
                    if (CheckRepeat(mass, num, Matrix->getObject(x, y - i)))
                    {
                        mass[num] = Matrix->getObject(x, y - i);
                        num++;
                    }
                    i++;
                }

                i = 1;
                while ((y + i) < FIELD_SIZE && Matrix->getObject(x, y + i)->getType() == currentItem)
                {
                    if (CheckRepeat(mass, num, Matrix->getObject(x, y + i)))
                    {
                        mass[num] = Matrix->getObject(x, y + i);
                        num++;
                    }
                    i++;
                }
            }
        }
    }

    for (i = 0; i < num; i++)
    {
        //printf("del %i %i %i\n", mass[i]->mat_x, mass[i]->mat_y, mass[i]->getType());
        Matrix->PlayerScore += mass[i]->GetScore();
        Matrix->DeleteObject(mass[i]);
    }

    delete[] mass;

    return ans;
}

bool Render::CheckRepeat(Object **mass, int num, Object* object)
{
    int j;

    for (j = 0; j < num; j++)
    {
        if (j < num && mass[j] == object)
            break;
    }

    if (j == num)
        return true;

    return false;
}



bool Render::AnimationPlaying(Builder *Matrix)
{
    for (int i = 0; i < FIELD_SIZE; i++)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            if (Matrix->getObject(i, j)->speedX != 0.0 || Matrix->getObject(i, j)->speedY != 0.0)
                return true;

            if (Matrix->getBonus(i*FIELD_SIZE + j) != nullptr && (Matrix->getBonus(i*FIELD_SIZE + j)->speedX != 0.0 || Matrix->getBonus(i*FIELD_SIZE + j)->speedY != 0.0))
                return true;
        }
    }

    return false;

}