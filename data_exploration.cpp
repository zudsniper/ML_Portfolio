#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <valarray>
#ifdef _WIN32
#include "windows.h"
#define PLATFORM "windows"
#endif
#ifdef __CYGWIN__
#include "windows.h"
#define PLATFORM "windows"
#endif

using namespace std;

enum COLOR_CODE {
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BLUE     = 34,
    FG_WHITE    = 97,
    FG_DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_DEFAULT  = 49
};

const string RESET_ALL = "\033[0m";

string color(COLOR_CODE color);
string bold(const string& text);
string underline(const string& text);

void print_stats(const vector<double>& vec);

double sum(const vector<double>& vec);
double mean(const vector<double>& vec);
double median(vector<double> vec);
double range(vector<double> vec);

double covar(vector<double> actual, std::vector<double> expected);
double cor(const vector<double>& actual, const std::vector<double>& expected);

int main(int argc, char** argv) {

    //this is necessary for console colors on windows
    if(PLATFORM && PLATFORM == "windows") {
        system(("chcp "s + std::to_string(CP_UTF8)).c_str());
    }

    ifstream inFS;
    string line;
    string rm_in, medv_in;

    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    cout << "\n------------------------------------" << endl;
    cout << "Trying to open Boston.csv... ";

    inFS.open("./Boston.csv");
    if(!inFS.is_open()) {
        cout << color(FG_RED) << "Could not open Boston.csv" << color(FG_DEFAULT) << endl;
        return 1;
    }
    cout << color(FG_GREEN) << "Opened." << color(FG_DEFAULT) << endl;

    cout << "reading column labels" << endl;
    getline(inFS, line);

    int numObservations = 0;
    while(inFS.good()) {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "new len: " << rm.size() << endl;
    cout << "closing file Boston.csv... ";
    inFS.close();
    cout << color(FG_GREEN) << "Closed." << color(FG_DEFAULT) << endl;
    cout << "------------------------------------\n" << endl;

    cout << bold("# records") << ": " << numObservations << endl;


    cout << "\n" << underline("Stats for rm") << endl;
    print_stats(rm);

    cout << "\n" << underline("Stats for medv") << endl;
    print_stats(medv);

    cout << "\n " << bold("Covariance") << "  = " << covar(rm, medv) << endl;
    cout << "\n " << bold("Correlation") << " = " << cor(rm, medv) << endl;

    cout << "\n" << color(BG_GREEN) << color(FG_WHITE) <<  bold("End.") << color(BG_DEFAULT) << color(BG_DEFAULT) << endl;

    return 0;
}

double sum(const vector<double>& vec) {
    if(vec.empty()) {
        return 0;
    }

    double s = 0;
    for (double i : vec) {
        s = s + i;
    }
    return s;
}

double mean(const vector<double>& vec) {
    if(vec.empty()) {
        return -1;
    }
    return sum(vec) / vec.size();
}

double median(vector<double> vec) {

    if(vec.empty()) {
        return -1;
    }

    if(vec.size() == 1) {
        return vec[0];
    }

    std::sort(vec.begin(), vec.end());

    unsigned int temp = vec.size() / 2;
    double mid;
    if(temp % 2 == 0) {
        mid = (vec[temp] + vec[temp - 1]) / 2;
    } else {
        mid = vec[temp];
    }
    return mid;
}

double range(vector<double> vec) {
    if(vec.empty()) {
        return 0;
    }

    std::sort(vec.begin(), vec.end());

    return (vec[vec.size() - 1] - vec[0]);
}

double cor(const vector<double>& actual, const std::vector<double>& expected) {
    if(actual.size() != expected.size()) {
        return -1;
    }

    if(actual.empty() || expected.empty()) {
        return 0;
    }

    if(actual.size() == 1) {
        return 0;
    }

    double aStd = sqrt(covar(actual, actual));
    double eStd = sqrt(covar(expected, expected));

    return covar(actual, expected) / (aStd * eStd);
}

double covar(vector<double> actual, std::vector<double> expected) {
    if(actual.size() != expected.size()) {
        return -1;
    }

    if(actual.empty() || expected.empty()) {
        return 0;
    }

    if(actual.size() == 1) {
        return 0;
    }

    double aMean = mean(actual);
    double eMean = mean(expected);

    double resMultSum = 0;
    for (int i = 0; i < actual.size(); ++i) {
        resMultSum = resMultSum + ((actual[i] - aMean) * (expected[i] - eMean));
    }

    return resMultSum / (actual.size() - 1);
}

void print_stats(const vector<double>& vec) {
    cout << " " << bold("sum") << "    : " << sum(vec) << endl;
    cout << " " << bold("mean") << "   : " << mean(vec) << endl;
    cout << " " << bold("median") << " : " << median(vec) << endl;
    cout << " " << bold("range") << "  : " << range(vec) << endl;
}

string color(COLOR_CODE color) {
    return "\033[" + to_string(color) + "m";
}

string bold(const string& text) {
    string bold = "\033[1m";
    string reset = RESET_ALL;

    return bold + text + reset;
}

string underline(const string &text) {
    string line = "\033[4m";
    string reset = RESET_ALL;

    return line + text + reset;
}




