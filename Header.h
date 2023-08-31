//
// Created by roucr on 26.07.2023.
//
#ifndef INC_3IN1_HEADER_H
#define INC_3IN1_HEADER_H
#ifndef HEADER_H
#define HEADER_H

#include <cmath>
#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"
#include <random>

#define FPS 120
#define FALL_ACCELERATION 1000.0
#define BONUS_VELOCITY 125.0
#define FIELD_SIZE 10
#define OBJECT_SIZE 64

enum ObjectType
{
    apple,
    banana,
    candy,
    grape,
    orange,
    strawberry,
    watermelon

};

class Object;
class Bonus;

class Builder {
public:
    Builder();
    ~Builder();

    void FixField() const;
    void ObjectFall(int x_t, int y_t) const;

    static void ToDispCoords(int x, int y, int* x_new, int* y_new);
    void DeleteObject(Object* object) const;
    void DeleteBonus(int i, Bonus* bonus);
    void DeleteAnimations() const;
    void DeleteBonuses() const;

    Object* getObject(int i, int j) const;
    void setObject(int i, int j, Object* object) const;
    Bonus* getBonus(int i) const;
    void setBonus(int i, Bonus* bonus) const;

    int PlayerScore = 0;

    Object*** ObjectsField;
    Bonus** BonusMass;
};

class Object {
private:
    sf::Texture tex;
    sf::Image img;
    sf::Sprite ItemTex;
    ObjectType type;
    int score{};

public:
    double displayX{}, displayY{};
    int mat_x{}, mat_y{};
    int size = 64;
    double speedX = 0; // with falling acceleration
    double speedY = 0; //constant

    Object() = default;
    Object(int positionX, int positionY, int x, int y, ObjectType objectType);
    ~Object() = default;

    void Move(int startX, int startY);
    void DrawItem(sf::RenderWindow *window);
    int GetScore() const;
    ObjectType getType();
    void setType(ObjectType newtype);
    void changeImage();
};

enum BonusType {
    bomb,
    colorchange,
    extrapoints
};

class Bonus {
public:
    sf::Texture tex;
    sf::Image img;
    sf::Sprite BonusTex;
    int score{};

    Bonus() = default;
    Bonus(double positionX, double positionY, int x, int y);
    ~Bonus() = default;

    void bombCreator(int x, int y);
    void colorchangeCreator(int x, int y);
    void extrapointsCreator(int x, int y);

    BonusType getType();
    void ProcessImg();

    int GetScore() const;
    void DrawBonus(sf::RenderWindow *window);
    void BonusMove(int startX, int startY);
    virtual void MakeAction(Builder* builder) = 0;

    double displayX{}, displayY{};
    int mat_x{}, mat_y{};
    int bonussize = 64;
    double speedX = 0.0;
    double speedY = 0.0;
};

class BonusBomb : public Bonus {
public:
    BonusBomb(double positionX, double positionY, int x, int y) : Bonus(positionX, positionY, x, y) {
        img.loadFromFile("resourses/textures/bomb.png"); //TODO: check random
        ProcessImg();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(-1, 1); //-1 0 1

        do {
            mat_x = x + 2 * dist(gen); // rand item in radius of 3 cells
            mat_y = y + 2 * dist(gen);
        } while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);

        score = 0;
    }

    void MakeAction(Builder* builder) override {
        if (builder->ObjectsField[mat_x][mat_y] != nullptr)
        {
            builder->DeleteObject(builder->ObjectsField[mat_x][mat_y]);
        }
    }
};


class BonusColorChange : public Bonus {
public:
    BonusColorChange(double positionX, double positionY, int x, int y) : Bonus(positionX, positionY, x, y) {
        img.loadFromFile("resourses/textures/brush.png");
        ProcessImg();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(-1, 1);

        do {
            mat_x = x + 2 * dist(gen); // rand item in radius of 3 cells
            mat_y = y + 2 * dist(gen);
        } while (mat_x < 0 || mat_x >= 10 || mat_y < 0 || mat_y >= 10);

        score = 0;
    }

    void MakeAction(Builder* builder) override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 6);

        builder->ObjectsField[mat_x][mat_y]->setType(static_cast<ObjectType>(dist(gen))); // Generate a random item type
        builder->ObjectsField[mat_x][mat_y]->changeImage();
    }
};

class BonusExtraPoints : public Bonus {
public:
    BonusExtraPoints(double positionX, double positionY, int x, int y) : Bonus(positionX, positionY, x, y) {
        img.loadFromFile("resourses/textures/extra_points.png");
        ProcessImg();
        mat_x = x - 1;
        mat_y = y; //one up
        score = 200;
    }

    void MakeAction(Builder* builder) override {
        builder->PlayerScore += GetScore();
    }
};


struct SelectedItems {
    int FirstItemX = -1, FirstItemY = -1; //TODO: rename
    int SecondItemX = -1, SecondItemY = -1;
};



class Render {
public:
    Render() = default;
    ~Render() = default;

    static void ToMatCoords(int dispX, int dispY, int* mat_x, int* mat_y);

    static void FieldUpdate(Builder *Matrix); // values
    static void PrintItems(Builder *Matrix, sf::RenderWindow *window);  // draw
    static void PrintBonuses(Builder *Matrix, sf::RenderWindow *window); // draw
    static bool AnimationPlaying(Builder *Matrix);
    static bool LineCheck(Builder *Matrix);
    static bool CheckRepeat( Object **mass, int num, Object* object);

    static void Swap(Builder *Matrix, SelectedItems *selected);
};

void DrawGrid(sf::RenderWindow *window);
void GameProcess(Builder *Matrix, Render *Field, sf::RenderWindow *window);
void Actions(Builder *Matrix, Render *Field, sf::RenderWindow *window, SelectedItems *Selected, bool *Gameover, sf::RectangleShape *rect);
void DeathScreen(Builder *Matrix, Render *Field, sf::RenderWindow *window);
void MainMenu(Builder *Matrix, Render *Field, sf::RenderWindow *window);


#endif
#endif