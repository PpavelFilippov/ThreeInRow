//
// Created by roucr on 26.07.2023.
//
#include "Header.h"

Object::Object(int positionX, int positionY, int x, int y, ObjectType object_type )
{
    type = object_type;
    score = 10 * type;
    displayX = (double)positionX;
    displayY = (double)positionY;
    mat_x = x;
    mat_y = y;

    changeImage();
}

void Object::DrawItem(sf::RenderWindow *window)
{
    if (this != nullptr)
    {
        ItemTex.setPosition( (float)(displayY - size / 2), (float)(displayX - size / 2) );

        if (displayX - size / 2 >= 0)
        {
            window->draw(this->ItemTex);
        }
    }

    }

int Object::GetScore() const
{
    return score;
}

ObjectType Object::getType()
{
    return type;
}

void Object::setType(ObjectType newtype)
{
    type = newtype;
}

void Object::changeImage()
{
    sf::String names[7] = {"apple", "banana", "candy", "grape", "orange", "strawberry", "watermelon" };

    img.loadFromFile("resourses/textures/" + names[(int)(type)] + ".png");
    img.createMaskFromColor(img.getPixel(0, 0));
    tex.loadFromImage(img);
    ItemTex.setTexture(tex, true);
}

void Object::Move(int startX, int startY)
{
    double edgeX, edgeY;
    double speedSign;
    int aroundPoint = size / 8;

    edgeX = (double)(size / 2 + size * (mat_x)+startX);
    edgeY = (double)(size / 2 + size * (mat_y)+startY);


    if (std::abs(displayX - edgeX) <= aroundPoint)
    {
        displayX = edgeX;
        speedX = 0.0;
    }
    else
    {
        speedSign = (edgeX - displayX) / fabs(displayX - edgeX);
        displayX += (speedX / (double)FPS);
        speedX += speedSign * FALL_ACCELERATION / FPS;
    }

    if (std::abs(displayY - edgeY) <= aroundPoint)
    {
        displayY = edgeY;
        speedY = 0.0;
    }
    else
    {
        speedSign = (edgeY - displayY) / fabs(displayY - edgeY);
        displayY += (speedY / (double)FPS);
        //speedy += speed_sign * FallAcceleration / FPS;
    }

}