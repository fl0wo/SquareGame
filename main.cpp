#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

#include "./src/input.cpp"
#include "./src/utils.cpp"
#include "./src/player.cpp"
#include "./src/camera2d.cpp"

using namespace std;
using namespace sf;

// costanti

int main(){
    srand(static_cast<unsigned int>(time(NULL)));

    Utils tx;

    RenderWindow window(VideoMode(tx.W,tx.H,32),tx.title,Style::None | Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(120);

    Player pl;
    Input in;
    Event event;
    LetterMap m(tx.W,tx.H,tx.S);
    Camera2D c(tx.W,tx.H);

    pair<int,int> matrixPos = m.getValidSpawn();

    pl.setSpawn(matrixPos.second, matrixPos.first);
    pl.respawn();

    m.playerPos(matrixPos.first,matrixPos.second);

    cout << "pl spawn : ("<<pl.getX() << "," << pl.getY() << ")\n";

    c.center(pl.getX(),pl.getY(),tx.S);
    
    //canGo

    const Time delay   = milliseconds(1000.0F/tx.FPS);
    const Time step_delay   = milliseconds(40.0F);
    Clock clock;
    clock.restart();

    if(!tx.setDefaultFont()) return EXIT_FAILURE;

    while (window.isOpen()){

        // add some sleep
        this_thread::sleep_for(chrono::milliseconds(delay.asMilliseconds()));


        while(window.pollEvent(event)){
            
            in.update(event);

            if (in.isEscape() || in.isClose(event)){
                window.close();
                break;
            }

            if (in.isSpace())
                if (!pl.isPlaying())
                    pl.start(),clock.restart();

            //  W S
            if(!in.isUp() && !in.isDown()) pl.setDirY(0.0F);
            else if(in.isUp()) pl.setDirY(-1.0F);
            else if(in.isDown()) pl.setDirY(+1.0F);

            // A D
            if(!in.isRight() && !in.isLeft()) pl.setDirX(0.0F);
            else if(in.isRight()) pl.setDirX(+1.0F);
            else if(in.isLeft()) pl.setDirX(-1.0F);
        }

        if(pl.isPlaying() && clock.getElapsedTime().asMilliseconds()>step_delay.asMilliseconds()){
            //float deltaTime = clock.restart().asMilliseconds();

            int futureX = pl.getX() + pl.getDirX();
            int futureY = pl.getY() + pl.getDirY();

            if(m.canGo(futureY,pl.getX())) {
                clock.restart();
                pl.setPos(pl.getX(),futureY);
                m.playerPos(futureY,pl.getX());
            }

            if(m.canGo(pl.getY(),futureX)) {
                clock.restart();
                pl.setPos(futureX,pl.getY());
                m.playerPos(pl.getY(),futureX);
            }

            c.target(pl.getX(),pl.getY(),tx.S);
            c.slerpFollow();
        }

        window.clear(Color::Black);

        if (pl.isPlaying()){

            vector<Text> letters = tx.genLetters(m);
            
            for(Text letter : letters){
                letter.move(-c.getX(),-c.getY()); // moves in opposite direction
                window.draw(letter);
            }
            
            window.draw(tx.create(tx.S,tx.W/2 /*- tx.S/2*/,tx.H/2 /*- tx.S/2*/,Color::Green,"+"));

        }
        else window.draw(tx.create(tx.S,tx.W/2.0F,tx.H/2.0F,Color::White,"press enter to start"));

        window.display();
    }

    return EXIT_SUCCESS;
}