#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <iostream>

// For avoiding multiple declarations : 

#ifndef LETTER_MAP
#define LETTER_MAP

#include "./circularpairmap.cpp"

using namespace sf;
using namespace std;

#define MAXN 50 // sqrt(max number of blocks)
#define MAXL 13 // max lato room
#define MINL 5 // min lato room
#define BORDER 2
#define ROOM_BORDER 1 // inside createRoom method
#define MAXRAY 10

#define MAXROOMS 15

#define p(a,b) make_pair(a,b)

#define ABS(a) (a<0)?-a:a

typedef pair<int,int> pi;

enum Cell{
    Wall = 0,
    Floor = 1,
    Water = 2,
    Box = 3,
    Portal = 4,
    Door = 5,
    Spawn = 6,
    User=7,
    RoomRoof=8,
    Unknown = -1
};

// should also add here a list of object that user can pass trough
// like creative game pass trough walls, or water walker jesus mode

/**
 * Wall border around inclusive.
 */
struct Room{
    int c;
    int r;
    int columns;
    int rows;
    bool exists = true;

    pi center(){ // replace pi with custom struct plz
        int midC = (c + (c+columns))/2;
        int midR = (r + (r+rows))/2;
        return p(midR,midC);
    }

    bool intersect(Room r1){
        if (c > r1.c+r1.columns || r1.c > c+columns) return false; 
        if (r > r1.r+r1.rows || r1.r > r + rows) return false; 
        return true;
    }
    // return is_inside(r1.r,r1.c) ||is_inside(r1.r+r1.rows,r1.c) ||is_inside(r1.r,r1.c+r1.columns) 
    // || is_inside(r1.r+r1.rows,r1.c+r1.columns);

    bool is_inside(int i,int j){
        return (i>=r && i<=r+rows) && (j>=c && j<=c + columns);
    }
};

class LetterMap{

private:  

    //Cell cellOccupied;

    string letters = "qwertyuiopasdfghjklzxcvbnm";
    string accents = "òàèùì";
    string numbers = "1234567890";
    string metchars = "?!#@[]·-_/*<>";

    string yo = "OY";

    // player pos
    int playerI=-1,playerJ=-1;

    circularpairmap<bool> explored;

public:

    char floor = '-';


    int _rows = MAXN;
    int _columns = MAXN;
    //set<pi> explored = set<pi>();

    Cell plane[MAXN+BORDER][MAXN+BORDER];
    vector<Room> rooms;

    int W,H,S;

    /**
     * W = width
     * H = height
     * S = letter size
     * N = n blocks
    */
    LetterMap(int W,int H,int S) {
        this->W=W;
        this->S=S;
        this->H=H;
        __bfs(MAXROOMS-1); // let -1
    } 

    LetterMap() {
        //does nothing
    } 

    int operator() ( int x, int y ) { return plane[x][y]; }

    int getNRows(){return this->_rows;}
    int getNCols(){return this->_columns;}

    // To optimize with cache
    vector<vector<int>> getPlane(){
        vector<vector<int>> plane_vectorized(MAXN+BORDER,vector<int>(MAXN+BORDER));
        for(int i=0;i<MAXN+BORDER;i++)
            for(int j=0;j<MAXN+BORDER;j++)
                plane_vectorized[i][j] = plane[i][j]==Floor?0:1;

        return plane_vectorized;
    }

    /**
     * Greedy algorithm for rooms creations. Time O(2n) 
     */
    void __bfs(int nRooms){

        clearMap();

        // creating a room
        Room from = createRoom();
        rooms.push_back(from);

        applyRoom(from,Floor);// Here before the material was Floor

        while(nRooms--){
            
            Room to = createRoom();
            if(!to.exists) break;    // means there's no more space

            rooms.push_back(to);
            applyRoom(to,Floor); // HereFloor before the material was 
            applyPath(connectRooms(from,to),Floor);

            from = to;
        }
  
    }

    /**
     * Creates room if possible with MAXL MINL params. Time : O(1)
     */
    Room createRoom(){
        int randx,randy,latox,latoy;
        
        int MAX_TRY = 50;

        do{
            randx = randomRange(0,MAXN-MINL);
            randy = randomRange(0,MAXN-MINL);

            latox = randomRange(MINL,min(MAXN-randx, MAXL));
            latoy = randomRange(MINL,min(MAXN-randy, MAXL));

        }while(!canBuildRoom(randx,randy,latox,latoy) && (MAX_TRY--)>0);

        bool isOk = MAX_TRY>0;

        Room r = {randx,randy,latox,latoy,isOk}; // if MAX_TRY>0 than exists

        return r;
    }

