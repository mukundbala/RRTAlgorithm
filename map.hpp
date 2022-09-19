#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include<iostream>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <memory>
struct Point{
    int x;
    int y;
    Point():x(0),y(0){}
    Point(int x, int y){
        this->x=x;
        this->y=y;
    }
    bool operator == (const Point& otherPoint)const{
        if (x==otherPoint.x && y==otherPoint.y){return true;}
        return false;
    }
    Point operator -(Point &rhs)const{
        Point ans;
        ans.x=x-rhs.x;
        ans.y=y-rhs.y;
        return ans;
    }
    Point operator +(Point& rhs)const{
        Point ans;
        ans.x=x+rhs.x;
        ans.y=y+rhs.y;
        return ans;
    }
    double mag(){
        return sqrt(x*x + y*y);
    }
    struct HashFunction{
        std::size_t operator() (const Point& point)const
        {
            std::size_t xHash=std::hash<int>()(point.x);
            std::size_t yHash=std::hash<int>()(point.y) << 1;
            return xHash ^ yHash;
        }
    };

};

class Map {    
private:
    int windowSize_x;
    int windowSize_y;
    double free_space_concentration;
    std::string obstacle_type; //"scatter" vs "big"
    int num_obstacles;
    int clickCount=0;
    Point startPoint;
    Point endPoint;
    bool gotStartEnd=0;
    bool terminate=0;
    void GenerateRandomPoints(); //number of points
public:
    Map();
    Map(int windowX, int windowY,double freeSpace); //constructor
    Map(int windowX, int windowY,double freeSpace,std::string obs_type);
    ~Map(){mapWindow.close();}
    sf::RenderWindow mapWindow;
    void drawObstacles(); //drawing the obstacles on the screen
    void DrawPoint(Point pt, const sf::Color &color);
    void DrawStartEnd(Point pt, const sf::Color &color);
    void DrawLine(Point &p1, Point &p2, const sf::Color &color);

    Point V2ftoPoint(sf::Vector2f &pt){
        Point mypoint(pt.x,pt.y);
        return mypoint;
    }
    sf::Vector2f PointtoV2f(Point &pt){
        sf::Vector2f myvec(pt.x,pt.y);
        return myvec;
    }
    void set_start(int x, int y);
    void set_end(int x, int y);
    Point get_start();
    Point get_end();

    void updateStartEnd();
    bool getStartEnd();

    //tracking clicks
    int getClick(){return clickCount;}
    void clickAdder(){this->clickCount ++;}

    bool getTerminate(){return terminate;}
    void triggerTerminate(){terminate=true;}
    //public variables
    std::unordered_set<Point,Point::HashFunction> obstacle_positions;

    const sf::Vector2f obs_size={20.0,20.0};
    const sf::Color OBS_COLOR=sf::Color::Red;
    const sf::Color LINE_COLOR=sf::Color::White;
    const sf::Color POINT_COLOR=sf::Color::Cyan;
    const sf::Color WINDOW_COLOR=sf::Color::Black;
    const sf::Color START_COLOR=sf::Color::Green;
    const sf::Color END_COLOR=sf::Color::Magenta;
    const sf::Color TRACE_COLOR=sf::Color::Yellow;
};

/*
RRT Function definitions
*/

class RRTPlanner{
private:
    double step;
    double bias;
    double goal_radius;
    int iterations;
    Point start_point;
    Point end_point;
    Map *map_ptr;
    double obstacle_tolerance;
    bool goal_reached;
    std::unordered_set<Point,Point::HashFunction> chosen_points;
    std::unordered_map<Point,Point,Point::HashFunction> parent_array; //point:parent
public:
    //constructor:
    RRTPlanner(double step,double bias, int iterations,Point start, Point end, Map* mapptr)
    :
    step(step),
    bias(bias),
    goal_radius(10.0),
    iterations(iterations),
    start_point(start),
    end_point(end),
    map_ptr(mapptr), 
    obstacle_tolerance(4),
    goal_reached(false){std::cout<<"[LOG]: Initializing RRT Planner\n";}
    
    Point chooseRandomPoint(); //chooses random points
    Point getNearestPoint(Point &stepped_point);
    Point getSteppedPoint(Point &random_point, Point &nearest_point);
    bool checkSteppedPoint(Point &stepped_point, Point &nearest_point);
    int orientation(Point &p, Point &q, Point& r); //returns 0 if collinear, 1 if clockwise, 2 if anticlockwise
    bool intersect(Point &rect1, Point & rect2, Point &nearest_point, Point & stepped_point);

    void planPrep();
    virtual void plan(); //plan() varies for RRT, RRT* and AnytimeRRT which inherit from RRT
};





//fixed map variables
//std::unordered_set<Point,Point::HashFunction> obstacle_positions;
std::string OBSTACLE_TYPE;
int WINDOWSIZE_X,WINDOWSIZE_Y;
double FREESPACE;
