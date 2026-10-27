// Task 2: Knight's Tour (backtracking + Warnsdorff's rule)
//
// Plain backtracking tries the 8 knight moves in a fixed order. In the worst
// case it explores almost every partial path: O(8^(N^2)), which is hopeless
// beyond N = 6 or so.
//
// Optimization - Warnsdorff's rule: from the current square, try the moves
// in order of how many onward moves the target square has (fewest first).
// Squares that are hard to reach get visited early, so dead ends are rare.
// Ties are broken by visiting the square farther from the centre first,
// which keeps the knight on the edges and makes the heuristic very reliable.
// Backtracking is kept, so if the heuristic does get stuck we still undo
// and try the next candidate - the algorithm stays complete.
//
// Complexity analysis (N x N board, N^2 squares):
//   - Each step looks at <= 8 moves, and for each counts <= 8 onward moves,
//     so one step costs O(64) = O(1). Sorting 8 candidates is also O(1).
//   - Best case: the heuristic never hits a dead end, so we make exactly
//     N^2 - 1 steps -> O(N^2). This is what happens for almost every start
//     square (see the table printed by main: calls = N^2, backtracks = 0).
//     For a few start squares (e.g. some on 7x7) the heuristic does hit a
//     dead end and backtracking has to fix it, which costs extra time.
//   - Worst case: backtracking is still possible in theory, so the bound is
//     O(8^(N^2)) like plain backtracking. Also for N = 2, 3, 4 no tour exists
//     and the search must exhaust the tree before saying so (small boards,
//     so this is quick). On odd boards, starts on the minority colour can
//     never have a tour; these are rejected in O(1) by a parity check
//     instead of an exponential search.
//   - Space: O(N^2) for the board, O(N^2) recursion depth.

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <chrono>
using namespace std;

const int DX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
const int DY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

class KnightsTour {
    int n;
    vector<vector<int>> board;    // 0 = unvisited, otherwise the move number
    long long calls;              // recursive calls made
    long long backtracks;         // moves that had to be undone
    bool useHeuristic;

    bool isFree(int x, int y) const {
        return x >= 0 && y >= 0 && x < n && y < n && board[x][y] == 0;
    }

    // number of onward moves from (x, y). Time: O(1)
    int degree(int x, int y) const {
        int count = 0;
        for (int k = 0; k < 8; k++)
            if (isFree(x + DX[k], y + DY[k]))
                count++;
        return count;
    }

    // squared distance from the centre, doubled to stay in integers
    int distFromCentre(int x, int y) const {
        int dx = 2 * x - (n - 1), dy = 2 * y - (n - 1);
        return dx * dx + dy * dy;
    }

    bool solve(int x, int y, int step) {
        calls++;
        if (step == n * n)
            return true;

        struct Move { int x, y, deg, dist; };
        Move moves[8];
        int count = 0;
        for (int k = 0; k < 8; k++) {
            int nx = x + DX[k], ny = y + DY[k];
            if (isFree(nx, ny))
                moves[count++] = {nx, ny, 0, 0};
        }

        if (useHeuristic) {
            for (int i = 0; i < count; i++) {
                board[moves[i].x][moves[i].y] = -1;    // pretend we stand there
                moves[i].deg  = degree(moves[i].x, moves[i].y);
                moves[i].dist = distFromCentre(moves[i].x, moves[i].y);
                board[moves[i].x][moves[i].y] = 0;
            }
            sort(moves, moves + count, [](const Move& a, const Move& b) {
                if (a.deg != b.deg) return a.deg < b.deg;
                return a.dist > b.dist;
            });
        }

        for (int i = 0; i < count; i++) {
            board[moves[i].x][moves[i].y] = step + 1;
            if (solve(moves[i].x, moves[i].y, step + 1))
                return true;
            board[moves[i].x][moves[i].y] = 0;      // undo
            backtracks++;
        }
        return false;
    }

public:
    explicit KnightsTour(int size) : n(size), calls(0), backtracks(0), useHeuristic(true) {}

