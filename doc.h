//
// Created by Sofia  Murillo on 11/27/21.
//

#ifndef INC_21F_FINAL_PROJ_TEMPLATE_DOC_H
#define INC_21F_FINAL_PROJ_TEMPLATE_DOC_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class doc {
private:
    int freq;
    string title;
    string text;

public:
    doc();
    doc(string);

    void setFreq(int);
    int getFreq();

    void incrFreq();

    void setTitle(string);
    string getTitle() const;


    void setText(string);
    string getText();

    bool operator<(const doc&) const;
    bool operator==(const doc&) const;

};


#endif //INC_21F_FINAL_PROJ_TEMPLATE_DOC_H
