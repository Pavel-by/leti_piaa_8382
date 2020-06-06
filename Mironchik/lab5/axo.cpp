#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

/**
 * Размер алфавита - разница между первым возможным символом алфавита и последним.
 */
#define ALPHABET_SIZE 32

void log(const string &message, bool wrapLine = true) {
    static bool LOG_ENABLED = true;

    if (LOG_ENABLED) {
        cout << message;

        if (wrapLine)
            cout << endl;
    }
}

/**
 * Вершина в дереве алгоритма.
 */
class Node {
public:
    /**
     * Массив дочерних вершин. Если вершина [c] не равна nullptr, то ребенок есть.
     */
    Node *son[ALPHABET_SIZE];

    /**
     * Переходы их данной вершины по символу (кеш для функции Axo::getLink).
     */
    Node *go[ALPHABET_SIZE];

    /**
     * Родительская вершина.
     */
    Node *parent = nullptr;

    /**
     * Ссылка на подходящий для текущего пути (от root до вершины) суффикс.
     */
    Node *suffLink = nullptr;

    /**
     * Ближайшая сжатая ссылка (указывающая на ближайший суффикс, последний символ которого который является терминалом).
     */
    Node *up = nullptr;

    /**
     * Символ между родительской вершиной и текущей.
     */
    char charToParent = 0;

    /**
     * Является ли вершина терминальной.
     */
    bool isLeaf = false;

    /**
     * Список паттернов, кончающихся в данной вершине.
     */
    vector<int> leafPatternNumber;

    /**
     * Расстояние от вершины до корня.
     */
    int index;

    /**
     * Количество дуг, которые выходят из вершины.
     */
    int sonsCount = -1;

    Node() {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            son[i] = nullptr;
            go[i] = nullptr;
        }
    }

    ~Node() {
        for (auto v : son)
            delete v;
    }
};

/**
 * Основной класс, занимающийся обработкой данных.
 */
class Axo {
public:
    /**
     * Количество обработанных паттернов (используется в модифицированном алгоритме).
     */
    int patternsCount = 0;

    /**
     * Корневая вершина дерева.
     */
    Node *root = new Node();

    ~Axo() {
        delete root;
    }

    /**
     * Функция получения суффикса для вершины v. Результат функции кешируется.
     *
     * @param v Вершина, суффикс которой необходимо найти.
     * @return Вершина, являющаяся последним символом максимального суффикса.
     */
    Node *getSuffLink(Node *v) {
        if (v->suffLink == nullptr) {
            log("Compute suffLink for symbol with code " + to_string(v->charToParent + 'A') + " on position " +
                to_string(v->index));
            if (v == root || v->parent == root)
                v->suffLink = root;
            else
                v->suffLink = getLink(getSuffLink(v->parent), v->charToParent);
        }

        return v->suffLink;
    }

    /**
     * Найти максимальный суффикс [v]+c. Рассчитанные значения кешируются.
     *
     * @param v Вершина, от коорой строится путь.
     * @param c Символ пути.
     * @return Вершина - последний символ максимального суффикса.
     */
    Node *getLink(Node *v, char c) {
        if (v->go[c] == nullptr) {
            log("Compute link for symbol with code " + to_string(v->charToParent + 'A') + " on position " +
                to_string(v->index));
            if (v->son[c] != nullptr)
                v->go[c] = v->son[c];
            else if (v == root)
                v->go[c] = root;
            else
                v->go[c] = getLink(getSuffLink(v), c);
        }

        return v->go[c];
    }

    /**
     * Найти сжатую ссылку для вершины v. Полученные значения кешируются.
     *
     * @param v Вершина, для которой необходимо найти сжатую ссылку.
     * @return Сжатая ссылка для вершины v.
     */
    Node *getUp(Node *v) {
        if (v->up == nullptr) {
            log("Compute up link for symbol with code " + to_string(v->charToParent + 'A') + " on position " +
                to_string(v->index));
            if (getSuffLink(v)->isLeaf)
                v->up = getSuffLink(v);
            else if (getSuffLink(v) == root)
                v->up = root;
            else
                v->up = getUp(getSuffLink(v));
        }

        return v->up;
    }

    /**
     * Добавить паттерн в дерево
     *
     * @param s Паттерн
     * @param number Идентификатор паттерна. В случае с добавлением паттернов модифицированного алгоритма в роли
     * идентификатора выступает смещение фрагмента относительно начала паттерна.
     */
    void addString(const string &s, int number) {
        log("Add string to bor: " + s + " with id " + to_string(number));

        Node *cur = root;

        for (int i = 0; i < s.length(); i++) {
            char c = s[i] - 'A';

            // Добавляем элемент, если его нет.
            if (cur->son[c] == nullptr) {
                log("Add new head to bor: " + to_string(c) + " on index " + to_string(i));
                cur->son[c] = new Node();
                cur->son[c]->parent = cur;
                cur->son[c]->charToParent = c;
                cur->son[c]->isLeaf = false;
                cur->son[c]->index = i;
            }

            cur = cur->son[c];
        }

        cur->isLeaf = true;
        cur->leafPatternNumber.push_back(number);
        patternsCount++;
    }

    /**
     * Заполнить дерево паттерном, использующим символ джокера.
     *
     * @param s Паттерн.
     * @param joker Символ джокера.
     */
    void setJokerString(const string &s, char joker) {
        log("Set joker string: " + s + " with joker " + to_string(joker));

        int start = -1;
        int i = 0;

        while (true) {
            if ((i == s.length() || s[i] == joker) && start != -1 && start != i) {
                addString(s.substr(start, i - start), start);
                start = -1;
            }

            if (i == s.length())
                break;

            if (start == -1 && s[i] != joker)
                start = i;

            i++;
        }
    }

