// Task 1: Peak Element in a 2D Array
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

    void input() {
        cout << "Enter number of rows: ";
        cin >> rows;
        cout << "Enter number of columns: ";
        cin >> cols;

        cout << "Enter the elements row by row:" << endl;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cin >> arr[i][j];
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

    // Returns the row of the largest element in column c. Time: O(n)
    int maxInColumn(int c) {
        int maxRow = 0;
        for (int i = 1; i < rows; i++) {
            if (arr[i][c] > arr[maxRow][c])
                maxRow = i;
        }
        return maxRow;
    }

    // Binary search on columns.
    // The max of the middle column is already >= its up and down neighbours,
    // so we only compare it with left and right. If a side is bigger, a peak
    // must exist in that half, so we move there.
    // log n columns are checked, each check scans n rows -> O(n log n)
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

    p.input();
    cout << endl << "Your array:" << endl;
    p.display();
    p.findPeak();

    return 0;
}
