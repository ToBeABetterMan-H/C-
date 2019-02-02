#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

using namespace std;
using namespace boost::property_tree;

int main()
{
    ptree pt;
    //open xml and read information to pt
    read_xml("test.xml",pt);
    
    //read value to val need a path
    string name = pt.get<string>("root.students.name");
    cout << "name: " << name << endl;
    int age = pt.get<int>("root.students.age");
    cout << "age: " << age << endl;
    return 0;
}
