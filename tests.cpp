//
// Created by jadon on 11/10/2021.
//
#include "catch.hpp"
#include "DSHashTable.h"
#include "DSAvlTree.h"
#include "iostream"
using namespace std;

TEST_CASE("All hashtable functions being tested", "[DSHashTable]") {


    DSHashTable<int, string> one;

    SECTION("insert and size function being tested") {
        int id = 48015413; // key value to be hashed
        string first_name = "Jadon";
        string last_name = "Stronge";
        string major = "Computer Science";

        one.insert(id, first_name);
        REQUIRE(one.Size() == 1);
        one.insert(id, last_name);
        REQUIRE(one.Size() == 1);

        one.insert(00001111, first_name);
        one.insert(00001111, major);
        REQUIRE(one.Size() == 2);
    }


    SECTION("tests find function") {
        int id = 48015413; // key value to be hashed
        string first_name = "Jadon";
        string last_name = "Stronge";
        string major = "Computer Science";
        one.insert(id, first_name);
        one.insert(id, last_name);
        one.insert(id, major);
        auto temp = one.find(id);
        REQUIRE(temp.at(0).value == "Jadon");
        REQUIRE(temp.at(1).value == "Stronge");
        REQUIRE(temp.at(2).value == "Computer Science");
    }

    SECTION("test contains function") {
        int id = 48015413;
        REQUIRE(one.contains(id) == false);
        string first_name = "Jadon";
        string last_name = "Stronge";
        one.insert(id, first_name);
        one.insert(id, last_name);
        REQUIRE(one.contains(id) == true);
        one.insert(id, "Computer Science");
        REQUIRE(one.contains(id) == true);
    }

}

TEST_CASE("all functions of AVLTree being tested", "[DSAvlTree]") {
    DSAvlTree<string, vector<string>> one;
    DSAvlTree<string, vector<string>> two;
    DSAvlTree<int, int> three;

    SECTION("tests find and overloaded assignment operator") {
        one.insert("Jadon");
        one.insert("Stronge");
        two = one;
        REQUIRE(one.contains("Jadon") == true);
        REQUIRE(two.contains("Jadon") == true);
        REQUIRE(two.contains("Stronge") == true);
        REQUIRE(two.contains("Computer Science") == false);
    }

    SECTION("tests to see if tree can hold 10k items") {
        for (int i = 0; i < 10000; i++) {
            three.insert(i);
        }
        REQUIRE(three.size() == 10000);
    }
}