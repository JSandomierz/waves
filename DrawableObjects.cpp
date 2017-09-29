#include <SFML/Graphics.hpp>

class WavePoint: public sf::Drawable, sf::Transformable{
    private:
    sf::Vector2f position;
    sf::Vector2f widthVec;
    sf::VertexArray vertices;
    public:
    WavePoint(sf::Vector2f startingPosition, float width){
        widthVec = sf::Vector2f(width, 0);

        vertices.setPrimitiveType(sf::Quads);
        //these will change
        vertices.append(*(new sf::Vertex(startingPosition)));
        vertices.append(*(new sf::Vertex(startingPosition)));
        //these are static
        vertices.append(*(new sf::Vertex(startingPosition+widthVec)));
        vertices.append(*(new sf::Vertex(startingPosition+widthVec)));
    }
    void setPosition(sf::Vector2f &position){
        vertices[0].position=position;
        vertices[3].position=position+widthVec;
    }
    void setFillColor(sf::Color color){
        for(int i=0; i<4;i++) vertices[i].color=color;
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
        target.draw(vertices);
    }
};
