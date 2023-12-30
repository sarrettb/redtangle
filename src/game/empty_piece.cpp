#include "game/redtangle.h"


// Renders the squares background 
void redtangle::EmptyPiece::render(const std::shared_ptr<RedtangleUI> ui, const Location& location) const {
    const double RECT_WIDTH = ui->get_rectWidth();
    const double RECT_HEIGHT = ui->get_rectHeight(); 
    const int EDGE_SIZE = ui->get_edgeSize(); 
    const Rect redtangle_surface = ui->get_redtangleSurface(); 
    Rect rect = { location.x * RECT_WIDTH + redtangle_surface.top_left.x,
                  location.y * RECT_HEIGHT + redtangle_surface.top_left.y,
                  location.x == GRID_SIZE - 1 ? RECT_WIDTH : RECT_WIDTH - EDGE_SIZE,
                  location.y == GRID_SIZE - 1 ? RECT_HEIGHT: RECT_HEIGHT - EDGE_SIZE
                };              
    ui->render_filledRect(rect, in_redtangle(location) ? RED : GOLD); 
}