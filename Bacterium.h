//
// Created by flo on 10/28/2021.
//

#ifndef NOMPROJET_BACTERIUM_H
#define NOMPROJET_BACTERIUM_H

#include<iostream>
#include "Entity.h"

// Bacterium Enemy Entity
class Bacterium: public Entity {
public:

    Bacterium(LetterMap &map,int visibility_ray,int delay_move) : Entity(map,visibility_ray,delay_move) {
        std::cout << "assurdo!\n";
    }

    virtual void draw(RenderWindow &window,int pixePerUnit,Camera2D &cam){
        CircleShape circle = CircleShape(1.0F/2.0F*pixePerUnit);
        circle.setFillColor(Color::Yellow);
        circle.setPosition((this->getRealCol()*pixePerUnit)-cam.getX(),(this->getRealRow()*pixePerUnit)-cam.getY());
        circle.setPointCount(100);
        //circle.move(-circle.getRadius() ,-circle.getRadius());

        //circle.setPosition(this->row,this->col);
        CircleShape ray2 = circle;
        ray2.setFillColor(Color::Transparent);
        ray2.setRadius(pixePerUnit * this->getVisibilityRay());
        ray2.setOutlineColor(Color(255,255,255,50));
        ray2.setOutlineThickness(1);
        ray2.move(-ray2.getRadius() ,-ray2.getRadius());

        vector<pair<int,int>> folPath = this->getFollowingPath();
        window.draw(circle);
        window.draw(ray2);

        for(pair<int,int> p:folPath){
            RectangleShape sq;
            sq.setPosition( (p.second*pixePerUnit)- cam.getX(), (p.first*pixePerUnit)  -cam.getY() );
            sq.setFillColor(Color::Blue);
            sq.setSize({0.6F*pixePerUnit,0.6F*pixePerUnit});

            window.draw(sq);
        }

    }

};


#endif //NOMPROJET_BACTERIUM_H
