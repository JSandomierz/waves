#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

class LinePoint{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    LinePoint* prevPoint;
    LinePoint* nextPoint;
    LinePoint(sf::Vector2f position){
        this->position=position;
        this->velocity=sf::Vector2f(0,0);
        this->nextPoint=NULL;
        this->prevPoint=NULL;
    }
};

class Line{
public:
    std::vector<LinePoint*> LinePoints;
    sf::Vector2f grad;
    float springness, stiffness, energyLossing, gravity;
    Line(int numberPoints, sf::Vector2f initPos, sf::Vector2f endPos){
        /*springness = 0.6f;
        stiffness = 0.0f;
        energyLossing= 0.8f;
        gravity = 0.05f;*/
        springness = 0.7f;
        stiffness = 0.00001f;
        energyLossing= 0.9f;
        gravity = 0.05f;//to new springless*/

        LinePoint* tmp;

        grad = sf::Vector2f((endPos.x-initPos.x)/numberPoints, (endPos.y-initPos.y)/numberPoints);
        for(int i=0;i<numberPoints;i++){
            sf::Vector2f position = sf::Vector2f(initPos.x + grad.x * i, initPos.y + grad.y*i);
            //std::cout<<"Knot "<<i<<" pos: "<<position.x<<", "<<position.y<<std::endl;
            tmp = new LinePoint(position);
            LinePoints.push_back(tmp);
        }

        //setting prev and next point
        LinePoints.at(0)->nextPoint = (LinePoints.at(1));
        for(int i=1;i<numberPoints-1;i++){
            LinePoints.at(i)->prevPoint = (LinePoints.at(i-1));
            LinePoints.at(i)->nextPoint = (LinePoints.at(i+1));
        }
        LinePoints.at(numberPoints-1)->prevPoint = (LinePoints.at(numberPoints-2));
        //LinePoints.at(numberPoints-1)->nextPoint = (LinePoints.at(0));
    }
    float getDistance(sf::Vector2f a, sf::Vector2f b){
        return sqrt( (b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y) );
    }
    float getCosAlpha(sf::Vector2f a, sf::Vector2f b){
        return acos( b.y-a.y / getDistance(a,b) )/3.14*180;
    }
    void passVelocities(){
        passVelocitiesSoftOld();
    }
    void passVelocitiesRigid(){
        for(int i=0;i<LinePoints.size()-1;i++){
            LinePoint* current = LinePoints.at(i);

            //velocity pass to next node
            current->nextPoint->velocity = sf::Vector2f(current->velocity.x*springness, current->velocity.y*springness);

            //line reflection force
            float distance = getDistance(current->position, sf::Vector2f(current->position.x,100));
            current->velocity = sf::Vector2f( sf::Vector2f(0, (100-current->position.y)*stiffness ) );

            //position change due to velocity
            current->position= sf::Vector2f(current->velocity.x + current->position.x , current->velocity.y + current->position.y);

            //current velocity energy loss
            current->velocity = sf::Vector2f(current->velocity.x*energyLossing, current->velocity.y*energyLossing);
        }
    }
    void passVelocitiesSoftOld(){
        for(int i=1;i<LinePoints.size()-1;i++){
            LinePoint* current = LinePoints.at(i);

            //line reflection force
            float distToPrev = getDistance(current->position, current->prevPoint->position);
            float distToNext = getDistance(current->position, current->nextPoint->position);
            float localSpringness=springness;
            //if(distToNext>distToPrev) localSpringness = springness*distToNext/grad.x;
            //else localSpringness = springness*distToPrev/grad.x;
            sf::Vector2f currentForce = sf::Vector2f(0, 0);
            //currentForce.y+=gravity;
            if(distToPrev > 1.2f*grad.x){
                //localSpringness = springness * distToPrev/grad.x;
                if(current->prevPoint->position.y > current->position.y){ currentForce+=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y<0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
                }
                else currentForce-=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y>0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
            }
            if(distToNext > 1.2f*grad.x){
                //localSpringness = springness * distToNext/grad.x;
                if(current->nextPoint->position.y > current->position.y){ currentForce+=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y<0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
                }
                else currentForce-=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y>0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
            }

            //calculate current velocity
            current->velocity += currentForce;
            //if(i==1) std::cout<<"Distance: next: "<<distToNext<<", prev: "<<distToPrev<<", Pos: "<<current->position.y<<"\nVel: "<<current->velocity.y<<"\n";

            //position change due to velocity
            if(abs(current->velocity.y) > stiffness){
                current->position= sf::Vector2f(current->velocity.x + current->position.x , current->velocity.y + current->position.y);
                //current velocity energy loss
                current->velocity = sf::Vector2f(current->velocity.x*energyLossing, current->velocity.y*energyLossing);
            }
        }
    }
    void passVelocitiesSoft(){
        for(int i=1;i<LinePoints.size()-1;i++){
            LinePoint* current = LinePoints.at(i);

            //line reflection force
            float distToPrev = getDistance(current->position, current->prevPoint->position);
            float distToNext = getDistance(current->position, current->nextPoint->position);
            float localSpringness;
            //if(distToNext>distToPrev) localSpringness = springness*distToNext/grad.x;
            //else localSpringness = springness*distToPrev/grad.x;
            sf::Vector2f currentForce = sf::Vector2f(0, 0);
            currentForce.y+=gravity;
            if(distToPrev > 1.2f*grad.x){
                localSpringness = springness * distToPrev/grad.x;
                if(current->prevPoint->position.y > current->position.y){ currentForce+=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y<0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
                }
                else currentForce-=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y>0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
            }
            if(distToNext > 1.2f*grad.x){
                localSpringness = springness * distToNext/grad.x;
                if(current->nextPoint->position.y > current->position.y){ currentForce+=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y<0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
                }
                else currentForce-=sf::Vector2f(0, localSpringness);
                    //if(current->velocity.y>0) current->velocity = sf::Vector2f( -current->velocity.x/3, -current->velocity.y/3 );
            }

            //calculate current velocity
            current->velocity += currentForce;
            //if(i==1) std::cout<<"Distance: next: "<<distToNext<<", prev: "<<distToPrev<<", Pos: "<<current->position.y<<"\nVel: "<<current->velocity.y<<"\n";

            //position change due to velocity
            if(abs(current->velocity.y) > stiffness){
                current->position= sf::Vector2f(current->velocity.x + current->position.x , current->velocity.y + current->position.y);
                //current velocity energy loss
                current->velocity = sf::Vector2f(current->velocity.x*energyLossing, current->velocity.y*energyLossing);
            }
        }
    }
    void reset(float initHeight){
        for(int i=1;i<LinePoints.size()-1;i++){
            LinePoints.at(i)->velocity = sf::Vector2f(0, 0);
            LinePoints.at(i)->position.y = initHeight;
        }
    }
};
