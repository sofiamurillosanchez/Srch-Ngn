//
// Created by Sofia  Murillo on 11/9/21.
//

#include "user_interface.h"

/* This function essentially takes the doc_parser object passed in from main and outputs the user-driven menu*/
void user_interface::readIn(document_parser& doc_parser) {

    /*Star up the menu and list options to the user. Then accept user input.*/
    cout << "Welcome to Jofia Strorillo's Search Engine!" << endl;
    cout << "\tPlease choose an option from the following menu - " << endl;
    cout << "\t[1] Load Data\n\t[2] Execute Query\n\t[3] Show Statistics\n\t[4] Write Persistence Files\n\t[5] Exit" << endl;
    cout << "\n\tOption: ";
    int user_request;
    cin >> user_request;

    bool loadedData = false;

    /*Check user input. If it isn't 5, then keep the loop running (5 is the exit option)*/
    while (user_request != 5) {
        /* 1 = Load Data */
        if (user_request == 1) {
            cout << "\n\tThank you! Please choose an option: " << endl;
            cout << "\t\t1) Load From Files\n\t\t2) Load From Persistence File" << endl;
            cout << "\t\tOption: ";
            int second_request;
            cin >> second_request;

            /* Ask user whether they want to load from files or from persistence file and accept their input.*/
            if (second_request == 1) {
                /* Won't cin a file path, it should be a command line argument*/
                cout << "\n\tPlease enter the file path: ";
                char filePath[500];
                cin >> filePath;
                doc_parser.set_path(filePath);
                cout << "\n\n\tLoading..." << endl << endl;
                doc_parser.open_dir_using_filesystem();
                cout << "\tDone!" << endl << endl;
                loadedData = true;
            } else if (second_request == 2) {
                /* Call the persistence file that was outputted into the cmake folder and pass it to process persistence for processing*/
                fstream inputAVL("../persistence_AVL.txt");
                fstream inputT("../persistence_AVLText.txt");
                fstream inputPeople("../persistence_people.txt");
                fstream inputOrgs("../persistence_orgs.txt");
                doc_parser.process_persistence(inputAVL, inputT, inputPeople, inputOrgs);
                cout << "Done!" << endl;
                loadedData = true;
            } else {
                cout << "Please enter a valid option. Exiting back to main menu :)" << endl;
            }
        } else if (user_request == 2) {
            if(loadedData){
                cout << "\n\tThank you! Please enter your query." << endl;
                cout << "\n\tQuery: ";
                string query;
                cin.ignore('\n');
                getline(cin, query);
                a.search(query, doc_parser);

                user_request = -1;
            } else{
                cout << "\nPlease load data before executing the query menu option." << endl << endl;
            }

        } else if (user_request == 3) {
            if(loadedData){
                /* Basically, call all of the stat functions which were created in the doc parser*/
                cout << "Thank you! Here are the statistics you requested: " << endl;
                cout << "\t Total Number of Articles: " << doc_parser.getNumArticles() << endl;
                cout << "\t Average Number of Words per Article: " << doc_parser.calculateAvgPerDoc() << endl;
                cout << "\t Total Number of Unique Words: " << doc_parser.getNumWords() << endl;
                cout << "\t Number of Unique Organizations: " << doc_parser.get_orgs().Size() << endl;
                cout << "\t Number of Unique People: " << doc_parser.get_people().Size() << endl;
                cout << "\t Top 50 Most Frequent Words: \n";
                doc_parser.calculateTop();
            }
            else{
                cout << "\nPlease load data before executing statistics menu option." << endl << endl;
            }

        } else if (user_request == 4) { /*Write to persistence file using output persistence from doc parser*/
            if(loadedData){
                cout << "\n\tThank you! Now writing persistence file." << endl;
                cout << "\tLoading..." << endl;
                doc_parser.output_persistence();
                cout << "\tDone writing to the persistence file!" << endl << endl;
            }
            else{   /*Don't output to persistence if there's nothing to output to the file. Must load files first.*/
                cout << "\tPlease load from files before writing to persistence files." << endl << endl;
            }

        }
        else {  /*Have an option ready for if the user enters a number that isn't on the menu.*/
            cout << "\tPlease enter a valid option. Reloading menu :)" << endl << endl;
            user_request = -1;
        }

        /* Reload the menu */
        cout << "\tPlease choose an option from the following menu - " << endl;
        cout << "\t[1] Load Data\n\t[2] Execute Query\n\t[3] Show Statistics\n\t[4] Write Persistence Files\n\t[5] Exit" << endl;
        cout << "\n\tOption: ";
        cin >> user_request;
    }

    /*Shut down message*/
    cout << "\n\tThank you!\n\tNow shutting down..." << endl;
}