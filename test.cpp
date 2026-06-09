#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;

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


class Strings {
    string test = "test";
    int b = 3;
    int &e;
public:
    Strings() : e(b){
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

class Human{

public:
    Human(){
        
        brain.Think();
    }
private:
    class Brain{ // Класс Brain внутри класса Human
    public:
        void Think(){
            cout << "I think" << endl;
        }
    };
    Brain brain;
};

class Animal {
    public:
    virtual void Test() {
        cout << 11;
    }
};

class Dog : public Animal {
    public:
    virtual void Test() override {
        cout << 55;
    }
};


#include <iostream>
#include <stdexcept>

// Шаблон позволяет контейнеру хранить любой тип данных (T)
template <typename T>
class MyContainer {
private:
    T* data;
    int size;

public:
    // Конструктор: выделяем память
    MyContainer(int n) : size(n) {
        data = new T[n];
    }

    // Деструктор: освобождаем память
    ~MyContainer() {
        delete[] data;
    }

    // ЗАЩИТА: Запрещаем копирование, чтобы избежать ошибки "двойного удаления"
    MyContainer(const MyContainer&) = delete;
    MyContainer& operator=(const MyContainer&) = delete;

    // Метод записи
    void set(int index, T value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }

    // Метод чтения с правильной обработкой ошибок
    T get(int index) {
        if (index >= 0 && index < size) {
            return data[index];
        }
        throw std::out_of_range("Index out of range"); 
    }

    int getSize() {
        return size;
    }
};

int main() {
    Ariphmetic ariphmetic;
    ariphmetic.Ariph(3, 4, &Ariphmetic::Sum);

    Strings strings;

    cout << endl;

    ifstream file("books.txt");
    string line;
    if(file.is_open()) {
        while (getline(file, line)) { 
            cout << line;
        }
    }
    file.close();
    
    cout << endl;

    Human human;

    cout << endl;

    Animal* anim; 
    anim = new Animal();
    anim -> Test();
    anim = new Dog();
    anim -> Test();

    cout << endl;

    MyContainer<int> intArr(5);
    intArr.set(0, 10);
    intArr.set(1, 20);
    
    std::cout << "Int element: " << intArr.get(0) << "\n";
    std::cout << "Size: " << intArr.getSize() << "\n\n";

    // Тот же класс, но теперь работает с дробными числами
    MyContainer<double> doubleArr(3);
    doubleArr.set(0, 3.14);
    
    std::cout << "Double element: " << doubleArr.get(0) << "\n";


    return 0;
}

