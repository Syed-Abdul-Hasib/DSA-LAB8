// Task 3: Tower of Hanoi (Iterative)
//
// Each peg is a stack. The moves keep cycling through 3 peg pairs:
//   A-C, A-B, B-C  (for even N swap B and C)
// and between two pegs only one move is legal, so we just make it.
//
// T(N) = 2T(N-1) + 1 = 2^N - 1 moves, each O(1)
// Time: O(2^N), same for every input. Space: O(N)
#include <iostream>
using namespace std;

const int MAX = 30;

// one stack per peg
class Stack {
private:
    int arr[MAX];
    int top;
    char name;    // A, B or C

public:
    Stack() {
        top = -1;
        name = ' ';
    }

    void setName(char c) {
        name = c;
    }

    char getName() {
        return name;
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
        if (isEmpty()) {
            cout << "Stack underflow" << endl;
            return -1;
        }
        return arr[top--];
    }

    int peek() {
        if (isEmpty()) {
            cout << "Stack is empty" << endl;
            return -1;
        }
        return arr[top];
    }

    // bottom to top
    void display() {
        cout << name << ": ";
        for (int i = 0; i <= top; i++)
            cout << arr[i] << " ";
        cout << endl;
    }
};

class TowerOfHanoi {
private:
    Stack source, auxiliary, destination;
    int n;
    int moveCount;
    bool showPegs;    // only for small N

    // legal move between a and b
    void moveBetween(Stack &a, Stack &b) {
        if (a.isEmpty() || (!b.isEmpty() && b.peek() < a.peek())) {
            int disk = b.pop();
            a.push(disk);
            printMove(disk, b.getName(), a.getName());
        }
        else {
            int disk = a.pop();
            b.push(disk);
            printMove(disk, a.getName(), b.getName());
        }
        if (showPegs)
            displayPegs();
    }

    void printMove(int disk, char from, char to) {
        moveCount++;
        cout << "Move " << moveCount << ": Disk " << disk
             << " from " << from << " to " << to << endl;
    }

    void displayPegs() {
        source.display();
        auxiliary.display();
        destination.display();
        cout << endl;
    }

public:
    TowerOfHanoi(int disks) {
        n = disks;
        moveCount = 0;
        showPegs = (n <= 4);
        source.setName('A');
        auxiliary.setName('B');
        destination.setName('C');

        // biggest disk at the bottom
        for (int i = n; i >= 1; i--)
            source.push(i);
    }

    void solve() {
        if (showPegs) {
            cout << "Start:" << endl;
            displayPegs();
        }

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
    }

    int getMoveCount() {
        return moveCount;
    }
};

int main() {
    int n;
    cout << "Enter number of disks: ";

    // 20 disks = 1 million moves
    if (!(cin >> n) || n < 1 || n > 20) {
        cout << "Please enter between 1 and 20 disks." << endl;
        return 0;
    }

    cout << endl << "Moving " << n << (n == 1 ? " disk" : " disks") << " from A to C:" << endl;
    TowerOfHanoi t(n);
    t.solve();

    cout << endl << "Total moves: " << t.getMoveCount()
         << " (2^" << n << " - 1 = " << (1 << n) - 1 << ")" << endl;

    return 0;
}
