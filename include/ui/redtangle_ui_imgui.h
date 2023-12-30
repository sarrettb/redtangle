#pragma once
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "redtangle_ui_sdl.h"

struct RemotePopupInfo {
    std::string user_name;
    std::string ip_address;
    int port;
    bool cancelled; 
}; 

// Extend the RedtangleUI_SDL to provide a status message and menu options 
class RedtangleUI_Imgui : public RedtangleUI_SDL {
    protected: 
        redtangle::Rect _menuBar_surface;  
        redtangle::Rect _statusBar_surface;
        std::string _status; 
        ImGuiIO _io;  
        void calculate_surfaces(); 
        void render_menuBar();
        void render_statusBar();
        void resizeWindow() override;  
        bool popup(char* user_name, size_t user_nameSize, char* ip_address, size_t ip_addressSize, int* port,  bool* cancelled); 
    public:
        RedtangleUI_Imgui(const std::string& icon_path = "");
        void InitBoard(); 
        void set_status(const std::string& status) override { _status = status; }
        bool poll_event() override;   
        void show() override; 
        RemotePopupInfo RemotePopup(); 
};