// Task 3: Tower of Hanoi (iterative)
//
// Each peg is a stack of disk sizes. The iterative solution uses the fact
// that the optimal sequence repeats a cycle of three peg pairs:
//     move i % 3 == 1 : between source and destination
//     move i % 3 == 2 : between source and auxiliary
//     move i % 3 == 0 : between auxiliary and destination
// For each pair there is only one legal direction (smaller disk on top of
// the larger one / empty peg), so we just make that move.
// If N is even, the auxiliary and destination pegs are swapped so the
// tower ends up on the destination peg.
//
// Complexity analysis:
//   - The minimum number of moves is T(N) = 2T(N-1) + 1 = 2^N - 1.
//   - Every move is O(1) (two stack tops are compared, one push/pop).
//   - Time:  O(2^N) - exponential, the same for best/worst case because
//            the number of moves is fixed for a given N.
//   - Space: O(N) - the three stacks together hold exactly N disks
//            (no recursion stack is needed, unlike the recursive version).

#include <iostream>
#include <stack>
#include <limits>
#include <cstdlib>
using namespace std;

struct Peg {
    char name;
    stack<int> disks;
};

class TowerOfHanoi {
    int n;
    Peg pegs[3];         // 0 = source, 1 = auxiliary, 2 = destination
    long long moveCount;
    bool printMoves;

    void moveDisk(Peg& from, Peg& to) {
        int disk = from.disks.top();
        from.disks.pop();
        to.disks.push(disk);
        moveCount++;
        if (printMoves)
            cout << "Move " << moveCount << ": disk " << disk
                 << " from " << from.name << " to " << to.name << endl;
    }

    // make the only legal move between two pegs
    void legalMove(Peg& a, Peg& b) {
        if (a.disks.empty())
            moveDisk(b, a);
        else if (b.disks.empty())
            moveDisk(a, b);
        else if (a.disks.top() < b.disks.top())
            moveDisk(a, b);
        else
            moveDisk(b, a);
    }

public:
    TowerOfHanoi(int disks, char src, char aux, char dst, bool print)
        : n(disks), moveCount(0), printMoves(print) {
        pegs[0].name = src;
        pegs[1].name = aux;
        pegs[2].name = dst;
        for (int d = n; d >= 1; d--)    // largest at the bottom
            pegs[0].disks.push(d);
    }

    void solve() {
        Peg& source = pegs[0];
        Peg* aux = &pegs[1];
        Peg* dest = &pegs[2];
        if (n % 2 == 0)
            swap(aux, dest);

        long long total = (1LL << n) - 1;
        for (long long i = 1; i <= total; i++) {
            if (i % 3 == 1)
                legalMove(source, *dest);
            else if (i % 3 == 2)
                legalMove(source, *aux);
            else
                legalMove(*aux, *dest);
        }
    }

    // destination holds 1..n in order and the other pegs are empty
    bool isSolved() const {
        if (!pegs[0].disks.empty() || !pegs[1].disks.empty())
            return false;
        stack<int> s = pegs[2].disks;
        for (int d = 1; d <= n; d++) {
            if (s.empty() || s.top() != d)
                return false;
            s.pop();
        }
        return s.empty();
    }

    long long getMoveCount() const { return moveCount; }
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
    cout << "===== Tower of Hanoi (Iterative) =====" << endl << endl;

    // move counts grow as 2^N - 1
    cout << "N\tmoves\t2^N - 1\tsolved" << endl;
    for (int n = 1; n <= 20; n++) {
        TowerOfHanoi t(n, 'A', 'B', 'C', false);
        t.solve();
        cout << n << "\t" << t.getMoveCount() << "\t" << ((1LL << n) - 1)
             << "\t" << (t.isSolved() ? "yes" : "no") << endl;
    }
    cout << endl;

    int n = readNumber<int>("Enter number of disks (1 to 25): ");
    if (n < 1 || n > 25) {
        cout << "Please choose between 1 and 25 disks (2^25 moves is already 33 million)." << endl;
        return 0;
    }

    bool print = true;
    if (n > 10) {
        cout << "That is " << ((1LL << n) - 1) << " moves. Print them all? (1 = yes, 0 = no): ";
        print = readNumber<int>("") == 1;
    }

    cout << endl << "Moving " << n << " disk(s) from A to C using B:" << endl;
    TowerOfHanoi game(n, 'A', 'B', 'C', print);
    game.solve();
    cout << endl << "Total moves: " << game.getMoveCount()
         << " (expected " << ((1LL << n) - 1) << ")" << endl;
    cout << "Tower on C: " << (game.isSolved() ? "yes" : "no") << endl;
    return 0;
}
