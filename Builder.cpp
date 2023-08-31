//
// Created by roucr on 26.07.2023.
//
#include "Header.h"



Builder::Builder()
{
    PlayerScore = 0;

    BonusMass = new Bonus*[FIELD_SIZE*FIELD_SIZE];

    ObjectsField = new Object**[FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; i++)
    {
        ObjectsField[i] = new Object*[FIELD_SIZE];
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            ObjectsField[i][j] = nullptr;
            BonusMass[i*FIELD_SIZE + j] = nullptr;
        }

    }
}

Builder::~Builder()
{
    for (int i = 0; i < FIELD_SIZE; i++)
        delete[] this->ObjectsField[i];

    delete[] ObjectsField;
    delete[] BonusMass;
}

void Builder::ToDispCoords(int x, int y, int* new_x, int* new_y)
{
    *new_x = OBJECT_SIZE + OBJECT_SIZE * x + OBJECT_SIZE / 2;
    *new_y = OBJECT_SIZE + OBJECT_SIZE * y + OBJECT_SIZE / 2;
}

void Builder::ObjectFall(int x_t, int y_t) const
{
    int i = x_t + 1;

    while (i < FIELD_SIZE && ObjectsField[i][y_t] == nullptr)
    {
        i++;
    }

    i -= (x_t + 1);  //Fall while you can

    if (i > 0)
    {
        ObjectsField[i + x_t][y_t] = ObjectsField[x_t][y_t];
        ObjectsField[i + x_t][y_t]->mat_x = i + x_t;
        ObjectsField[i + x_t][y_t]->mat_y = y_t;
        ObjectsField[x_t][y_t] = nullptr;
    }

    }

void Builder::FixField() const
{
    int i = 0;
    int new_x, new_y;
    ObjectType new_type;

    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister random number engine
    std::uniform_int_distribution<> dist(0, 6); // To generate values from 0 to 6

    for (i = FIELD_SIZE - 2; i >= 0; i--)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            if (ObjectsField[i][j] != nullptr)
                ObjectFall(i, j);
        }
    }

    for (i = FIELD_SIZE - 1; i >= 0; i--)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            if (ObjectsField[i][j] == nullptr)
            {
                // ToDispCoords(-(FieldSize - i) * size / 32 - j * size / 32, j, &new_x, &new_y);
                ToDispCoords(-FIELD_SIZE / 16, j, &new_x, &new_y);
                new_type = static_cast<ObjectType>(dist(gen)); // Get random ItemType between 0 and 6
                ObjectsField[i][j] = new Object(new_x, new_y, i, j, new_type);
                ObjectsField[i][j]->speedX = 3.0 * BONUS_VELOCITY * (1.0 - static_cast<double>(j) / 20.0);
            }
        }
    }

    // printf("\n Fix Complete!\n");
}


void Builder::DeleteObject(Object* object) const
{
    int x = object->mat_x;
    int y = object->mat_y;

    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister random number engine
    std::uniform_int_distribution<> dist(0, 99); // To generate values from 0 to 99

    if (dist(gen) >= 65)	//creating bonus
    {
        int i = 0;
        std::uniform_int_distribution<> bonusDist(0, 2); // To generate values from 0 to 2

        BonusType new_type = static_cast<BonusType>(bonusDist(gen));

        while (i < FIELD_SIZE*FIELD_SIZE && BonusMass[i] != nullptr)
            i++;

        if (new_type == bomb)
            for (int j = 0; j < 4; j++)
                BonusMass[i] = new BonusBomb(object->displayX, object->displayY, object->mat_x, object->mat_y);
        else if(new_type == colorchange)
            BonusMass[i] = new BonusColorChange(object->displayX, object->displayY, object->mat_x, object->mat_y);
        else
            BonusMass[i] = new BonusExtraPoints(object->displayX, object->displayY, object->mat_x, object->mat_y);
    }

    Object* t = ObjectsField[object->mat_x][object->mat_y];
    delete t;
    ObjectsField[x][y] = nullptr;
    FixField();
}


void Builder::DeleteBonus(int i, Bonus* bonus)
{
    bonus->MakeAction(this);

    delete bonus;
    BonusMass[i] = nullptr;
}

void Builder::DeleteBonuses() const
{
    for (int i = 0; i < FIELD_SIZE; i++)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            if (BonusMass[i*FIELD_SIZE + j] != nullptr)
            {
                delete BonusMass[i*FIELD_SIZE + j];
                BonusMass[i*FIELD_SIZE + j] = nullptr;
            }

        }
    }
}

void Builder::DeleteAnimations() const
{
    int _x, _y;

    for (int i = 0; i < FIELD_SIZE; i++)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {

            if (BonusMass[i*FIELD_SIZE + j] != nullptr)
            {
                ToDispCoords(BonusMass[i*FIELD_SIZE + j]->mat_x, BonusMass[i*FIELD_SIZE + j]->mat_y, &_x, &_y);
                BonusMass[i*FIELD_SIZE + j]->displayX = _x;
                BonusMass[i*FIELD_SIZE + j]->displayY = _y;
            }
        }
    }
}

Object* Builder::getObject(int i, int j) const
{
    return ObjectsField[i][j];
}

Bonus* Builder::getBonus(int i) const
{
    return BonusMass[i];
}

void Builder::setBonus(int i, Bonus *bonus) const
{
    BonusMass[i] = bonus;
}

void Builder::setObject(int i, int j, Object* object) const
{
    ObjectsField[i][j] = object;
}