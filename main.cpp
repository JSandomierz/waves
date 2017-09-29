#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "LinePhysics.cpp"
#include "DrawableObjects.cpp"

using namespace std;

int main()
{
    //window props
    int width=1600, height=800;

    //test
    int counter=0;

    //flags
    bool bend = false;

    //data prep
    int numJoints = 1000;
    sf::Vector2f startingPos = sf::Vector2f(0-width/numJoints,height/2);//100,100
    sf::Vector2f endingPos = sf::Vector2f(width+width/numJoints,height/2);//600,100
    Line line(numJoints, startingPos, endingPos);

    //line.LinePoints.at(0)->velocity=sf::Vector2f(sf::Vector2f(0,50));
    //line.passVelocities();

    //squares representing line points
    std::vector<WavePoint*> shapeVector;
    //sf::RectangleShape* tmp;
    WavePoint* tmp;
    for(int i=0;i<numJoints;i++){
        //tmp = new sf::RectangleShape(sf::Vector2f(line.grad.x,line.grad.x));
        tmp = new WavePoint(sf::Vector2f(line.LinePoints.at(i)->position.x, height), line.grad.x);
        tmp->setFillColor(sf::Color::Blue);
        //tmp->setPosition(line.LinePoints.at(i)->position);
        shapeVector.push_back(tmp);
    }

    //main window loop
    sf::RenderWindow* window=new sf::RenderWindow(sf::VideoMode(width, height), "Waves");
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(false);
    while(window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            //event processing
            if (event.type == sf::Event::Closed) window->close();
            if(event.type==sf::Event::MouseButtonPressed)
            {
                sf::Mouse m;
                sf::Vector2i clickPos = m.getPosition(*window);
                //clickPos -= window->getPosition();
                float force = (clickPos.y - height*0.5f) *0.1;
                int id = int(((float)clickPos.x)/line.grad.x);
                cout<< clickPos.x <<", "<< clickPos.y<<endl<<id<<", "<<force<<endl;
                line.LinePoints.at( id )->velocity.y = force;
            }
            if(event.type == sf::Event::KeyPressed){
                //bend=true;
                if(event.key.code == sf::Keyboard::Space) line.reset(height/2);
                line.passVelocities();
            }
            if(event.type == sf::Event::KeyReleased){
                bend=false;
            }
        }
        //prepare to draw line
        for(int i=0;i<numJoints;i++) shapeVector.at(i)->setPosition(line.LinePoints.at(i)->position);

        window->clear();
        for(int i=0;i<numJoints;i++) window->draw(*shapeVector.at(i));
        if(!bend) line.passVelocities();
        window->display();
    }
    return 0;
}
