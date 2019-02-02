#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

using namespace std;
using namespace boost::property_tree;

int main()
{
    ptree pt;

    //open xml and read information to pt
    read_xml("test1.xml",pt);

    //iter all child value
    auto child = pt.get_child("root.students");
    for(auto i = child.begin();i != child.end();i++)
    {
        string name = i->second.get_value<string>();
        cout << name << endl;
    }

    return 0;
}