#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnSizeChange)
EVT_MENU(10000, MainWindow::OnPlayButtonClicked)
EVT_MENU(10001, MainWindow::OnPauseButtonClicked)
EVT_MENU(10002, MainWindow::OnNextButtonClicked)
EVT_MENU(10003, MainWindow::OnTrashButtonClicked)
EVT_TIMER(10004, MainWindow::OnTimer) // Event for timer
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxDefaultPosition, wxSize(400, 400))
{
    drawingPanel = new DrawingPanel(this, gameBoard);

    // Bind the window resize event to the event handler
    this->Bind(wxEVT_SIZE, &MainWindow::OnSizeChange, this);

    // Create status bar
    statusBar = CreateStatusBar();

    // Create toolbar
    toolbar = CreateToolBar();
    toolbar->SetToolBitmapSize(wxSize(32, 32)); // Set the size of toolbar icons

    // Add play button to toolbar
    wxBitmap playBitmap(play_xpm);
    toolbar->AddTool(10000, "Play", playBitmap);
    toolbar->AddSeparator(); // Add separator between buttons

    // Add pause button to toolbar
    wxBitmap pauseBitmap(pause_xpm);
    toolbar->AddTool(10001, "Pause", pauseBitmap);
    toolbar->AddSeparator(); // Add separator between buttons

    // Add next button to toolbar
    wxBitmap nextBitmap(next_xpm);
    toolbar->AddTool(10002, "Next", nextBitmap);
    toolbar->AddSeparator(); // Add separator between buttons

    // Add trash button to toolbar
    wxBitmap trashBitmap(trash_xpm);
    toolbar->AddTool(10003, "Trash", trashBitmap);

    // Realize toolbar to display it
    toolbar->Realize();

    // Update status bar text
    UpdateStatusBar();

    // Refresh the layout to ensure status bar and toolbar are visible
    this->Layout();

    InitializeGrid(); // Initialize the game board grid

    // Initialize the timer
    gameTimer = new wxTimer(this, 10004);
    gameTimer->Stop(); // Initially stopped
}

MainWindow::~MainWindow()
{
    delete drawingPanel;
}

void MainWindow::OnSizeChange(wxSizeEvent& event)
{
    wxSize size = event.GetSize();
    drawingPanel->SetSize(size); // Call the SetSize method of the drawing panel
    event.Skip(); // Allow default processing
}

void MainWindow::InitializeGrid()
{
    // Resize the game board vector to the size of the grid
    gameBoard.resize(gridSize);

    // Resize each sub-vector to the size of the grid
    for (int i = 0; i < gridSize; ++i)
    {
        gameBoard[i].resize(gridSize);
    }

    // Pass the grid size to the drawing panel
    drawingPanel->SetGridSize(gridSize);
}

void MainWindow::UpdateStatusBar()
{
    // Update living cells count
    livingCellsCount = 0;
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (i >= gameBoard.size() || j >= gameBoard[i].size()) {
                // Debug output for out-of-range indices
                wxLogDebug("Out of range: i=%d, j=%d", i, j);
            }
            else if (gameBoard[i][j])
            {
                // Debug output for living cells
                wxLogDebug("Living cell found at (%d, %d)", i, j);
                ++livingCellsCount;
            }
        }
    }

    // Update status bar text with generation count and living cells count
    wxString statusText = wxString::Format("Generation: %d, Living Cells: %d", generationCount, livingCellsCount);
    statusBar->SetStatusText(statusText);
}

int MainWindow::CountLivingNeighbors(int row, int col)
{
    int count = 0;

    // Iterate through the neighboring cells
    for (int i = row - 1; i <= row + 1; ++i)
    {
        for (int j = col - 1; j <= col + 1; ++j)
        {
            // Check if the index is valid and not the cell itself
            if (i >= 0 && i < gridSize && j >= 0 && j < gridSize && !(i == row && j == col))
            {
                // If the neighbor cell is alive, increment the count
                if (gameBoard[i][j])
                {
                    ++count;
                }
            }
        }
    }

    return count;
}

void MainWindow::NextGeneration()
{
    std::vector<std::vector<bool>> sandbox(gridSize, std::vector<bool>(gridSize, false));

    // Iterate through each cell in the game board
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            // Count the number of living neighbors for the current cell
            int neighbors = CountLivingNeighbors(i, j);

            // Apply the rules of the Game of Life
            if (gameBoard[i][j])
            {
                // Living cells with less than 2 living neighbors die
                // Living cells with more than 3 living neighbors die
                if (neighbors < 2 || neighbors > 3)
                {
                    sandbox[i][j] = false; // Cell dies
                }
                else
                {
                    sandbox[i][j] = true; // Cell lives
                }
            }
            else
            {
                // Dead cells with exactly 3 living neighbors become alive
                if (neighbors == 3)
                {
                    sandbox[i][j] = true; // Cell becomes alive
                }
            }
        }
    }

    // Update the game board with the new generation
    gameBoard.swap(sandbox);

    // Update generation count and status bar text
    ++generationCount;
    UpdateStatusBar();

    // Refresh the drawing panel
    drawingPanel->Refresh();
}

void MainWindow::ClearBoard()
{
    // Reset all cells in the game board to false
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            gameBoard[i][j] = false;
        }
    }

    // Reset generation count and living cell count
    generationCount = 0;
    livingCellsCount = 0;

    // Update status bar text
    UpdateStatusBar();

    // Refresh the drawing panel
    drawingPanel->Refresh();
}

void MainWindow::OnPlayButtonClicked(wxCommandEvent& event)
{
    StartTimer();
}

void MainWindow::OnPauseButtonClicked(wxCommandEvent& event)
{
    StopTimer();// Handle pause button click event
}

void MainWindow::OnNextButtonClicked(wxCommandEvent& event)
{
    NextGeneration(); // Call the method to calculate the next generation
}

void MainWindow::OnTrashButtonClicked(wxCommandEvent& event)
{
    ClearBoard();
}

void MainWindow::StartTimer()
{
    gameTimer->Start(timerInterval); // Start the timer with the specified interval
}

void MainWindow::StopTimer()
{
    gameTimer->Stop(); // Stop the timer
}

void MainWindow::OnTimer(wxTimerEvent& event)
{
    if (gameTimer->IsRunning()) {
        NextGeneration(); // Call the method to calculate the next generation only when the timer is running
    }
}
