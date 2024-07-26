#include <cstdlib> //abort
#include <cmath> // sin, cos
#include <iostream> //cerr

#include <SDL2/SDL.h>

#include "shapes.hpp"

using namespace ilrd;

// sudo apt-get install libsdl2-dev
// then, link with -lSDL2


//==============================================================================
// A useful macro to quick exit on SDL errors, with some diagnostics but without
// any cleanups. Think about some ways to better handle errors in the future...
//==============================================================================
#define ABORT_ON_SDL_ERROR(msg) \
do { \
    std::cerr << msg << ": " << SDL_GetError() << '\n'; \
    abort(); \
} \
while (0)

//==============================================================================

int main()
{
    //==========================================================================
    // init video (should be called at program start...)
    //==========================================================================
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        ABORT_ON_SDL_ERROR("SDL_Init");

    //==========================================================================
    // determine the screen size, to fit the display
    //==========================================================================
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) < 0)
        ABORT_ON_SDL_ERROR("SDL_GetCurrentDisplayMode");
    const int screen_width = dm.w;
    const int screen_height = dm.h;

    //==========================================================================
    // create windows and renderer objects, which are necessary for every SDL app
    //==========================================================================
    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_CreateWindowAndRenderer(screen_width, screen_height,
                                    SDL_WINDOW_RESIZABLE, &window, &renderer))
       ABORT_ON_SDL_ERROR("SDL_CreateWindowAndRenderer");

    
    //==========================================================================
    // Create objects
    //==========================================================================
    bool create_clone = false;
    bool create_group = false;
    
    // my circle
    Color base_cl1(255,0,0);
    Color base_cl2(210,60,35);
    Point my_cent1(screen_width / 2, screen_height / 2);
    Point my_cent2(screen_width / 3, screen_height / 3);
    
    int delta = 20;
    Color get_colr;
            
    Color try_cl(220,16,70);
    Color green_cl(0,255,0);
    Color blue_cl(0,0,255);
    
    Point delta_right(delta,0);
    Point delta_left(-delta,0);
    Point delta_down(0,-delta);
    Point delta_up(0,delta);
    
    Point delta_cross_up_right(delta,delta);
    Point delta_cross_down_left(-delta,delta);
    Point delta_cross_up_left(-delta,-delta);
    Point delta_cross_down_right(delta,-delta);
    
    Shape* circ1 = new Circle(renderer, base_cl1, my_cent1, 100);
    Shape* circ2 = new Circle(renderer, base_cl2, my_cent2, 200);
    Shape* circ3;
    
    // my triangle
    Color tr_cl(160, 68, 75);
    Point pt1(400,800);
    Point pt2(600,800);
    Point pt3(500,400);
    Shape* triang1 = new Triangle(renderer, tr_cl, pt1, pt2, pt3);
    
    // my rectangular
    Color rc_cl(200, 250, 255);
    Point pt1_rec(500,200);
    Point pt2_rec(700,200);
    Point pt3_rec(500,100);
    Point pt4_rec(700,100);
    Shape* rect1 = new Rectangular(renderer, tr_cl, pt1_rec, pt2_rec, pt3_rec, pt4_rec);
    
    // my group
    Group* group = new Group();
    group->add(circ1);
    group->add(circ2); 
    group->add(triang1);
    
    Group* group2 = group->clone();
    group2->add(rect1);
    
    
    std::vector<Shape*> all_shapes;
    all_shapes.push_back(circ1);
    all_shapes.push_back(circ2);
    all_shapes.push_back(triang1);
    all_shapes.push_back(rect1);
    //shapes.push_back(group);
    
    //==========================================================================
    // finally, the main event loop!
    //==========================================================================
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
                
            // handle keyboard events (optional)
            case SDL_KEYDOWN:
            {    
                switch(event.key.keysym.sym){
                    case SDLK_1:
                    {
                        circ1->set_color(try_cl);
                        break;
                    }
                    case SDLK_2:
                    {
                        circ1->set_color(green_cl);
                        break;
                    }
                    case SDLK_3:
                    {
                        circ1->set_color(blue_cl);
                        break;
                    }
                    case SDLK_4:
                    {
                        circ2->set_color(circ1->get_color());
                        break;
                    }
                    case SDLK_d:
                    {
                        circ1->erase();
                        break;
                    }
                    case SDLK_s:
                    {
                        circ1->resize(0.5);
                        break;
                    }
                    case SDLK_b:
                    {
                        circ1->resize(2);
                        break;
                    }
                    case SDLK_h:
                    {
                        group->move(delta_right);
                        break;
                    }
                    case SDLK_g:
                    {
                        group->move(delta_left);
                        break;
                    }
                    case SDLK_j:
                    {
                        create_group = true;
                        break;
                    }
                    case SDLK_k:
                    {
                        group->erase();
                        break;
                    }
                    case SDLK_l:
                    {
                        group->remove();
                        break;
                    }
                    case SDLK_o:
                    {
                        group2->clone();
                        break;
                    }
                    case SDLK_KP_6:
                    {
                        circ1->move(delta_right);
                        break;
                    }
                    case SDLK_KP_4:
                    {
                        circ1->move(delta_left);
                        break;
                    }
                    case SDLK_KP_5:
                    {
                        create_clone = true;
                        circ3 = circ1->clone();
                        break;
                    }
                    case SDLK_KP_8:
                    {
                        circ1->move(delta_down);
                        break;
                    }
                    case SDLK_KP_2:
                    {
                        circ1->move(delta_up);
                        break;
                    }
                    case SDLK_KP_9:
                    {
                        circ1->move(delta_cross_down_right);
                        break;
                    }
                    case SDLK_KP_1:
                    {
                        circ1->move(delta_cross_down_left);
                        break;
                    }
                    case SDLK_KP_3:
                    {
                        circ1->move(delta_cross_up_right);
                        break;
                    }
                    case SDLK_KP_7:
                    {
                        circ1->move(delta_cross_up_left);
                        break;
                    }
                }
                break;
            }    
            case SDL_KEYUP: break;
            
            // handle mouse events (optional)
            case SDL_MOUSEBUTTONDOWN: break;
            case SDL_MOUSEBUTTONUP: break;
            case SDL_MOUSEMOTION: break;
                
            default:
                std::cerr << "unknown event: " << event.type << '\n';
            }
            
            // refresh screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            //==================================================================
            // some actual drawing example
            //==================================================================

            // about colors: you can only change it globally, i.e. for the render
            // the first three numbers are RGB values.
            // the fourth is irrelevant...I have no idea what it does
            
            // try to model the color in a more "c++ way"
            
            //~ SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //~ draw_circle(renderer, screen_width / 2 + i, screen_height / 2+i, 100);
            //SDL_RenderClear(renderer);
            
            //~ SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            //~ draw_line(renderer, screen_width / 3, screen_height / 3, 2 * screen_width / 3, 2 * screen_height / 3);
            
            //~ SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            //~ draw_line(renderer, screen_width / 3, 2 * screen_height / 3, 2 * screen_width / 3, screen_height / 3);

            //draw circles
            //~ circ1->draw();
            //~ circ2->draw();
            //~ if(create_clone == true)
            //~ {
                //~ circ3->draw();
            //~ }
            
            //~ //draw triangles
            //~ triang1->draw();
            
            //draw rectangular
            //~ rect1->draw();
            
            //draw group
            //~ if(create_group == true)
            //~ {
                //~ group->draw();
            //~ }
            
            //group2->draw();
            for(size_t i = 0; i < all_shapes.size(); ++i){
                all_shapes[i]->draw();
            }
            
            
            
            //~ // square
            //~ SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
            //~ draw_line(renderer, 200, 250, 250, 250);
            //~ draw_line(renderer, 200, 300, 250, 300);
            //~ draw_line(renderer, 250, 250, 250, 300);
            //~ draw_line(renderer, 200, 250, 200, 300);

            //~ // rectangular
            //~ SDL_SetRenderDrawColor(renderer, 200, 250, 255, 255);
            //~ draw_line(renderer, 500, 200, 700, 200);
            //~ draw_line(renderer, 500, 200, 500, 100);
            //~ draw_line(renderer, 700, 200, 700, 100);
            //~ draw_line(renderer, 500, 100, 700, 100); 
            
            //~ // triangle
            //~ SDL_SetRenderDrawColor(renderer, 160, 68, 75, 255);
            //~ draw_line(renderer, 400, 800, 600, 800);
            //~ draw_line(renderer, 400, 800, 500, 400);
            //~ draw_line(renderer, 500, 400, 600, 800);
            
            
            SDL_RenderPresent(renderer);
        }
    }

    //==========================================================================
    // some final cleanups. how would you do it "the C++ way"?
    //==========================================================================   
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}
