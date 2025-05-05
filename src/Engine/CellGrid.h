#ifndef CELLGRID_H
#define CELLGRID_H

#include <mutex>
#include <vector>

#include "Cell.h"


namespace Naito {

/// Literally a grid of cells. It runs a cellular automaton that simulates all the elements in the game.
class CellGrid {
public:
    CellGrid(size_t width, size_t height);

    [[nodiscard]] Cell getCell(Uint16 x, Uint16 y) const;
    void setCell(Uint16 x, Uint16 y, Cell cell);

    /// Swaps the cell at (x, y) with the cell at (x + dx, y + dy).
    void swapCells(Uint16 x, Uint16 y, Uint16 dx, Uint16 dy);

    /// Performs one simulation step.
    void update();

    /// Runs the simulation until the game closes.
    void updateLoop();

    /// Adds the currently selected element in a square of size ``brushSize`` around the given coordinates.
    void paint(int x, int y, int brushSize);

    // Element update methods -> definitions in Elements.cpp to avoid cluttering CellGrid.cpp
    void updateSand(Uint16 x, Uint16 y);
    void updateWater(Uint16 x, Uint16 y);
    void updateDirt(Uint16 x, Uint16 y);
    void updateGrass(Uint16 x, Uint16 y);
    void updateFire(Uint16 x, Uint16 y);
    void updateWood(Uint16 x, Uint16 y);

    [[nodiscard]] bool getClock() const;

    [[nodiscard]] std::mutex& getMutex();

    void copyToFrontbuffer();

    bool drawGui();

private:
    /// Counts the amount of cells of a given element in the grid.
    [[nodiscard]] std::array<unsigned int, static_cast<int>(Element::Count)> countCells() const;

    bool clock;

    // The simulation is performed on the backbuffer. When the update has been completed,
    // the data is copied over to the frontbuffer, which will be read for drawing, when ready.
    // While not optimal, this keeps both the simulation thread and the main thread locked only
    // for the time it takes to copy the buffer, instead of an entire update.
    std::vector<Cell> backbuffer;
    std::vector<Cell> frontbuffer;

    std::mutex mutex;

    Uint16 height;
    Uint16 width;

    float desiredTickrate;
    std::chrono::high_resolution_clock::duration tickInterval;
    std::chrono::high_resolution_clock::time_point lastUpdate;

    float actualTickrate;
    std::chrono::high_resolution_clock::duration actualTickDuration;
    std::chrono::high_resolution_clock::duration activeTickDuration;
};

}
#endif //CELLGRID_H
