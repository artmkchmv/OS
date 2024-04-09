#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>

using namespace std;

int cinCheckInt() {

    int temp;
    cout << "Enter the number of equations >> ";
    while (!(cin >> temp) || (cin.peek() != '\n') || (temp < 2)) {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Invalid input!" << endl;
        cout << "Enter the number of equations >> ";
    }

    return temp;
}

double cinCheckDouble(int index1, int index2) {

    double temp;
    cout << "Enter the x" << "[" << index1 + 1 << "]" << "[" << index2 + 1 << "]" << " >> ";

    while (!(cin >> temp) || (cin.peek() != '\n')) {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Invalid input!" << endl;
        cout << "Enter the x" << "[" << index1 + 1 << "]" << "[" << index2 + 1 << "]" << " >> ";
    }

    return temp;
}

vector<vector<double>> getMinor(vector<vector<double>> matrix, int row, int col) {

    vector<vector<double>> minor;

    for (int i = 0; i < matrix.size(); ++i) {
        if (i != row) {
            vector<double> tempRow;
            for (int j = 0; j < matrix.size(); ++j) {
                if (j != col) {
                    tempRow.push_back(matrix[i][j]);
                }
            }
            minor.push_back(tempRow);
        }
    }

    return minor;
}

double det(vector<vector<double>> matrix) {

    if (matrix.size() == 1) {
        return matrix[0][0];
    }

    double result = 0;
    int sign = 1;

    for (int j = 0; j < matrix.size(); ++j) {
        vector<vector<double>> minor = getMinor(matrix, 0, j);
        result += sign * matrix[0][j] * det(minor);
        sign = -sign;
    }

    return result;
}

vector<double> CramerMethod(vector<vector<double>> matrix, vector<double> consts) {

    vector<double> result;
    
    double detMain = det(matrix);

    if (detMain == 0) {
        cout << "Matrix is degenerate. Cramer's method is not suitable." << endl;
        return result;
    }

    vector<vector<double>> temp = matrix;

    for (int j = 0; j < consts.size(); ++j) {
        vector<vector<double>> temp = matrix;
        for (int i = 0; i < consts.size(); ++i) {
            temp[i][j] = consts[i];
        }
        double detTemp = det(temp);
        result.push_back(detTemp / detMain);
    }

    return result;
}

void help() {
    cout << "This program use Cramer's method for the solution of a system of linear equations." << endl;
    cout << "If you want to find the solution of a system of linear equations run program without keys." << endl;
}

int main(int argc, char* argv[]) {
    if (argc == 2 && !strcmp(argv[1], "--help")) {
        help();
        return 0;
    } else if (argc != 1) {
        cerr << "Run program with '--help' for information." << endl;
        return 1;
    } else {
        int channel_to_server[2];
        int channel_to_client[2];

        pipe(channel_to_server);
        pipe(channel_to_client);

        int SIZE = cinCheckInt();

        pid_t pid = fork();

        if (!pid) {

            close(channel_to_server[1]);
            close(channel_to_client[0]);

            vector<vector<double>> coef(SIZE, vector<double>(SIZE, 0));
            vector<double> consts(SIZE);

            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    read(channel_to_server[0], &coef[i][j], sizeof(double));
                }
            }

            for (int i = 0; i < SIZE; ++i) {
                read(channel_to_server[0], &consts[i], sizeof(double));
            }

            close(channel_to_server[0]);

            vector<double> result = CramerMethod(coef, consts);

            for (int i = 0; i < SIZE; ++i) {
                write(channel_to_client[1], &result[i], sizeof(double));
            }

            close(channel_to_client[1]);
        } 
        else if (pid > 0) {

            close(channel_to_server[0]);
            close(channel_to_client[1]);

            vector<vector<double>> Coef(SIZE, vector<double>(SIZE, 0));
            vector<double> ConstTerms;

            double tempElem;

            cout << "Enter the coefficients of equations:" << endl;
            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    tempElem = cinCheckDouble(i, j);
                    Coef[i][j] = tempElem;
                }
            }

            cout << "Enter the constants terms:" << endl;
            for (int i = 0; i < SIZE; ++i) {
                tempElem = cinCheckDouble(i, SIZE);
                ConstTerms.push_back(tempElem);
            }
            
            double temp;

            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    temp = Coef[i][j];
                    write(channel_to_server[1], &temp, sizeof(double));
                }
            }

            for (int i = 0; i < SIZE; ++i) {
                double temp = ConstTerms[i];
                write(channel_to_server[1], &temp, sizeof(double));
            }

            vector<double> result;

            for (int i = 0; i < SIZE; ++i) {
                double temp;
                read(channel_to_client[0], &temp, sizeof(double));
                result.push_back(temp);
            }
            
            cout << "Result of calculations: " << endl;
            for (int i = 0; i < SIZE; i++) {
                cout << "x" << i << " = " << result[i] << endl;
            }

            close(channel_to_server[1]);
            close(channel_to_client[0]);
            
        } else {
            cerr << "Error! Fork doesn't exist!" << endl;
            return 1;
        }

        return 0;
    }
}
