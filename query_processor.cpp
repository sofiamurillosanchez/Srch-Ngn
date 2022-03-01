/*
 Created by Sofia  Murillo on 11/9/21.
*/


#include "query_processor.h"
#include <algorithm>

void query_processor::search(string & query, document_parser & docu) {
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);
    stringstream temp(query);
    process(temp, docu);
}

void query_processor::process(stringstream& temp, document_parser& docu) {
    stringstream final("");
    string tempword;
    int count = 0;

    /*stacks for each boolean term re-instantiated
     * every time process function is called to limit
     clearing them*/
    stack<string> and_stack;
    stack<string> or_stack;
    stack<string> not_stack;
    stack<string> org_stack;
    stack<string> person_stack;
    stack<string> general_stack; /*for queries that don't start with a boolean term*/

    /*stack flags flip to populate corresponding stack based on processed query*/
    bool and_flag = false; bool or_flag = false; bool not_flag = false;
    bool org_flag = false; bool person_flag = false;

    while(temp >> tempword) {
        Porter2Stemmer::stem(tempword);
        /*pushes words following operand into their corresponding stack*/
        if (tempword == "and") {
            and_flag = true;
            or_flag = false, not_flag = false, org_flag = false, person_flag = false;
            continue;
        }
        if (tempword == "or") {
            or_flag = true;
            and_flag = false, not_flag = false, org_flag = false, person_flag = false;
            continue;
        }
        if (tempword == "not") {
            not_flag = true;
            and_flag = false, or_flag = false, org_flag = false, person_flag = false;
            continue;
        }
        if (tempword == "person") {
            person_flag = true;
            and_flag = false, not_flag = false, or_flag = false, org_flag = false;
            continue;
        }
        if (tempword == "org") {
            and_flag = false, or_flag = false, not_flag = false, person_flag = false;
            continue;
        }
        if(and_flag){
            and_stack.push(tempword);
            continue;
        }
        if(or_flag){
            or_stack.push(tempword);
            continue;
        }
        if(not_flag){
            not_stack.push(tempword);
            continue;
        }
        if(person_flag) {
            person_stack.push(tempword);
            continue;
        }
        if (org_flag) {
            org_stack.push(tempword);
            continue;
        }
        if ((tempword != "and") && (tempword != "or") && (tempword != "org") &&
            (tempword != "person") && (tempword != "not") && (count == 0)) {
            general_stack.push(tempword);
            continue;
        }
        count++;
    } /* end of while*/

/*
    todo: finish if chain for each function combination
*/
    if (!and_stack.empty()) {
       and_vector = and_function(docu, and_stack);
    } else if (!or_stack.empty()) {
        or_vector = or_function(docu, or_stack);
    }
    if (!not_stack.empty()) {
        not_vector = not_function(docu, not_stack);
    }
    if (!general_stack.empty()) {
        general_vector = general_function(docu, general_stack);
    }
    if (!person_stack.empty()) {
        person_vector = person_function(docu, person_stack);
    }
    if (!org_stack.empty()) {
        org_vector = org_function(docu, org_stack);
    }

    int counter = 1; /*assures we print top 15 articles*/


    bool aV = false;
    bool oV = false;
    bool gV = false;
    if (and_vector.size() != 0) {
        aV = true;
        /*checks to see if user also queried specific persons or organizations*/
        if (!person_vector.empty()) {
            vector<doc> temp;
            std::sort(and_vector.begin(), and_vector.end());
            std::sort(person_vector.begin(), person_vector.end());
            set_intersection(and_vector.begin(), and_vector.end(),
                             person_vector.begin(), person_vector.end(),
                             back_inserter(temp));
            and_vector = temp;
        }
        if (!org_vector.empty()) {
            vector<doc> temp;
            std::sort(and_vector.begin(), and_vector.end());
            std::sort(org_vector.begin(), org_vector.end());
            set_intersection(and_vector.begin(), and_vector.end(),
                             org_vector.begin(), org_vector.end(),
                             back_inserter(temp));
            and_vector = temp;
        }
        final_ranking(and_vector);
        for (const auto& itr: and_vector) {
            for (const auto& iter: not_vector) {
                std::remove(and_vector.begin(), and_vector.end(), iter);
            }
            cout << counter << ": " << itr.getTitle() << endl;
            counter++;
            if (counter == 15) {break;} /*stop printing files after top 15 articles*/
        }
    }
    else if (or_vector.size() != 0) {
        oV = true;
        /*checks to see if user also queried specific persons or organizations*/
        if (!person_vector.empty()) {
            vector<doc> temp;
            std::sort(or_vector.begin(), or_vector.end());
            std::sort(person_vector.begin(), person_vector.end());
            set_intersection(or_vector.begin(), or_vector.end(),
                             person_vector.begin(), person_vector.end(),
                             back_inserter(temp));
            or_vector = temp;
        }
        if (!org_vector.empty()) {
            vector<doc> temp;
            std::sort(or_vector.begin(), or_vector.end());
            std::sort(org_vector.begin(), org_vector.end());
            set_intersection(or_vector.begin(), or_vector.end(),
                             org_vector.begin(), org_vector.end(),
                             back_inserter(temp));
            or_vector = temp;
        }
        final_ranking(or_vector);
        for (auto itr: or_vector) {
            for (auto iter: not_vector) {
                std::remove(or_vector.begin(), or_vector.end(), iter);
            }
            cout << counter << ": " << itr.getTitle() << endl;
            counter++;
            if (counter == 15) {break;} /*stop printing files after top 15 articles*/
        }
    }
    else { /*todo: words not found in avl currently throws instance of std::length_error*/
        gV = true;
        if (!person_vector.empty()) {
            vector<doc> temp;
            std::sort(general_vector.begin(), general_vector.end());
            std::sort(person_vector.begin(), person_vector.end());
            set_intersection(general_vector.begin(), general_vector.end(),
                             person_vector.begin(), person_vector.end(),
                             back_inserter(temp));
            general_vector = temp;
        }
        if (!org_vector.empty()) {
            vector<doc> temp;
            std::sort(general_vector.begin(), general_vector.end());
            std::sort(org_vector.begin(), org_vector.end());
            set_intersection(general_vector.begin(), general_vector.end(),
                             org_vector.begin(), org_vector.end(),
                             back_inserter(temp));
            general_vector = temp;
        }
        final_ranking(general_vector);
            for (const auto& itr: general_vector) {
                for (auto iter: not_vector) {
                    std::remove(general_vector.begin(), general_vector.end(), iter);
                }
                cout << counter << ": " << itr.getTitle() << endl;
                counter++;
                if (counter == 15) {break;} /*stop printing files after top 15 articles*/
            }
    }

    cout << "\n\nWould you like to read any of the articles? (Y/N): ";
    string response;
    cin >> response;

    while(response == "Y"){
        cout << "\nWhich one would you like to read: ";
        int num;
        cin >> num;
        setInput(num);
        if(aV){
            cout << findText(and_vector);
        }else if(oV){
            cout << findText(or_vector);
        }else{
            cout << findText(general_vector);
        }
        setInput(-1);

        cout << "\n\nWould you like to read any more of the articles? (Y/N): ";
        cin >> response;
    }



} /*end of function*/


