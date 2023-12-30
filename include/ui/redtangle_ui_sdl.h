#pragma once
#include <SDL.h>
#include "game/redtangle.h"

#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 800


// Redtangle UI implemenation using the SDL library 
class RedtangleUI_SDL : public redtangle::RedtangleUI {
    protected:
        SDL_Window* _window; 
        SDL_Renderer* _renderer; 
        SDL_Surface* _icon; 
        SDL_Event _event; 
        virtual void resizeWindow(); 
    public: 
        RedtangleUI_SDL(const std::string& icon_path = "", int options = 0); 
        ~RedtangleUI_SDL() override; 
        RedtangleUI::EventType get_eventType() override;
        redtangle::Location get_location() const override; 
        bool poll_event() override; 
        void show() override; 
        void set_status(const std::string& str) override {}
        void clear() override; 
        void render_filledRect(const redtangle::Rect& rect, const redtangle::Color& color) const override;
        void render_filledCircle(const redtangle::Circle& circle, const redtangle::Color& color) const override;
        void render_filledTriangle(const std::vector<redtangle::Point>& vertices, const redtangle::Color& color) const override;
}; 