#include <iostream>
#include <stack>
#include <vector>
#include <thread>

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
            nMazeWidth = 80;
            nMazeHeight = 50;

            maze = new int[nMazeWidth * nMazeHeight];
            memset(maze, 0x00, nMazeWidth * nMazeHeight * sizeof(int));

            m_stack.push(std::make_pair(0, 0));
            maze[0] = CELL_VISITED;
            nVisitedCells = 1;

            nPathWidth = 3;

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {
            // Clear screen
            FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

            auto offset = [&](int x, int y) {
                return (m_stack.top().second + y) * nMazeWidth + (m_stack.top().first + x);
            };

            // Do Maze Algorithm
            if (nVisitedCells < nMazeWidth * nMazeHeight) {
                // Step 1: Create a set of the unvisited neighbours

                std::vector<int> neighbours;

                // North neighbour
                if (m_stack.top().second > 0 && (maze[offset(0, -1)] & CELL_VISITED) == 0) {
                        neighbours.push_back(0); // northern neighbour exists and is unvisited
                }
                // East neighbour
                if (m_stack.top().first < nMazeWidth - 1 && (maze[offset(1, 0)] & CELL_VISITED) == 0) {
                        neighbours.push_back(1); // eastern neighbour exists and is unvisited
                }
                // South neighbour
                if (m_stack.top().second < nMazeHeight - 1 && (maze[offset(0, 1)] & CELL_VISITED) == 0) {
                        neighbours.push_back(2); // southern neighbour exists and is unvisited
                }
                // West neighbour
                if (m_stack.top().first > 0 && (maze[offset(-1, 0)] & CELL_VISITED) == 0) {
                        neighbours.push_back(3); // western neighbour exists and is unvisited
                }

                // Are there any unvisited neighbours available?
                if (!neighbours.empty()) {
                    // Choose one available neighbour at random
                    int next_cell_dir = neighbours[rand() % neighbours.size()];

                    // Create a path between the neighbour and the current cell
                    switch (next_cell_dir) {
                        case 0: // North
                            maze[offset(0, 0)] |= CELL_PATH_N;
                            maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
                            m_stack.push(std::make_pair((m_stack.top().first + 0), (m_stack.top().second - 1)));
                            break;
                        
                        case 1: // East
                            maze[offset(0, 0)] |= CELL_PATH_E;
                            maze[offset(1, 0)] |= CELL_VISITED | CELL_PATH_W;
                            m_stack.push(std::make_pair((m_stack.top().first + 1), (m_stack.top().second + 0)));
                            break;

                        case 2: // South
                            maze[offset(0, 0)] |= CELL_PATH_S;
                            maze[offset(0, 1)] |= CELL_VISITED | CELL_PATH_N;
                            m_stack.push(std::make_pair((m_stack.top().first + 0), (m_stack.top().second + 1)));
                            break;

                        case 3: // West
                            maze[offset(0, 0)] |= CELL_PATH_W;
                            maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
                            m_stack.push(std::make_pair((m_stack.top().first - 1), (m_stack.top().second + 0)));
                            break;
                    }

                    nVisitedCells++;
                }
                else {
                    m_stack.pop(); // Backtrack
                }
            }

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

                    // Draw passageway between cells
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

            // Draw Unit - the top of the stack
            for (int py = 0; py < nPathWidth; py++)
                for (int px = 0; px < nPathWidth; px++)
                    Draw(m_stack.top().first * (nPathWidth + 1) + px, m_stack.top().second * (nPathWidth + 1) + py, olc::DARK_GREEN); // Draw Cell

            return true;
        }
};


int main() {
    // Use olcPixelGameEngine derived app
    Maze game;
    game.Construct(320, 200, 4, 4, false, true);
    game.Start();

    return 0;
}