    bool findTour(int startX, int startY, bool heuristic) {
        board.assign(n, vector<int>(n, 0));
        calls = backtracks = 0;
        useHeuristic = heuristic;
        // a knight changes square colour every move, so on an odd board the
        // tour must start on the majority colour ((x + y) even). Rejecting
        // the other squares in O(1) avoids a full exponential search.
        if (n % 2 == 1 && (startX + startY) % 2 == 1)
            return false;
        board[startX][startY] = 1;
        return solve(startX, startY, 1);
    }

    // checks every consecutive pair of moves is a legal knight jump
    bool verify() const {
        vector<int> px(n * n + 1, -1), py(n * n + 1, -1);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                int s = board[i][j];
                if (s < 1 || s > n * n || px[s] != -1)
                    return false;
                px[s] = i;
                py[s] = j;
            }
        for (int s = 1; s < n * n; s++) {
            int dx = abs(px[s] - px[s + 1]), dy = abs(py[s] - py[s + 1]);
            if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1)))
                return false;
        }
        return true;
    }

    void print() const {
        int w = to_string(n * n).size() + 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << setw(w) << board[i][j];
            cout << endl;
        }
    }

    long long getCalls() const { return calls; }
    long long getBacktracks() const { return backtracks; }
};

template <typename T>
T readNumber(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value)
            return value;
        if (cin.eof())
            exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "   Please enter a valid number." << endl;
    }
}

int main() {
    cout << "===== Knight's Tour =====" << endl << endl;

    // 1) plain vs optimized backtracking on small boards
    cout << "Plain backtracking vs Warnsdorff (start at 0,0):" << endl;
    cout << setw(6) << "N" << setw(18) << "plain calls" << setw(18) << "warnsdorff calls" << endl;
    for (int n = 5; n <= 6; n++) {
        KnightsTour plain(n), smart(n);
        plain.findTour(0, 0, false);
        smart.findTour(0, 0, true);
        cout << setw(6) << n << setw(18) << plain.getCalls() << setw(18) << smart.getCalls() << endl;
    }
    cout << endl;

    // 2) growth of the optimized version: calls should be exactly N^2
    cout << "Warnsdorff growth (start at 0,0):" << endl;
    cout << setw(6) << "N" << setw(10) << "N^2" << setw(10) << "calls"
         << setw(12) << "backtracks" << setw(12) << "time (ms)" << setw(10) << "valid" << endl;
    int sizes[] = {5, 8, 10, 20, 50, 100};
    for (int n : sizes) {
        KnightsTour t(n);
        auto start = chrono::steady_clock::now();
        bool ok = t.findTour(0, 0, true);
        auto end = chrono::steady_clock::now();
        double ms = chrono::duration<double, milli>(end - start).count();
        cout << setw(6) << n << setw(10) << n * n << setw(10) << t.getCalls()
             << setw(12) << t.getBacktracks() << setw(12) << fixed << setprecision(3) << ms
             << setw(10) << (ok && t.verify() ? "yes" : "no") << endl;
    }
    cout << endl;

    // 3) user input
    // recursion depth is N^2, so very large boards would overflow the stack
    int n = readNumber<int>("Enter board size N (1 to 100, 0 to quit): ");
    if (n <= 0)
        return 0;
    if (n > 100) {
        cout << "Board size must be at most 100." << endl;
        return 0;
    }
    int x = readNumber<int>("Enter starting row (0 to N-1): ");
    int y = readNumber<int>("Enter starting column (0 to N-1): ");
    if (x < 0 || y < 0 || x >= n || y >= n) {
        cout << "Starting position is outside the board." << endl;
        return 0;
    }

    KnightsTour tour(n);
    if (tour.findTour(x, y, true)) {
        cout << endl << "Tour found (numbers show the order of visits):" << endl;
        tour.print();
        cout << "Calls: " << tour.getCalls() << ", backtracks: " << tour.getBacktracks()
             << ", valid: " << (tour.verify() ? "yes" : "no") << endl;
    }
    else {
        cout << "No knight's tour exists from (" << x << ", " << y << ") on a "
             << n << "x" << n << " board." << endl;
    }
    return 0;
}