/*
function searches documents and returns those that include all queried terms
*/
vector<doc> query_processor::and_function(document_parser& docu, stack<string>& and_stack) {
    string query1, query2; auto& temp = docu.get_avl();
    vector<doc> intersection;

    /*first perform a prime intersection of to terms*/
    query1 = and_stack.top(); and_stack.pop();
    auto& hold1 = temp.find(query1);
    sort(hold1.begin(), hold1.end());

    query2 = and_stack.top(); and_stack.pop();
    auto& hold2 = temp.find(query2);
    sort(hold2.begin(), hold2.end());

    set_intersection(hold1.begin(), hold1.end(),
                     hold2.begin(), hold2.end(),
                     back_inserter(intersection));
    /*iterate through remaining stack and perform intersection
     * between new term and previous intersection*/
    while (!and_stack.empty()) {
        query1 = and_stack.top(); and_stack.pop();
        hold1 = temp.find(query1);
        sort(hold1.begin(), hold1.end());

        vector<doc> clean_vector;
        set_intersection(hold1.begin(), hold1.end(),
                         intersection.begin(), intersection.end(),
                         back_inserter(clean_vector));
        intersection = clean_vector;
    }

    /*remove duplicates from final vector*/
    vector<doc> final;
    sort(intersection.begin(), intersection.end());
    for(const auto& itr: intersection) {
        auto iter = find(final.begin(), final.end(), itr);
        if (iter == final.end()) {
            final.emplace_back(itr);
        }
    }
    return final;
}

/*
returns docs that contain any of the received queried terms
*/
vector<doc> query_processor::or_function(document_parser& docu, stack<string>& or_stack) {
    string query1, query2; auto& temp = docu.get_avl();
    vector<doc> merge;

    query1 = or_stack.top(); or_stack.pop();
    auto& hold1 = temp.find(query1);

    /*iterate through stack, merging vectors of queried terms*/
    while(!or_stack.empty()) {
        query2 = or_stack.top(); or_stack.pop();
        auto& hold2 = temp.find(query2);
        for (const auto& itr: hold2) {
            hold1.emplace_back(itr);
        }
    }
    /*populate final vector without duplicating docs*/
    sort(hold1.begin(), hold1.end());
    for(const auto& itr: hold1) {
        auto iter = find(merge.begin(), merge.end(), itr);
        if (iter == merge.end()) {
            merge.emplace_back(itr);
        }
    }
    return merge;
}

