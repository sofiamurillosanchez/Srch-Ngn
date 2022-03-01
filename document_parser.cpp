/*
 Created by Sofia  Murillo on 11/9/21.
  References (Also stated within code):
     Eric Miao's code: https://www.dropbox.com/s/sepzesb0m7uotw5/TraverseFileSystem.cpp?dl=0
     https://www.cplusplus.com/reference/algorithm/find/
     https://slaystudy.com/c-vector-quicksort/
*/


#include "document_parser.h"

/* This function reads the file of stop words (which I created from the list of stop words from the link Dr. Fontenot posted
 on the handout)*/
void document_parser::read_stop(fstream &file) {
    string stop; /*String of stop word*/

    getline(file, stop, '\n');
    /* Start file reading*/
    while (!file.eof()) {
        /* When a stop word is found, it's added to the stop_word hashtable (to be used later)*/
        stopwords.insert(stop, stop);

        getline(file, stop, '\n');
    }

    file.close();   /*Close that file!*/
}


void
document_parser::open_dir_using_filesystem() {  /*Referenced Eric Miao's code: https://www.dropbox.com/s/sepzesb0m7uotw5/TraverseFileSystem.cpp?dl=0*/
    for (auto &entry: fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            const char *temp_path = entry.path().c_str();
            for (const auto &ent: fs::directory_iterator(temp_path)) {
                if (ent.is_directory()) {
                    const char *temp_path2 = ent.path().c_str();
                    for (auto &e: fs::directory_iterator(temp_path2)) {
                        const char *nameString = e.path().c_str();
                        if (e.path().extension() == ".json") { /*Ensures that the system ONLY opens JSON files*/
                            FILE *input = fopen(nameString, "r"); /* If testing on windows, use "rb" instead of "r"*/
                            parse_json(input);
                            fclose(input);
                            numArticles++;
                        }
                    }
                } else {
                    const char *nameString = ent.path().c_str();
                    if (ent.path().extension() == ".json") { /* Ensures that the system ONLY opens JSON files*/
                        FILE *input = fopen(nameString, "r");  /*If testing on windows, use "rb" instead of "r"*/
                        parse_json(input);
                        fclose(input);
                        numArticles++;
                    }

                }
            }
        } else {
            const char *nameString = entry.path().c_str();
            if (entry.path().extension() == ".json") { /* Ensures that the system ONLY opens JSON files*/
                FILE *input = fopen(nameString, "r"); /* If testing on windows, use "rb" instead of "r"*/
                parse_json(input);
                fclose(input);
                numArticles++;
            }
        }
    }
}

/*Parses all the JSON files using the rapidjson filestream.*/
void document_parser::parse_json(FILE *input) {
    rapidjson::Document docu;

    /*Use rapidjson's filestream to parse through the files*/
    char buffer[100000];
    FileReadStream is(input, buffer, sizeof(buffer));
    docu.ParseStream(is);

    /*Get the title of the article*/
    string id = docu["thread"]["title"].GetString();

    /*Get the text of the article*/
    string text = docu["text"].GetString();

    /*Create a doc object and pass the article title to it*/
    doc curr(id);
    curr.setText(text);


    /* Looping through organizations and people to get their specific values (and store them into their respective vectors)*/
    for (auto &v: docu["entities"]["organizations"].GetArray()) {
        orgs.insert(v["name"].GetString(), id);
    }

    for (auto &w: docu["entities"]["persons"].GetArray()) {
        people.insert(w["name"].GetString(), id);
    }
    remove_special_chars(text);
    remove_stop(text, curr);
}

/* This function removes any special characters by utilizing the erase function and actively removing any special characters
 that match the string of special characters I check for*/
void document_parser::remove_special_chars(string &text) {
    for (int i = 0; i < text.length(); i++) {
        if (text.at(i) >= 65 && text.at(i) <= 90) {
            text.at(i) += 32;
        } else if (!(text.at(i) == '-' || text.at(i) <= 57 && text.at(i) >= 48 ||
                     text.at(i) <= 122 && text.at(i) >= 97)) {
            text.at(i) = ' ';
        }
    }
}


/* In order to remove stop words (and maintain efficiency), I will remove the stop words by implementing the
 find function (reference here: https://www.cplusplus.com/reference/algorithm/find/) to see if the words
 in the stop_words vector match any of the words in the string (also using the string find function, implementation ref here: https://stackoverflow.com/questions/4034750/find-all-a-substrings-occurrences-and-locations)

 If there is a match, the word will be removed from the string.
 If there isn't a match, then no words will be removed.*/
