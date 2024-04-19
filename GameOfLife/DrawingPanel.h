#pragma once

#include "wx/wx.h"
#include <vector>

class DrawingPanel : public wxPanel
{
public:
    DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef);
    ~DrawingPanel();

    void OnPaint(wxPaintEvent& event);
    void SetSize(const wxSize& size);
    void SetGridSize(int size);

    void OnMouseUp(wxMouseEvent& event);

private:
    const int gridSize = 15;
    int cellWidth;
    int cellHeight;
    std::vector<std::vector<bool>>& gameBoard;
};