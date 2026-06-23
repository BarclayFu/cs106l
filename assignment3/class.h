#include <string>

class Student {
public:
    Student();

    Student (const std::string& name, int age);

    void setAge(int age);
    void setName(const std::string& name);

    std::string getName() const;
    int getAge() const;

private:
    std::string name;
    int age;
    bool isValidAge(int age) const;
};