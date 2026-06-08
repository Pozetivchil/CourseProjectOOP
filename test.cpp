#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;

typedef double (*action)(double a, double b);

class Ariphmetic {
public:
    static double Sum(double a, double b) {
        return a + b;
    }
    static double Sub(double a, double b) {
        return a - b;
    }
    void Ariph(double a, double b, action act) {
        cout << act(a, b);
    }
};

using std::string;

class Strings {
    string test = "test";
public:
    Strings() {
        cout << test.length() << endl;
        cout << test.size() << endl;
        test += test;   cout << test.substr(1, 6) << endl;
        cout << (test < "123456789") << endl;
        cout << (test == "123456789") << endl;
        cout << test.find("st") << endl;
        test.append("test_appended");   cout << test << endl;
        test.insert(0, "inserted_");    cout << test << endl;
        test.replace(0, 4, "replaced");   cout << test << endl;
        test.clear();   cout << test.empty() << endl;
    }
};
// lenght, size, +=, <, ==, substr, find, append, insert, replace, clear, empty
int main() {
    Ariphmetic ariphmetic;
    ariphmetic.Ariph(3, 4, &Ariphmetic::Sum);

    Strings strings;

    return 0;
}