/*returns vector of IDs we later remove from vector of queried IDs*/
vector<doc> query_processor::not_function(document_parser & docu, stack<string> & not_stack) {
    auto& temp = docu.get_avl();
    auto& hold = temp.find(not_stack.top()); /*searches avl for vector of ids to exclude*/

    /*remove duplicate docs from final vector*/
    vector<doc> final;
    for (const auto& itr: hold) {
        auto iter = find(final.begin(), final.end(), itr);
        if (iter == final.end()) {
            final.emplace_back(itr);
        }
    }
    return final;
}

/*populates when first word of query is not an operand
 * or user only performs one word query*/
vector<doc> query_processor::general_function(document_parser& docu, stack<string>& general_stack) {
    /*get the AVL tree from the doc_parser
    traverses avl and holds returned vector of doc objects*/
    auto& temp = docu.get_avl();
    auto& hold = temp.find(general_stack.top());

    /*remove duplicate docs from final vector*/
    vector<doc> final;
    for (const auto& itr: hold) {
        auto iter = find(final.begin(), final.end(), itr);
            if (iter == final.end()) {
                final.emplace_back(itr);
            }
    }
    return final;
}

/*return vector of docs with queried persons*/
vector<doc> query_processor::person_function(document_parser& docu, stack<string> & person_stack) {
    auto& temp1 = docu.get_people(); /*hashtable of people*/
    auto& hold1 = temp1.find(person_stack.top()); /*vector of Nodes containing person and paper id*/
    person_stack.pop();
    while (!person_stack.empty()) { /*while loop accounts for searching multiple ppl*/
        auto hold2 = temp1.find(person_stack.top());
        for (const auto& iter: hold2) {
            hold1.emplace_back(iter);
        }
        person_stack.pop();
    }
    /* "convert" vector<node> to vector<doc> */
    vector<doc> temp;
    for (const auto& itr: hold1) {
        temp.emplace_back(itr.value);
    }
    /* remove duplicates */
    person_vector.clear();
    for (const auto& itr: temp) {
        auto hold = std::find(person_vector.begin(), person_vector.end(), itr);
        if (hold == person_vector.end()) {
            person_vector.emplace_back(itr);
        }
    }
    return person_vector;
}

/*return vector of docs with queried organizations*/
vector<doc> query_processor::org_function(document_parser & docu, stack<string> & org_stack) {
    auto& temp1 = docu.get_orgs(); /*hashtable of orgs*/
    auto& hold1 = temp1.find(org_stack.top()); /*vector of Nodes containing orgs and paper id*/
    org_stack.pop();
    while (!org_stack.empty()) { /*while loop accounts for searching multiple orgs*/
        auto hold2 = temp1.find(org_stack.top());
        for (const auto& iter: hold2) {
            hold1.emplace_back(iter);
        }
        org_stack.pop();
    }
    /* "convert" vector<node> to vector<doc> */
    vector<doc> temp;
    for (const auto& itr: hold1) {
        temp.emplace_back(itr.value);
    }
    /* remove duplicates */
    org_vector.clear();
    for (const auto& itr: temp) {
        auto hold = std::find(org_vector.begin(), org_vector.end(), itr);
        if (hold == org_vector.end()) {
            org_vector.emplace_back(itr);
        }
    }
    return org_vector;
}

/* ranks document importance by word frequency in the docs*/
void query_processor::final_ranking(vector<doc>& finalVec) {
    QuickSort(finalVec, 0, finalVec.size()-1);
}

/*
Quick sort function for rank function
*/
int query_processor::Partition(vector<doc> &v, int start, int end){ // Referenced: https://slaystudy.com/c-vector-quicksort/
    int pivot = end;
    int j = start;
    for(int i=start;i<end;++i){
        if(v[i].getFreq()<v[pivot].getFreq()){
            swap(v[i],v[j]);
            ++j;
        }
    }
    swap(v[j],v[pivot]);
    return j;
}

/*
 Quick sort function for rank function
*/
void query_processor::QuickSort(vector<doc> &v, int start, int end) { // Referenced: https://slaystudy.com/c-vector-quicksort/
    if (start < end) {
        int p = Partition(v, start, end);
        QuickSort(v, start, p - 1);
        QuickSort(v, p + 1, end);
    }
}

void query_processor::setInput(int index){
    inp = index;
}

int query_processor::getInput(){
    return inp;
}

string query_processor::findText(vector<doc> vect){
    if(inp >= 1 && inp <= 15){
        return "\n\n"+vect.at(inp-1).getText()+"\n\n";
    }

    return "\nYou entered an invalid article number!\n";
}