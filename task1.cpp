// Task 1: Peak Element in a 2D Array
// A peak is an element that is >= all of its neighbours (up, down, left, right).
//
// Approach (binary search on columns):
//   1. Pick the middle column mid and find the row r holding its largest value.
//      Then a[r][mid] is >= its up/down neighbours automatically.
//   2. If a[r][mid] is also >= its left and right neighbours, it is a peak.
//   3. Otherwise one side has a bigger neighbour. Move to that half: the
//      maximum of that half can't be beaten by anything in our column, so
//      a peak must exist inside it.
//   Each step halves the columns, and each step scans one column (n rows).
//
// Time:  O(n log m) for an n x m array -> O(n log n) for an n x n array
// Space: O(1) extra (iterative)

#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

typedef vector<vector<int>> Matrix;

struct Peak {
    int row;
    int col;
};

// row index of the largest value in column c. Time: O(n)
int maxRowInColumn(const Matrix& a, int c) {
    int best = 0;
    for (int r = 1; r < (int)a.size(); r++)
        if (a[r][c] > a[best][c])
            best = r;
    return best;
}

// O(n log m) peak finder
Peak findPeak(const Matrix& a) {
    int cols = a[0].size();
    int lo = 0, hi = cols - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int r = maxRowInColumn(a, mid);

        bool leftBigger  = mid > 0        && a[r][mid - 1] > a[r][mid];
        bool rightBigger = mid < cols - 1 && a[r][mid + 1] > a[r][mid];

        if (!leftBigger && !rightBigger)
            return {r, mid};
        if (leftBigger)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return {-1, -1};    // never reached for a non-empty array
}

// brute force check used to verify the answer. Time: O(1)
bool isPeak(const Matrix& a, int r, int c) {
    int n = a.size(), m = a[0].size();
    int v = a[r][c];
    if (r > 0     && a[r - 1][c] > v) return false;
    if (r < n - 1 && a[r + 1][c] > v) return false;
    if (c > 0     && a[r][c - 1] > v) return false;
    if (c < m - 1 && a[r][c + 1] > v) return false;
    return true;
}

void printMatrix(const Matrix& a, const Peak& p) {
    for (int r = 0; r < (int)a.size(); r++) {
        for (int c = 0; c < (int)a[r].size(); c++) {
            if (r == p.row && c == p.col)
                cout << "[" << a[r][c] << "]\t";
            else
                cout << " " << a[r][c] << "\t";
        }
        cout << endl;
    }
}

void runTest(const string& title, const Matrix& a) {
    cout << "--- " << title << " (" << a.size() << " x " << a[0].size() << ") ---" << endl;
    Peak p = findPeak(a);
    printMatrix(a, p);
    cout << "Peak " << a[p.row][p.col] << " at (" << p.row << ", " << p.col << ")  -> "
         << (isPeak(a, p.row, p.col) ? "verified" : "NOT a peak") << endl << endl;
}

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
    cout << "===== Peak Element in a 2D Array =====" << endl << endl;

    runTest("Sample 1", {
        {10,  8, 10, 10},
        {14, 13, 12, 11},
        {15,  9, 11, 21},
        {16, 17, 19, 20}
    });

    runTest("Sample 2 (all equal)", {
        {5, 5, 5},
        {5, 5, 5},
        {5, 5, 5}
    });

    runTest("Sample 3 (increasing)", {
        {1, 2, 3, 4, 5},
        {2, 3, 4, 5, 6},
        {3, 4, 5, 6, 7}
    });

    // stress test: random arrays, every answer is checked
    srand(time(nullptr));
    int failures = 0;
    for (int t = 0; t < 1000; t++) {
        int n = rand() % 20 + 1, m = rand() % 20 + 1;
        Matrix a(n, vector<int>(m));
        for (auto& row : a)
            for (int& x : row)
                x = rand() % 50;
        Peak p = findPeak(a);
        if (!isPeak(a, p.row, p.col))
            failures++;
    }
    cout << "Random stress test: 1000 arrays, " << failures << " failure(s)" << endl << endl;

    // user input
    int n = readNumber<int>("Enter number of rows (0 to skip): ");
    if (n <= 0)
        return 0;
    int m = readNumber<int>("Enter number of columns: ");
    if (m <= 0) {
        cout << "Columns must be positive." << endl;
        return 0;
    }

    Matrix a(n, vector<int>(m));
    cout << "Enter the elements row by row:" << endl;
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            a[r][c] = readNumber<int>("");

    cout << endl;
    runTest("Your array", a);
    return 0;
}
