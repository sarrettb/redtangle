#include <filesystem> 
#include "apps/redtangle_builder.h"

// Comment these out to disable options 
#define USE_GRPC
#define USE_IMGUI

#ifdef USE_GRPC
    static const GameType game_type = GameType::REMOTE;
#else 
    static const GameType game_type = GameType::TWO_PLAYER;
#endif 

#ifdef USE_IMGUI
    static const UIType ui_type = UIType::IMGUI;
#else 
    static const UIType ui_type = UIType::SDL;
#endif 

int main(int argc, char** argv) {   
    try {
            std::filesystem::path icon_path = std::filesystem::path(argv[0]).replace_filename("redtangle.png");  
            RedtangleBuilder builder;  
            builder.set_gameType(game_type);
            builder.set_UIType(ui_type); 
            builder.set_iconPath(icon_path.string()); 
            builder.build(); 
            auto game = builder.get_game();
            auto ui = builder.get_UI(); 
            bool quit = !builder.build_valid(); 
            while (!quit) {
                while (ui->poll_event()) {
                    switch (ui->get_eventType()) {
                        case redtangle::RedtangleUI::EventType::QUIT : {
                            quit = true;
                            break;
                        }
                        case redtangle::RedtangleUI::EventType::SELECTION : {
                            game->select(ui->get_location()); 
                            break;
                        }
                        case redtangle::RedtangleUI::EventType::ROTATION_CLOCKWISE : {
                            game->rotate(true);
                            break;
                        }
                        case redtangle::RedtangleUI::EventType::ROTATION_COUNTERCLOCKWISE : {
                            game->rotate(false); 
                            break;
                        }
                        case redtangle::RedtangleUI::EventType::END_TURN : {
                            game->end_turn(); 
                            break;
                        }
                    }
                }
                game->render_board(ui); 
            }
        }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl; 
        return EXIT_FAILURE; 
    }
    return EXIT_SUCCESS; 
}