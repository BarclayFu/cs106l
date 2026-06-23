#include <string>
#include "class.h"
Student::Student(){
    name = "Unknown";
    age = 0;
}

Student::Student(const std::string& name, int age){
    setName(name);
    setAge(age);
}

void Student::setAge(int age){
    if (isValidAge(age)) {
        this->age = age;
    }
}

void Student::setName(const std::string& name){
    this->name = name;
}

std::string Student::getName() const{
    return name;
}

int Student::getAge() const{
    return age;
}

bool Student::isValidAge(int age) const {
    return age >= 0 && age <= 150;
}