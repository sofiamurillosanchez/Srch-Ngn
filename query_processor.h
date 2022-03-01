/*
 Created by Sofia  Murillo on 11/9/21.
*/


#ifndef INC_21F_FINAL_PROJ_TEMPLATE_QUERY_PROCESSOR_H
#define INC_21F_FINAL_PROJ_TEMPLATE_QUERY_PROCESSOR_H

#include <iostream>
#include <vector>
#include <sstream>
#include <stack>
#include "DSAvlTree.h"
#include "DSHashTable.h"
#include "document_parser.h"

using namespace std;

class query_processor {
private:
    vector<doc> and_vector, or_vector, not_vector, general_vector, org_vector, person_vector;
    int inp;

public:
     void search(string&, document_parser&);

    /*Functions return vector of id based on operator stack that is populated
     returned vectors are cross-references in process and sent to final ranking for output*/

    vector<doc> and_function(document_parser&, stack<string>&);
    vector<doc> or_function(document_parser&, stack<string>&);
    vector<doc> not_function(document_parser&, stack<string>&);
    vector<doc> general_function(document_parser&, stack<string>&);
    vector<doc> person_function(document_parser&, stack<string>&);
    vector<doc> org_function(document_parser&, stack<string>&);


    void process(stringstream&, document_parser&);
    void final_ranking(vector<doc>&);
    void QuickSort(vector<doc>&, int, int);
    int Partition(vector<doc>&, int, int);

    void setInput(int);
    int getInput();
    string findText(vector<doc>);
};


#endif //INC_21F_FINAL_PROJ_TEMPLATE_QUERY_PROCESSOR_H
