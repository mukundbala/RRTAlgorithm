#include "map_old.hpp"

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
    GenerateRandomPoints();
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
    GenerateRandomPoints();
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
    GenerateRandomPoints();
    //todo: else for solid large obstacles
}

void Map::GenerateRandomPoints(){
    while (this->obstacle_positions.size()<num_obstacles){
        std::random_device xgen;
        std::mt19937 rngx(xgen());
        std::uniform_int_distribution<std::mt19937::result_type> distx(obs_size.x+1,windowSize_x-obs_size.x-1);
        
        std::random_device ygen;
        std::mt19937 rngy(ygen());
        std::uniform_int_distribution<std::mt19937::result_type> disty(obs_size.y+1,windowSize_y-obs_size.y-1);
        
        Point newPoint(distx(rngx),disty(rngy));
        if (this->obstacle_positions.find(newPoint)==this->obstacle_positions.end()){
            this->obstacle_positions.emplace(newPoint);
        }
    }
    std::cout<<"[LOG]: Window Size: "<<this->windowSize_x<<" "<<this->windowSize_y<<"\n";
    std::cout<<"[LOG]: Window Area: "<<this->windowSize_x*this->windowSize_y<<"\n";
    std::cout<<"[LOG]: Obstacle Area: "<<obs_size.x*obs_size.x<<"\n";
    std::cout<<"[LOG]: Free Space: "<<this->free_space_concentration<<"\n"; 
    std::cout<<"[LOG]: Number of Computed Obstacles: "<<num_obstacles<<"\n";
    std::cout<<"[LOG]: Number of Points Generated: " <<this->obstacle_positions.size()<<"\n";
    if (this->obstacle_positions.size() == num_obstacles){std::cout<<"[LOG]: Obstacle Points Created Successfully"<<"\n";}
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
    sf::CircleShape point_to_draw;
    point_to_draw.setRadius(2.0);
    point_to_draw.setFillColor(color);
    point_to_draw.setOrigin(point_to_draw.getRadius(),point_to_draw.getRadius());
    point_to_draw.setPosition(sf::Vector2f(pt.x,pt.y));
    mapWindow.draw(point_to_draw);
}
void Map::DrawStartEnd(Point pt, const sf::Color &color){
    sf::CircleShape point_to_draw;
    point_to_draw.setRadius(3.0);
    point_to_draw.setFillColor(color);
    point_to_draw.setOrigin(point_to_draw.getRadius(),point_to_draw.getRadius());
    point_to_draw.setPosition(sf::Vector2f(pt.x,pt.y));
    mapWindow.draw(point_to_draw);
}

void Map::DrawLine(Point &p1, Point &p2,const sf::Color &color ){
    sf::Vertex line[] =
    {
        sf::Vertex(PointtoV2f(p1),color),
        sf::Vertex(PointtoV2f(p2),color),
    };
    mapWindow.draw(line, 2, sf::Lines);
}

void Map::set_start(int x, int y){
    this->startPoint.x=x;
    this->startPoint.y=y;
}
void Map::set_end(int x, int y){
    this->endPoint.x=x;
    this->endPoint.y=y;
}
Point Map::get_start(){return this->startPoint;}
Point Map::get_end(){return this->endPoint;}
bool Map::getStartEnd(){return this->gotStartEnd;}
void Map::updateStartEnd(){gotStartEnd=!gotStartEnd;}

