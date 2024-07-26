#ifndef SHAPES_HPP_
#define SHAPES_HPP_

#include <cstddef> // size_t
#include <iosfwd>  // istream, ostream
#include <vector>
#include <SDL2/SDL.h>

#if __cplusplus < 201103L
#define nullptr 0
#endif //__cplusplus < 201103L

namespace ilrd{

struct Color
{
    Color(int r=100, int g=150, int b=250): m_r(r), m_g(g), m_b(b){}
    int m_r;
    int m_g;
    int m_b;
};

struct Point
{
    Point(int x=0, int y=0): m_x(x), m_y(y){}
    int m_x;
    int m_y;
};

//Abstract base class
class Shape{
public:
    explicit Shape(SDL_Renderer* renderer = nullptr, Color cl = Color(), bool show = true);
    virtual ~Shape() {}
    
    virtual Shape* clone()=0;
    virtual void move(Point delta)=0;
    virtual void resize(double factor)=0;
    virtual void erase()=0;
    virtual void draw()=0;
    
    void set_color(Color cl);
    Color get_color();
    
protected:
    Color m_color;
    SDL_Renderer* m_renderer;
    bool m_show;

private:    
    Shape(const Shape& other); // disable cctor
    Shape& operator=(const Shape& other); // disable assignment operator
};


class Circle: public Shape{
public:
    Circle(SDL_Renderer* renderer, Color cl, Point center, float radius);
   ~Circle() {};
   
    Shape* clone();
    void move(Point delta);
    void erase();
    void draw();
    void resize(double factor);
private:
    Point m_center;
    float m_radius;
    
    void draw_circle(SDL_Renderer* renderer, int x_cent, int y_cent, float radius);
};

class Triangle: public Shape{
public:
    Triangle(SDL_Renderer* renderer, Color cl, Point pt1, Point pt2, Point pt3);
   ~Triangle() {};
   
    Shape* clone();
    void move(Point delta);
    void erase();
    void draw();
    void resize(double factor) {}
    
private:
    Point m_pt1;
    Point m_pt2;
    Point m_pt3;
    
    void draw_triangle(SDL_Renderer* renderer, Point pt1, Point pt2, Point pt3);
};

class Rectangular: public Shape{
public:
    Rectangular(SDL_Renderer* renderer, Color cl, Point pt1, Point pt2, Point pt3, Point pt4);
   ~Rectangular() {};
   
    Shape* clone();
    void move(Point delta);
    void erase();
    void draw();
    void resize(double factor) {}
    
private:
    Point m_pt1;
    Point m_pt2;
    Point m_pt3;
    Point m_pt4;
    
    void draw_rectangular(SDL_Renderer* renderer, Point pt1, Point pt2, Point pt3, Point pt4);
};

class Group: public Shape{
public:
     Group() {};
    ~Group() {};
    
    void add(Shape *shape);
    void remove();

