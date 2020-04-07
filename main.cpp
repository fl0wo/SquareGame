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
Utils tx;
Player pl;
Input in;
Event event;
LetterMap m(tx.W,tx.H,tx.S);
Camera2D c(tx.W,tx.H);

void update(RenderWindow &window);

int main(){
    srand(static_cast<unsigned int>(time(NULL)));

    RenderWindow window(VideoMode(tx.W,tx.H,32),tx.title,Style::None | Style::Close);

   // window.setVerticalSyncEnabled(true);
   // window.setFramerateLimit(120);

    pair<int,int> matrixPos = m.getValidSpawn();

    pl.setSpawn(matrixPos.second, matrixPos.first);
    pl.respawn();

    m.playerPos(matrixPos.first,matrixPos.second);
    pl.setOnRoom(m.isPlayerOnRoom());
/*
    FloatRect rect_container = FloatRect(0,0, tx.W,tx.H);
    View container(rect_container);
*/
    cout << "pl spawn : ("<<pl.getX() << "," << pl.getY() << ")\n";

    c.center(pl.getX(),pl.getY(),tx.S);

    float oldZoom = 1.0F;    

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
            

            if(pl.isPlaying()){
                // R
                if (in.isKeyJustReleased(Keyboard::R))
                    pl.switchLight();

                //  W S
                if(!in.isUp() && !in.isDown()) pl.setDirY(0.0F);
                else if(in.isUp()) pl.setDirY(-1.0F);
                else if(in.isDown()) pl.setDirY(+1.0F);

                // A D
                if(!in.isRight() && !in.isLeft()) pl.setDirX(0.0F);
                else if(in.isRight()) pl.setDirX(+1.0F);
                else if(in.isLeft()) pl.setDirX(-1.0F);

                pl.incRay(in.mouseWheelScrool());
            }
        }

        if(pl.isPlaying() && clock.getElapsedTime().asMilliseconds()>step_delay.asMilliseconds()){

            int futureX = pl.getX() + pl.getDirX();
            int futureY = pl.getY() + pl.getDirY();
       //    float deltaTime = fps.restart().asSeconds();

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

            //pl.setOnRoom(m.isPlayerOnRoom());

            c.target(pl.getX(),pl.getY(),tx.S); // zoom is included in camera class
            c.slerpFollow();
            //c.zoomActived(pl.insideRoom(),deltaTime); // if is inside a room, zoom
            
            //cout << "current zoom : " << c.getZoom() << "\n";
        }
        /*
        if(pl.insideRoom() && oldZoom!=1.0F){
            container.reset(rect_container);
            container.zoom(.9F); 
            window.setView(container);
        }else if(oldZoom!=1.0F){
            container.reset(rect_container);
            container.zoom(1.0F); 
            window.setView(container);
        }*/

        window.clear(Color::Black);
        update(window);
        window.display();
    }

    return EXIT_SUCCESS;
}

void drawHome(RenderWindow &window);
void drawDebug(RenderWindow &window);
void drawDisplay(RenderWindow &window);
void drawCrossHair(RenderWindow &window);

void update(RenderWindow &window){
    if (pl.isPlaying()){
       drawDisplay(window);
    }
    else{
        drawHome(window);
    }
}

void drawHome(RenderWindow &window){
    window.draw(tx.create(tx.S,tx.W/2.0F,tx.H/2.0F,Color::White,"press enter to start"));
}

void drawDebug(RenderWindow &window){


    drawCrossHair(window);

    
    vector<Text> letters = tx.genLetters(m);
    for(Text letter : letters){
        letter.move(-c.getX(),-c.getY()); // moves in opposite direction
        window.draw(letter);
    }
    
}


void drawDisplay(RenderWindow &window){
    vector<RectangleShape> rects = tx.genRects(m,1,pl.isLightOn(),pl.getVisionRay());
    for(RectangleShape rect : rects){
        rect.move(-c.getX(),-c.getY()); // moves in opposite direction
        window.draw(rect);
    }
    drawCrossHair(window);
}

void drawCrossHair(RenderWindow &window){
    window.draw(tx.create(tx.S,tx.W/2 /*- tx.S/2*/,tx.H/2 /*- tx.S/2*/,Color::Green,"+"));
}