    void getExplored(vector<pi> &cells){
        for(auto x : explored.m)
            cells.push_back(x.first);
        // for(auto x : explored)
        //     cells.push_back(x.first);
    }

    void addExplored(vector<pi> justViewed){    

        for(pi x : justViewed){
            pi tmp =  make_pair(x.first, x.second); 
            if(explored.m.find(x)==explored.m.end())  
                explored.insert(tmp,true);
            // if(explored.find(x)==explored.end())  
            //     explored[tmp]=true;
                //if(find(explored.begin(),explored.end(),tmp) == explored.end()) { // can remove it if u want!

        }

        //cout << "e oraaa : " << explored.size() << "\n";
    }

    /**
     *  Connects two separate rooms. Time : O(n)
     * */
    vector<pi> connectRooms(Room a,Room b){
        pi from = a.center();
        pi to = b.center();

        int incI = from.second<to.second ? 1 : -1;
        int incJ = from.first<to.first ? 1 : -1;

        // Random 50% L direction
        if(rand()%2){
        }

        vector<pi> points;

        while(from.first != to.first) points.push_back(p(from.first,from.second)),from.first+=incJ;
        while(from.second != to.second) points.push_back(p(from.first,from.second)),from.second+=incI;

        return points;
    }

    /**
     * Returns valid random spawn coords.
     **/
    pi getValidSpawn(){
        pi spawn;

        do{
            spawn.first = randomRange(0,MAXN);
            spawn.second = randomRange(0,MAXN);
        }while(plane[spawn.first][spawn.second] != Floor);

        //plane[spawn.first][spawn.second] = Spawn;

        return spawn;
    }

    /**
     *  V.01 -> Time : O(n^2) bad complexity!  
     *  V.02 -> Time : 0(n) ok!
     */
    bool canBuildRoom(int randx,int randy,int latox,int latoy){
        
        if(latox<0 || latoy<0) return false;
        if(randx + latox > MAXN || randx < 0) return false;
        if(randy + latoy > MAXN || randy < 0) return false;
        
        Room r = {randx,randy,latox,latoy,false};

        for(Room r1 : rooms)
            if(r1.intersect(r))
                return false;

        return true;
    }
    /**
     *  Time : O((n-1)^2) that's ok!  
     *  Maybe add border to Room of 1 block.
     */
    void applyRoom(Room r,Cell material){
        int b = ROOM_BORDER;

        for(int i=r.r+b;i<r.r + r.rows-b;i++)
            for(int j=r.c+b;j<r.c + r.columns-b;j++)
                plane[i][j]=material;
    }


    /**
     * Time : O(n) that's ok!
     */
    void applyPath(vector<pi> tunnel,Cell material){
        for(pi x : tunnel) plane[x.first][x.second] = material;
    }

    void clearMap(){
        memset(plane,Wall,sizeof plane);
    }

    char factoryLetter(Cell cell){
        switch(cell){
            case Wall : return ((rand()%2)==0) ? 'Y' : 'O';
            case Floor : return floor;
            case User : return '@';
        };
        return 'Y';
    }


    char get(int i,int j){
        if(!isLegit(i,j)) return 'E';
        else return factoryLetter(plane[i][j]);
    }

    Cell getType(int i,int j){
        if(!isLegit(i,j)) return Unknown;
        return (Cell) plane[i][j];
    }

    bool isWall(int i,int j){
        if(!isLegit(i,j)) return false;
        return plane[i][j] == Wall;
    }


    // Attention : now u can go only on Floor and RoomRoof
    bool canGo(int i,int j){
        if(i<0 || j<0 || i>getNRows() || j>getNCols()) return false;
        return plane[i][j] == Floor || plane[i][j] == RoomRoof || (playerI == i && playerJ == j);
    }

    // Attention : now u can go only on Floor and RoomRoof
    bool isLegit(int i,int j){
        return !(i<0 || j<0 || i>getNRows() || j>getNCols());
    }

    void __pr(){
        for(int i=0;i<MAXN;i++){
            for(int j=0;j<MAXN;j++)
                cout << plane[i][j] << " ";
            cout << "\n";
        }
    }

