#pragma once
#include "game/redtangle.h"

enum class GameType {
    TWO_PLAYER = 0,
    REMOTE =  1
}; 

enum class UIType {
    SDL = 0,
    IMGUI = 1
};

class RedtangleBuilder{
    protected:
        std::shared_ptr<redtangle::Redtangle> _game;
        std::shared_ptr<redtangle::RedtangleUI> _ui;
        std::string _icon_path;
        GameType _game_type; 
        UIType _ui_type; 
    public:
        RedtangleBuilder(GameType game_type = GameType::TWO_PLAYER, UIType ui_type = UIType::SDL);
        void build(); 
        bool build_valid() const {return _game != nullptr && _ui != nullptr; }
        void set_gameType(const GameType& game_type) { _game_type = game_type; }
        void set_UIType(const UIType& ui_type) { _ui_type = ui_type; }
        void set_iconPath(const std::string& icon_path) {_icon_path = icon_path; }
        std::shared_ptr<redtangle::Redtangle> get_game() const { return _game; }
        std::shared_ptr<redtangle::RedtangleUI> get_UI() const { return _ui; }

}; 