int main(){
    //temporary input method for debuggin only
    OBSTACLE_TYPE="scatter";
    WINDOWSIZE_X=1280;
    WINDOWSIZE_Y=1000;
    FREESPACE=0.9;
    //initialise windowObject
    Map myMap(WINDOWSIZE_X,WINDOWSIZE_Y,FREESPACE,OBSTACLE_TYPE);
    Map* map_ptr=&myMap;
    bool startPlan=0;
    while(myMap.mapWindow.isOpen()){
        myMap.mapWindow.clear(myMap.WINDOW_COLOR); //ready to draw;
        myMap.drawObstacles();
        sf::Event event;
        sf::Event mousevent;
        while(myMap.mapWindow.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                myMap.~Map();
            }
            if (event.type==sf::Event::MouseButtonPressed && myMap.getClick()<2){
                if (event.mouseButton.button==sf::Mouse::Left && myMap.getClick()==0){
                    myMap.set_start(event.mouseButton.x,event.mouseButton.y);
                    myMap.clickAdder();
                }
                if (event.mouseButton.button==sf::Mouse::Right && myMap.getClick()==1){
                    myMap.set_end(event.mouseButton.x,event.mouseButton.y);
                    myMap.clickAdder();
                    myMap.updateStartEnd();
                    std::cout<<"[LOG]: Start Point: ("<<myMap.get_start().x<<" "<<myMap.get_start().y<<")\n";
                    std::cout<<"[LOG]: End Point: ("<<myMap.get_end().x<<" "<<myMap.get_end().y<<")\n";
                }
            }
            if (event.type==sf::Event::KeyPressed && myMap.getStartEnd() && event.key.code==sf::Keyboard::Enter){
                startPlan=1;
            }
        }
        myMap.mapWindow.display();
        if (myMap.getStartEnd()){
            myMap.DrawStartEnd(myMap.get_start(),myMap.START_COLOR);
            myMap.DrawStartEnd(myMap.get_end(),myMap.END_COLOR);
            myMap.mapWindow.display();
        }

       if (startPlan){ //start planning on enter key!
            RRTPlanner rrtplanner(9.0,0.4,3000,myMap.get_start(),myMap.get_end(),map_ptr);
            rrtplanner.plan(); //planning starts
            if (myMap.getTerminate()){
                myMap.~Map();
            }
       }
    }
}

/*
#####################################################################################################
###################################### RRT PLANNER ##################################################
#####################################################################################################
*/
void RRTPlanner::plan(){
    int iter_count=0;
    planPrep();
    while(iter_count<iterations){
        std::cout<<"[LOG]: ITERATION "<<iter_count+1<<"\n";
        Point random_point=chooseRandomPoint();
        Point nearest_point=getNearestPoint(random_point);
        Point stepped_point=getSteppedPoint(random_point,nearest_point);
        bool stepped_point_validity=checkSteppedPoint(stepped_point,nearest_point);

        if (!stepped_point_validity){
            iter_count++;
            sf::sleep(sf::milliseconds(200));
            continue;}

        recordNewNode(stepped_point,nearest_point);
        goalReachedAction(stepped_point);
        pathTracebackAction();
        if (goal_reached){
            map_ptr->triggerTerminate();
            break;
        }
        iter_count++;
        sf::sleep(sf::milliseconds(100));
    }
}

void RRTPlanner::planPrep(){
    std::cout<<"[LOG]: Preparing RRT Planner\n";
    chosen_points.emplace(start_point); //we have already chosen the start point
    parent_array.emplace(start_point,Point(-1,-1));
    std::cout<<"[LOG]:RRT Planner Starting\n";
    sf::sleep(sf::milliseconds(1000)); 
}
Point RRTPlanner::chooseRandomPoint(){
    int rand_num=1+(rand()%100);
    if (rand_num<=100*bias){
        Point rand_point=end_point;
        return rand_point; //choose the end point as random
    }
    std::random_device xgen;
    std::mt19937 rngx(xgen());
    std::uniform_int_distribution<std::mt19937::result_type> distx(0,WINDOWSIZE_X);
    std::random_device ygen;
    std::mt19937 rngy(ygen());
    std::uniform_int_distribution<std::mt19937::result_type> disty(0,WINDOWSIZE_Y);
    Point rand_point(distx(rngx),disty(rngy));
    return rand_point;
}

Point RRTPlanner::getNearestPoint(Point &random_point){
    double closest_distance=sqrt(WINDOWSIZE_X*WINDOWSIZE_X + WINDOWSIZE_Y*WINDOWSIZE_Y)+1;
    Point nearest_point(-1,-1);
    for (auto& pt:chosen_points){
        Point dist_vec=pt-random_point;
        double mag=dist_vec.mag();
        if (mag<closest_distance){
            nearest_point=pt;
            closest_distance=mag;
        }
    }
    return nearest_point;
}
//returns false if our point fails, else returns true if all checks cleared
Point RRTPlanner::getSteppedPoint(Point &random_point, Point &nearest_point){
    sf::Vector2f randpt=map_ptr->PointtoV2f(random_point); 
    sf::Vector2f nearestpt=map_ptr->PointtoV2f(nearest_point);
    sf::Vector2f dist_vec=randpt-nearestpt;
    Point dist_vec_in_point=random_point-nearest_point;
    float mag=dist_vec_in_point.mag();
    sf::Vector2f direction_vector=dist_vec/mag;
    sf::Vector2f stepped_vector=direction_vector*(float)step;
    sf::Vector2f steppedpt=stepped_vector+nearestpt;
    Point stepped_point(steppedpt.x+0.5, steppedpt.y+0.5);
    return stepped_point;
}

