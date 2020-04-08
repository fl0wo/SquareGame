#include<iostream>
using namespace std;
class Player{

private:

    float x=0.0F,y=0.0F;
    float dirX=0.0F,dirY=0.0F;

    float speed = 0.38F;
    bool playing = false;
    pair<float,float> spawn = {0.0f,0.0f};
    bool isInsideRoom = false;
    bool lightOn=true;
    int vision_ray = 10;

    const int MIN_VISION_RAY = 0;
    const int MAX_VISION_RAY = 20;

public:

    float getX(){return x;}
    float getY(){return y;}
    float getDirX(){return dirX;}
    float getDirY(){return dirY;}
    float getSpeed(){return speed;}
    bool isPlaying(){return playing;}
    bool start(){playing = true;}
    bool lose(){playing = false;}

    bool insideRoom(){return isInsideRoom;}
    void setOnRoom(bool insideRoom){isInsideRoom=insideRoom;}

    void setSpeed(float speed){
        this->speed = speed;
    }

    void move(float offSetX,float offSetY){
        moveX(offSetX);
        moveY(offSetY);
    }  
    
    void setPos(float x,float y){
        setX(x);
        setY(y);
    }  


    void moveX(float offSet){
        x += (offSet * speed);
    }

    void moveY(float offSet){
        y += (offSet * speed);
    }

    void setX(float x){
        this->x =x;
    }

    void setY(float y){
        this->y = y;
    }

    void cleanDir(){
        dirX = 0.0F;
        dirY = 0.0F;
    }

    void incDir(float offSetX,float offSetY){
        dirX += offSetX;
        dirY += offSetY;
    }

    void incRay(int incDelta){
        vision_ray+=incDelta;
        vision_ray=min(MAX_VISION_RAY,vision_ray);
        vision_ray=max(vision_ray,MIN_VISION_RAY);
    }
    
    int getVisionRay(){
        return vision_ray;
    }

    void setSpawn(float x,float y){
        spawn = {x,y};
    }

    void setSpawn(std::pair<float,float> spawns){
        spawn = spawns;
    }

    void respawn(){
        x=spawn.first;
        y=spawn.second;
    }

    void setDirY(float setY){dirY = setY;}
    void setDirX(float setX){dirX = setX;}

    void switchLight(){lightOn=!lightOn;}
    bool isLightOn(){return lightOn;}

};