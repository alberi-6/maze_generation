#include <iostream>
#include <stack>
#include <vector>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


class Maze : public olc::PixelGameEngine {
    public:
        Maze() {
            sAppName = "Maze";
        }
    
    private:
        int nMazeWidth;
        int nMazeHeight;
        int *maze;  // array storing a value for all the cells of the maze
        // these values tell which neighour the cell is connected to, or if it is visited
        enum {
            CELL_PATH_N = 0x01,     // north --> binary 0001
            CELL_PATH_E = 0x02,     // east  --> binary 0010
            CELL_PATH_S = 0x04,     // south --> binary 0100
            CELL_PATH_W = 0x08,     // west  --> binary 1000
            CELL_VISITED = 0x10,    // binary (000)10000
        };

        int nVisitedCells;

        std::stack<std::pair<int, int>> m_stack;
        // stack storing object of type pair.
        // pair type allows to store two things at the same time
        // in this case a x, and y coordinate, both of type int

        int nPathWidth;

    public:
        bool OnUserCreate() override {
            // Maze parameters
            nMazeWidth = 40;
            nMazeHeight = 25;

            maze = new int[nMazeWidth * nMazeHeight];
            memset(maze, 0x00, nMazeWidth * nMazeHeight * sizeof(int));

            m_stack.push(std::make_pair(0, 0));
            maze[0] = CELL_VISITED;
            nVisitedCells = 1;

            nPathWidth = 3;

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {

            // === DRAWING STUFF ===

            // Draw maze
            for (int x = 0; x < nMazeWidth; x++) {
                for (int y = 0; y < nMazeHeight; y++) {
                    for (int px = 0; px < nPathWidth; px++) {
                        for (int py = 0; py < nPathWidth; py++) {
                            if (maze[y * nMazeWidth + x] & CELL_VISITED) {
                                Draw(x * (nPathWidth + 1) + px, y * (nPathWidth + 1) + py, olc::WHITE);
                            }
                            else {
                                Draw(x * (nPathWidth + 1) + px, y * (nPathWidth + 1) + py, olc::BLUE);
                            }
                        }
                    }

                    for (int p = 0; p < nPathWidth; p++) {
                        if (maze[y * nMazeWidth + x] & CELL_PATH_S) {
                            Draw(x * (nPathWidth + 1) + p, y * (nPathWidth + 1) + nPathWidth, olc::WHITE);
                        }

                        if (maze[y * nMazeWidth + x] & CELL_PATH_E) {
                            Draw(x * (nPathWidth + 1) + nPathWidth, y * (nPathWidth + 1) + p, olc::WHITE);
                        }
                    }
                }
            }

            return true;
        }
};


int main() {
    // Use olcPixelGameEngine derived app
    Maze game;
    game.Construct(160, 100, 8, 8);
    game.Start();

    return 0;
}