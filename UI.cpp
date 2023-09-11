//
// Created by roucr on 26.07.2023.
//
#include "Header.h"

void DrawGrid(sf::RenderWindow *window)
{
    sf::RectangleShape gridLine(sf::Vector2f(OBJECT_SIZE * FIELD_SIZE, 2));
    gridLine.setFillColor(sf::Color::Black);

    // horizontal lines
    for (int y = 1; y <= FIELD_SIZE + 1; ++y)
    {
        gridLine.setPosition(OBJECT_SIZE, y * OBJECT_SIZE );
        window->draw(gridLine);
    }

    // vertical lines
    for (int x = 1; x <= FIELD_SIZE + 1; ++x)
    {
        gridLine.setPosition(x * OBJECT_SIZE, OBJECT_SIZE);
        gridLine.setRotation(90);
        window->draw(gridLine);
    }
}

void GameProcess(Builder *Matrix, Render *Field, sf::RenderWindow *window)
{
    //Settings for game
    int MouseDelay=0;
    int disp_x = sf::Mouse::getPosition(*window).x;
    int disp_y = sf::Mouse::getPosition(*window).y;
    bool GameOver = false;
    struct SelectedItems *Selected;
    Selected = new SelectedItems;

    //Rectangle selected
    sf::RectangleShape BackOfShell;
    BackOfShell.setSize(sf::Vector2f(64, 64));
    BackOfShell.setOutlineColor(sf::Color::Yellow);
    BackOfShell.setFillColor(sf::Color::Red);
    BackOfShell.setOutlineThickness(2);
    BackOfShell.setPosition(1000, 2000);

    //Sprite Background
    sf::Texture backTexture;
    backTexture.loadFromFile("resourses/textures/scene.png");
    sf::Sprite background(backTexture);

    //Text player score
    sf::Font font;
    font.loadFromFile("resourses/Bahnschrift.ttf");
    sf::Text PlayerScore("", font, 25);
    PlayerScore.setFillColor(sf::Color::Black);
    PlayerScore.setPosition(960, 150);

    //Button
    sf::RectangleShape Button;
    Button.setSize(sf::Vector2f(250,75));
    Button.setFillColor(sf::Color::Green);
    Button.setOutlineColor(sf::Color::Black);
    Button.setPosition(900, 700);
    sf::Text ActionButton("", font, 25);
    ActionButton.setFillColor(sf::Color::Black);
    ActionButton.setPosition(900, 700);

    while (Render::LineCheck(Matrix) )
    {
        Matrix->FixField();
        Matrix->DeleteBonuses();
    }

    //Matrix->DeleteAnimations();

    Matrix->PlayerScore=0;

    //Render
    while (window->isOpen() && !GameOver)
    {
        MouseDelay++;
        MouseDelay = MouseDelay % 60;

        window->clear();
        window->draw(background);
        DrawGrid(window);
        window->draw(PlayerScore);
        window->draw(BackOfShell);
        window->draw(Button);
        window->draw(ActionButton);

        PlayerScore.setString(" Score:  " + std::to_string(Matrix->PlayerScore));
        ActionButton.setString(" Tap to end ");

        Render::FieldUpdate(Matrix);
        Render::PrintItems(Matrix, window);
        Render::PrintBonuses(Matrix, window);

        if (MouseDelay % (FPS/10) == 0 )
        {
            if (!Render::AnimationPlaying(Matrix) )
            {
                if (!Render::LineCheck(Matrix) )
                {
                    if (Selected->SecondItemX != -1)
                    {

                        Render::Swap(Matrix, Selected);
                        Selected->SecondItemX = Selected->FirstItemX = -1;
                        Selected->SecondItemY = Selected->FirstItemY = -1;

                    }
                    Actions(Matrix, Field, window, Selected, &GameOver, &BackOfShell);
                }
                else
                {
                    Selected->SecondItemX = Selected->FirstItemX = -1;
                    Selected->SecondItemY = Selected->FirstItemY = -1;
                }
            }
        }

        sf::Event event{};
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (disp_x >= 900 && disp_x <= 1150 && disp_y >= 700 && disp_y <= 775 ))
                window->close();
        }
        window->display();
    }

    delete Selected;
}