void document_parser::remove_stop(string &text, doc &curr) {
    /* Loop through the stop_words and while doing this, find all instances of those words in the text
     As words are found, they are removed from the text*/

/*
     Create a current word string to keep track of the word being checked in the stop_words vector
*/
    stringstream temp(text);
    stringstream final("");
    string tempword;
    while (temp >> tempword) {
        if (!stopwords.contains(tempword)) {
            final << tempword << " ";
        }
        tempword.clear();
    }
    text = final.str();

    stem(text, curr);
}

/* The stemming function will implement the Porter 2 Stemmer*/
void document_parser::stem(string &text, doc &id) {
    stringstream s(text);
    string word;

    while (s >> word) {
        Porter2Stemmer::stem(word);

        /*
                Check to see if AVL tree contains word, if it does, just push back the article title and increase its frequency
        */
        if (AVL.contains(word)) {
            auto &type = AVL.find(word);
            if (type.size() == 1) {
                if (type.at(type.size() - 1).getTitle() == id.getTitle()) {
                    id.incrFreq();
                }
            } else if (type.size() > 1) {
                if (type.at(type.size() - 2).getTitle() == id.getTitle()) {
                    id.incrFreq();
                }
            }
            type.push_back(id);

            /* Logic to create vector with top 50 words (by checking the size of the vector of documents)*/
            if (type.size() >= 10000 && find(topWords.begin(), topWords.end(), word) == topWords.end()) {
                topWords.push_back(word);
                vecSizes.push_back(type.size());
            } else if (find(topWords.begin(), topWords.end(), word) != topWords.end()) {
                int index = find(topWords.begin(), topWords.end(), word) - topWords.begin();
                vecSizes.at(index) = type.size();
            }
        } else { /*If it doesn't create a new node and push the article title to its vector*/
            auto &type = AVL.insert(word);

            type.push_back(id);
        }
        numWordsTotal++;
    }
}

/*
 Getter for the AVL Tree
*/
DSAvlTree<string, vector<doc>> &document_parser::get_avl() {
    return AVL;
}

/*
 Getter for orgs hash table
*/
DSHashTable<string, string> &document_parser::get_orgs() {
    return orgs;
}

/* Getter for people hash table*/
DSHashTable<string, string> &document_parser::get_people() {
    return people;
}

/*
 Set the path to the char* passed in
*/
void document_parser::set_path(char *p) {
    strcpy(path, p);
}

/*
 Get the number of articles processed (for the stats portion)
*/
int document_parser::getNumArticles() {
    return numArticles;
}

/*
 Return the size of the AVL tree to get the number of words
*/
int document_parser::getNumWords() {
    return AVL.size();
}

/*
 This function processes the AVL tree & people/orgs hash tables to print them to the persistence index (to
 then be processed if needed.
*/
void document_parser::output_persistence() {
/*
     Create ofstream object with persistence index file to output the words (AVL Tree), people, and orgs hash tables
*/
    ofstream output("../persistence_AVL.txt");
    ofstream outputT("../persistence_AVLText.txt");
    // Clear the file before outputting anything to it
    output.clear();
    outputT.clear();

    /* To output the AVL tree,*/
    AVL.post_order_persist_pub();
    auto vec = AVL.getVec();
    /*
         Print the vector to the persistence file
    */
    for (int i = 0; i < vec.size(); i++) {
        /*First term is the word*/
        output << vec.at(i).first << endl;

        /* Then iterate through the vector, the title of the docs should print and then the frequency in which those words appear
         should follow the title. Only used spaces and no commas or anything cause that overcomplicates things.
        */

        map<string, doc> docs;
        for (int j = 0; j < vec.at(i).second.size(); j++) {
            docs.insert(make_pair(vec.at(i).second.at(j).getTitle(), vec.at(i).second.at(j)));
        }

        for (auto itr: docs) {
            output << itr.first << "~" << itr.second.getFreq() << endl;
            outputT << itr.second.getText() << "~" << endl;
        }
    }
    output.close();
    outputT.close();


    auto peopleVec = people.getTraversed();
    /* To output the people hash table, traverse through the hash table and take the vector
     created by the traversal and print it to the output file*/

    ofstream outputPeople("../persistence_people.txt");
    outputPeople.clear();

    for (int i = 0; i < peopleVec.size(); i++) {
        outputPeople << peopleVec.at(i).key << "~" << peopleVec.at(i).value << endl;
    }
    outputPeople.close();

    /*
        To output the orgs hash table, do the same as above but with the organization hash table
    */

    ofstream outputOrgs("../persistence_orgs.txt");
    outputOrgs.clear();
    auto orgsVec = orgs.getTraversed();
    for (int i = 0; i < orgsVec.size(); i++) {
        outputOrgs << orgsVec.at(i).key << "~" << orgsVec.at(i).value << endl;
    }
    outputOrgs.close();

}

/*
 This function processes the persistence index by storing words into the AVL tree as well as people and orgs into
 their respective hash tables*/
