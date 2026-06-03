#pragma once
#include <fstream>
#include <iostream>
#include <string>

class LibraryItem {
protected:
	int id;
	std::string title;
	int year;
	int totalCopies;
	int availableCopies;
	LibraryItem();
public:
	LibraryItem(int id, const std::string& title, int year, int copies);
	virtual ~LibraryItem() {}

	virtual void display() const = 0;
	virtual std::string getType() const = 0;

	int getId()              const { return id; }
	const std::string& getTitle() const { return title; }
	int getYear()            const { return year; }
	int getTotalCopies()     const { return totalCopies; }
	int getAvailableCopies() const { return availableCopies; }

	void issueOne();
	void returnOne();
};

class Book : public LibraryItem {
private:
	std::string author;
public:
	Book();
	Book(int id, const std::string& title, const std::string& author,
		int year, int copies);
	void display() const override;
	std::string getType() const override { return "Book"; }
	void writeTo(std::ofstream& out) const;
	void readFrom(std::ifstream& in);
	const std::string& getAuthor() const { return author; }
};

class Journal : public LibraryItem {
private:
	int issueNumber;
	std::string periodicity;
public:
	Journal();
	Journal(int id, const std::string& title, int year, int copies,
		int issue, const std::string& periodicity);
	void display() const override;
	std::string getType() const override { return "Journal"; }
	void writeTo(std::ofstream& out) const;
	void readFrom(std::ifstream& in);
	int getIssueNumber() const { return issueNumber; }
	const std::string& getPeriodicity() const { return periodicity; }
};
