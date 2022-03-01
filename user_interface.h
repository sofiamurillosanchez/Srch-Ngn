//
// Created by Sofia  Murillo on 11/9/21.
//

#ifndef INC_21F_FINAL_PROJ_TEMPLATE_USER_INTERFACE_H
#define INC_21F_FINAL_PROJ_TEMPLATE_USER_INTERFACE_H

#include "query_processor.h"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class user_interface {
private:
    /*invoking variable*/
    query_processor a;

public:
    /* User Menu Function */
    void readIn(document_parser&);

};


#endif //INC_21F_FINAL_PROJ_TEMPLATE_USER_INTERFACE_H
