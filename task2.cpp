// Task 2: Knight's Tour (Backtracking + Warnsdorff's rule)
//
// Plain backtracking would be: try the 8 moves in the same order every
// time and undo when stuck. It works, but it's really slow because in the
// worst case it ends up trying almost every path, O(8^(N*N)). Already on
// a 6x6 board it needs hundreds of thousands of calls.
//
// To speed it up I used Warnsdorff's rule. Out of the moves available,
// go to the square that has the fewest moves left from it. The idea is
// to visit the hard to reach squares (corners, edges) early, before they
// get cut off. If two squares are tied, I pick the one farther from the
// centre. I kept the backtracking too, so if the rule ever gets stuck
// the program can still go back and try something else.
//
// Complexity:
// - For every move we look at max 8 squares and count max 8 moves for each,
//   which is a fixed amount of work, so each move is O(1).
// - Best case: the rule never gets stuck, so we just make N*N moves and
//   the whole thing is O(N^2). This is what happens almost every time
//   (for 8x8 it took exactly 64 calls).
// - Worst case: it still has to backtrack, so in theory it's the same as
//   plain backtracking, O(8^(N*N)). This happens for N = 2, 3, 4, where no
//   tour exists and it has to check every option before giving up.
// - Space: O(N^2) for the board, and the recursion can go N*N calls deep.
#include <iostream>
#include <iomanip>
using namespace std;

const int MAX = 30;

// all 8 L-shaped moves, moveX[i] and moveY[i] go together
int moveX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int moveY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

class KnightTour {
private:
    int board[MAX][MAX];    // 0 means not visited, otherwise which move we got here on
    int n;
    int steps;              // counts recursive calls, to see how much work was done

public:
    KnightTour(int size) {
        n = size;
        steps = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                board[i][j] = 0;
    }

    // square is inside the board and not visited yet
    bool isSafe(int x, int y) {
        return (x >= 0 && y >= 0 && x < n && y < n && board[x][y] == 0);
    }

    // how many free squares the knight could jump to from (x, y)
    int countMoves(int x, int y) {
        int count = 0;
        for (int k = 0; k < 8; k++) {
            if (isSafe(x + moveX[k], y + moveY[k]))
                count++;
        }
        return count;
    }

    // (squared) distance from the middle of the board, only used for ties.
    // I doubled x and y so it works with ints even when the centre is x.5
    int distance(int x, int y) {
        int dx = 2 * x - (n - 1);
        int dy = 2 * y - (n - 1);
        return dx * dx + dy * dy;
    }

    bool solve(int x, int y, int move) {
        steps++;
        // every square has a number now, so the tour is done
        if (move == n * n)
            return true;

        // first find all the squares we can actually go to from here
        int nextX[8], nextY[8], degree[8], dist[8];
        int total = 0;
        for (int k = 0; k < 8; k++) {
            int nx = x + moveX[k];
            int ny = y + moveY[k];
            if (isSafe(nx, ny)) {
                board[nx][ny] = -1;          // block it for a sec so it doesn't count itself
                nextX[total] = nx;
                nextY[total] = ny;
                degree[total] = countMoves(nx, ny);
                dist[total] = distance(nx, ny);
                board[nx][ny] = 0;
                total++;
            }
        }

        // sort them so the square with the fewest onward moves comes first.
        // there are at most 8, so a simple insertion sort is enough
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

        // try them in that order. if one leads to a dead end, clear it
        // and try the next one (this is the backtracking part)
        for (int i = 0; i < total; i++) {
            board[nextX[i]][nextY[i]] = move + 1;
            if (solve(nextX[i], nextY[i], move + 1))
                return true;
            board[nextX[i]][nextY[i]] = 0;
        }
        return false;
    }

    bool findTour(int x, int y) {
        // a knight always lands on the other colour, so the tour keeps
        // switching colours. on an odd board one colour has one extra square,
        // and the tour has to start on that colour ((x + y) even). checking
        // this here saves a really long search that can never succeed
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
