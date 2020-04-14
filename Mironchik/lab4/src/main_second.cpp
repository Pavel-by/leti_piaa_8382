#include <iostream>
#include <cstring>

using namespace std;

bool TEST = true;

/**
 * Prints [msg] to cout, is [TEST] is true
 *
 * @param msg Message to print
 */
void log(const string& msg) {
    if (TEST)
        cout << msg << endl;
}

/**
 * Prints [position] and, if [ifFirst] is false, ',' before. Sets [isFirst] to false
 *
 * @param isFirst Should or not ',' be printed
 * @param position Number to print
 */
void printPosition(bool& isFirst, int position) {
    if (!isFirst)
        cout << ",";

    isFirst = false;
    cout << (position);
    log("\n");
}

/**
 * Computes index of [real_position] in string with length [length]
 * @param real_position Real position of cursor
 * @param length Length of string
 * @return real_position % length - position of cursor in string
 */
int index(int real_position, int length) {
    return real_position % length;
}

int main() {
    // image
    string P;
    // string (where image will be searched. Or not:)
    string T;
    // read image and string
    getline(cin, T);
    getline(cin, P);

    // If P is not equals to T, P is not be created from prefix and suffix of
    // T, as it required with task
    if (P.length() != T.length()) {
        cout << "-1";
        return 0;
    }

    // Array of P's prefixes. Memory allocates dynamically because 20Mb's stack is
    // not pass on Stepik :(
    int *pi = new int[P.length()];

    // like in 1st task
    int size = 0, i = 1, j;
    pi[0] = 0;

    // creating prefixes array like if 1st task
    while (i < P.length()) {
        if (P[size] == P[i]) {
            log("Suffix's size increased. Suffix on " + to_string(i) + " is " + to_string(size + 1));
            pi[i] = size + 1;
            size++;
            i++;
            continue;
        }

        if (size == 0) {
            log("Suffix's size is 0 on " + to_string(i));
            pi[i] = 0;
            i++;
            continue;
        }

        log("Decrease suffix size from " + to_string(size) + " to " + to_string(pi[size - 1]));
        size = pi[size - 1];
    }

    i = 0;
    j = 0;
    bool isFirst = true;

    // just one more useless check
    if (P.length() == 0) {
        log("P's length is 0. End.");
        cout << "-1";
        return 0;
    }

    i = 0;
    // length of T (will be used often, so it's just for usability)
    int Tlen = (int) T.length();

    // Iterating while i is less than Tlen*2. It's necessary, because be can have lines like
    // AAAC, CAAA,
    // and we don't need more, because in that case search will start anew
    while (i < T.length() * 2) {
        if (j == P.length()) {
            printPosition(isFirst, (int) (index(i - (int) P.length(), Tlen)));
            log("Found ingoing on position " + to_string(i - P.length()));
            j = pi[j-1];
            break;
        }

        if (T[index(i, Tlen)] == P[j]) {
            log("Symbols on T[" + to_string(i) + "], P[" + to_string(j) + "] are equals. Increase equals part size.");
            i++;
            j++;
            continue;
        }

        if (j == 0) {
            log("There is no equals substring on " + to_string(i) + " position. Increase T's cursor");
            i++;
            continue;
        }

        log("Decrease P's cursor.");
        j = pi[j-1];
    }

    if (isFirst)
        cout << "-1";
}