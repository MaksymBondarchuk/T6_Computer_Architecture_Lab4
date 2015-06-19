//
// Created by max on 04.06.15.
//

#ifndef COMPUTER_ARCHITECTURE_LAB4_TASK_HPP
#define COMPUTER_ARCHITECTURE_LAB4_TASK_HPP

#include <string>
#include <vector>
#include <algorithm>

using namespace std;


string palindromes(string text) {
    string res;
    vector<string> pals = vector<string>();
    for (unsigned long n = 2; n < 20; n++)
        for (long i = 0; i <= (long)(text.length() - n); i++) {
            string possible_pal = text.substr((unsigned long) i, n);
            if (possible_pal == string(possible_pal.rbegin(), possible_pal.rend()) &&
                find(pals.begin(), pals.end(), possible_pal) == pals.end()) {
                pals.push_back(possible_pal);
                res += possible_pal;
                res += '\n';
            }
        }
    return res;
}

#endif
