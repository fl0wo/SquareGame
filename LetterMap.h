//
// Created by flo on 10/28/2021.
//

#ifndef NOMPROJET_LETTERMAP_H
#define NOMPROJET_LETTERMAP_H


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

#include "Circularpairmap.h"

// For avoiding multiple declarations :

#ifndef LETTER_MAP
#define LETTER_MAP

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

struct Point {
    int x;
    int y;
};

struct queueNode {
    Point pt;
    int dist;
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


    ///////////////////////////////////////////////////////////////////////////////////////////

    bool isValid(int row, int col) {
        return (row >= 0) && (row < getNRows()) && (col >= 0) && (col < getNCols());
    }


    int path_find(int c1,int r1,int r2,int c2,vector<pair<int,int>> &ans) {

        Point src = {r1,c1};
        Point dest = {r2,c2};

        int rowNum[] = {-1, 0, 0, 1};
        int colNum[] = {0, -1, 1, 0};

        if (isWall(src.x,src.y) || isWall(dest.x,dest.y))
            return -1;

        bool visited[getNRows()][getNCols()];

        memset(visited, false, sizeof visited);

        visited[src.x][src.y] = true;

        queue<queueNode> q;

        queueNode s = {src, 0};
        q.push(s);

        queue<vector<pair<int,int>>> parents;

        // path vector to store the current path
        vector<pair<int,int>> path_tracer;
        path_tracer.push_back({src.x,src.y});

        parents.push(path_tracer);

        while (!q.empty()) {
            queueNode curr = q.front();
            Point pt = curr.pt;

            if (pt.x == dest.x && pt.y == dest.y){
                ans = parents.front();
                return ans.size();
            }

            q.pop();
            path_tracer = parents.front();
            parents.pop();

            for (int i = 0; i < 4; i++) {
                int row = pt.x + rowNum[i];
                int col = pt.y + colNum[i];

                if (isValid(row, col) && !isWall(row,col) && !visited[row][col]) {
                    visited[row][col] = true;
                    queueNode Adjcell = { {row, col}, curr.dist + 1 };
                    q.push(Adjcell);

                    vector< pair<int,int> > newpath(path_tracer);
                    newpath.push_back({row,col});
                    parents.push(newpath);
                }
            }
        }

        return -1;
    }
};

#endif


#endif //NOMPROJET_LETTERMAP_H
