// Task 3: Tower of Hanoi (Iterative)
//
// Each peg is a stack. The moves always repeat between the same peg pairs:
//   move 1, 4, 7, ...  source <-> destination
//   move 2, 5, 8, ...  source <-> auxiliary
//   move 3, 6, 9, ...  auxiliary <-> destination
// Only one direction is legal (smaller disk on top), so we move that way.
// For even N, auxiliary and destination are swapped.
//
// Complexity:
// - Time: 2^N - 1 moves, each O(1) -> O(2^N). Every extra disk doubles it.
// - Space: the stacks only hold N disks and there's no recursion -> O(N)
#include <iostream>
using namespace std;

const int MAX = 30;

// simple array stack, one for each peg
class Stack {
private:
    int arr[MAX];
    int top;

public:
    char name;    // A, B or C, just for printing

    Stack() {
        top = -1;
        name = ' ';
    }

    bool isEmpty() {
        return top == -1;
    }

    void push(int x) {
        if (top == MAX - 1) {
            cout << "Stack overflow" << endl;
            return;
        }
        arr[++top] = x;
    }

    int pop() {
        if (isEmpty())
            return -1;
        return arr[top--];
    }

    int peek() {
        if (isEmpty())
            return -1;
        return arr[top];
    }
};

class TowerOfHanoi {
private:
    Stack source, auxiliary, destination;
    int n;
    int moveCount;

    // moves a disk between a and b, whichever way is legal
    void moveBetween(Stack &a, Stack &b) {
        if (a.isEmpty() || (!b.isEmpty() && b.peek() < a.peek())) {
            int disk = b.pop();
            a.push(disk);
            printMove(disk, b.name, a.name);
        }
        else {
            int disk = a.pop();
            b.push(disk);
            printMove(disk, a.name, b.name);
        }
    }

    void printMove(int disk, char from, char to) {
        moveCount++;
        cout << "Move " << moveCount << ": Disk " << disk
             << " from " << from << " to " << to << endl;
    }

public:
    TowerOfHanoi(int disks) {
        n = disks;
        moveCount = 0;
        source.name = 'A';
        auxiliary.name = 'B';
        destination.name = 'C';

        // all disks on A, biggest at the bottom
        for (int i = n; i >= 1; i--)
            source.push(i);
    }

    void solve() {
        Stack *aux = &auxiliary;
        Stack *dest = &destination;
        // swap for even N
        if (n % 2 == 0) {
            aux = &destination;
            dest = &auxiliary;
        }

        int totalMoves = (1 << n) - 1;
        for (int i = 1; i <= totalMoves; i++) {
            if (i % 3 == 1)
                moveBetween(source, *dest);
            else if (i % 3 == 2)
                moveBetween(source, *aux);
            else
                moveBetween(*aux, *dest);
        }

        cout << endl << "Total moves: " << moveCount << endl;
    }
};

int main() {
    int n;
    cout << "Enter number of disks: ";
    cin >> n;

    // 20 disks is already a million moves
    if (n < 1 || n > 20) {
        cout << "Please enter between 1 and 20 disks." << endl;
        return 0;
    }

    cout << endl << "Moving " << n << " disks from A to C:" << endl;
    TowerOfHanoi t(n);
    t.solve();

    return 0;
}
