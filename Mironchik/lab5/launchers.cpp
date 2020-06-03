#include <iostream>
#include <cmath>
#include "axo.cpp"

void launchIdz() {
    string T;
    cin >> T;

    int count;
    cin >> count;

    string P[count];
    Axo algh;

    for (int i = 0; i < count; i++) {
        cin >> P[i];
        algh.addString(P[i], i + 1);
    }

    auto result = algh.process(T);

    for (auto& item : result) {
        item.first--;
        item.second--;
    }

    // Поиск вершины с наибольшим количеством дуг.
    int maxSonsCount = algh.getSonsCount(algh.findMaxNode());
    cout << "Maximum child count: " << maxSonsCount << endl;

    int start = 0;
    int index = 0;

    // Вывод строки без вхождений.
    cout << "Line without matched substrings: ";
    while (start < T.length()) {
        if (index < result.size() && start >= result[index].first) {
            log("Found pattern before cursor: [" + to_string(result[index].first) + ":" + to_string(result[index].first + P[result[index].second].length()) + ")");
            start = max<int>(start, result[index].first + P[result[index].second].length());
            index++;
            continue;
        }

        log("Echo symbol " + to_string(T[start]));
        cout << T[start];
        start++;
    }
    cout << endl;
}

/**
 * Запустить стандартный алгоритм Ахо-Корасика.
 */
void launchAxo() {
    string T;
    cin >> T;

    int count;
    cin >> count;

    Axo algh;

    for (int i = 0; i < count; i++) {
        string P;
        cin >> P;
        algh.addString(P, i + 1);
    }

    algh.process(T);
}

/**
 * Запустить алгоритм Ахо-Корасика с масками.
 */
void launchJoker() {
    string T;
    cin >> T;

    string P;
    cin >> P;

    char joker;
    cin >> joker;

    Axo algh;

    algh.setJokerString(P, joker);
    algh.processJoker(T, P);
}
