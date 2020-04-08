#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <set>
#include <map>

#include "./visibility.cpp"

using namespace sf;
using namespace std;

typedef pair<int,int> pi;

enum KindVisibility {
    JustVisited = 1,
    ExploredOnce = 2,
};

class Utils{

private: 

    Keyboard::Key curPressed,lastPressed;
    Font font;
    SoundBuffer sound;
    Visibility vieww;
    //if(!ballSoundBuffer.loadFromFile("resources/ball.wav")) return EXIT_FAILURE;     Sound ballSound(ballSoundBuffer);

public:

    const int W = 800;
    const int H = 800;
    const int S = 14;
    const int FPS = 60;
    const string title = "l3tt3r4";

    bool setFont(string path){
        return font.loadFromFile("resources/sansation.ttf");
    }

    bool setDefaultFont(){
        return setFont("../resources/sansation.ttf");
    }
 
    /**
     * 
     */
    Text create(int size,float x,float y, Color c,string word){
        Text msg;
        msg.setFont(font);
        msg.setCharacterSize(size);
        msg.setPosition(x, y);
        msg.setFillColor(c);
        msg.setString(word);
        return msg;
    }

    // duplication of Text create (ABSOLUTELY TO CHANGE IN EXTERNAL CLASS!)
    RectangleShape createRectangle(int size,float x,float y, Color c){
        RectangleShape msg;
        msg.setPosition(x, y);
        msg.setFillColor(c);
        msg.setSize({size,size});
        return msg;
    }

    // duplication of genLetters (to fix)
    vector<RectangleShape> genRects(LetterMap &m,float zoom,bool floorIncluded,int vision_ray){
        vector<RectangleShape> squares;

        int ray = vision_ray;// m.getPlayerRay(); for now...
        int playerRow = m.getPlayerRow();
        int playerCol = m.getPlayerColumn();

        vector<pi> visibles;
        vector<pi> explored;
        
        m.getExplored(explored);

        // calculate shadows
        //vieww.show(m,ray,playerRow,playerCol,visibles);

        //for test purpose only
        visibles.resize((MAXN + BORDER)*(MAXN + BORDER));
        for(int i=0;i<MAXN+BORDER;i++)
            for(int j=0;j<MAXN+BORDER;j++)
                visibles.push_back({i,j});


        map<pi,KindVisibility> filter;

        for(pi cell : explored) filter[cell] = ExploredOnce;    // First old view
        for(pi cell : visibles) filter[cell] = JustVisited;      // Than override with just visti

        for(pair<pi,KindVisibility> block : filter){
            pi cell = block.first;
            KindVisibility kv = block.second; // defines the trasparency

            int j = cell.first;
            int i = cell.second;

            RectangleShape letter = factoryRectangle(m,i,j,kv,zoom);

            squares.push_back(letter);
        }

        for(pi cell : visibles){
            int j = cell.first;
            int i = cell.second;

            if(!m.isWall(j,i) && floorIncluded){

                string x = to_string(m.floor);
                Color c = Color::Red;
                RectangleShape letter = createRectangle(S,i*S,j*S,c);

                squares.push_back(letter);
            }

        }
        //Exception for the player
        RectangleShape player = createRectangle(S,playerCol*S,playerRow*S,Color::Blue);
        squares.push_back(player);
        
        // what i just saw, now is explored.
        m.addExplored(visibles);
        filter.clear();

        return squares;
    }

    // duplication of factoryLetter (2 fix plz!)
    RectangleShape factoryRectangle(LetterMap &m,int i,int j,KindVisibility kv,float zoom){
        float a = factoryTrasparency(kv);
        Cell type = m.getType(j,i);
        string x = string(1,m.get(j,i));
        Color c = factoryColor(type);

        c.a *= a;

        RectangleShape letter = createRectangle(S*zoom,i*S*zoom,j*S*zoom,c);

        return letter;
    }


    vector<Text> genLetters(LetterMap &m){
        vector<Text> letters;

        int ray = m.getPlayerRay();
        int playerRow = m.getPlayerRow();
        int playerCol = m.getPlayerColumn();

        vector<pi> visibles;
        vector<pi> explored;
        
        m.getExplored(explored);
        vieww.show(m,ray,playerRow,playerCol,visibles);

        map<pi,KindVisibility> filter;

        for(pi cell : explored) filter[cell] = ExploredOnce;    // First old view
        for(pi cell : visibles) filter[cell] = JustVisited;      // Than override with just visti

        for(pair<pi,KindVisibility> block : filter){
            pi cell = block.first;
            KindVisibility kv = block.second; // defines the trasparency

            int j = cell.first;
            int i = cell.second;

            Text letter = factoryLetter(m,i,j,kv);

            letters.push_back(letter);
        }

        for(pi cell : visibles){
            int j = cell.first;
            int i = cell.second;

            if(!m.isWall(j,i)){

                string x = to_string(m.floor);
                Color c = Color::Red;
                Text letter = create(S,i*S,j*S,c,x);

                letters.push_back(letter);
            }

        }

        //Exception for the player
        letters.push_back(create(S,playerCol*S,playerRow*S,Color::Magenta,"@"));
        
        // what i just saw, now is explored.
        m.addExplored(visibles);

        filter.clear();

        return letters;
    }

    Text factoryLetter(LetterMap &m,int i,int j,KindVisibility kv){
        float a = factoryTrasparency(kv);
        Cell type = m.getType(j,i);
        string x = string(1,m.get(j,i));
        Color c = factoryColor(type);

        c.a *= a;

        Text letter = create(S,i*S,j*S,c,x);

        return letter;
    }

    float factoryTrasparency(KindVisibility kv){
        switch (kv){
            case ExploredOnce:
                return 0.2F;
            case JustVisited:
                return 1.0F;
            default:
                return 1.0F;
        };
    }

    Color factoryColor(Cell s){
        switch (s){
            case User:
                return Color::Red;
            case Wall:
                return Color::White;
            case Floor:
                return Color(255,4,255,0.65F);
            default:
                return Color::Red;
        };
    }

};

/*
        for(pi cell : visibles){
            int j = cell.first;
            int i = cell.second;

            if(!m.isWall(j,i)){
                string x = ".";
                Text letter = create(S,cell.second*S,cell.first*S,Color::White,x);
                letters.push_back(letter);
            }else{
                string x = string(1,m.get(j,i));
                Text letter = create(S,cell.second*S,cell.first*S,Color::Yellow,x);
                letters.push_back(letter);
            }
        }
*/