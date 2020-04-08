#include <list>
#include <algorithm>
#include <iostream>

#define MAXROW 100
#define MAXCOL 100
#define NEIGHBOURS 8

using namespace std;
 
class point {
public:
    point( int a = 0, int b = 0 ) { x = a; y = b; }
    bool operator ==( const point& o ) { return o.x == x && o.y == y; }
    point operator +( const point& o ) { return point( o.x + x, o.y + y ); }
    int x, y;
};
 
class mappa {
public:
    mappa(vector<vector<int>> t) {
        int rows =  t.size(); // 52 rows  
        int cols = rows>0?t[0].size():0; // 52 cols

        w =rows;
        h =cols;
        cout << "righe : " << w <<"\n";
        cout << "cols : " << h <<"\n";

        for(int r=0;r<h;r++)
            for(int s=0;s<w;s++)
                m[s][r]=t[r][s];
    }

    mappa(){

    }

    int operator() ( int x, int y ) { return m[x][y]; }
    int m[MAXROW][MAXCOL];
    int w, h;
};
 
class node {
public:
    bool operator == (const node& o ) { return pos == o.pos; }
    bool operator == (const point& o ) { return pos == o; }
    bool operator < (const node& o ) { return dist + cost < o.dist + o.cost; }
    point pos, parent;
    int dist, cost;
};
 
class AStar {
public:
    mappa m; point end, start;
    point neighbours[NEIGHBOURS];
    list<node> open;
    list<node> closed;

    AStar() {
        neighbours[0] = point( -1, -1 ); neighbours[1] = point(  1, -1 );
        neighbours[2] = point( -1,  1 ); neighbours[3] = point(  1,  1 );
        neighbours[4] = point(  0, -1 ); neighbours[5] = point( -1,  0 );
        neighbours[6] = point(  0,  1 ); neighbours[7] = point(  1,  0 );
    }
 
    int calcDist( point& p ){
        // need a better heuristic
        int x = end.x - p.x, y = end.y - p.y;
        return( x * x + y * y );
    }
 
    bool isValid( point& p ) {
        return ( p.x >-1 && p.y > -1 && p.x < m.w && p.y < m.h );
    }
 
    bool existPoint( point& p, int cost ) {
        list<node>::iterator i;
        i = find( closed.begin(), closed.end(), p );
        if( i != closed.end() ) {
            if( ( *i ).cost + ( *i ).dist < cost ) return true;
            else { closed.erase( i ); return false; }
        }
        i = find( open.begin(), open.end(), p );
        if( i != open.end() ) {
            if( ( *i ).cost + ( *i ).dist < cost ) return true;
            else { open.erase( i ); return false; }
        }
        return false;
    }
 
    bool fillOpen( node& n ) {
        int stepCost, nc, dist;
        point neighbour;
 
        for( int x = 0; x < NEIGHBOURS; x++ ) {
            // one can make diagonals have different cost
            stepCost = x < 4 ? 1 : 1;
            neighbour = n.pos + neighbours[x];
            if( neighbour == end ) return true;
 
            if( isValid( neighbour ) && m( neighbour.x, neighbour.y ) != 1 ) {
                nc = stepCost + n.cost;
                dist = calcDist( neighbour );
                if( !existPoint( neighbour, nc + dist ) ) {
                    node m;
                    m.cost = nc; m.dist = dist;
                    m.pos = neighbour; 
                    m.parent = n.pos;
                    open.push_back( m );
                }
            }
        }
        return false;
    }
 
    bool search( point& s, point& e, mappa& mp ) {
        node n;end=e;start=s;m=mp;
        n.cost=0;n.pos=s;n.parent=0;n.dist=calcDist(s); 
        open.push_back(n);
        while( !open.empty()) {
            //open.sort();
            node n = open.front();
            open.pop_front();
            closed.push_back(n);
            if(fillOpen(n))return true;
        }
        return false;
    }
 
    int path(list<point>& path ) {
        path.push_front( end );
        int cost = 1 + closed.back().cost; 
        path.push_front( closed.back().pos );
        point parent = closed.back().parent;
 
        for(list<node>::reverse_iterator i = closed.rbegin(); i != closed.rend(); i++ ) {
            if( ( *i ).pos == parent && !( ( *i ).pos == start ) ) {
                path.push_front( ( *i ).pos );
                parent = ( *i ).parent;
            }
        }
        path.push_front( start );
        return cost;
    }
};