/* This is Jofia Strorillo's Search Engine. It essentially reads in JSON files (documents, in this context),
 * parses through them, stems, etc. to the point where a user can type in a query and search for words within
 * these documents.*/

#include <iostream>
#include "catch_setup.h"
#include "document_parser.h"
#include "user_interface.h"
#include "time.h"

int main(int argc, char** argv) {

//    if(argc == 1){
//        runCatchTests();
//    }

    document_parser p;
    fstream file("../stop-words/stop-words.txt");
    p.read_stop(file);

    user_interface a;
    a.readIn(p);

    return 0;
}
