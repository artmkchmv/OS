#include <iostream>
#include <map>

using namespace std;

int cinCheckLen() {
    int len;
    cout << "Enter the length of the sequence >> ";
    while (!(cin >> len) || (cin.peek() != '\n') || !(len > 0)) {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Invalid input!\n" << "Enter the length of the sequence >> ";
    }
    return len;
}

int cinCheckNum() {
    int num;
    while (!(cin >> num) || (cin.peek() != '\n')) {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Invalid input!\n" << "Enter a valid integer >> ";
    }
    return num;
}

char validContinue() {
    cout << "Do you want to continue? (y/n) >> ";
    char answer;
    cin >> answer;
    answer = tolower(answer);
    while ((answer != 'y') && (answer != 'n') || cin.peek() != '\n') {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Invalid input!\n" << "Do you want to continue? (y/n) >> ";
    }
    return answer;
}

int main() {
    char answer;
    map<int, int> countMap;
  
    do {
        int n, number, maxCount = 0, resultNumber;
        n = cinCheckLen();
        cout << "Enter the sequence of numbers: ";
        for (int i = 0; i < n; ++i) {
            number = cinCheckNum();
            countMap[number]++;
            if (countMap[number] > maxCount) {
                maxCount = countMap[number];
                resultNumber = number;
            }
        }

        cout << "The number that appears most frequently is " << resultNumber << "\n";
        answer = validContinue();
        resultNumber = 0;
        countMap.clear();

    } while (answer == 'y');

    return 0;
}
