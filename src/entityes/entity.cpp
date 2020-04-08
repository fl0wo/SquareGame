#include <iostream>
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
    list<point> path; // percorso che deve seguire corrente
    AStar as;
    mappa m;

public:

    int row,col;
    int targetRow,targetCol;

    Entity(LetterMap &map,int visibility_ray,int delay_move){
        this->map=map;
        this->visibility_ray=visibility_ray;
        this->delay_move=delay_move;
        changeRandomTargetPos();

        vector<vector<int>> grid = map.getPlane();
        
        m = mappa(grid);
    }

    virtual void draw(RenderWindow &window,int pixePerUnit,Camera2D &cam)=0;

    void setRow(int r) {this->row = r;}
    void setCol(int c) {this->col = c;}

    int getRow(){return row;}
    int getCol(){return col;}
    int getVisibilityRay(){return visibility_ray;}
    int getMillisecDelayMove(){return delay_move;}

    bool nextStep(){
        if(path.size()<=0)return false;
        localPosInPath++;
        std::cout << "passo numero : " << localPosInPath << " \n";
        point next = path.front();
        path.pop_front();
        setRow(next.x);
        setCol(next.y);
        return true;
    }

    void targetPos(int targetRow,int targetCol){
        if(targetRow==this->row && targetCol==this->col)return;
        path.clear();
        setTargetRow(targetRow);
        setTargetCol(targetCol);
        vector<pair<int,int>> ans;

        //aStarSearch(grid, src, dest,ans);

        point s(this->row,this->col), e(targetRow,targetCol);

        if(as.search(s,e,m)){
            int c = as.path(path);
            cout << "cost path : "  << c << "\n";}
        else
            cout << "smth wrong \n";
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
        for(point p:path)
            rispath.push_back({p.x,p.y});
        return rispath;
    }
    
};