void
document_parser::process_persistence(fstream &inputAVL, fstream &inputText, fstream &inputPeople, fstream &inputOrgs) {
    string line;
    string word;
    string fre;
    string docName;
    string tempStr;
    string text;
    string actual_word;
    int freq;

    /* Clear out the current AVL trees and hash tables (for overwriting purposes) and reset values for stats*/
    AVL.clear();
    DSHashTable<string, string> newPeople;
    people = newPeople;
    DSHashTable<string, string> newOrgs;
    orgs = newOrgs;
    numArticles = 0;
    numWords = 0;
    numWordsTotal = 0;
    topWords.clear();
    vecSizes.clear();

    vector<string> words;
    vector<string> articles;
    int pos = 0;
    /* Read in the file line by line*/
    while (getline(inputAVL, line, '\n')) {
        stringstream temp(line);

        int track = 0;

        while (temp >> word) {
            /*Check the position, if it's 0, it's the word which needs to be inserted into the AVL tree*/
            if (pos == 0) {
                words.push_back(word);
                AVL.insert(word);
                actual_word = word;
                numWords++;
                ++pos;
            } else if (pos == 1) {
                docName = word;
                while (getline(temp, tempStr, '~')) {
                    docName += tempStr;
                }
                doc d(docName);

                /*Get the frequency of the word in the document*/

                /*Find the word in the AVL tree and set the frequency*/
                d.setFreq(stoi(tempStr));
                AVL.find(actual_word).push_back(d);


                /*Iterate through inputText and get all the articles*/
                int cntWords = 0;
                while (getline(inputText, line, '~')) {
                    articles.push_back(line);

                    /*Number of words is equal to the number of spaces found plus 1*/
                    for(int i = 0; i<line.length(); i++){
                        if(line.at(i) == ' ')
                            cntWords++;
                    }
                }

                cntWords++;
                numWordsTotal = cntWords;

                for (int i = 0; i < AVL.find(actual_word).size(); i++) {
                    for (int j = 0; j < AVL.find(actual_word).size(); j++) {
                        AVL.find(actual_word).at(i).setText(articles.at(j));
                    }

                    for (int k = AVL.find(actual_word).size(); k < AVL.find(actual_word).size(); k++) {
                        articles.at(track) = articles.at(k);
                        track++;
                    }
                }

                if(AVL.find(actual_word).size() >= 10000){
                    topWords.push_back(actual_word);
                    vecSizes.push_back(AVL.find(actual_word).size());
                }

            }
            numArticles = articles.size();

        }
    }

    /*Create people from file reading and store into hash table*/
    while (getline(inputPeople, line, '\n')) {
        stringstream temp(line);
        string person;
        string article;
        getline(temp, person, '~');
        getline(temp, article, '\n');

        people.insert(person, article);
    }

    /*Create orgs from file reading and store into hash table*/
    while (getline(inputOrgs, line, '\n')) {
        stringstream temp(line);
        string organiz;
        string article;
        getline(temp, organiz, '~');
        getline(temp, article, '\n');

        orgs.insert(organiz, article);
    }
}


/* Essentially, I quicksort the vector of top words and the top 50 words with the largest vector sizes (which means they
 appear in the most docs) will get printed. It's great.*/
void document_parser::calculateTop() {
    QuickSort(topWords, vecSizes, 0, topWords.size() - 1);

    for (int i = 1; i < 51; i++) {
        cout << "\t\t" << i << ": " << topWords.at(i) << endl;
    }

    cout << endl << endl;
}

/*
 Quick sort function for rank function
*/
int document_parser::Partition(vector<string> &words, vector<int> &sizes, int start,
                               int end) { /* Referenced: https://slaystudy.com/c-vector-quicksort/*/
    int pivot = end;
    int j = start;
    for (int i = start; i < end; ++i) {
        if (sizes.at(i) < sizes[pivot]) {
            swap(sizes[i], sizes[j]);
            swap(words[i], words[j]);
            ++j;
        }
    }
    swap(sizes[j], sizes[pivot]);
    swap(words[j], words[pivot]);

    return j;
}

/*
 Quick sort function for rank function
*/
void document_parser::QuickSort(vector<string> &words, vector<int> &sizes, int start,
                                int end) { /* Referenced: https://slaystudy.com/c-vector-quicksort/*/
    if (start < end) {
        int p = Partition(words, sizes, start, end);
        QuickSort(words, sizes, start, p - 1);
        QuickSort(words, sizes, p + 1, end);
    }
}

/*
 Getter for the total number of words (not unique ones)
*/
int document_parser::getTotNumWords() {
    return numWordsTotal;
}

/*
 Calculates average words per document
*/
int document_parser::calculateAvgPerDoc() {
    /* Divide the total number of words by the total number of articles and return that value*/
    int avg = getTotNumWords() / getNumArticles();

    return avg;
}