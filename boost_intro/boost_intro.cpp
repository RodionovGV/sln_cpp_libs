// boost_intro.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/filesystem.hpp>

int main()
{
    std::cout << "Hello World!\n";
    boost::filesystem::path p("D:\\g_rodionov\\");
    bool check = boost::filesystem::exists(p);
    if (check) {
        std::cout << "Exist" << std::endl;
        create_directory(p / "newdir");

    }
    else {
        std::cout << "NOT Exist" << std::endl;
    }
    for (auto& entry : boost::filesystem::directory_iterator(p)) {
        std::cout << entry.path() << std::endl;
    }
    std::cout << "Root path: " << p << std::endl;
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
