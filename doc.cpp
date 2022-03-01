/*
 Created by Sofia  Murillo on 11/27/21.
*/


#include "doc.h"

/*Constructor for doc*/
doc::doc() {
    freq = 0;
    title = "";
}

doc::doc(string t) {
    freq = 0;
    title = t; /*Set title to string passed in*/
}


/*Setter and getter for frequency at which a word appears in document*/
void doc::setFreq(int f) {
    freq = f;
}

int doc::getFreq() {
    return freq;
}

/*Increment the frequency*/
void doc::incrFreq(){
    freq++;
}

/*Getter and setter for the title*/
void doc::setTitle(string t) {
    title = t;
}

string doc::getTitle() const{
    return title;
}

/*Getter and setter for article text*/

void doc::setText(string t) {
    text = t;
}

string doc::getText() {
    return text;
}

/*Overloaded < and == operators (for sorting purposes)*/
bool doc::operator<(const doc & arg) const {
    return title < arg.title;
}

bool doc::operator==(const doc & arg) const {
    return title == arg.title;
}
