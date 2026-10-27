// Task 1: Peak Element in a 2D Array
// A peak is >= all its neighbours (up, down, left, right).
#include <iostream>
using namespace std;

const int MAX = 50;

class PeakFinder {
private:
    int arr[MAX][MAX];
    int rows;
    int cols;

public:
    PeakFinder() {
        rows = 0;
        cols = 0;
    }

    // returns false if the input is bad
    bool input() {
        int r, c;
        cout << "Enter number of rows (1 to " << MAX << "): ";
        if (!(cin >> r) || r < 1 || r > MAX) {
            cout << "Invalid number of rows." << endl;
            return false;
        }
        cout << "Enter number of columns (1 to " << MAX << "): ";
        if (!(cin >> c) || c < 1 || c > MAX) {
            cout << "Invalid number of columns." << endl;
            return false;
        }

        cout << "Enter the elements row by row:" << endl;
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (!(cin >> arr[i][j])) {
                    cout << "Invalid element, numbers only." << endl;
                    return false;
                }
            }
        }
        rows = r;
        cols = c;
        return true;
    }

    void setArray(int a[][MAX], int r, int c) {
        if (r < 1 || r > MAX || c < 1 || c > MAX) {
            cout << "Invalid array size." << endl;
            return;
        }
        rows = r;
        cols = c;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                arr[i][j] = a[i][j];
    }

    void display() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++)
                cout << arr[i][j] << "\t";
            cout << endl;
        }
    }

    // row of the biggest value in column c
    int maxInColumn(int c) {
        int maxRow = 0;
        for (int i = 1; i < rows; i++) {
            if (arr[i][c] > arr[maxRow][c])
                maxRow = i;
        }
        return maxRow;
    }

    // Binary search on columns. The max of the middle column already beats
    // the ones above and below it, so we just check left and right and move
    // towards the bigger side (a peak always exists there).
    // Time: O(n log n), log n columns x n rows each. Space: O(1)
    bool findPeak(int &peakRow, int &peakCol) {
        int low = 0, high = cols - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            int r = maxInColumn(mid);

            bool leftBigger = false, rightBigger = false;
            if (mid > 0 && arr[r][mid - 1] > arr[r][mid])
                leftBigger = true;
            if (mid < cols - 1 && arr[r][mid + 1] > arr[r][mid])
                rightBigger = true;

            // nothing bigger around it
            if (!leftBigger && !rightBigger) {
                peakRow = r;
                peakCol = mid;
                return true;
            }
            else if (leftBigger)
                high = mid - 1;
            else
                low = mid + 1;
        }
        return false;
    }

    int get(int r, int c) {
        return arr[r][c];
    }

    void printPeak() {
        int r, c;
        if (findPeak(r, c))
            cout << "Peak element: " << get(r, c)
                 << " at position (" << r << ", " << c << ")" << endl;
        else
            cout << "Array is empty." << endl;
    }
};

int main() {
    PeakFinder p;

    // sample array first
    int sample[MAX][MAX] = {
        {10,  8, 10, 10},
        {14, 13, 12, 11},
        {15,  9, 11, 21},
        {16, 17, 19, 20}
    };

    cout << "Sample array:" << endl;
    p.setArray(sample, 4, 4);
    p.display();
    p.printPeak();
    cout << endl;

    // then the user's array
    if (!p.input())
        return 0;
    cout << endl << "Your array:" << endl;
    p.display();
    p.printPeak();

    return 0;
}
