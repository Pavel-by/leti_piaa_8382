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

int main() {
    string P;
    getline(cin, P);

    // Array of prefixes
    int pi[P.length()];
    pi[0] = 0;
    // size - current prefix(postfix)'s length
    // i - current symbol (last postfix's symbol position)
    // j is used in the next cycle.
    int size = 0, i = 1, j;

    while (i < P.length()) {
        // If symbols are equals, move cursor and increase postfix's size
        if (P[size] == P[i]) {
            log("Suffix's size increased. Suffix on " + to_string(i) + " is " + to_string(size + 1));
            pi[i] = size + 1;
            size++;
            i++;
            continue;
        }

        // If symbols are not equals and postfix's length is 0, maximum length of prefix and postfix
        // when they are equals is 0
        if (size == 0) {
            log("Suffix's size is 0 on " + to_string(i));
            pi[i] = 0;
            i++;
            continue;
        }

        log("Decrease suffix size from " + to_string(size) + " to " + to_string(pi[size - 1]));
        // Decrease postfix's size until it's possible and prefix is not equals suffix with that size
        size = pi[size - 1];
    }

    // Current symbol's index in string
    i = 0;

    // Current symbol's index in image
    j = 0;

    // True if there yet was not found any position, false otherwise
    bool isFirst = true;

    // If image is empty, there is no equals substrings (or any position is available)
    if (P.length() == 0) {
        log("P's length is 0. End.");
        cout << "-1";
        return 0;
    }

    // Current symbol in string
    char c;
    cin >> c;

    // Read while we can
    while (!cin.eof()) {
        // If j is outside of image, we have found image in the string.
        // We should print found position and decrease j on the nearest available
        // position, so P[0:j-1] is equals string[i-j;i-1]
        if (j == P.length()) {
            log("Found ingoing on position " + to_string(i - P.length()));
            printPosition(isFirst, (int) (i - P.length()));
            j = pi[j-1];
            continue;
        }

        // If current symbol in string is equals with current symbol in image,
        // move cursors to the next positions
        if (c == P[j]) {
            log("Symbols on T[" + to_string(i) + "], P[" + to_string(j) + "] are equals. Increase equals part size.");
            i++;
            j++;
            cin >> c;
            continue;
        }

        // If j is 0 and P[j] is not equals to c, just move cursor in string
        if (j == 0) {
            log("There is no equals substring on " + to_string(i) + " position. Increase T's cursor");
            i++;
            cin >> c;
            continue;
        }

        log("Decrease P's cursor.");
        // Decrease image's cursor while j is not 0 and P[j] is not equals to c
        j = pi[j-1];
    }

    // If j==P.length(), there was found image position at the end of string
    if (j == P.length()) {
        log("Found ingoing on position " + to_string(i - P.length()));
        printPosition(isFirst, (int) (i - P.length()));
    }

    // If isFirst is false (or if there was not found any substrings in string
    // equals to P, print -1
    if (isFirst) {
        log("No ingoing found in T. End.");
        cout << "-1";
    } else {
        log("End.");
    }
}