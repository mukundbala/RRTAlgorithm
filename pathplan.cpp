#include "map.hpp"

Map::Map():
windowSize_x(1280),
windowSize_y(1000),
free_space_concentration(0.5),
obstacle_type("scatter")
{
    mapWindow.create(sf::VideoMode(windowSize_x,windowSize_y),"Path Planner");
    if (obstacle_type=="scatter"){
        int totalPossibleObstacles=(windowSize_x*windowSize_y)/(obs_size.x*obs_size.y);
        this->num_obstacles=totalPossibleObstacles * (1-free_space_concentration);
    }
    //todo: else for solid large obstacles
    GenerateRandomPoints(obstacle_positions);
}

Map::Map(int windowX, int windowY,double freeSpace):
windowSize_x(windowX),
windowSize_y(windowY),
free_space_concentration(freeSpace),
obstacle_type("scatter")
{
    mapWindow.create(sf::VideoMode(windowSize_x,windowSize_y),"Path Planner");
    if (obstacle_type=="scatter"){
        int totalPossibleObstacles=(windowSize_x*windowSize_y)/(obs_size.x*obs_size.y);
        this->num_obstacles=totalPossibleObstacles * (1-free_space_concentration);
    }
    GenerateRandomPoints(obstacle_positions);
    //todo: else for solid large obstacles
}

Map::Map(int windowX, int windowY, double freeSpace, std::string obs_type):
windowSize_x(windowX),
windowSize_y(windowY),
free_space_concentration(freeSpace),
obstacle_type("scatter")
{
    mapWindow.create(sf::VideoMode(windowSize_x,windowSize_y),"Path Planner");
    if (obstacle_type=="scatter"){
        int totalPossibleObstacles=(windowSize_x*windowSize_y)/(obs_size.x*obs_size.y);
        this->num_obstacles=totalPossibleObstacles * (1-free_space_concentration);
    }
    GenerateRandomPoints(obstacle_positions);
    //todo: else for solid large obstacles
}

void Map::GenerateRandomPoints(std::unordered_set<Point,Point::HashFunction>& obsPos){
    while (obsPos.size()<num_obstacles){
        std::random_device xgen;
        std::mt19937 rngx(xgen());
        std::uniform_int_distribution<std::mt19937::result_type> distx(obs_size.x+1,windowSize_x-obs_size.x-1);
        
        std::random_device ygen;
        std::mt19937 rngy(ygen());
        std::uniform_int_distribution<std::mt19937::result_type> disty(obs_size.y+1,windowSize_y-obs_size.y-1);
        
        Point newPoint(distx(rngx),disty(rngy));
        if (obsPos.find(newPoint)==obsPos.end()){
            obsPos.emplace(newPoint);
        }
    }
    std::cout<<"[LOG]: Window Size: "<<this->windowSize_x<<" "<<this->windowSize_y<<"\n";
    std::cout<<"[LOG]: Window Area: "<<this->windowSize_x*this->windowSize_y<<"\n";
    std::cout<<"[LOG]: Obstacle Area: "<<obs_size.x*obs_size.x<<"\n";
    std::cout<<"[LOG]: Free Space: "<<this->free_space_concentration<<"\n"; 
    std::cout<<"[LOG]: Number of Computed Obstacles: "<<num_obstacles<<"\n";
    std::cout<<"[LOG]: Number of Points Generated: " <<obsPos.size()<<"\n";
    if (obsPos.size() == num_obstacles){std::cout<<"[LOG]: Obstacle Points Created Successfully"<<"\n";}
    else{std::cout<<"[LOG]: Obstacle Points Creating Failed!"<<"\n";}
}

void Map::drawObstacles(){
    for (auto point:obstacle_positions){
        sf::RectangleShape obstacle;
        obstacle.setFillColor(OBS_COLOR);
        obstacle.setSize(obs_size);
        obstacle.setPosition(sf::Vector2f(point.x,point.y));
        this->mapWindow.draw(obstacle);
    }
}

void Map::DrawPoint(Point pt, const sf::Color &color){
    // sf::CircleShape point_to_draw(5.0);
    // point_to_draw.setFillColor(color);
    // point_to_draw.setPosition(sf::Vector2f(pt.x,pt.y));
    // mapWindow.draw(point_to_draw);
    sf::Vertex point_to_draw(sf::Vector2f(pt.x,pt.y),color);
    mapWindow.draw(&point_to_draw,1,sf::Points);
}
void Map::DrawStartEnd(Point pt, const sf::Color &color){
    sf::CircleShape point_to_draw(3.0);
    point_to_draw.setFillColor(color);
    point_to_draw.setPosition(sf::Vector2f(pt.x,pt.y));
    mapWindow.draw(point_to_draw);
}

int main(){
    //temporary input method for debuggin only
    OBSTACLE_TYPE="scatter";
    WINDOWSIZE_X=1280;
    WINDOWSIZE_Y=1000;
    FREESPACE=0.9;

    //initialise windowObject
    Map myMap(WINDOWSIZE_X,WINDOWSIZE_Y,FREESPACE,OBSTACLE_TYPE);
    Map* map_ptr=&myMap;
    while(myMap.mapWindow.isOpen()){
        myMap.mapWindow.clear(WINDOW_COLOR); //ready to draw;
        myMap.drawObstacles();
        sf::Event event;
        sf::Event mousevent;
        while(myMap.mapWindow.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                myMap.~Map();
            }
            if (event.type==sf::Event::MouseButtonPressed && myMap.getClick()<2){
                if (event.mouseButton.button==sf::Mouse::Left && myMap.getClick()==0){
                    startPoint.x=event.mouseButton.x;
                    startPoint.y=event.mouseButton.y;
                    myMap.clickAdder();
                }
                if (event.mouseButton.button==sf::Mouse::Right && myMap.getClick()==1){
                    endPoint.x=event.mouseButton.x;
                    endPoint.y=event.mouseButton.y;
                    myMap.clickAdder();
                    gotStartEnd=!gotStartEnd;
                    std::cout<<"[LOG]: Start Point: ("<<startPoint.x<<" "<<startPoint.y<<")\n";
                    std::cout<<"[LOG]: End Point: ("<<endPoint.x<<" "<<endPoint.y<<")\n";
                }
            }
        }
        if(gotStartEnd){
        myMap.DrawStartEnd(startPoint,START_COLOR);
        myMap.DrawStartEnd(endPoint,END_COLOR);}
        myMap.mapWindow.display();



    }

}

void RRTPlanner::plan(){
    int iter_count=0;
    while(iter_count<iterations){
        Point random_point=chooseRandomPoint();
        
    }

}

Point RRTPlanner::chooseRandomPoint(){
    std::random_device xgen;
    std::mt19937 rngx(xgen());
    std::uniform_int_distribution<std::mt19937::result_type> distx(0,WINDOWSIZE_X);
    
    std::random_device ygen;
    std::mt19937 rngy(ygen());
    std::uniform_int_distribution<std::mt19937::result_type> disty(0,WINDOWSIZE_Y);
    Point rand_point(distx(rngx),disty(rngy));
    return rand_point;
}