    Group* clone();
    void move(Point delta);
    void erase();
    void draw();
    void resize(double factor) {} // not implemented

private:
    std::vector<Shape*> shapes;
};

//~ class Square: public Shape{
//~ public:

//~ private:
    //~ Point points[4];

//~ };

Shape::Shape(SDL_Renderer* renderer, Color cl, bool show): m_color(cl), m_renderer(renderer), m_show(show){}

void Shape::set_color(Color cl)
{
   m_color = cl;
}

Color Shape::get_color()
{
    return m_color;
}

Shape* Circle::clone()
{
    Point new_center(m_center.m_x, m_center.m_y);
    return new Circle(m_renderer, get_color(), new_center, m_radius);
}

Circle::Circle(SDL_Renderer* renderer, Color cl, Point center, float radius): 
               Shape(renderer, cl), m_center(center), m_radius(radius){}

void Circle::erase()
{
    m_show = false;
}

void Circle::draw()
{
    if(m_show)
    {
        SDL_SetRenderDrawColor(m_renderer, m_color.m_r, m_color.m_g, m_color.m_b, 255);
        draw_circle(m_renderer, m_center.m_x, m_center.m_y, m_radius);
    }
}

void Circle::resize(double factor)
{
    m_radius *= factor;
}

void Circle::move(Point delta)
{
    m_center.m_x += delta.m_x;
    m_center.m_y += delta.m_y;
}

void Circle::draw_circle(SDL_Renderer* renderer, int x_cent, int y_cent, float radius)
{
    const float step = 1 / radius;

    for (float theta = 0; theta < 360; theta += step)
       SDL_RenderDrawPoint(renderer, x_cent + radius * cos(theta),
                                     y_cent + radius * sin(theta));
}

Rectangular::Rectangular(SDL_Renderer* renderer, Color cl, Point pt1, Point pt2, Point pt3, Point pt4):
                        Shape(renderer, cl), m_pt1(pt1), m_pt2(pt2), m_pt3(pt3), m_pt4(pt4) {}

Shape* Rectangular::clone()
{
    return new Rectangular(m_renderer, get_color(), m_pt1, m_pt2, m_pt3, m_pt4);
}

void Rectangular::move(Point delta)
{
    m_pt1.m_x += delta.m_x;
    m_pt1.m_y += delta.m_y;
    
    m_pt2.m_x += delta.m_x;
    m_pt2.m_y += delta.m_y;
    
    m_pt3.m_x += delta.m_x;
    m_pt3.m_y += delta.m_y;
    
    m_pt4.m_x += delta.m_x;
    m_pt4.m_y += delta.m_y;
}

void Rectangular::erase()
{
    m_show = false;
}

void Rectangular::draw()
{
    if(m_show)
    {
        SDL_SetRenderDrawColor(m_renderer, m_color.m_r, m_color.m_g, m_color.m_b, 255);
        draw_rectangular(m_renderer, m_pt1, m_pt2, m_pt3, m_pt4);
    }
} 

void Rectangular::draw_rectangular(SDL_Renderer* renderer, Point pt1, Point pt2, Point pt3, Point pt4)
{
    SDL_RenderDrawLine(renderer, pt1.m_x, pt1.m_y, pt2.m_x, pt2.m_y);
    SDL_RenderDrawLine(renderer, pt1.m_x, pt1.m_y, pt3.m_x, pt3.m_y);
    SDL_RenderDrawLine(renderer, pt2.m_x, pt2.m_y, pt4.m_x, pt4.m_y);
    SDL_RenderDrawLine(renderer, pt3.m_x, pt3.m_y, pt4.m_x, pt4.m_y);
}
            
Triangle::Triangle(SDL_Renderer* renderer, Color cl, Point pt1, Point pt2, Point pt3):
                   Shape(renderer, cl), m_pt1(pt1), m_pt2(pt2), m_pt3(pt3) {}

void Triangle::draw()
{
    if(m_show)
    {
        SDL_SetRenderDrawColor(m_renderer, m_color.m_r, m_color.m_g, m_color.m_b, 255);
        draw_triangle(m_renderer, m_pt1, m_pt2, m_pt3);
    }
}

void Triangle::draw_triangle(SDL_Renderer* renderer, Point pt1, Point pt2, Point pt3)
{
    SDL_RenderDrawLine(renderer, pt1.m_x, pt1.m_y, pt2.m_x, pt2.m_y);
    SDL_RenderDrawLine(renderer, pt1.m_x, pt1.m_y, pt3.m_x, pt3.m_y);
    SDL_RenderDrawLine(renderer, pt3.m_x, pt3.m_y, pt2.m_x, pt2.m_y);
}

void Triangle::erase()
{
    m_show = false;
}

Shape* Triangle::clone()
{
    return new Triangle(m_renderer, get_color(), m_pt1, m_pt2, m_pt3);
}

void Triangle::move(Point delta)
{
    m_pt1.m_x += delta.m_x;
    m_pt1.m_y += delta.m_y;
    
    m_pt2.m_x += delta.m_x;
    m_pt2.m_y += delta.m_y;
    
    m_pt3.m_x += delta.m_x;
    m_pt3.m_y += delta.m_y;
}

void Group::add(Shape *shape)
{
    shapes.push_back(shape);
}

void Group::remove()
{
    shapes.pop_back();
}

void Group::move(Point delta)
{
    for(size_t i = 0; i < shapes.size(); ++i){
        shapes[i]->move(delta);
    }
}

void Group::draw()
{
    if(m_show)
    {
        for(size_t i = 0; i < shapes.size(); ++i){
            shapes[i]->draw();
        }
    }
}

Group* Group::clone()
{
    Group* new_group = new Group();
    
    for(size_t i = 0; i < shapes.size(); ++i){
        new_group->add(shapes[i]->clone());
    }
    return new_group;
}

void Group::erase()
{
    m_show = false;
}

} // namespace ilrd

#endif // SHAPES_HPP_
