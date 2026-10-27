// Task 1: Peak Element in a 2D Array
// A peak is an element that is >= all of its neighbours (up, down, left, right).
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

    // reads the array from the user, returns false if the input is bad
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

    // goes down column c and returns the row where the biggest value is
    int maxInColumn(int c) {
        int maxRow = 0;
        for (int i = 1; i < rows; i++) {
            if (arr[i][c] > arr[maxRow][c])
                maxRow = i;
        }
        return maxRow;
    }

    // Binary search on the columns. The biggest value in the middle column is
    // already >= the ones above and below it, so we only check left and right.
    // If one side is bigger, a peak must be on that side, so we go there.
    // Why: that neighbour is bigger than everything in the middle column.
    // So the max of that half is bigger too, and it can never be next to
    // something bigger in our column. That max is always a peak.
    // Time: log c steps, each scanning r rows -> O(r log c) for an r x c
    // array, which is O(n log n) for an n x n one. Space: O(1)
    void findPeak() {
        int low = 0, high = cols - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            int r = maxInColumn(mid);

            bool leftBigger = false, rightBigger = false;
            if (mid > 0 && arr[r][mid - 1] > arr[r][mid])
                leftBigger = true;
            if (mid < cols - 1 && arr[r][mid + 1] > arr[r][mid])
                rightBigger = true;

            // nothing around it is bigger, so this is a peak
            if (!leftBigger && !rightBigger) {
                cout << "Peak element: " << arr[r][mid]
                     << " at position (" << r << ", " << mid << ")" << endl;
                return;
            }
            else if (leftBigger)
                high = mid - 1;
            else
                low = mid + 1;
        }
    }
};

int main() {
    PeakFinder p;

    // fixed array first, so we know the expected output
    int sample[MAX][MAX] = {
        {10,  8, 10, 10},
        {14, 13, 12, 11},
        {15,  9, 11, 21},
        {16, 17, 19, 20}
    };

    cout << "Sample array:" << endl;
    p.setArray(sample, 4, 4);
    p.display();
    p.findPeak();
    cout << endl;

    // then the user's own array
    if (!p.input())
        return 0;
    cout << endl << "Your array:" << endl;
    p.display();
    p.findPeak();

    return 0;
}
