#include "apps/redtangle_builder.h"
#include "game/redtangle.h"
#include "game/redtangle_remote.h"
#include "ui/redtangle_ui_sdl.h"
#include "ui/redtangle_ui_imgui.h"

RedtangleBuilder::RedtangleBuilder(GameType game_type, UIType ui_type) {
    set_gameType(game_type);
    set_UIType(ui_type); 
}

void RedtangleBuilder::build() {
    if (_game_type == GameType::REMOTE && _ui_type == UIType::IMGUI) {
        std::shared_ptr<RedtangleUI_Imgui> imgui_ui = std::make_shared<RedtangleUI_Imgui>(_icon_path);
        RemotePopupInfo info = imgui_ui->RemotePopup(); 
        if (info.cancelled) {
            return; // cancelled
        }
        auto remote_game = std::make_shared<RedtangleRemote>(info.user_name, info.ip_address, info.port);
        imgui_ui->InitBoard(); 
        remote_game->render_board(imgui_ui); 
        _game = remote_game;
        _ui = imgui_ui; 
    }
    else if (_game_type == GameType::TWO_PLAYER && _ui_type == UIType::IMGUI) {
        std::cout << "Creating game locally with IMGUI\n"; 
        std::shared_ptr<RedtangleUI_Imgui> imgui_ui = std::make_shared<RedtangleUI_Imgui>(_icon_path);
        imgui_ui->InitBoard();  
        _ui = imgui_ui; 
         _game = std::make_shared<redtangle::RedtangleGame>();
    }
    else {
         _game = std::make_shared<redtangle::RedtangleGame>();
        _ui = _ui_type == UIType::SDL ? std::make_shared<RedtangleUI_SDL>() : std::make_shared<RedtangleUI_Imgui>(_icon_path);
    }
}