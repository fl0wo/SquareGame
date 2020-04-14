#include <iostream>
#include <cmath>
#include <math.h>

#include "./../lettermap.cpp";
#include "./path_find/astar.cpp";

using namespace std;

typedef pair<int,int> pi; 

// Base class Interface
class Entity {

private:
    LetterMap map;
    int delay_move; // millisec w8ing delay b4 moving again
    int visibility_ray;

    int localPosInPath=0;
    vector<pi> path; // percorso che deve seguire corrente

public:

    int row_pos,col_pos;
    float real_row,real_col; // slerp smooth row and col (pixels)
    int targetRow,targetCol;
    float smoothness = 0.25;

    Entity(LetterMap &map,int visibility_ray,int delay_move){
        this->map=map;
        this->visibility_ray=visibility_ray;
        this->delay_move=delay_move;
        //changeRandomTargetPos();
    }

    virtual void draw(RenderWindow &window,int pixePerUnit,Camera2D &cam)=0;

    void setRow(int r) {this->row_pos = r;}
    void setCol(int c) {this->col_pos = c;}

    int getRow(){return row_pos;}
    int getCol(){return col_pos;}
    int getVisibilityRay(){return visibility_ray;}
    int getMillisecDelayMove(){return delay_move;}

    bool nextStep(){
        if(path.size()<=1)return false;
        localPosInPath++;
        //std::cout << "passo numero : " << localPosInPath << " \n";
        pi next = path[1];
        path.erase(path.begin());
        setRow(next.first);
        setCol(next.second);
        return true;
    }

    double calcDist(int row, int col, int x, int y) {
        return ((double) sqrt((row - x) * (row - x) + (col - y) * (col - y)));
    }

    bool searchTargetAround(int x,int y){
        float distPlayer = calcDist(x,y,getCol(),getRow());
        cout << "distance : " << distPlayer << "\n";
        if(distPlayer<=visibility_ray){
            cout << "oddio sei entrato assurdo";
            targetPos(x,y);
            return true;
        }
        return false;
    }

    void targetPos(int targetRow,int targetCol){
        //if(targetRow==this->getCol() && targetCol==this->getRow())return; // seems bugged, but it isnt :/
        nextStep();
        path.clear();
        setTargetRow(targetRow);
        setTargetCol(targetCol);
        //cout << "troviamo shortest path!!\n";
        if(map.path_find(this->getCol(),this->getRow(),targetCol,targetRow,this->path)){
            int c = path.size();
          //  cout << " cost path : " << c << "\n";
        }else{
           // cout << "problema!\n";
        }
    }

    void changeRandomTargetPos(){
        localPosInPath=0;
        pi spawn = this->map.getValidSpawn();
        targetPos(spawn.first,spawn.second);
    }
    
    void setTargetRow(int targetRow){
        this->targetRow=targetRow;
    }
    void setTargetCol(int targetCol){
        this->targetCol=targetCol;
    }

    vector<pair<int,int>> getFollowingPath(){
        vector<pair<int,int>> rispath;
        for(pair<int,int> p:path)
            rispath.push_back({p.first,p.second});
        return rispath;
    }


    float getRealRow(){return real_row;}
    float getRealCol(){return real_col;}

    void setRealRow(float newX){real_row=newX;}
    void setRealCol(float newY){real_col=newY;}

    void move(float offsetX,float offsetY){
        setRealRow(real_row+offsetX);
        setRealCol(real_col+offsetY);
    }

    void slerpFollowPosition(){
        move((row_pos-real_row)*smoothness,(col_pos-real_col)*smoothness);
    }
    
};


        //aStarSearch(grid, src, dest,ans);
/*
        point s(this->row,this->col), e(targetRow,targetCol);

        if(as.search(s,e,m)){
            int c = as.path(path);
            cout << "cost path : "  << c << "\n";}
        else
            cout << "smth wrong \n";
            */