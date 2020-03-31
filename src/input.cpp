#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstring>

using namespace sf;
using namespace std;

class Input{

private:

    bool curPressed[Keyboard::KeyCount];

public:

    Input() { memset(curPressed,false, sizeof curPressed); } 

    void update(Event event){
        if((event.type != Event::KeyPressed && event.type != Event::KeyReleased)
         || event.key.code==Keyboard::Unknown) return;

        curPressed[event.key.code] = (event.type == Event::KeyPressed); // se è premuto
        curPressed[event.key.code] &= (event.type != Event::KeyReleased); // se è rilasciato
    }

    bool isKey(Keyboard::Key k){
        return curPressed[k];
    }

    bool isLeft(){ return isKey(Keyboard::Left) || isKey(Keyboard::A); }

    bool isRight(){return isKey(Keyboard::Right) || isKey(Keyboard::D);}

    bool isUp(){return isKey(Keyboard::Up) || isKey(Keyboard::W);}

    bool isDown(){return isKey(Keyboard::Down) || isKey(Keyboard::S);}

    bool isSpace(){return isKey(Keyboard::Space);}

    bool isEscape(){return isKey(Keyboard::Escape);}

    bool isClose(Event e){return (e.type == Event::Closed);}
};