bool RRTPlanner::checkSteppedPoint(Point &stepped_point, Point &nearest_point){
    //1. Check if stepped_point lies inside or close to (by 4 pixels) to an obstacle
    for (auto& obspt:map_ptr->obstacle_positions){
        if (stepped_point.x >= obspt.x-obstacle_tolerance &&
            stepped_point.x <= obspt.x+map_ptr->obs_size.x+obstacle_tolerance &&
            stepped_point.y >= obspt.y-obstacle_tolerance &&
            stepped_point.y <= obspt.y+map_ptr->obs_size.y+obstacle_tolerance)
        {
            return false; //our stepped point lies inside
        }
    }
    //2. Check if stepped_point is a point that has already been chosen
    for (auto& pt:chosen_points){
        if (pt==stepped_point){
            return false;
        }
    }
    //3. Check if line between stepped_point and nearest_point intersect
    for (auto& obspt:map_ptr->obstacle_positions){
        Point tl=obspt;
        Point tr(obspt.x+map_ptr->obs_size.x,obspt.y);
        Point bl(obspt.x,obspt.y+map_ptr->obs_size.y);
        Point br(obspt.x+map_ptr->obs_size.x,obspt.y+map_ptr->obs_size.y);
        int intersect_count=intersect(tl,tr,nearest_point,stepped_point)+
                            intersect(tr,br,nearest_point,stepped_point)+
                            intersect(tl,bl,nearest_point,stepped_point)+
                            intersect(bl,br,nearest_point,stepped_point);
        if (intersect_count>0){return false;}
    }
    return true; //all checks passed
    
}
int RRTPlanner::orientation(Point &p, Point &q, Point& r){
    int val=(q.y-p.y)*(r.x-q.x) - (q.x-p.x) * (r.y-q.y);
    if (val==0){return 0;} //collinear
    return (val>0)?1:2; //clock or counterclockwise
}
bool RRTPlanner::intersect(Point &rect1, Point & rect2, Point &nearest_point, Point & stepped_point){
    int o1=orientation(rect1,rect2,nearest_point);
    int o2=orientation(rect1,rect2,stepped_point);
    int o3=orientation(nearest_point,stepped_point,rect1);
    int o4=orientation(nearest_point,stepped_point,rect2);
    if (o1!=o2 && o3!=o4){
        return true;
    }
    return false;
}

void RRTPlanner::recordNewNode(Point &stepped_point, Point &nearest_point){
    chosen_points.emplace(stepped_point);
    parent_array.emplace(stepped_point,nearest_point);
    map_ptr->DrawPoint(stepped_point,map_ptr->POINT_COLOR);
    map_ptr->DrawLine(nearest_point,stepped_point,map_ptr->LINE_COLOR);
    map_ptr->mapWindow.display();
}

void RRTPlanner::goalReachedAction(Point& stepped_point){
    Point dist_to_end=end_point-stepped_point;
    sf::Vector2f dist_to_end_pt=map_ptr->PointtoV2f(dist_to_end);
    float mag=sqrt(dist_to_end_pt.x*dist_to_end_pt.x + dist_to_end_pt.y*dist_to_end_pt.y);
    if (mag<=this->goal_radius){
        this->goal_reached=true;
        chosen_points.emplace(end_point);
        parent_array.emplace(end_point,stepped_point);
        map_ptr->DrawLine(stepped_point,end_point,map_ptr->LINE_COLOR);
    }
    else return;
}
void RRTPlanner::pathTracebackAction(){
    if (this->goal_reached){
        Point currPoint=end_point;
        std::vector<Point> path;
        while(parent_array[currPoint].x!=-1){
            path.push_back(currPoint);
            currPoint=parent_array[currPoint];
        }
        path.push_back(currPoint);
        std::reverse(path.begin(),path.end());
        sf::sleep(sf::milliseconds(100));
        map_ptr->DrawStartEnd(end_point,map_ptr->END_COLOR);
        map_ptr->DrawStartEnd(start_point,map_ptr->START_COLOR);
        for (int i=0;i<path.size()-1;++i){
            map_ptr->DrawLine(path[i],path[i+1],map_ptr->TRACE_COLOR);
            map_ptr->mapWindow.display();
            sf::sleep(sf::milliseconds(100));
        }
        std::cout<<"[LOG]: PATH COMPLETED!"<<"\n";
        sf::sleep(sf::seconds(10.0)); //freeze the screen
        return;
    }
    return;
}