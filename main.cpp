/*
The entry point for the RRT planner
*/
#include <iostream>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <utility>
#include <atomic>
#include <map.hpp>
#include <SFML/Graphics.hpp>

//SFML SPECIFIC FUNCTION DECLARATIONS//
void draw_obstacles(sf::RenderWindow &window , std::vector<Obstacle> &obstacles);
void draw_start_point(sf::RenderWindow &window ,sf::Vector2f &start);
void draw_end_point(sf::RenderWindow &window ,sf::Vector2f &end);
sf::Vector2f get_null_pos();

int main()
{
    std::cout << "\n##########################################################" << std::endl;
    std::cout << "############# WELCOME TO RRT PLANNER #####################" << std::endl;
    std::cout << "##########################################################\n" << std::endl;

    /*
    Get the paths to the config folder and app config file. We assume
    relative paths from the base folder path.
    */
    auto base_folder_path = std::filesystem::current_path().parent_path();
    auto yaml_folder_path = base_folder_path / "config";
    auto app_yaml_path = yaml_folder_path / "app.yaml";

    // Check if the yaml folder exists
    if (!std::filesystem::exists(yaml_folder_path))
    {
        std::cerr << "Error: yaml folder does not exist" << std::endl;
        return 1;
    }

    // Check if the app.yaml file exists
    if (!std::filesystem::exists(app_yaml_path))
    {
        std::cerr << "Error: app.yaml file does not exist" << std::endl;
        return 1;
    }

    // Open the app.yaml file adn read it as a YAML object
    YAML::Node app_yaml;
    auto string_path = app_yaml_path.string();
    app_yaml = YAML::LoadFile(string_path);

    //Load the map  parameters
    std::pair<unsigned int,unsigned int> window_size = {app_yaml["window_size_x"].as<unsigned int>(), app_yaml["window_size_y"].as<unsigned int>()};
    int num_obstacles = app_yaml["num_obstacles"].as<int>();
    float obstacle_density = app_yaml["obstacle_density"].as<float>();
    float inflation = app_yaml["inflation"].as<float>();
    std::string rrt_variant = app_yaml["variant"].as<std::string>();
    auto planner_path = rrt_variant + ".yaml";

    // Load params for RRT Variant
    YAML::Node planner_param = YAML::LoadFile(yaml_folder_path / planner_path);

    std::cout<< "Map Parameters: " << std::endl;
    std::cout << "Window Size: " << window_size.first << "x" << window_size.second << std::endl;
    std::cout << "Number of Obstacles: " << num_obstacles << std::endl;
    std::cout << "Percentage of Map covered by Obstacles: " << obstacle_density * 100 << "%" << std::endl;
    std::cout << "Inflation: " << inflation << std::endl;
    std::cout << "RRT Variant: " << rrt_variant << std::endl;

    // Instructions for the user
    std::cout << "Instructions\n";
    std::cout << "S : Set Start Point\n";
    std::cout << "E : Set End Point\n";
    std::cout << "G : Start Planner\n";
    std::cout << "C : Clear Start and End Point\n";
    std::cout << "R : Reset Planner, Start and End Points\n";
    std::cout << "P : Pause Planner\n";

    // Set up the map with the given parameters
    Map world_map(num_obstacles, obstacle_density, inflation, window_size);

    // Setup map
    world_map.setup_map();

    // Get the obstacles
    auto all_obstacles = world_map.get_obstacles();

    //Start the UI rendering
    sf::RenderWindow window(sf::VideoMode({window_size.first, window_size.second}), "RRT Planner");
    window.setKeyRepeatEnabled(false);

    // The start and end points
    bool select_start = false;
    bool select_end = false;
    sf::Vector2f start_point(-1.0,-1.0);
    sf::Vector2f end_point(-1.0,-1.0);

    // Atomic flags to handle concurrency between GUI thread (aka the main thread) and Planner thread
    std::atomic<bool> planner_running = false;
    std::atomic<bool> planner_pause = false;


    // Main loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::S && !planner_running)
                {
                    select_start = true;
                    select_end = false;
                    std::cout << "Please Select A Start Point" << std::endl;
                }

                if (keyPressed->code == sf::Keyboard::Key::E && !planner_running)
                {
                    select_end = true;
                    select_start = false;
                    std::cout << "Please Select An End Point" << std::endl;
                }

                if (keyPressed -> code == sf::Keyboard::Key::C && !planner_running)
                {
                    std::cout << "Clear Start and End Points\n";
                    start_point = get_null_pos();
                    end_point   = get_null_pos();
                    select_start = false;
                    select_end = false;
                }

                if (keyPressed -> code == sf::Keyboard::Key::G && !planner_running && start_point.x >= 0 && end_point.x >= 0)
                {
                    std::cout <<"Starting RRT Planner (Not Implemented)\n";
                    planner_running = true;
                }

                if (keyPressed -> code == sf::Keyboard::Key::P && planner_running)
                {
                    planner_pause = !planner_pause;
                    std::string_view text = planner_pause ? "Planner Paused!" : "Planner Resumed!";
                    std::cout << text << std::endl;
                }

                if (keyPressed -> code == sf::Keyboard::Key::R && planner_running)
                {
                    planner_running = false;
                    start_point = get_null_pos();
                    end_point   = get_null_pos();

                    std::cout << "Planner Stopped & Start & End Points Cleared (Not Fully Implemented)" << std::endl;
                }


            }
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if (select_start || select_end)
                {
                    // The position of the mose
                    auto mouse_pos = sf::Mouse::getPosition(window);
                    
                    
                    // Check if the mouse position is within the window
                    bool mouse_within_window = mouse_pos.x >= 0 && mouse_pos.x <= window_size.first && mouse_pos.y >= 0 && mouse_pos.y <= window_size.second;
                    if (mouse_within_window)
                    {
                        //Mouse position as a floating point
                        auto mouse_map_pose = sf::Vector2f(mouse_pos);
                        
                        bool coll = false;
                        // Do a linear search for collisions
                        for (auto &obs : all_obstacles)
                        {
                            // Get the position and size
                            auto [obs_pos,obs_size] = obs.get_inflated_obstacle_dims();
                            float inflated_radius = obs_size.x;
                            
                            // Check if the point falls within
                            auto euclidean_distance = std::sqrt(std::pow(mouse_map_pose.x-obs_pos.x,2) + std::pow(mouse_map_pose.y-obs_pos.y,2));
                            if (euclidean_distance <= inflated_radius)
                            {
                                coll = true;
                                break;
                            }
                        }

                        if (coll)
                        {
                            std::cout << "Please Select Point from Free Space!" << std::endl;
                        }

                        else
                        {
                            // If there are no collisions,assign the point accordingly
                            if (select_start)
                            {
                                start_point = mouse_map_pose;
                                std::cout << "Start Point: (" << start_point.x << "," << start_point.y << ")\n";
                                select_start = false;                          
                            }
                            
                            else
                            {
                                end_point = mouse_map_pose;
                                std::cout << "End Point: (" << end_point.x << "," << end_point.y << ")\n";
                                select_end = false;
                            }
                        }

                    }
                }
                            
            }

        }
        window.clear(sf::Color::Black); 

        // Draw the obstacles
        draw_obstacles(window, all_obstacles);

        //Draw the start and end point
        draw_start_point(window,start_point);
        draw_end_point(window,end_point);

        window.display();
    }
}

