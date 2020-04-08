#include "./entity.cpp"
#include<iostream>

// Bacterium Enemy Entity
class Bacterium: public Entity {
   public:

        Bacterium(LetterMap &map,int visibility_ray,int delay_move) : Entity(map,visibility_ray,delay_move) {
            std::cout << "assurdo!\n";
        }

        virtual void draw(RenderWindow &window,int pixePerUnit,Camera2D &cam){
            CircleShape circle = CircleShape(1.0F/2.0F*pixePerUnit);
            circle.setFillColor(Color::Yellow);
            circle.setPosition( (this->row*pixePerUnit) - cam.getX(),(this->col*pixePerUnit) -cam.getY() );
            //circle.setPosition(this->row,this->col);

            vector<pair<int,int>> folPath = this->getFollowingPath();
            window.draw(circle);

            for(pair<int,int> p:folPath){
                RectangleShape sq;
                sq.setPosition( (p.first*pixePerUnit)- cam.getX(), (p.second*pixePerUnit)  -cam.getY() );
                sq.setFillColor(Color::Blue);
                sq.setSize({0.6F*pixePerUnit,0.6F*pixePerUnit});    

                window.draw(sq);        
            }

        }
        
};