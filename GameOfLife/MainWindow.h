#pragma once

#include "wx/wx.h"
#include "DrawingPanel.h"

class MainWindow : public wxFrame
{
public:
    MainWindow();
    ~MainWindow();

    void InitializeGrid(); // Declaration for grid initialization method
    void OnSizeChange(wxSizeEvent& event); // Declaration for window resize event handler
    void UpdateStatusBar(); // Declaration for updating status bar
    void OnPlayButtonClicked(wxCommandEvent& event); // Declaration for play button event handler
    void OnPauseButtonClicked(wxCommandEvent& event); // Declaration for pause button event handler
    void OnNextButtonClicked(wxCommandEvent& event); // Declaration for next button event handler
    void OnTrashButtonClicked(wxCommandEvent& event); // Declaration for trash button event handler
    int CountLivingNeighbors(int row, int col); // Declaration for counting living neighbors
    void NextGeneration(); // Declaration for generating the next generation
    void ClearBoard(); // Declaration for clearing the game board
    void StartTimer(); // New method to start the timer
    void StopTimer(); // New method to stop the timer
    void OnTimer(wxTimerEvent& event); // Declaration for timer event handler

private:
    DrawingPanel* drawingPanel;
    std::vector<std::vector<bool>> gameBoard;
    const int gridSize = 15; // Default grid size
    int generationCount = 0; // Number of generations
    int livingCellsCount = 0; // Number of living cells

    wxStatusBar* statusBar; // Status bar variable
    wxToolBar* toolbar; // Toolbar variable
    wxTimer* gameTimer; // New member for the timer
    int timerInterval = 50;

    wxDECLARE_EVENT_TABLE(); // Event table
};