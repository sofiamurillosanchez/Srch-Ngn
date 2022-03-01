//
// Created by jadon on 11/9/2021.
// reference: Mark Allen Weiss
//

#ifndef INC_21F_FINAL_PROJ_TEMPLATE_DSAVLTREE_H
#define INC_21F_FINAL_PROJ_TEMPLATE_DSAVLTREE_H

#include "doc.h"
#include <vector>
#include <fstream>
template<typename T, typename V>
class DSAvlTree {

private:
    struct AVLNode {
        T data;
        V value;
        AVLNode *left;
        AVLNode *right;
        int height{};

        AVLNode() = default;
        explicit AVLNode(const T &element, AVLNode *l = nullptr, AVLNode *r = nullptr, int h = 0):
                data(element), left(l), right(r), height(h) {}

    };

    AVLNode *root = nullptr;
    int count_nodes = 0;
    /*contains every node inserted to tree for future outputting*/
    vector<pair<T, V>> vec;

    /**
     * CASE ROTATIONS Designed to maintain avl property of a parent
     * node's children maintaining height diff no greater than one
     */
    void rotateWithLeftChild(AVLNode*& k2) { /*case 1*/
        if (k2 == nullptr) throw std::out_of_range("DSContainer::AVLTree::node is nullptr");
        AVLNode *k1 = k2->left; // k2 is parent
        k2->left = k1->right;
        k1->right = k2;

        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    void doubleWithLeftChild(AVLNode*& k3) {  /*case 2*/
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void rotateWithRightChild(AVLNode*& k1) {   /*case 4*/
        if (k1 == nullptr) throw std::out_of_range("DSContainer::AVLTree::node is nullptr");
        AVLNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = std::max(height(k1->left), height(k1->right)) + 1;
        k2->height = std::max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }

    void doubleWithRightChild(AVLNode*& k1) {    /*case 3*/
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }

    /*automatically re-balances after insertion*/
    V& insert(const T &x, AVLNode*& t) {
        if (t == nullptr) {
            t = new AVLNode(x, nullptr, nullptr);
        }
        else if (x < t->data) {
            insert(x, t->left);
            if (height(t->left) - height(t->right) == 2) {
                if (x < t->left->data) { rotateWithLeftChild(t); }
                else { doubleWithLeftChild(t); }
            }
        } else if (t->data < x) {
            insert(x, t->right);
            if (height(t->right) - height(t->left) == 2) {
                if (t->right->data < x) { rotateWithRightChild(t); }
                else { doubleWithRightChild(t); }
            }
        } else if (t->data == x) {
            return t->value;
        }
        t->height = std::max(height(t->left), height(t->right)) + 1;
        return t->value;
    }

    /*return bool if element exists within tree*/
    bool contains(const T &element, AVLNode* temp) {
        while (temp != nullptr) {
            if (temp->data == element) return true;
            else if (temp->data < element) temp = temp->right;
            else temp = temp->left;
        }
        return false;
    }

    /*return value by ref. when given key so
     * that node can be augmented outside of class*/
    V& search(const T &element, AVLNode*& temp) {
        if(!temp){
            cout << "Please try again." << endl;
        }
        else{
            if (temp->data == element) return temp->value;
            else if (temp->data < element) return search(element, temp->right);
            else return search(element, temp->left);
        }
    }

    /*empties tree*/
    void clear(AVLNode*& temp) {
        if (temp != nullptr) {
            clear(temp->left);
            clear(temp->right);
            delete temp;
            temp = nullptr;
        }
    }

    void post_order_persist(AVLNode*& temp){
        if (temp != nullptr) {
            post_order_persist(temp->left);
            post_order_persist(temp->right);
            vec.push_back(make_pair(temp->data, temp->value));
        }
    }

    /**
     * Height of a node, if it is not found, return -1
     * @param node node in tree to receive height of
     * @return Height of the given node
     */
    int height(AVLNode *node) const {
        return (node == nullptr ? -1 : node->height);
    }

public:
    DSAvlTree() {
        root = nullptr;
    }

    DSAvlTree(const DSAvlTree &tree) {
        *this = tree;
        this->count_nodes = tree.count_nodes;
    }

    ~DSAvlTree() {
        clear();
    }

    void clear() {
        clear(root);
        root = nullptr;
        count_nodes = 0;
    }

    V& insert(const T &element) {
        count_nodes++;
        return insert(element, root);
    }

    int size() {
        return count_nodes;
    }

    /*returns bool whether tree contains info or not*/
    bool is_empty() {
        return count_nodes == 0;
    }


    /*return value of node if data exist in the tree*/
    V& find (const T& element) {
        try{
            return search(element, root);
        } catch(exception &e){
            cout << "Uh Oh... The word(s) you're looking for weren't found." << endl;
        }
    }

    /**
     * searches if the given element is in the tree
     * returns true if element is found and false if not
     */
    bool contains(const T &element) {
        return (contains(element, root));
    }

    void post_order_persist_pub(){
        post_order_persist(root);
    }

    /*returns vector containing every node in the tree*/
    vector<pair<T, V>>& getVec(){
        return vec;
    }
};

#endif /*INC_21F_FINAL_PROJ_TEMPLATE_DSAVLTREE_H*/