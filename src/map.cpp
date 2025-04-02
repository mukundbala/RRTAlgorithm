#include "map.hpp"

//Constructor for the Obstacle class
Obstacle::Obstacle(int id, sf::Vector2f position, sf::Vector2f size, sf::Color color, int inflation,std::string_view type)
{
    this->id = id;
    this->position = position;
    this->size = size;
    this->color = color;
    this->inflation = inflation;
    this->type = type;
}

std::pair<sf::Vector2f,sf::Vector2f> Obstacle::get_obstacle_dims()
{
    auto rez = std::make_pair(this->position,this->size);

    return rez;
}

std::pair<sf::Vector2f,sf::Vector2f> Obstacle::get_inflated_obstacle_dims()
{
    float radius = this->size.x;
    float inflated_radius = radius + this->inflation + this->inflation;

    sf::Vector2f inflated_size(inflated_radius,inflated_radius);

    auto rez = std::make_pair(this->position,inflated_size);

    return rez;
}


//Constructor for the map class
Map::Map(int num_obstacles, float obstacle_density, int inflation, std::pair<unsigned int,unsigned int> window_size)
{
    this->num_obstacles = num_obstacles;
    this->obstacle_density = obstacle_density;
    this->inflation = inflation;
    this->window_size = window_size;
    this->ready = false;

    //Set the start and end points to -1,-1
    this->start_point = sf::Vector2f(-1, -1);
    this->end_point = sf::Vector2f(-1, -1);
}

// Setter for start point


//Random circle obstacle generation
std::vector<Obstacle> Map::generate_random_circle_obstacles(int num_obstacles,std::pair<int,int> map_size,int inflation,float obstacle_density)
{ 
    /*
    Generate random circle obstacles on the map
    */
   
    auto map_x = map_size.first;
    auto map_y = map_size.second;

    auto total_map_area = map_x * map_y;
    auto total_obstacle_area = total_map_area * obstacle_density;
    auto obstacle_area = total_obstacle_area / num_obstacles;
    auto obstacle_radius = sqrt(obstacle_area / M_PI);

    //Recompute the bounds of the map to prevent obstacles from overflowing the map
    auto map_x_min = inflation + inflation + obstacle_radius;
    auto map_y_min = inflation + inflation + obstacle_radius;

    auto map_x_max = map_x - inflation - inflation - obstacle_radius;
    auto map_y_max = map_y - inflation - inflation - obstacle_radius;

    std::random_device rdx;
    std::random_device rdy;
    std::mt19937 genx(rdx());
    std::mt19937 geny(rdy());

    std::uniform_int_distribution<> distribx(map_x_min, map_x_max);
    std::uniform_int_distribution<> distriby(map_y_min, map_y_max);

    //Create and reserve space for the obstacles
    std::vector<Obstacle> obstacles;
    obstacles.reserve(num_obstacles);

    //Run through the obstacles and generate random circles
    for (int i = 0; i < num_obstacles; i++)
    {
        auto x_rand = distribx(genx);
        auto y_rand = distriby(geny);

        auto radius = obstacle_radius;

        sf::Vector2f position(x_rand, y_rand);
        sf::Vector2f size(radius, radius);
        sf::Color color = sf::Color::Red;
        
        //Check if there are any collisions
        bool collision = false;
        for (const auto &obs : obstacles)
        {
            auto euclidean_distance = sqrt(pow(position.x - obs.position.x, 2) + pow(position.y - obs.position.y, 2));
            if (euclidean_distance < radius + obs.size.x)
            {
                collision = true;
                break;
            }
        }

        if (collision)
        {
            i--;
            continue;
        }

        obstacles.emplace_back(i, position, size, color, inflation,"circle");
    }

    return obstacles;
}

//Random square obstacle generation
std::vector<Obstacle> Map::generate_random_square_obstacles(int num_obstacles,std::pair<int,int> map_size,int inflation,float obstacle_density)
{
    /*
    Generate random square obstacles on the map
    */
    auto map_x = map_size.first;
    auto map_y = map_size.second;

    auto total_map_area = map_x * map_y;
    auto total_obstacle_area = total_map_area * obstacle_density;
    auto obstacle_area = total_obstacle_area / num_obstacles;
    auto obstacle_side = sqrt(obstacle_area);

    //Recompute the bounds of the map to prevent obstacles from overflowing the map
    auto map_x_min = inflation + inflation + obstacle_side;
    auto map_y_min = inflation + inflation + obstacle_side;

    auto map_x_max = map_x - inflation - inflation - obstacle_side;
    auto map_y_max = map_y - inflation - inflation - obstacle_side;

    std::random_device rdx;
    std::random_device rdy;
    std::mt19937 genx(rdx());
    std::mt19937 geny(rdy());

    std::uniform_int_distribution<> distribx(map_x_min, map_x_max);
    std::uniform_int_distribution<> distriby(map_y_min, map_y_max);

    //Create and reserve space for the obstacles
    std::vector<Obstacle> obstacles;
    obstacles.reserve(num_obstacles);

    //Run through the obstacles and generate random squares
    for (int i = 0; i < num_obstacles; i++)
    {
        auto x_rand = distribx(genx);
        auto y_rand = distriby(geny);

        auto side = obstacle_side;

        sf::Vector2f position(x_rand, y_rand);
        sf::Vector2f size(side, side);
        sf::Color color = sf::Color::Red;
        
        //Check if there are any collisions
        bool collision = false;
        for (const auto &obs : obstacles)
        {
            auto euclidean_distance = sqrt(pow(position.x - obs.position.x, 2) + pow(position.y - obs.position.y, 2));
            if (euclidean_distance < side + obs.size.x)
            {
                collision = true;
                break;
            }
        }

        if (collision)
        {
            i--;
            continue;
        }

        obstacles.emplace_back(i, position, size, color, inflation,"square");
    }

    return obstacles;
}


void Map::setup_map()
{
    /*
    Setup the map with the start and end points
    */
    if (this->ready)
    {
        return;
    }

    //Generate random circle obstacles
    this->obstacles = generate_random_circle_obstacles(this->num_obstacles,this->window_size,this->inflation,this->obstacle_density);

}

void Map::set_start(sf::Vector2f &start)
{
    this->start_point = start;
}

void Map::set_end(sf::Vector2f &end)
{
    this->end_point = end;
}

void Map::set_start_end(sf::Vector2f &start,sf::Vector2f &end)
{
    this->start_point = start;
    this->end_point = end;

    // The map has all the information required for the planner
    this->ready = true;
}

void Map::reset_start_end()
{
    this->start_point = sf::Vector2f(-1, -1);
    this->end_point = sf::Vector2f(-1, -1);

    // Set ready to false because the map does not have all the information yet
    this->ready = false;
}

std::vector<Obstacle> Map::get_obstacles()
{
    return this->obstacles;
}