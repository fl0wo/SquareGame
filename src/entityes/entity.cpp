
typedef pair<int,int> pi; 
#include "./../lettermap.cpp";

// Base class Interface
class Entity {

    private:
        LetterMap map;
        int delay_move; // millisec w8ing delay b4 moving again
        int visibility_ray;

    public:

        Entity(LetterMap &map,int visibility_ray,int delay_move){
            this->map=map;
            this->visibility_ray=visibility_ray;
            this->delay_move=delay_move;
            pi spawn = this->map.getValidSpawn();
            setRow(spawn.first);
            setCol(spawn.second);
        }

        virtual void draw();

        void setRow(int r) {row = r;}
        void setCol(int c) {column = c;}

        int getRow(){return row;}
        int getCol(){return column;}
        int getVisibilityRay(){return visibility_ray;}
        int getMillisecDelayMove(){return delay_move;}

        void applyMove(){

        }

        void follow(){}


    protected:
        int row;
        int column;
};