void draw_obstacles(sf::RenderWindow &window , std::vector<Obstacle> &obstacles)
{
    for (const auto &obs : obstacles)
    {
        if (obs.type == "circle")
        {
            sf::CircleShape circo;

            circo.setRadius(obs.size.x);
            circo.setOrigin(sf::Vector2f(circo.getRadius(), circo.getRadius()));
            circo.setFillColor(obs.color);
            circo.setPosition(obs.position);
            window.draw(circo);
        }

        if (obs.type == "square")
        {
            sf::RectangleShape square;

            square.setSize(obs.size);
            square.setOrigin(sf::Vector2f(obs.size.x/2, obs.size.y/2));
            square.setFillColor(obs.color);
            square.setPosition(obs.position);
            window.draw(square);
        }

    }
}

void draw_start_point(sf::RenderWindow &window ,sf::Vector2f &start)
{
    if (start.x < 0 or start.y < 0)
    {
        return;
    }

    sf::CircleShape c;
    c.setRadius(10.0);
    c.setOrigin(sf::Vector2f(c.getRadius(), c.getRadius()));
    c.setPosition(start);
    c.setFillColor(sf::Color::Green);
    
    //draw it on the window
    window.draw(c);
}

void draw_end_point(sf::RenderWindow &window , sf::Vector2f &end)
{
    if (end.x < 0 or end.y < 0)
    {
        return;
    }

    sf::CircleShape c;
    c.setRadius(10.0);
    c.setOrigin(sf::Vector2f(c.getRadius(), c.getRadius()));
    c.setPosition(end);
    c.setFillColor(sf::Color::Blue);

    window.draw(c);
}

sf::Vector2f get_null_pos()
{
    auto n = sf::Vector2f(-1,-1);
    return n;
}