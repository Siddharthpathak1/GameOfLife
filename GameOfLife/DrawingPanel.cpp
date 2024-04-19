#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef)
    : wxPanel(parent), gameBoard(gameBoardRef)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingPanel::OnMouseUp, this); // Bind left mouse up event
}

DrawingPanel::~DrawingPanel()
{

}

void DrawingPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);
    if (!context)
        return;

    context->SetPen(*wxBLACK_PEN);

    int width, height;
    GetSize(&width, &height);

    cellWidth = width / gridSize;
    cellHeight = height / gridSize;

    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            int x = j * cellWidth;
            int y = i * cellHeight;

            // Set brush color based on game board state
            if (gameBoard[i][j]) // Check if the cell is alive
            {
                context->SetBrush(*wxLIGHT_GREY_BRUSH); // Set brush to light grey for alive cells
            }
            else
            {
                context->SetBrush(*wxWHITE_BRUSH); // Set brush to white for dead cells
            }

            // Draw rectangle representing the cell
            context->DrawRectangle(x, y, cellWidth, cellHeight);
        }
    }

    delete context;
}


void DrawingPanel::SetSize(const wxSize& size)
{
    wxPanel::SetSize(size);
    Refresh();
}

void DrawingPanel::SetGridSize(int size)
{
    Refresh();
}

void DrawingPanel::OnMouseUp(wxMouseEvent& event)
{
    int x = event.GetX();
    int y = event.GetY();

    int width, height;
    GetSize(&width, &height);

    cellWidth = width / gridSize;
    cellHeight = height / gridSize;

    int row = y / cellHeight;
    int col = x / cellWidth;

    // Toggle the state of the clicked cell
    if (row >= 0 && row < gridSize && col >= 0 && col < gridSize)
    {
        gameBoard[row][col] = !gameBoard[row][col];
        Refresh();
    }

    event.Skip();
}