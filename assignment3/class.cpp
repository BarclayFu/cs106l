#include <string>
#include "class.h"

template <typename T>
Student<T>::Student(){
    name = "Unknown";
    age = T{};
}

template <typename T>
Student<T>::Student(const std::string& name, T age){
    setName(name);
    setAge(age);
}

template <typename T>
void Student<T>::setAge(T age){
    if (isValidAge(age)) {
        this->age = age;
    }
}

template <typename T>
void Student<T>::setName(const std::string& name){
    this->name = name;
}

template <typename T>
std::string Student<T>::getName() const{
    return name;
}

template <typename T>
T Student<T>::getAge() const{
    return age;
}

template <typename T>
bool Student<T>::isValidAge(T age) const {
    return age >= T{} && age <= T{150};
}
