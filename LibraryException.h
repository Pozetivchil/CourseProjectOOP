#pragma once
#include <string>

class LibraryException {
private:
	std::string message;
public:
	LibraryException(const std::string& msg) : message(msg) {}
	const std::string& what() const { return message; }
};