    /**
     * Обработать строку s при помощи предварительно заполненного дерева модифицированным алгоритмом.
     *
     * @param s Строка, в которой производится поиск.
     * @param P Применяемый паттерн. Данный паттерн не добавляется в дерево, используется лишь его длина.
     */
    void processJoker(const string &s, const string &P) {
        log("Process string " + s + " with pattern " + P);

        // C[N] - количество паттернов, которые начинаются с индекса N, с учетом смещения относительно паттерна с
        // джокером.
        int C[s.length()];

        for (int i = 0; i < s.length(); i++)
            C[i] = 0;

        Node *cur = root;

        // Заполняем массив C
        for (int i = 0; i < s.length(); i++) {
            log("Process symbol with code " + to_string(s[i]) + " on position " + to_string(i));
            char c = s[i] - 'A';
            cur = getLink(cur, c);

            Node *suff = cur;
            while (suff != root) {
                for (int offset : suff->leafPatternNumber) {
                    log("Found subpattern on position " + to_string(i - offset - suff->index + 1) + ": " +
                        s.substr(i - suff->index, suff->index + 1) + ".", false);
                    if (i - offset - suff->index >= 0) {
                        C[i - offset - suff->index]++;
                        log("");
                    } else {
                        log(". Position is lower than 0, so pattern can't be here.");
                    }
                }

                suff = getUp(suff);
            }
        }

        // Выводим заполненный массив. Если C[i] == patternsCount, то с i-го символа начинаются все паттерны (при учете
        // смещения относительно основного паттерна с джокером), а значит достигнуто полное совпадение.
        for (int i = 0; i < s.length(); i++) {
            if (i + P.length() > s.length())
                break;

            if (C[i] == patternsCount) {
                log("Found pattern on position " + to_string(i + 1) + ". ", false);
                log("By the algorithm, pattern considered found on position k when m subpatterns founded on position k"
                    ", where m - total count of subpatterns. So, as you can see, in that case " + to_string(C[i])
                    + " subpatterns was found on position " + to_string(i) + ", and we can say, that pattern " + P
                    + " is really located here.");
                cout << i + 1 << endl;
            } else {
                log("Found " + to_string(C[i]) + " subpatterns on position " + to_string(i + 1));
            }
        }
    }

    /**
     * Обработать строку s стандартным алгоритмом, используя предзаполненное дерево паттернов.
     *
     * @param s Строка, в которой производится поиск.
     */
    vector<pair<int, int>> process(const string &s) {
        log("Process string " + s);

        Node *cur = root;
        vector<pair<int, int>> result;

        // Ищем вхождения паттернов в строку
        for (int i = 0; i < s.length(); i++) {
            log("Process symbol with code " + to_string(s[i]) + " on position " + to_string(i));
            char c = s[i] - 'A';
            cur = getLink(cur, c);
            Node *suff = cur;

            while (suff != root) {
                for (int number : suff->leafPatternNumber) {
                    log("Found pattern " + s.substr(i - suff->index, suff->index + 1) + " on position " +
                        to_string(i - suff->index + 1));
                    result.emplace_back(i - suff->index + 1, number);
                }

                suff = getUp(suff);
            }
        }

        log("Sorting results");
        // Сортируем и выводим результаты
        sort(result.begin(), result.end(), cmp);
        for (auto &val : result) {
            cout << val.first << " " << val.second << endl;
        }

        return result;
    }

    /**
     * Найти вершину, из которой выходит наибольшее количество дуг.
     *
     * @return Вершина, из которой выходит наибольшее количество дуг.
     */
    Node *findMaxNode() {
        return findMaxNode(root);
    }

    /**
     * Найти вершину, из которой выходит наибольшее количество дуг.
     *
     * @param v Стартовая вершина - корень дерева, по которому производится поиск.
     * @return Вершина, из которой выходит наибольшее количество дуг.
     */
    Node *findMaxNode(Node *v) {
        Node *maxSon = nullptr;

        for (Node *son : v->son) {
            if (son == nullptr)
                continue;

            son = findMaxNode(son);

            if (maxSon == nullptr) {
                maxSon = son;
                continue;
            }

            if (getSonsCount(maxSon) < getSonsCount(son))
                maxSon = son;
        }

        if (maxSon != nullptr && getSonsCount(maxSon) > getSonsCount(v))
            return maxSon;

        return v;
    }

    /**
     * Получить количество дуг, которые идут из вершины.
     *
     * @param v Вершина, количество дуг из которой необходимо найти.
     * @return Количество дуг, идущих из вершины v
     */
    int getSonsCount(Node *v) {
        if (v->sonsCount == -1) {
            v->sonsCount = 0;

            for (Node *son : v->son)
                if (son != nullptr)
                    v->sonsCount++;
        }

        return v->sonsCount;
    }

    /**
     * Функция сравнения результатов поиска стандартным способом.
     *
     * @param a Первый элемент
     * @param b Второй элемент
     * @return Меньше ли первый элемент второго
     */
    static bool cmp(const pair<int, int> &a, const pair<int, int> &b) {
        if (a.first < b.first)
            return true;

        if (a.first > b.first)
            return false;

        return a.second < b.second;
    }
};