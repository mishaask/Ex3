/**
 * This program contains some issues that are detected by clang-tidy.
 * 
 * Run `make tidy` to find out.
 */

#include <iostream>
#include <string>
#include <array>
using namespace std;

const int NUM_OF_SUBMISSIONS_IN_SEMESTER = 12;

int main() {
    cout << "Each student should submit " << NUM_OF_SUBMISSIONS_IN_SEMESTER << " exercises." << '\n';

    int submission_count = 11;
    if (submission_count >= NUM_OF_SUBMISSIONS_IN_SEMESTER) 
        {cout << "OK!"  << '\n';}
    else 
        {cout << "Try again." << '\n';}

    string submission_names[3] = {"a", "b", "c"};
    // array<string,3> submission_names = {"a", "b", "c"};  // Array with a fixed size, like in C
    cout << submission_names[0] << '\n';
    return 0;
}

