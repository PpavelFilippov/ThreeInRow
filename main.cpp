#include "Header.h"

int main()
{
    Render Field;
    Builder Matrix;

    Matrix.FixField();

    //Window settings
    sf::RenderWindow window;
    window.create(sf::VideoMode(1440, 900), "Three in row");
    window.setFramerateLimit(FPS);

    while (window.isOpen())
    {
        //Main Menu
        //MainMenu(&Matrix, &Field, &window);

        //main game
        GameProcess(&Matrix, &Field, &window);

        //End
       // DeathScreen(&Matrix, &Field, &window);
    }

    return 0;

}