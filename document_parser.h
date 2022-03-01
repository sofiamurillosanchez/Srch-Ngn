/*
 Created by Sofia  Murillo on 11/9/21.

 References (Also stated within code):
 Eric Miao's code: https://www.dropbox.com/s/sepzesb0m7uotw5/TraverseFileSystem.cpp?dl=0
 https://www.cplusplus.com/reference/algorithm/find/

 */


#ifndef INC_21F_FINAL_PROJ_TEMPLATE_DOCUMENT_PARSER_H
#define INC_21F_FINAL_PROJ_TEMPLATE_DOCUMENT_PARSER_H


#include <iostream>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdio>
#include <sstream>
#include <map>


#include "DSAvlTree.h"
#include "DSHashTable.h"
#include "doc.h"
#include "rapidjson-master/include/rapidjson/document.h"
#include "rapidjson-master/include/rapidjson/stringbuffer.h"
#include "rapidjson-master/include/rapidjson/writer.h"
#include "rapidjson-master/include/rapidjson/filereadstream.h"
#include "porter2_stemmer-master/porter2_stemmer.h"

// Referenced Eric Miao's code: https://www.dropbox.com/s/sepzesb0m7uotw5/TraverseFileSystem.cpp?dl=0
using namespace std;
namespace fs = std::filesystem;
using namespace rapidjson;

class document_parser {
private:
    /*AVL tree and three hash tables needed (stop words, people, and organizations) */
    DSAvlTree<string, vector<doc>> AVL;
    DSHashTable<string, string> stopwords;
    DSHashTable<string, string> people;
    DSHashTable<string, string> orgs;
    int numArticles = 0;
    int numWords = 0;
    int numWordsTotal = 0; /* Total number of words (not unique)*/

    /* Parallel vectors containing the top words and their vector sizes */
    vector<string> topWords;
    vector<int> vecSizes;


public:

    /* JSON Parsing Functions*/
    char path[500];
    void open_dir_using_filesystem();
    void parse_json(FILE*);
    void read_stop(fstream& file);
    void remove_stop(string&, doc&);
    void remove_special_chars(string &text);
    void stem(string&, doc&);

    /* Top 50 Sorting Functions*/
    int Partition(vector<string>&, vector<int>&, int, int);
    void QuickSort(vector<string>&, vector<int>&, int, int);

    /* Getters */
    DSAvlTree<string, vector<doc>>& get_avl();
    DSHashTable<string, string>& get_orgs();
    DSHashTable<string, string>& get_people();

    /* Persistence Index functions*/
    void output_persistence();
    void process_persistence(fstream&, fstream&, fstream&, fstream&);

    /* Stats Functions*/
    void set_path(char*);
    int getNumArticles();
    int getTotNumWords();
    int getNumWords();
    void calculateTop();
    int calculateAvgPerDoc();
};


#endif //INC_21F_FINAL_PROJ_TEMPLATE_DOCUMENT_PARSER_H
