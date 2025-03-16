#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>



struct Obstacle
{
    //id of the obstacle
    int id;

    //Position of obstacle (x,y)
    sf::Vector2f position;

    //Size of obstacle  (width,height)
    sf::Vector2f size;

    //Color of the obstacle
    sf::Color color;

    //Inflation radius of the obstacle
    int inflation;

    //Constructor
    Obstacle(int id, sf::Vector2f position, sf::Vector2f size, sf::Color color, int inflation);
};

class Map 
{
private:

    /*
    The 2D position of the start and end points
    */
    sf::Vector2f start_point;
    sf::Vector2f end_point;
    bool ready; // flag to check if start and end points are set

    // Obstacle related
    int num_obstacles;
    std::vector<Obstacle> obstacles;
    int inflation; //inflation radius for the obstacles

public:
    Map();
    ~Map();
    

};

#endif // MAP_HPP