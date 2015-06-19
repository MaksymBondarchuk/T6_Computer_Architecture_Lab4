//
// Created by max on 17.06.15.
//

#ifndef COMPUTER_ARCHITECTURE_LAB4_SERIALIZATION_HPP
#define COMPUTER_ARCHITECTURE_LAB4_SERIALIZATION_HPP

#ifndef BUF_SIZE
#define BUF_SIZE 10000
#endif


#include <sstream>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/text_oarchive.hpp>

using namespace std;


void serialize(char* line_for_serialization) {
    string buf = line_for_serialization;

    stringstream ss_send;
    boost::archive::text_oarchive oa{ss_send};
    oa << buf;

    string st = ss_send.str();
    strcpy(line_for_serialization, st.c_str());
}

void deserialize(char* line_for_deserialization) {
    stringstream ss_recieve;
    string buf = line_for_deserialization;
    ss_recieve << buf;

    boost::archive::text_iarchive ia{ss_recieve};
    string deserialized_line;
    ia >> deserialized_line;

    strcpy(line_for_deserialization, deserialized_line.c_str());
}


#endif //COMPUTER_ARCHITECTURE_LAB4_SERIALIZATION_HPP
