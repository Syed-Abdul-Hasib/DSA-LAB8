// Task 2: Knight's Tour (Backtracking + Warnsdorff's rule)
//
// Simple backtracking tries all 8 moves in a fixed order -> O(8^(N*N)).
// Optimization (Warnsdorff's rule): always try the square that has the
// fewest onward moves first. This way the knight rarely gets stuck, so it
// usually finishes without backtracking at all.
//
// Complexity:
//   Each move checks 8 squares and counts 8 moves for each -> O(1) per move.
//   Best case:  no backtracking, N*N moves -> O(N^2)
//   Worst case: backtracking still happens -> O(8^(N*N))
//               (e.g. N = 2, 3, 4 where no tour exists)
//   Space: O(N^2) for the board and recursion
#include <iostream>
#include <iomanip>
using namespace std;

const int MAX = 30;

// the 8 possible knight moves
int moveX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int moveY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

class KnightTour {
private:
    int board[MAX][MAX];    // 0 = not visited, else the move number
    int n;
    int steps;              // number of recursive calls

public:
    KnightTour(int size) {
        n = size;
        steps = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                board[i][j] = 0;
    }

    bool isSafe(int x, int y) {
        return (x >= 0 && y >= 0 && x < n && y < n && board[x][y] == 0);
    }

    // number of free squares the knight can jump to from (x, y)
    int countMoves(int x, int y) {
        int count = 0;
        for (int k = 0; k < 8; k++) {
            if (isSafe(x + moveX[k], y + moveY[k]))
                count++;
        }
        return count;
    }

    // distance from the centre, used to break ties (farther goes first)
    int distance(int x, int y) {
        int dx = 2 * x - (n - 1);
        int dy = 2 * y - (n - 1);
        return dx * dx + dy * dy;
    }

    bool solve(int x, int y, int move) {
        steps++;
        if (move == n * n)
            return true;

        // collect all valid next squares
        int nextX[8], nextY[8], degree[8], dist[8];
        int total = 0;
        for (int k = 0; k < 8; k++) {
            int nx = x + moveX[k];
            int ny = y + moveY[k];
            if (isSafe(nx, ny)) {
                board[nx][ny] = -1;          // mark so it is not counted
                nextX[total] = nx;
                nextY[total] = ny;
                degree[total] = countMoves(nx, ny);
                dist[total] = distance(nx, ny);
                board[nx][ny] = 0;
                total++;
            }
        }

        // sort by fewest onward moves (insertion sort, at most 8 items)
        for (int i = 1; i < total; i++) {
            int tx = nextX[i], ty = nextY[i], td = degree[i], tdist = dist[i];
            int j = i - 1;
            while (j >= 0 && (degree[j] > td || (degree[j] == td && dist[j] < tdist))) {
                nextX[j + 1] = nextX[j];
                nextY[j + 1] = nextY[j];
                degree[j + 1] = degree[j];
                dist[j + 1] = dist[j];
                j--;
            }
            nextX[j + 1] = tx;
            nextY[j + 1] = ty;
            degree[j + 1] = td;
            dist[j + 1] = tdist;
        }

        // try each square, undo if it leads to a dead end (backtracking)
        for (int i = 0; i < total; i++) {
            board[nextX[i]][nextY[i]] = move + 1;
            if (solve(nextX[i], nextY[i], move + 1))
                return true;
            board[nextX[i]][nextY[i]] = 0;
        }
        return false;
    }

    bool findTour(int x, int y) {
        // a knight changes colour every move, so on an odd board the tour
        // can only start on a square where (x + y) is even
        if (n % 2 == 1 && (x + y) % 2 == 1)
            return false;

        board[x][y] = 1;
        return solve(x, y, 1);
    }

    void display() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << setw(4) << board[i][j];
            cout << endl;
        }
    }

    int getSteps() {
        return steps;
    }
};

int main() {
    int n, x, y;

    cout << "Enter board size N (max " << MAX << "): ";
    cin >> n;
    if (n < 1 || n > MAX) {
        cout << "Invalid board size." << endl;
        return 0;
    }

    cout << "Enter starting position (row col): ";
    cin >> x >> y;
    if (x < 0 || y < 0 || x >= n || y >= n) {
        cout << "Position is outside the board." << endl;
        return 0;
    }

    KnightTour k(n);
    if (k.findTour(x, y)) {
        cout << endl << "Knight's tour found:" << endl;
        k.display();
        cout << endl << "Squares on board: " << n * n << endl;
        cout << "Recursive calls:  " << k.getSteps() << endl;
    }
    else {
        cout << "No knight's tour exists from (" << x << ", " << y << ")." << endl;
    }

    return 0;
}
