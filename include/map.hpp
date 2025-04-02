#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <utility>
#include <random>
#include <cmath>

struct Obstacle
{
    //id of the obstacle
    int id;

    std::string_view type; //circle or sq

    // Position of obstacle (x,y)
    sf::Vector2f position;

    // Size of obstacle  (width,height) or (radius,radius)
    sf::Vector2f size;

    // Color of the obstacle
    sf::Color color;

    // Inflation radius of the obstacle
    int inflation;

    // Constructor
    Obstacle(int id, sf::Vector2f position, sf::Vector2f size, sf::Color color, int inflation,std::string_view type);

    // Get the position,size of the obstacle 
    std::pair<sf::Vector2f,sf::Vector2f> get_obstacle_dims();

    // Get the position and the inflated size of the obstacle
    std::pair<sf::Vector2f,sf::Vector2f> get_inflated_obstacle_dims();

};

class Map 
{
private:

    /*
    ----->x
    |
    |
    |
    v
    y
    */

    /*
    The 2D position of the start and end points
    */
    sf::Vector2f start_point;
    sf::Vector2f end_point;
    bool ready; // flag to check if start and end points are set

    // Obstacle related
    int num_obstacles;
    float obstacle_density;
    std::vector<Obstacle> obstacles;
    int inflation; //inflation radius for the obstacles
    std::pair<int,int> window_size; // window size for the map (x,y)

public:
    Map(int num_obstacles, float obstacle_density, int inflation, std::pair<unsigned int,unsigned int> window_size); //constructor
    
    std::vector<Obstacle> generate_random_circle_obstacles(int num_obstacles,std::pair<int,int> map_size,int inflation,float obstacle_density);

    std::vector<Obstacle> generate_random_square_obstacles(int num_obstacles,std::pair<int,int> map_size,int inflation,float obstacle_density);

    void setup_map();

    void set_start(sf::Vector2f &start);

    void set_end(sf::Vector2f &end);

    void set_start_end(sf::Vector2f &start,sf::Vector2f &end);

    void reset_start_end();

    std::vector<Obstacle> get_obstacles();
};

#endif // MAP_HPP