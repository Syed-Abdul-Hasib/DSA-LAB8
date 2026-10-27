// Task 3: Tower of Hanoi (Iterative)
//
// Usually this is solved with recursion, but here it had to be iterative,
// so each peg is a stack and we move disks between the stacks in a loop.
//
// If you write out the moves of the normal solution, they always go
// between the same pairs of pegs in a repeating pattern:
//   move 1, 4, 7, ...  between source and destination
//   move 2, 5, 8, ...  between source and auxiliary
//   move 3, 6, 9, ...  between auxiliary and destination
// For any two pegs only one direction is allowed (the smaller disk has to
// go on top), so we just check the top disks and move the smaller one.
// When N is even the pattern is the same but with auxiliary and
// destination swapped, otherwise the tower ends up on the wrong peg.
//
// Complexity:
// - Time: we need 2^N - 1 moves to solve it, and every move is just a few
//   push/pop calls, O(1). So the total is O(2^N), and it's the same for
//   every input of size N since the number of moves never changes.
//   Every extra disk doubles the work.
// - Space: O(N), because all three stacks together only ever hold the N
//   disks. There's no recursion, so no extra call stack either.
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

    // moves a disk between a and b in whichever direction is legal.
    // if a is empty, or b has the smaller disk on top, it goes b -> a
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

        // put all disks on A, biggest first so it ends up at the bottom
        for (int i = n; i >= 1; i--)
            source.push(i);
    }

    void solve() {
        Stack *aux = &auxiliary;
        Stack *dest = &destination;
        // swap the roles for even N (see the note at the top)
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

    // more than 20 disks is over a million moves to print, so I capped it
    if (n < 1 || n > 20) {
        cout << "Please enter between 1 and 20 disks." << endl;
        return 0;
    }

    cout << endl << "Moving " << n << " disks from A to C:" << endl;
    TowerOfHanoi t(n);
    t.solve();

    return 0;
}
