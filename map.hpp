#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include<iostream>
#include <unordered_set>
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
        if (this->x == otherPoint.x && this->y==otherPoint.y) return true;
        else return false;
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
    void GenerateRandomPoints(std::unordered_set<Point,Point::HashFunction>& obsPos); //number of points
public:
    Map();
    Map(int windowX, int windowY,double freeSpace); //constructor
    Map(int windowX, int windowY,double freeSpace,std::string obs_type);
    ~Map(){mapWindow.close();}
    sf::RenderWindow mapWindow;
    void drawObstacles(); //drawing the obstacles on the screen
    void DrawPoint(Point pt, const sf::Color &color);
    void DrawStartEnd(Point pt, const sf::Color &color);
    Point V2ftoPoint(sf::Vector2f &pt){
        Point mypoint(pt.x,pt.y);
        return mypoint;
    }
    sf::Vector2f PointtoV2f(Point &pt){
        sf::Vector2f myvec(pt.x,pt.y);
        return myvec;
    }
    //tracking clicks
    int getClick(){return clickCount;}
    void clickAdder(){this->clickCount ++;}
    
};


//fixed map variables
const sf::Vector2f obs_size={20.0,20.0};
const sf::Color OBS_COLOR=sf::Color::Red;
const sf::Color LINE_COLOR=sf::Color::White;
const sf::Color POINT_COLOR=sf::Color::Cyan;
const sf::Color WINDOW_COLOR=sf::Color::Black;
const sf::Color START_COLOR=sf::Color::Green;
const sf::Color END_COLOR=sf::Color::Magenta;

std::unordered_set<Point,Point::HashFunction> obstacle_positions;
std::string OBSTACLE_TYPE;
int WINDOWSIZE_X,WINDOWSIZE_Y;
double FREESPACE;

Point startPoint;
Point endPoint;
bool gotStartEnd=0;



