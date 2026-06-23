#pragma once

#include <string>

template <typename T>
class Student {
public:
    Student();

    Student (const std::string& name, T age);

    void setAge(T age);
    void setName(const std::string& name);

    std::string getName() const;
    T getAge() const;

private:
    std::string name;
    T age;
    bool isValidAge(T age) const;
};

#include "class.cpp"
