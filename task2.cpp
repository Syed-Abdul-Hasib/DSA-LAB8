// Task 2: Knight's Tour (Backtracking + Warnsdorff's rule)
//
// Plain backtracking is O(8^(N*N)), way too slow. Warnsdorff's rule fixes
// that: go to the square with the fewest onward moves first. If it still
// gets stuck, the attempt is stopped after 2*N*N calls and we retry with a
// different move order (8 tries max).
//
// Best case: no backtracking, N*N moves -> O(N^2)
// Worst case: 8 tries x 2*N*N = 16*N*N -> still O(N^2)
//   (tested every start up to 30x30, worst was 7x7 at ~13*N*N)
// If all tries fail it falls back to full backtracking, only seen on 4x4.
// Space: O(N^2)
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
    long long steps;        // total recursive calls
    long long tryCalls;     // calls in this attempt
    long long budget;       // call limit, 0 = no limit
    int offset;             // which move to try first
    int attempts;
    bool useRule;           // false = plain backtracking

    void clearBoard() {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                board[i][j] = 0;
    }

    bool overBudget() {
        return budget > 0 && tryCalls > budget;
    }

public:
    KnightTour(int size) {
        n = size;
        steps = 0;
        tryCalls = 0;
        budget = 0;
        offset = 0;
        attempts = 0;
        useRule = true;
        clearBoard();
    }

    // inside the board and not visited yet
    bool isSafe(int x, int y) {
        return (x >= 0 && y >= 0 && x < n && y < n && board[x][y] == 0);
    }

    // free squares reachable from (x, y)
    int countMoves(int x, int y) {
        int count = 0;
        for (int k = 0; k < 8; k++) {
            if (isSafe(x + moveX[k], y + moveY[k]))
                count++;
        }
        return count;
    }

    // distance from centre, for ties
    int distance(int x, int y) {
        int dx = 2 * x - (n - 1);
        int dy = 2 * y - (n - 1);
        return dx * dx + dy * dy;
    }

    bool solve(int x, int y, int move) {
        steps++;
        tryCalls++;
        // all squares visited
        if (move == n * n)
            return true;
        if (overBudget())
            return false;

        // collect the next squares
        int nextX[8], nextY[8], degree[8], dist[8];
        int total = 0;
        for (int q = 0; q < 8; q++) {
            int k = (q + offset) % 8;
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

        // sort by fewest onward moves (skipped in plain mode)
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

        // try each one, undo if dead end
        for (int i = 0; i < total; i++) {
            board[nextX[i]][nextY[i]] = move + 1;
            if (solve(nextX[i], nextY[i], move + 1))
                return true;
            board[nextX[i]][nextY[i]] = 0;
            if (overBudget())
                return false;
        }
        return false;
    }

    // one try with a move order and call limit
    bool attempt(int x, int y, int order, long long limit) {
        clearBoard();
        offset = order;
        budget = limit;
        tryCalls = 0;
        attempts++;
        board[x][y] = 1;
        return solve(x, y, 1);
    }

    bool findTour(int x, int y, bool rule) {
        useRule = rule;
        steps = 0;
        attempts = 0;

        // on odd boards a tour can only start where (x + y) is even
        if (n % 2 == 1 && (x + y) % 2 == 1)
            return false;

        // plain: one try, no limit
        if (!useRule)
            return attempt(x, y, 0, 0);

        for (int order = 0; order < 8; order++) {
            if (attempt(x, y, order, 2LL * n * n))
                return true;
            // finished without hitting the limit, so no tour
            if (!overBudget())
                return false;
        }
        // all 8 cut off, do a full search
        return attempt(x, y, 0, 0);
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

    int getAttempts() {
        return attempts;
    }
};

// plain vs Warnsdorff call count
void compare(int n) {
    KnightTour plain(n), fast(n);
    plain.findTour(0, 0, false);
    fast.findTour(0, 0, true);
    cout << n << "x" << n << "\t" << plain.getSteps() << "\t\t" << fast.getSteps() << endl;
}

// start square that needed the most calls
void worstCase(int n) {
    long long worst = 0;
    int wx = 0, wy = 0;
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            KnightTour k(n);
            if (k.findTour(x, y, true) && k.getSteps() > worst) {
                worst = k.getSteps();
                wx = x;
                wy = y;
            }
        }
    }
    cout << n << "x" << n << "\t" << n * n << "\t" << worst
         << "\t(" << wx << ", " << wy << ")" << endl;
}

int main() {
    int n, x, y;

    // plain is too slow after 6x6
    cout << "Recursive calls starting from (0, 0):" << endl;
    cout << "Board\tPlain\t\tWarnsdorff" << endl;
    compare(5);
    compare(6);
    cout << endl;

    // should stay close to N*N
    cout << "Worst start square (Warnsdorff):" << endl;
    cout << "Board\tN*N\tCalls\tStart" << endl;
    for (int size = 5; size <= 10; size++)
        worstCase(size);
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
        cout << "Attempts:         " << k.getAttempts() << endl;
    }
    else {
        cout << "No knight's tour exists from (" << x << ", " << y << ")." << endl;
    }

    return 0;
}
