#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const int N = 8;

class Board {
public:
    vector<int> queens;  // queens[i] = row of queen in ith column

    Board() {
        queens = vector<int>(N, 0);
        for (int i = 0; i < N; ++i) {
            queens[i] = rand() % N;
        }
    }

    int calculateConflicts() const {
        int conflicts = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                if (queens[i] == queens[j] || abs(queens[i] - queens[j]) == abs(i - j)) {
                    conflicts++;
                }
            }
        }
        return conflicts;
    }

    void print() const {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (queens[j] == i) {
                    cout << "Q ";
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
        cout << "Conflicts: " << calculateConflicts() << endl;
    }
};

Board hillClimbing(Board current) {
    while (true) {
        int currentConflicts = current.calculateConflicts();
        if (currentConflicts == 0) break;

        Board next = current;
        int minConflicts = currentConflicts;
        for (int col = 0; col < N; ++col) {
            for (int row = 0; row < N; ++row) {
                if (current.queens[col] == row) continue;
                Board neighbor = current;
                neighbor.queens[col] = row;
                int neighborConflicts = neighbor.calculateConflicts();
                if (neighborConflicts < minConflicts) {
                    next = neighbor;
                    minConflicts = neighborConflicts;
                }
            }
        }

        if (minConflicts >= currentConflicts) break;
        current = next;
    }
    return current;
}

Board stochasticHillClimbing(Board current) {
    while (true) {
        int currentConflicts = current.calculateConflicts();
        if (currentConflicts == 0) break;

        vector<Board> neighbors;
        for (int col = 0; col < N; ++col) {
            for (int row = 0; row < N; ++row) {
                if (current.queens[col] == row) continue;
                Board neighbor = current;
                neighbor.queens[col] = row;
                neighbors.push_back(neighbor);
            }
        }

        Board next = neighbors[rand() % neighbors.size()];
        int nextConflicts = next.calculateConflicts();

        if (nextConflicts < currentConflicts) {
            current = next;
        } else if (nextConflicts == currentConflicts && rand() % 2) {
            current = next;
        }
    }
    return current;
}

Board simulatedAnnealing(Board current, double temperature, double coolingRate) {
    while (temperature > 1) {
        int currentConflicts = current.calculateConflicts();
        if (currentConflicts == 0) break;

        Board next = current;
        int col = rand() % N;
        int row = rand() % N;
        next.queens[col] = row;

        int nextConflicts = next.calculateConflicts();
        int delta = nextConflicts - currentConflicts;

        if (delta < 0 || exp(-delta / temperature) > (double)rand() / RAND_MAX) {
            current = next;
        }

        temperature *= coolingRate;
    }
    return current;
}

int main() {
    srand(time(0));

    cout << "Hill Climbing:" << endl;
    Board board1;
    board1 = hillClimbing(board1);
    board1.print();

    cout << "Stochastic Hill Climbing:" << endl;
    Board board2;
    board2 = stochasticHillClimbing(board2);
    board2.print();

    cout << "Simulated Annealing:" << endl;
    Board board3;
    board3 = simulatedAnnealing(board3, 1000, 0.995);
    board3.print();

    return 0;
}