void Actions(Builder *Matrix, Render *Field, sf::RenderWindow *window, SelectedItems *Selected, bool *GameOver, sf::RectangleShape *rect)
{
    int disp_x, disp_y;
    int *mat_x, *mat_y;
    mat_x = new int;
    mat_y = new int;


	//std::cout << "X: " << sf::Mouse::getPosition(*window).x << "\tY: " << sf::Mouse::getPosition(*window).y << "\n";

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        disp_x = sf::Mouse::getPosition(*window).x;
        disp_y = sf::Mouse::getPosition(*window).y;


            if (disp_x >= OBJECT_SIZE && disp_x <= FIELD_SIZE * OBJECT_SIZE + OBJECT_SIZE
            && disp_y >= OBJECT_SIZE && disp_y <= FIELD_SIZE * OBJECT_SIZE + OBJECT_SIZE)
        {
            Render::ToMatCoords(disp_x, disp_y, mat_x, mat_y);
            if(Selected->FirstItemX!=-1)
            {
                if (*mat_x != Selected->FirstItemX || *mat_y != Selected->FirstItemY)
                {
                    if ((abs(*mat_x - Selected->FirstItemX) == 1 && Selected->FirstItemY == *mat_y) ||
                        (abs(*mat_y - Selected->FirstItemY) == 1 && Selected->FirstItemX == *mat_x))
                    {
                        Selected->SecondItemX = *mat_x;
                        Selected->SecondItemY = *mat_y;
                        Render::Swap(Matrix, Selected);
                        printf("Selected second\n");
                    }
                    else
                    {
                        Selected->SecondItemX = Selected->FirstItemX = -1;
                        Selected->SecondItemY = Selected->FirstItemY = -1;
                    }
                    rect->setPosition(1000, 2000);
                }
            }
            else
            {
                Selected->FirstItemX = *mat_x;
                Selected->FirstItemY = *mat_y;
                rect->setPosition( (float)(disp_x - disp_x%64), (float)(disp_y - disp_y % 64));
                printf("Selected first\n");
            }
        }

        if (disp_x >= 900 && disp_x <= 1150 && disp_y >= 700 && disp_y <= 775)
        {
            *GameOver = true;
        }
    }
}

void DeathScreen(Builder *Matrix, Render *Field, sf::RenderWindow *window)
{

    //Background
    sf::Texture backTexture;
    backTexture.loadFromFile("resourses/textures/scene.png");
    sf::Sprite background;
    background.setTexture(backTexture);

    //PlayerScore
    sf::Font font;
    font.loadFromFile("resourses/Bahnschrift.ttf");

    sf::Text PlayerScore("", font, 40);
    sf::Text ToRestart("", font, 40);

    PlayerScore.setFillColor(sf::Color::Black);
    PlayerScore.setPosition(550, 150);
    PlayerScore.setString(" Score:  " + std::to_string(Matrix->PlayerScore));

    ToRestart.setFillColor(sf::Color::Black);
    ToRestart.setPosition(550, 800);
    ToRestart.setString(" Tap any button to restart ");

    //Restart
    bool Restart = false;

    while (window->isOpen() && !Restart)
    {
        window->clear();
        window->draw(background);
        window->draw(PlayerScore);
        window->draw(ToRestart);

        sf::Event event{};
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::KeyPressed or event.type == sf::Event::MouseButtonPressed)
                Restart = true;
        }
        window->display();
    }
}

void MainMenu(Builder *Matrix, Render *Field, sf::RenderWindow *window)
{
    //Background
    sf::Texture backTexture;
    backTexture.loadFromFile("resourses/textures/scene.png");
    sf::Sprite background;
    background.setTexture(backTexture);

    sf::Font font;
    font.loadFromFile("resourses/Bahnschrift.ttf");
    sf::Text TextMenu("", font, 40);

    TextMenu.setFillColor(sf::Color::Black);
    TextMenu.setPosition(550, 150);
    TextMenu.setString("\t\t\t\t  Menu\n\n\n\n\n\n\n\n\n\n\n\nTap any button to start");

    while (window->isOpen())
    {
        window->clear();
        window->draw(background);
        window->draw(TextMenu);

        sf::Event event{};
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed or event.type == sf::Event::MouseButtonPressed)
            {
                // Press any key
                return;
            }
        }
        window->display();
    }
}



