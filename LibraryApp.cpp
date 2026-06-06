#include "LibraryApp.h"
#include "LibraryException.h"
#include <iostream>
using namespace std;
LibraryApp::LibraryApp() : running(true), currentDate(Date::today()) {
    library.loadAll();
}
string LibraryApp::readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

bool LibraryApp::isBlank(const string& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] != ' ' && s[i] != '\t') return false;
    }
    return true;
}

string LibraryApp::readNonEmpty(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        if (!isBlank(s)) return s;
        cout << "  Ошибка: значение не может быть пустым.\n";
    }
}

int LibraryApp::readInt(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        bool ok = !s.empty();
        int sign = 1;
        size_t i = 0;
        if (ok && s[0] == '-') {
            sign = -1; ++i;
            if (i == s.size()) ok = false;
        }
        int value = 0;
        for (; ok && i < s.size(); ++i) {
            if (s[i] < '0' || s[i] > '9') { ok = false; break; }
            value = value * 10 + (s[i] - '0');
        }
        if (ok) return value * sign;
        cout << "  Ошибка: ожидалось целое число.\n";
    }
}
int LibraryApp::readPositiveInt(const string& prompt) {
    while (true) {
        int v = readInt(prompt);
        if (v > 0) return v;
        cout << "  Ошибка: значение должно быть положительным.\n";
    }
}
void LibraryApp::run() {
    cout << "Текущая дата: ";
    currentDate.print(cout);
    cout << "\n";
    while (running) {
        cout << "\n=== Главное меню ===\n"
            << "1. Книги и журналы\n"
            << "2. Читатели\n"
            << "3. Выдачи\n"
            << "4. Поиск\n"
            << "5. Отчёты\n"
            << "6. Сохранить данные\n"
            << "0. Выход\n";
        int cmd = readInt("Выбор: ");
        try {
            switch (cmd) {
            case 1: menuBooks();   break;
            case 2: menuReaders(); break;
            case 3: menuLoans();   break;
            case 4: menuSearch();  break;
            case 5: menuReports(); break;
            case 6: doSave();      break;
            case 0: doExit();      break;
            default:
                cout << "Неизвестная команда.\n";
            }
        }
        catch (const LibraryException& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}
void LibraryApp::printAllItems() const {
    if (library.getItems().empty()) {
        cout << "Фонд пуст.\n";
        return;
    }

    for (auto p : library.getItems()) p->display();
}
void LibraryApp::printAllReaders() const {
    if (library.getReaders().empty()) {
        cout << "Список читателей пуст.\n";
        return;
    }
    for (const auto& r : library.getReaders()) {
        cout << "#" << r.getId() << "  " << r.getFullName()
            << "  билет: " << r.getTicketNumber()
            << "  активных: " << r.getActiveLoansCount() << "\n";
    }
}
void LibraryApp::printLoan(const Loan& l) const {
    const LibraryItem* item = library.findItem(l.getItemId());
    const Reader* r = library.findReader(l.getReaderId());
    cout << "#" << l.getId() << "  ";
    if (r != nullptr) cout << r->getFullName(); else cout << "(нет читателя)";
    cout << " — ";
    if (item != nullptr) cout << item->getTitle(); else cout << "(нет издания)";
    cout << "  до ";
    l.getDueDate().print(cout);
    cout << "\n";
}
void LibraryApp::menuBooks() {
    cout << "\n-- Книги и журналы --\n"
        << "1. Список\n2. Добавить книгу\n3. Добавить журнал\n"
        << "4. Удалить (по id)\n0. Назад\n";
    int c = readInt("Выбор: ");
    if (c == 1) {
        printAllItems();
    }
    else if (c == 2) {
        string t = readNonEmpty("Название: ");
        string a = readNonEmpty("Автор: ");
        int y = readInt("Год издания: ");
        int n = readPositiveInt("Количество экземпляров: ");
        library.addBook(t, a, y, n);
        cout << "Книга добавлена.\n";
    }
    else if (c == 3) {
        string t = readNonEmpty("Название: ");
        int y = readInt("Год: ");
        int n = readPositiveInt("Количество экземпляров: ");
        int i = readPositiveInt("Номер выпуска: ");
        string p = readNonEmpty("Периодичность: ");
        library.addJournal(t, y, n, i, p);
        cout << "Журнал добавлен.\n";
    }
    else if (c == 4) {
        int id = readPositiveInt("id для удаления: ");
        if (library.removeItem(id)) cout << "Удалено.\n";
        else                        cout << "Не найдено.\n";
    }
}
void LibraryApp::menuReaders() {
    cout << "\n-- Читатели --\n"
        << "1. Список\n2. Зарегистрировать\n3. Удалить (по id)\n0. Назад\n";
    int c = readInt("Выбор: ");
    if (c == 1) {
        printAllReaders();
    }
    else if (c == 2) {
        string ln = readNonEmpty("Фамилия: ");
        string fn = readNonEmpty("Имя: ");
        string mn = readLine("Отчество (можно пропустить): ");
        if (isBlank(mn)) mn = "";
        string tk = readNonEmpty("Номер билета: ");
        library.registerReader(ln, fn, mn, tk);
        cout << "Читатель зарегистрирован.\n";
    }
    else if (c == 3) {
        int id = readPositiveInt("id: ");
        if (library.removeReader(id)) cout << "Удалён.\n";
        else                          cout << "Не найден.\n";
    }
}
void LibraryApp::menuLoans() {
    cout << "\n-- Выдачи --\n"
        << "1. Выдать\n2. Принять возврат\n"
        << "3. Активные\n4. Просроченные\n0. Назад\n";
    int c = readInt("Выбор: ");
    if (c == 1) {
        int id = readPositiveInt("id издания: ");
        string tk = readNonEmpty("Билет читателя: ");
        int loanId = library.issueLoan(id, tk, currentDate);
        cout << "Выдача оформлена. Номер записи: " << loanId << "\n";
    }
    else if (c == 2) {
        int id = readPositiveInt("id записи о выдаче: ");
        if (library.returnLoan(id, currentDate)) cout << "Возврат принят.\n";
        else                                     cout << "Запись не найдена или уже закрыта.\n";
    }
    else if (c == 3) {
        auto active = library.activeLoans();
        if (active.empty()) cout << "Нет активных выдач.\n";
        for (auto l : active) printLoan(*l);
    }
    else if (c == 4) {
        auto over = library.overdueLoans(currentDate);
        if (over.empty()) cout << "Просроченных нет.\n";
        for (auto l : over) {
            printLoan(*l);
            cout << "    просрочено дней: " << l->overdueDays(currentDate) << "\n";
        }
    }
}
void LibraryApp::menuSearch() {
    cout << "\n-- Поиск --\n"
        << "1. По названию (подстрока)\n2. По автору (подстрока)\n3. По году\n0. Назад\n";
    int c = readInt("Выбор: ");
    if (c == 1) {
        string q = readNonEmpty("Запрос: ");
        auto found = library.searchByTitle(q);
        if (found.empty()) cout << "Ничего не найдено.\n";
        for (auto p : found) p->display();
    }
    else if (c == 2) {
        string q = readNonEmpty("Автор: ");
        auto found = library.searchByAuthor(q);
        if (found.empty()) cout << "Ничего не найдено.\n";
        for (auto b : found) b->display();
    }
    else if (c == 3) {
        int y = readInt("Год: ");
        auto found = library.searchByYear(y);
        if (found.empty()) cout << "Ничего не найдено.\n";
        for (auto p : found) p->display();
    }
}
void LibraryApp::menuReports() {
    int booksCount = 0, journalsCount = 0;
    for (auto p : library.getItems()) {
        if (dynamic_cast<const Book*>(p))    ++booksCount;
        if (dynamic_cast<const Journal*>(p)) ++journalsCount;
    }
    cout << "\nВсего книг: " << booksCount
        << "\nВсего журналов: " << journalsCount
        << "\nВсего читателей: " << library.getReaders().size()
        << "\nВсего выдач: " << library.getLoans().size()
        << "\nАктивных выдач: " << library.activeLoans().size()
        << "\nПросроченных: " << library.overdueLoans(currentDate).size()
        << "\n";
}
void LibraryApp::doSave() {
    library.saveAll();
    cout << "Данные сохранены.\n";
}
void LibraryApp::doExit() {
    string ans = readLine("Сохранить перед выходом? (y/n): ");
    if (ans == "y" || ans == "Y") library.saveAll();
    running = false;
}
