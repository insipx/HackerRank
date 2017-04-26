#include <iostream>
#include <sstream>
#include <string>
using namespace std;

/*
Enter code for class Student here.
Read statement for specification.
*/

class Student {
  private:
    int age, standard;
    string first_name, last_name;
  public:

    void set_age(int age)
    {
      this->age = age;
    }

    void set_standard(int standard)
    {
      this->standard = standard;
    }

    void set_first_name(string first)
    {
      this->first_name = first;
    }

    void set_last_name(string last)
    {
      this->last_name = last;
    }

    int get_age()
    {
      return this->age;
    }

    string get_first_name()
    {
      return this->first_name;
    }

    string get_last_name()
    {
      return this->last_name;
    }

    int get_standard()
    {
      return this->standard;
    }

    string to_string()
    {
      string temp;
      temp += std::to_string(this->age) + ',' + this->first_name + ','+ this->last_name + ',' + std::to_string(this->standard) + '\n';
      return temp;
    }

};

int main() {
    int age, standard;
    string first_name, last_name;

    cin >> age >> first_name >> last_name >> standard;

    Student st;
    st.set_age(age);
    st.set_standard(standard);
    st.set_first_name(first_name);
    st.set_last_name(last_name);

    cout << st.get_age() << "\n";
    cout << st.get_last_name() << ", " << st.get_first_name() << "\n";
    cout << st.get_standard() << "\n";
    cout << "\n";
    cout << st.to_string();

    return 0;
}


