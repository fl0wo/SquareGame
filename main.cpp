#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <ctime>
#include <cstdlib>
#include <thread>

#include "Utils.h"
#include "Player.h"
#include "Input.h"
#include "Camera2D.h"
#include "Bacterium.h"

using namespace std;
using namespace sf;


Utils tx;
Player pl;
Input in;
Event event;
LetterMap m(tx.W,tx.H,tx.S);
Camera2D c(tx.W,tx.H);
Entity *enemy = new Bacterium(m,20,1);

void update(RenderWindow &window);

int main(){
    srand(static_cast<unsigned int>(time(NULL)));

    RenderWindow window(VideoMode(tx.W,tx.H,32),tx.title,Style::None | Style::Close);

   // window.setVerticalSyncEnabled(true);
   // window.setFramerateLimit(120);

    pair<int,int> matrixPos = m.getValidSpawn();
    pair<int,int> enemyPos = m.getValidSpawn();

    enemy->setRow(enemyPos.first);
    enemy->setCol(enemyPos.second);

    pl.setSpawn(matrixPos.second, matrixPos.first);
    pl.respawn();

    m.playerPos(matrixPos.first,matrixPos.second);
    pl.setOnRoom(m.isPlayerOnRoom());

    c.center(pl.getX(),pl.getY(),tx.S);

    float oldZoom = 1.0F;    

    const Time delay   = milliseconds(1000.0F/tx.FPS);
    const Time step_delay   = milliseconds(40.0F);
    Clock clock,enemyClock;
    clock.restart();
    enemyClock.restart();
    // if(!tx.setDefaultFont()) return EXIT_FAILURE;
    enemy->targetPos(pl.getX(),pl.getY());

    while (window.isOpen()){

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
       //   float deltaTime = fps.restart().asSeconds();

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
            if(enemyClock.getElapsedTime().asMilliseconds() > milliseconds(70.0F).asMilliseconds()){
                enemyClock.restart();
               // enemy->searchTargetAround(pl.getY(),pl.getX());
                enemy->searchTargetAround(pl.getX(),pl.getY());
                //enemy->targetPos(pl.getX(),pl.getY());

                // if(!enemy->nextStep()){
                //     //nothing
                // }
           //     cout << " enemy pos ("<<enemy->getCol() << ";" << enemy->getRow() << ");\n";
             //   cout << " player pos ("<<pl.getY() << ";" << pl.getX() << ");\n";
            }
            enemy->slerpFollowPosition();

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
    if (pl.isPlaying()) drawDisplay(window);
    else drawHome(window);
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
    //window.draw(CircleShape(200));
    enemy->draw(window,tx.S,c);

    drawCrossHair(window);
}

void drawCrossHair(RenderWindow &window){
    window.draw(tx.create(tx.S,tx.W/2 /*- tx.S/2*/,tx.H/2 /*- tx.S/2*/,Color::Green,"+"));
}
