#include <thread> 
#include "ui/redtangle_ui_imgui.h"

using redtangle::Point; 

static const int STATUS_BAR_HEIGHT = 50;
static const int BORDER_WIDTH = 2; 
static const std::string DIRECTIONS_URL = "https://github.com/sarrettb/dev/blob/main/redtangle/docs/directions.pdf";
static const std::string CONTROLS_URL = "https://github.com/sarrettb/dev/blob/main/redtangle/docs/controls.md";
static const int POPUP_WIDTH = 340;
static const int POPUP_HEIGHT = 109; 

#ifdef _WIN32
    static const std::string CMD = "start "; 
#else 
    static const std::string CMD = "xdg-open "; 
#endif 

RedtangleUI_Imgui::RedtangleUI_Imgui(const std::string& icon_path) :  RedtangleUI_SDL(icon_path), _menuBar_surface({{0, 0}, 0, 0}), _statusBar_surface({{0, 0}, 0, 0}) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    _io = ImGui::GetIO();
    _io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    _io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsLight(); 
    ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
    ImGui_ImplSDLRenderer2_Init(_renderer);  
    SDL_HideWindow(_window); 
}

void RedtangleUI_Imgui::InitBoard() {
    ImGui::StyleColorsLight(); 
    SDL_SetWindowSize(_window, DEFAULT_WIDTH, DEFAULT_HEIGHT); 
    SDL_ShowWindow(_window); 
    calculate_surfaces(); 
}

void RedtangleUI_Imgui::calculate_surfaces() {
    // Call show to update width height of surfaces
    // This is the only known way to get the height of the menu bar
    show();
    SDL_GetWindowSize(_window, &_window_width, &_window_height);
    _redtangle_surface = { BORDER_WIDTH, 
                           _menuBar_surface.top_left.y + _menuBar_surface.h + 1, 
                           _window_width - BORDER_WIDTH + 1, 
                           _window_height - _menuBar_surface.h - STATUS_BAR_HEIGHT
                         }; 
    coerce_redtangleSurface(); 
    SDL_SetWindowSize(_window, 2 * BORDER_WIDTH + _redtangle_surface.w, _window_height); 
    SDL_GetWindowSize(_window, &_window_width, &_window_height);
    _statusBar_surface = {0, 
                          _redtangle_surface.top_left.y + _redtangle_surface.h + 1,
                          _window_width,
                          _window_height - (_redtangle_surface.top_left.y + _redtangle_surface.h)
                         };
}

bool RedtangleUI_Imgui::poll_event() {
    bool result = RedtangleUI_SDL::poll_event(); 
    ImGui_ImplSDL2_ProcessEvent(&_event);
    return result;
}

void RedtangleUI_Imgui::show() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame(); 
    render_menuBar();
    render_statusBar(); 
    ImGui::Render();
    SDL_RenderSetScale(_renderer, _io.DisplayFramebufferScale.x, _io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(_renderer); 
}

void RedtangleUI_Imgui::render_menuBar() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Redtangle Directions")) {
                system(std::string(CMD + DIRECTIONS_URL).c_str());
            }
            if (ImGui::MenuItem("Redtangle Controls")) {
                system(std::string(CMD + CONTROLS_URL).c_str());
            } 
            ImGui::EndMenu();
        }
        auto size = ImGui::GetWindowSize();
        _menuBar_surface.w = size.x;
        _menuBar_surface.h = size.y;
        ImGui::EndMainMenuBar();
    }
}

void RedtangleUI_Imgui::render_statusBar() {
    bool is_open = false;
    ImGui::Begin("Redtangle Status", &is_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);   
    ImGui::SetWindowPos( { static_cast<float>(_statusBar_surface.top_left.x), static_cast<float>(_statusBar_surface.top_left.y) });  
    ImGui::SetWindowSize({ static_cast<float>(_statusBar_surface.w), static_cast<float>(_statusBar_surface.h) }); 
    ImGui::Text(_status.c_str());
    ImGui::End();
}

void RedtangleUI_Imgui::resizeWindow() {
    calculate_surfaces(); 
}

// Return true when user selects 'OK' or 'Cancel'
bool RedtangleUI_Imgui::popup(char* user_name, size_t user_nameSize, char* ip_address, size_t ip_addressSize, int* port, bool* cancelled) {  
    ImGui::OpenPopup("Popup");
    bool result = false;
    if (ImGui::BeginPopupModal("Popup", NULL, ImGuiWindowFlags_NoTitleBar)) {
        ImGui::SetWindowPos({0, 0}); 
        ImGui::SetWindowSize({POPUP_WIDTH, POPUP_HEIGHT}); 
        ImGui::SetItemDefaultFocus();
        ImGui::Text("User Name:");
        ImGui::SameLine(); 
        ImGui::InputText("##user_name", user_name, user_nameSize);
        ImGui::Text("IP Address:");
        ImGui::SameLine(); 
        ImGui::InputText("##ip_address", ip_address, ip_addressSize);
        ImGui::Text("Port:");
        ImGui::SameLine(); 
        ImGui::InputInt("##port", port); 
        ImGui::Separator();
        if (ImGui::Button("Connect")) {
            ImGui::CloseCurrentPopup();
            result = true; 
        }
        ImGui::SameLine(); 
        if(ImGui::Button("Cancel")) { 
            ImGui::CloseCurrentPopup(); 
            result = true;
            *cancelled = true;
        }
        ImGui::EndPopup();
    }
    return result;
}

// Blocking function to display popup for user to input parameters to connect to remote server
RemotePopupInfo RedtangleUI_Imgui::RemotePopup() {
    const int BUFFER_SIZE = 100;
    char user_name[BUFFER_SIZE] = ""; 
    char ip_address[BUFFER_SIZE] = "18.223.134.77"; // ip address of AWS Server
    int port = 50052; // port used by AWS Server 
    bool done = false;
    bool cancelled = false;
    SDL_SetWindowSize(_window, POPUP_WIDTH, POPUP_HEIGHT); 
    SDL_ShowWindow(_window); 
    ImGui::StyleColorsDark(); 
    while (!done) {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame(); 
        done = popup(user_name, BUFFER_SIZE, ip_address, BUFFER_SIZE, &port, &cancelled);
        ImGui::Render();
        SDL_RenderSetScale(_renderer, _io.DisplayFramebufferScale.x, _io.DisplayFramebufferScale.y);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(_renderer); 
        while (poll_event()) {
            if (get_eventType() == redtangle::RedtangleUI::EventType::QUIT) {
                cancelled = true; 
                done = true;
                break; 
            }
         }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    SDL_HideWindow(_window); 
    return { user_name, ip_address, port, cancelled }; 
}
