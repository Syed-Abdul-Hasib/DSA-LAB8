// Task 2: Knight's Tour (Backtracking + Warnsdorff's rule)
//
// Plain backtracking tries the 8 moves in a fixed order and is really slow,
// O(8^(N*N)). To speed it up I used Warnsdorff's rule: always go to the
// square with the fewest moves left from it, so corners and edges get
// visited before they're cut off. Backtracking is kept in case it gets stuck.
//
// Complexity:
// - Each move checks at most 8 squares x 8 moves, so it's O(1) per move.
// - Best case: never gets stuck, N*N moves -> O(N^2). This is what happens
//   for most start squares (8x8 from (0,0) takes exactly 64 calls).
// - Worst case: the rule is a heuristic, not a guarantee. When it gets
//   stuck we backtrack, and in theory that's still O(8^(N*N)). For example
//   7x7 from (2,4) takes about 1.4 million calls instead of 49.
//   So O(N^2) is the best/usual case, not a promise for every input.
// - Space: O(N^2) for the board and the recursion.
//
// main() first runs both versions on 5x5 and 6x6 to show the difference
// (6x6: about 248,000 calls without the rule, 36 with it).
#include <iostream>
#include <iomanip>
using namespace std;

const int MAX = 30;

// the 8 L-shaped knight moves
const int moveX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
const int moveY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

class KnightTour {
private:
    int board[MAX][MAX];    // 0 = not visited, else the move number
    int n;
    long long steps;        // number of recursive calls
    bool useRule;           // false = plain backtracking, for comparing

public:
    KnightTour(int size) {
        n = size;
        steps = 0;
        useRule = true;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                board[i][j] = 0;
    }

    // inside the board and not visited yet
    bool isSafe(int x, int y) {
        return (x >= 0 && y >= 0 && x < n && y < n && board[x][y] == 0);
    }

    // how many free squares we can jump to from (x, y)
    int countMoves(int x, int y) {
        int count = 0;
        for (int k = 0; k < 8; k++) {
            if (isSafe(x + moveX[k], y + moveY[k]))
                count++;
        }
        return count;
    }

    // distance from the centre, only used to break ties
    int distance(int x, int y) {
        int dx = 2 * x - (n - 1);
        int dy = 2 * y - (n - 1);
        return dx * dx + dy * dy;
    }

    bool solve(int x, int y, int move) {
        steps++;
        // all squares visited
        if (move == n * n)
            return true;

        // find all squares we can go to from here
        int nextX[8], nextY[8], degree[8], dist[8];
        int total = 0;
        for (int k = 0; k < 8; k++) {
            int nx = x + moveX[k];
            int ny = y + moveY[k];
            if (isSafe(nx, ny)) {
                board[nx][ny] = -1;          // so it doesn't count itself
                nextX[total] = nx;
                nextY[total] = ny;
                degree[total] = countMoves(nx, ny);
                dist[total] = distance(nx, ny);
                board[nx][ny] = 0;
                total++;
            }
        }

        // fewest onward moves first (insertion sort, max 8 items).
        // skipped in plain mode, then moves are tried in the fixed order
        for (int i = 1; i < total && useRule; i++) {
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

        // try each one, undo it if it's a dead end (backtracking)
        for (int i = 0; i < total; i++) {
            board[nextX[i]][nextY[i]] = move + 1;
            if (solve(nextX[i], nextY[i], move + 1))
                return true;
            board[nextX[i]][nextY[i]] = 0;
        }
        return false;
    }

    bool findTour(int x, int y, bool rule) {
        useRule = rule;

        // a knight switches colour every move, so on an odd board the tour
        // has to start where (x + y) is even. saves a long useless search
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

    long long getSteps() {
        return steps;
    }
};

// runs plain backtracking and Warnsdorff on the same board and prints both
void compare(int n) {
    KnightTour plain(n), fast(n);
    plain.findTour(0, 0, false);
    fast.findTour(0, 0, true);
    cout << n << "x" << n << "\t" << plain.getSteps() << "\t\t" << fast.getSteps() << endl;
}

int main() {
    int n, x, y;

    // plain backtracking gets too slow after 6x6, so only compare these two
    cout << "Recursive calls starting from (0, 0):" << endl;
    cout << "Board\tPlain\t\tWarnsdorff" << endl;
    compare(5);
    compare(6);
    cout << endl;

    cout << "Enter board size N (1 to " << MAX << "): ";
    if (!(cin >> n) || n < 1 || n > MAX) {
        cout << "Invalid board size." << endl;
        return 0;
    }

    cout << "Enter starting position (row col): ";
    if (!(cin >> x >> y) || x < 0 || y < 0 || x >= n || y >= n) {
        cout << "Invalid position, it has to be inside the board." << endl;
        return 0;
    }

    KnightTour k(n);
    if (k.findTour(x, y, true)) {
        cout << endl << "Knight's tour found:" << endl;
        k.display();
        cout << endl << "Squares on board: " << n * n << endl;
        cout << "Recursive calls:  " << k.getSteps() << endl;
        if (k.getSteps() > n * n)
            cout << "(the rule got stuck here, so backtracking was needed)" << endl;
    }
    else {
        cout << "No knight's tour exists from (" << x << ", " << y << ")." << endl;
    }

    return 0;
}
