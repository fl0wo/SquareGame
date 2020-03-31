#include <string>
#include <set>
#include <set>
#include <vector>
#include <map>

#include "./lettermap.cpp"

using namespace sf;
using namespace std;

#define p(a, b) make_pair(a, b)

// represents the slope Y/X as a rational number

struct slope{
    uint X, Y;

    slope(uint y, uint x){
        Y = y;
        X = x;
    }

    bool greater(uint y, uint x) { return Y * x > X * y; }    // this > y/x
    bool greaterEq(uint y, uint x) { return Y * x >= X * y; } // this >= y/x
    bool less(uint y, uint x) { return Y * x < X * y; }       // this < y/x
    bool lessEq(uint y, uint x) { return Y * x <= X * y; }    // this <= y/x
};

class Visibility{

private:

    LetterMap current;

public:

    vector<pi> show(LetterMap l, int maxRay, int originX, int originY,vector<pi> &view){
        pi origin = p(originX, originY);

        this->current = l;

        //view.clear();
        view.push_back(origin);

        for (uint otto = 0; otto < 8; otto++)
            compute(otto, origin, maxRay, 1, slope(1, 1), slope(0, 1),view);

        return view;
    }

    void compute(uint octant, pi origin, int rangeLimit, uint x, slope top, slope bottom,vector<pi> &view){
        for (; x <= (uint)rangeLimit; x++){
            uint topY;
            if (top.X == 1){
                topY = x;
            }else{
                topY = ((x * 2 - 1) * top.Y + top.X) / (top.X * 2);
                if (BlocksLight(x, topY, octant, origin)){
                    if (top.greaterEq(topY * 2 + 1, x * 2) && !BlocksLight(x, topY + 1, octant, origin))
                        topY++;
                }else{
                    uint ax = x * 2;
                    if (BlocksLight(x + 1, topY + 1, octant, origin))
                        ax++;
                    if (top.greater(topY * 2 + 1, ax))
                        topY++;
                }
            }
            uint bottomY;
            if (bottom.Y == 0){
                bottomY = 0;
            }else{
                bottomY = ((x * 2 - 1) * bottom.Y + bottom.X) / (bottom.X * 2); // the tile that the bottom vector enters from the left
                if (bottom.greaterEq(bottomY * 2 + 1, x * 2) && BlocksLight(x, bottomY, octant, origin) &&
                    !BlocksLight(x, bottomY + 1, octant, origin)){
                    bottomY++;
                }
            }

            int wasOpaque = -1;
            for (uint y = topY; (int)y >= (int)bottomY; y--){
                if (rangeLimit < 0 || GetDistance((int)x, (int)y) <= rangeLimit){
                    bool isOpaque = BlocksLight(x, y, octant, origin);
                    bool isVisible =
                        isOpaque || ((y != topY || top.greater(y * 4 - 1, x * 4 + 1)) && (y != bottomY || bottom.less(y * 4 + 1, x * 4 - 1)));
                    if (isVisible)
                        SetVisible(x, y, octant, origin,view);

                    if (x != rangeLimit){
                        if (isOpaque){
                            if (wasOpaque == 0){
                                uint nx = x * 2, ny = y * 2 + 1;

                                if (BlocksLight(x, y + 1, octant, origin))
                                    nx--;
                                if (top.greater(ny, nx)){

                                    if (y == bottomY){
                                        bottom = slope(ny, nx);
                                        break;
                                    }else
                                        compute(octant, origin, rangeLimit, x + 1, top, slope(ny, nx),view);
                                }else{
                                    if (y == bottomY)
                                        return;
                                }
                            }
                            wasOpaque = 1;
                        }else{
                            if (wasOpaque > 0){
                                uint nx = x * 2, ny = y * 2 + 1;

                                if (BlocksLight(x + 1, y + 1, octant, origin))
                                    nx++;

                                if (bottom.greaterEq(ny, nx))
                                    return;
                                top = slope(ny, nx);
                            }
                            wasOpaque = 0;
                        }
                    }
                }
            }

            if (wasOpaque != 0)
                break;
        }
    }

    bool BlocksLight(uint x, uint y, uint octant, pi origin){
        uint nx = origin.first, ny = origin.second;
        switch(octant){
            case 0: nx += x; ny -= y; break;
            case 1: nx += y; ny -= x; break;
            case 2: nx -= y; ny -= x; break;
            case 3: nx -= x; ny -= y; break;
            case 4: nx -= x; ny += y; break;
            case 5: nx -= y; ny += x; break;
            case 6: nx += y; ny += x; break;
            case 7: nx += x; ny += y; break;
        }
        return current.isWall((int)nx, (int)ny);
    }

    void SetVisible(uint x, uint y, uint octant, pi origin,vector<pi> &view){
        uint nx = origin.first, ny = origin.second;
        switch(octant){
            case 0: nx += x; ny -= y; break;
            case 1: nx += y; ny -= x; break;
            case 2: nx -= y; ny -= x; break;
            case 3: nx -= x; ny -= y; break;
            case 4: nx -= x; ny += y; break;
            case 5: nx -= y; ny += x; break;
            case 6: nx += y; ny += x; break;
            case 7: nx += x; ny += y; break;
        }

        view.push_back(p((int)nx, (int)ny));
    }

    // distance from x,y and 0,0
    int GetDistance(int x,int y){
        return x + y; // L going
    }

};