    int getMAXN(){
        return MAXN;
    }

    int randomRange(int min,int max){
        return min + (rand() % (max-min));
    }

    void playerPos(int i,int j){
        if(playerI!=-1 && playerJ!=-1){
            plane[playerI][playerJ]=Floor;
        }
        playerI = i;
        playerJ = j;
        //cellOccupied = plane[playerI][playerJ];
        plane[playerI][playerJ]=User;
    }
    
    int getPlayerRay(){
        return MAXRAY;
    }

    int getPlayerRow(){
        return playerI;
    }

    int getPlayerColumn(){
        return playerJ;
    }

    bool isPlayerOnRoom(){
        return plane[playerI][playerJ]=User;
    }

    bool path_between_ric(int r1,int c1,int r2,int c2,vector<pair<int,int>> &ans,bool v[MAXN][MAXN]){
        if(!isLegit(r1,c1))return false;
        if(r1==r2 && c1==c2)return true;
        if(v[r1][c1]) return false;

        v[r1][c1]=true;
        int dirs[4][2] = {{0,1},{1,0},{-1,0},{0,-1}};

        for(auto d:dirs){
            if(isLegit(r1+d[0],c1+d[1]) && path_between_r(r1+d[0],c1+d[1],r2,c2,ans)){
                ans.emplace_back(make_pair(r1+d[0],c1+d[1]));
                return true;
            }
        }

        return false;
    }

    bool path_between_r(int r1,int c1,int r2,int c2,vector<pair<int,int>> &ans){
        bool visited[MAXN][MAXN];
        memset(visited,false,sizeof(visited));
        return path_between_ric(r1,c1,r2,c2,ans,visited);
    }


    /**
     * r1 = riga enemy
     * c1 = colonna enemy
     * r2 = riga target
     * c2 = colonna target
     * ans = shortest path in grid
     * Time : O(n*m) (where n is abs(r1-r2) and m is abs(c1-c2))
     * bfs (cause on short distance has smaller ms time than A*)
     * (even tho the complexity is higher)
     */
    bool path_between(int r1,int c1,int r2,int c2,vector<pi> &ans){

        cout << "CERCHIAMO wdwdwd PATH : (" << r1 << "," << c1 << ") - (" << r2 << "," << c2<<")\n";

        if(r1==r2 && c1==c2)return true;

        int h = getNRows();
        if (h == 0) return false;
        int l = getNCols();

        bool v[h][l]; // visited false all
        //pi paths[abs(r1-r2)+1][abs(c1-c2)+1]; // shortest paths
        pi paths[h][l];

        priority_queue<pi> q;   // queue

        q.push({r1,c1}); // where im situated at the beginning
        paths[r1][c1] = {r1,c1}; // to get where i am, i dont have to move

        bool found = false;

        while(!q.empty() && !found){   // till is not empty
            pi x = q.top();q.pop(); // get the first inserted
            int r = x.first;
            int c = x.second;
          //  cout << " sono  a (" << r <<"," << c <<")\n";

            if (!isLegit(r,c) || v[r][c] || isWall(r,c))continue;
            v[r][c] = true;
            pi curPos = {r,c};

            if(r==r2 && c==c2){found=true;break;}

            if(isLegit(r,c-1)){q.push({r,c-1});if(!v[r][c-1]) paths[r][c-1]=curPos;} //go left
            if(isLegit(r,c+1)){q.push({r,c+1});if(!v[r][c+1]) paths[r][c+1]=curPos;} //go right
            if(isLegit(r-1,c)){q.push({r-1,c});if(!v[r-1][c]) paths[r-1][c]=curPos;} //go up
            if(isLegit(r+1,c)){q.push({r+1,c});if(!v[r+1][c]) paths[r+1][c]=curPos;} //go down
        }

        if(!found)return false;

        pi step = paths[r2][c2];

        cout << "di fatto trovata\n";
        int tmp=20;
        while(tmp--){
            cout << "siamo a : " << step.first << "," << step.second << "\n";
            ans.emplace_back(step);
            step = paths[step.first][step.second];
        }
/*
        while(step.first!=r1 && step.second!=c1){
            cout << "siamo a : " << step.first << "," << step.second << "\n";
            ans.emplace_back(step);
            step = paths[step.first][step.second];
        }
*/
        return true;
    }

};
// end declaration
#endif