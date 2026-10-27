// Task 1: Peak Element in a 2D Array
// A peak is an element that is not smaller than any of its 4 neighbours
// (up, down, left, right). Edge elements just have fewer neighbours.
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

    // goes down column c and returns the row where the biggest value is
    int maxInColumn(int c) {
        int maxRow = 0;
        for (int i = 1; i < rows; i++) {
            if (arr[i][c] > arr[maxRow][c])
                maxRow = i;
        }
        return maxRow;
    }

    // Idea: binary search on the columns instead of checking every element.
    // Take the middle column and find its biggest value. Since it's the
    // biggest in its column, it's already >= the elements above and below it,
    // so only left and right are left to check.
    // If one of them is bigger, we go to that side. A peak has to be there
    // because the values keep going up that way and can't do it forever.
    //
    // Time: the search halves the columns each time (log n steps) and each
    // step scans one full column (n rows), so it's O(n log n) overall.
    // Space: O(1), nothing extra is used apart from a few variables.
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

    // first run it on a fixed array so we know what output to expect
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

    // then let the user try their own array
    p.input();
    cout << endl << "Your array:" << endl;
    p.display();
    p.findPeak();

    return 0;
}
