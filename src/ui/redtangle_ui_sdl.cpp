#include <SDL.h>
#include <SDL_image.h>
#include <math.h> 
#include <filesystem>
#include "ui/redtangle_ui_sdl.h"

using redtangle::Point; 
using redtangle::Circle; 
using redtangle::Rect; 
using redtangle::Color; 

bool in_circle(const SDL_Point& point, const Circle& circle); 
void add_circlePoints(std::vector<SDL_Point>& points, const Circle& circle); 
void add_filledCirclePoints(std::vector<SDL_Point>& points, const Circle& circle);
void insert_sdlError(); 
SDL_Color color_toSDLColor(const Color& color); 

// Create the window and renderer
RedtangleUI_SDL::RedtangleUI_SDL(const std::string& icon_path, int options) : RedtangleUI(DEFAULT_WIDTH, DEFAULT_HEIGHT), _icon(nullptr) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        insert_sdlError(); 
    }
    _window = SDL_CreateWindow("Redtangle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _window_width, _window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | options); 
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); 
    if (_window == nullptr || _renderer == nullptr) {
        insert_sdlError(); 
    }
    if (!icon_path.empty()) {
        if (std::filesystem::exists(icon_path)) {
            _icon = IMG_Load(icon_path.c_str()); 
            SDL_SetWindowIcon(_window, _icon); 
            if (_icon == nullptr) {
                insert_sdlError(); 
            }
        }
    }
}

bool RedtangleUI_SDL::poll_event(){
    return SDL_PollEvent(&_event);
}

redtangle::Location RedtangleUI_SDL::get_location() const {
    return { static_cast<int>(_event.button.x / _rect_width), static_cast<int>(_event.button.y / _rect_height) }; 
}

// Show the updated screen 
void RedtangleUI_SDL::show() {
    SDL_RenderPresent(_renderer); 
}

// Clear the screen 
void RedtangleUI_SDL::clear() { 
    SDL_SetRenderDrawColor(_renderer, 240, 240, 240, 255);
    SDL_RenderClear(_renderer); 
}

// Actions to perform on window resize event 
void RedtangleUI_SDL::resizeWindow() {
    SDL_GetWindowSize(_window, &_window_width, &_window_height); 
    _redtangle_surface.w = _window_width;
    _redtangle_surface.h = _window_height; 
    coerce_redtangleSurface(); 
    SDL_SetWindowSize(_window, _redtangle_surface.w, _redtangle_surface.h); 
}

redtangle::RedtangleUI::EventType RedtangleUI_SDL::get_eventType() {
    _debugging = false; 
    switch (_event.type) {
        case SDL_QUIT:
            return redtangle::RedtangleUI::EventType::QUIT; 
        case SDL_MOUSEBUTTONDOWN:
            if (_event.button.button == SDL_BUTTON_LEFT) {
                return RedtangleUI::EventType::SELECTION; 
            }
            else if (_event.button.button == SDL_BUTTON_RIGHT) {
                return RedtangleUI::EventType::END_TURN; 
            }
            else {
                return RedtangleUI::EventType::UNKNOWN; 
            }
        case SDL_MOUSEWHEEL:
            return _event.wheel.y > 0 ? RedtangleUI::EventType::ROTATION_CLOCKWISE : RedtangleUI::EventType::ROTATION_COUNTERCLOCKWISE;
        case SDL_WINDOWEVENT: {
            if (_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                SDL_GetWindowSize(_window, &_window_width, &_window_height); 
                resizeWindow();
            }
            return RedtangleUI::EventType::UI_EVENT; 
        }
        default:
            return RedtangleUI::EventType::UNKNOWN;
    }
}

// Render a filled rectangle with given color 
void RedtangleUI_SDL::render_filledRect(const Rect& rect, const Color& color) const {
    SDL_Rect sdl_rect = { rect.top_left.x, rect.top_left.y, rect.w, rect.h }; 
    SDL_SetRenderDrawColor(_renderer, color.red, color.green, color.blue, 255); 
    SDL_RenderFillRect(_renderer, &sdl_rect); 
}

