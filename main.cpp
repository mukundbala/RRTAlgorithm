/*
The entry point for the RRT planner
*/
#include <iostream>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <utility>
int main()
{
    std::cout << "##########################################################" << std::endl;
    std::cout << "############# WELCOME TO RRT PLANNER #####################" << std::endl;
    std::cout << "##########################################################\n" << std::endl;

    // Get the path to the current working directory
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

    // Open the app.yaml file
    YAML::Node app_yaml;
    auto string_path = app_yaml_path.string();
    std::cout << "Reading app.yaml file from: " << string_path << std::endl;
    app_yaml = YAML::LoadFile(string_path);

    //Load the map  parameters
    std::pair<int,int> window_size = {app_yaml["window_size_x"].as<int>(), app_yaml["window_size_y"].as<int>()};
    int num_onstacles = app_yaml["num_obstacles"].as<int>();
    float inflation = app_yaml["inflation"].as<float>();
    std::string rrt_variant = app_yaml["variant"].as<std::string>();
    auto planner_path = rrt_variant + ".yaml";

    // Load params for RRT Variant
    YAML::Node planner_param = YAML::LoadFile(yaml_folder_path / planner_path);

    std::cout<< "Map Parameters: " << std::endl;
    std::cout << "Window Size: " << window_size.first << "x" << window_size.second << std::endl;
    std::cout << "Number of Obstacles: " << num_onstacles << std::endl;
    std::cout << "Inflation: " << inflation << std::endl;
    std::cout << "RRT Variant: " << rrt_variant << std::endl;
}