// Render a filled circle with given color 
void RedtangleUI_SDL::render_filledCircle(const redtangle::Circle& circle, const redtangle::Color& color) const {
    std::vector<SDL_Point> points;
    add_filledCirclePoints(points, circle); // adds points on the edge of the circle
    add_circlePoints(points, circle); // adds points inside the edge
    SDL_SetRenderDrawColor(_renderer, color.red, color.green, color.blue, 255); 
    SDL_RenderDrawPoints(_renderer, points.data(), points.size());
}

// Renders a filled triangle with given color 
void RedtangleUI_SDL::render_filledTriangle(const std::vector<Point>& vertices, const Color& color) const  {
    SDL_Color c = color_toSDLColor(color); 
    SDL_Vertex verts[3] = {
                            { SDL_FPoint{ (float) vertices[0].x, (float) vertices[0].y }, c, SDL_FPoint{ 0 } },
                            { SDL_FPoint{ (float) vertices[1].x, (float) vertices[1].y }, c, SDL_FPoint{ 0 } },
                            { SDL_FPoint{ (float) vertices[2].x, (float) vertices[2].y }, c, SDL_FPoint{ 0 } }
                          };
    SDL_RenderGeometry(_renderer, nullptr, verts, 3, nullptr, 0); 
}

// Destroy UI 
RedtangleUI_SDL::~RedtangleUI_SDL() {
  SDL_DestroyRenderer(_renderer); 
  SDL_DestroyWindow(_window);
  SDL_FreeSurface(_icon); 
  SDL_Quit(); 
}

// return true if the point lies on or inside the circle
bool in_circle(const SDL_Point& point, const Circle& circle) {
    return pow(point.x - circle.center.x, 2) + pow(point.y - circle.center.y, 2) - circle.r * circle.r <= 0; 
}

// Adapted mid-point circle drawing algorithm for SDL2
// Source: https://www.geeksforgeeks.org/mid-point-circle-drawing-algorithm/
// Detailed Algorithm Explanation: https://sachithradangalla.medium.com/mid-point-algorithm-how-to-draw-a-circle-b6ed7c47c803 
void add_circlePoints(std::vector<SDL_Point>& points, const Circle& circle) {
    int x = circle.r;
    int y = 0;

    if (circle.r > 0) {
        points.push_back({x + circle.center.x, y + circle.center.y}); 
        points.push_back({y + circle.center.x, x + circle.center.y}); 
        points.push_back({-y + circle.center.x, x + circle.center.y}); 
    }
     
    // Initialize the value of P
    int P = 1 - circle.r;
    while (x > y)
    {
        y++;
         
        // Mid-point is inside or on the perimeter
        if (P <= 0) {
            P = P + 2*y + 1;
        }
        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2*y - 2*x + 1;
        }
         
        // All the perimeter points have already been printed
        if (x < y) {
            break;
        }
         
        // Generated points and its reflection in the other octants after translation
        points.push_back({x + circle.center.x, y + circle.center.y});
        points.push_back({-x + circle.center.x, y + circle.center.y});
        points.push_back({x + circle.center.x, -y + circle.center.y});
        points.push_back({-x + circle.center.x, -y + circle.center.y});
         
        // If the generated point is on the line x = y then the perimeter points have already been generated
        if (x != y) {
            points.push_back({y + circle.center.x, x + circle.center.y});
            points.push_back({-y + circle.center.x, x + circle.center.y});
            points.push_back({y + circle.center.x, -x + circle.center.y});
            points.push_back({-y + circle.center.x, -x + circle.center.y});
        }
    }
}

// Traverses a rectangle bounded by a circle. 
// If the point is on or inside the circle, then add it to the list of points
void add_filledCirclePoints(std::vector<SDL_Point>& points, const Circle& circle) {
   for (int y = circle.center.y - circle.r; y <= circle.center.y + circle.r; y++) {
       for (int x = circle.center.x - circle.r; x <= circle.center.x + circle.r; x++) {
            SDL_Point point = {x, y}; 
            if (in_circle(point, circle)) {
                points.push_back(point); 
            }
        }
   }
}

// Converts redtangle::Color to SDL_Color
SDL_Color color_toSDLColor(const Color& color) {
    return { static_cast<Uint8>(color.red), static_cast<Uint8>(color.green), static_cast<Uint8>(color.blue), 255 }; 
}

void insert_sdlError() {
    throw std::runtime_error(SDL_GetError()); 
}