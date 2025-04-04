#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
    void rotateLeft(AVLNode<Key, Value>* x);
    void rotateRight(AVLNode<Key, Value>* x);
    void insert(const std::pair<const Key, Value> &new_item); 
    void insertFix(AVLNode<Key, Value>* x, AVLNode<Key, Value>* item); 
    void remove(const Key& key); 
    void removeFix(AVLNode<Key, Value>* x, int8_t diff); 
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void rotateRight(AVLNode<Key, Value>*x); 
    virtual void rotateLeft(AVLNode<Key, Value>*x); 
    virtual void insertFix(AVLNode<Key, Value>* node, AVLNode<Key,Value>* item);
    virtual void removeFix(AVLNode<Key, Value>* node, int8_t diff); 

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{

    // TODO
    if(this->root_ == NULL){
       this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);  
    }
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_); 
    AVLNode<Key, Value>* parent = NULL; 
    while(curr != NULL){
      parent = curr; 
      if(new_item.first < curr->getKey()){
        curr = curr->getLeft(); 
      } else if (new_item.first > curr->getKey()){
        curr = curr->getRight(); 
      } else {
        curr->setValue(new_item.second); 
        return; 
      }
    }
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if(new_item.first < parent->getKey()){
      parent->setLeft(newNode); 
    } else {
      parent->setRight(newNode); 
    }

    int8_t dir = 0;
    if(parent->getBalance() != 0){
      parent->setBalance(0); 
    } else {
 
      if(new_item.first < parent->getKey()){
        dir = -1; 
      } else {
        dir = 1; 
      }
      parent->updateBalance(dir); 
      insertFix(parent, newNode); 
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key)); 
    if(temp == NULL){
      return; 
    }
    if(temp->getLeft() != NULL && temp->getRight() != NULL){
      AVLNode<Key, Value>* succ = static_cast<AVLNode<Key, Value>*> (BinarySearchTree<Key, Value>::predecessor(temp));
      if(succ != NULL){
        nodeSwap(temp, succ); 
      }
    }
    AVLNode<Key, Value>* p = temp->getParent();
    AVLNode<Key, Value>* child = NULL;
    if(temp->getLeft() != NULL){
      child = temp->getLeft(); 
    } else {
      child = temp->getRight(); 
    }
    int8_t diff = 0; 
    if(p != NULL){
      if(temp == p->getLeft()){
        p->setLeft(child);
        diff = 1; 
      } else if (temp == p->getRight()){
        p->setRight(child);
        diff = -1; 
      }
    }
    delete temp; 
    if(p == NULL)
      this->root_ = child;
    if(child != NULL)
      child->setParent(p);
    removeFix(p, diff); 
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x)
{
    AVLNode<Key, Value>* c = x->getLeft(); 
    AVLNode<Key, Value>* p = x->getParent(); 
    AVLNode<Key, Value>* temp = c->getRight(); 

    if(x->getParent() != NULL){
      if(p->getRight() == x){
        p->setRight(c); 
      } else {
        p->setLeft(c); 
      } 
    } else {
      this->root_ = c; 
    }
    if(temp != NULL){
      temp->setParent(x); 
    }
    x->setLeft(temp); 
    c->setParent(p); 
    c->setRight(x); 
    x->setParent(c); 
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x)
{
    AVLNode<Key, Value>* c = x->getRight(); 
    AVLNode<Key, Value>* p = x->getParent(); 
    AVLNode<Key, Value>* temp = c->getLeft(); 

    if(x->getParent() != NULL){
      if(p->getRight() == x){
        p->setRight(c); 
      } else {
        p->setLeft(c); 
      }
    } else {
      this->root_ = c; 
    }
    if(temp != NULL){
      temp->setParent(x); 
    }
    x->setRight(temp);
    c->setParent(p);
    c->setLeft(x); 
    x->setParent(c); 
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* node, AVLNode<Key, Value>* item)
{
  if(node == NULL || node->getParent() == NULL){
    return; 
  }
  AVLNode<Key, Value>* gp = node->getParent(); 
  if(node == gp->getLeft()){
    gp->updateBalance(-1); 
    if(gp->getBalance() == 0){
      return; 
    } else if (gp->getBalance() == -1){
      insertFix(gp, node); 
    } else if (gp->getBalance() == -2){
      if(node->getBalance() == -1){
        rotateRight(gp); 
        node->setBalance(0); 
        gp->setBalance(0); 
      } else if(node->getBalance() == 1){
        rotateLeft(node); 
        rotateRight(gp); 
        if(item->getBalance() == -1){
          node->setBalance(0); 
          gp->setBalance(1); 
          item->setBalance(0); 
        } else if(item->getBalance() == 0){
          node->setBalance(0); 
          gp->setBalance(0); 
          item->setBalance(0);  
        } else if (item->getBalance() == 1){
          node->setBalance(-1);
          gp->setBalance(0); 
          item->setBalance(0); 
        }
      }
    }
  }
  if(node == gp->getRight()){
    gp->updateBalance(1); 
    if(gp->getBalance() == 0){
      return; 
    } else if(gp->getBalance() == 1){
      insertFix(gp, node); 
    } else if (gp->getBalance() == 2){
      if(node->getBalance() == 1){
        rotateLeft(gp); 
        node->setBalance(0); 
        gp->setBalance(0);  
      } else if (node->getBalance() == -1){
        rotateRight(node); 
        rotateLeft(gp); 
        if(item->getBalance() == 1){
          node->setBalance(0); 
          gp->setBalance(-1); 
          item->setBalance(0); 
        } else if(item->getBalance() == 0){
          node->setBalance(0); 
          gp->setBalance(0); 
          item->setBalance(0); 
        } else if (item->getBalance() == -1){
          node->setBalance(1); 
          gp->setBalance(0); 
          item->setBalance(0); 
        }
      }
    }  
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
  if(node == NULL){
    return; 
  }
  AVLNode<Key, Value>* p = node->getParent(); 
  int8_t ndiff = 0; 
  if(p != NULL){
    if(node == p->getLeft()){
      ndiff = 1; 
    } else {
      ndiff = -1; 
    }
  }
  if(diff == -1){
    if(node->getBalance() + diff == -2){
      AVLNode<Key, Value>* c = node->getLeft(); 
      if(c->getBalance() == -1){
        rotateRight(node); 
        node->setBalance(0); 
        c->setBalance(0); 
        removeFix(p, ndiff); 
      } else if (c->getBalance() == 0){
        rotateRight(node); 
        node->setBalance(-1); 
        c->setBalance(1); 
      } else if (c->getBalance() == 1){
        AVLNode<Key, Value>* g = c->getRight(); 
        rotateLeft(c); 
        rotateRight(node); 
        if(g->getBalance() == 1){
          node->setBalance(0); 
          c->setBalance(-1); 
          g->setBalance(0); 
        } else if (g->getBalance() == 0){
          node->setBalance(0); 
          c->setBalance(0); 
          g->setBalance(0); 
        } else if (g->getBalance() == -1){
          node->setBalance(1); 
          c->setBalance(0); 
          g->setBalance(0); 
        }
        removeFix(p, ndiff); 
      }
    } else if (node->getBalance() + diff == -1){
      node->setBalance(-1); 
    } else if (node->getBalance() + diff == 0){
      node->setBalance(0); 
      removeFix(p, ndiff); 
    }
  } else if(diff == 1){
    if(node->getBalance() + diff == 2){
      AVLNode<Key, Value>* c = node->getRight(); 
      if(c->getBalance() == 1){
        rotateLeft(node); 
        node->setBalance(0); 
        c->setBalance(0); 
        removeFix(p, ndiff); 
      } else if (c->getBalance() == 0){
        rotateLeft(node); 
        node->setBalance(1); 
        c->setBalance(-1); 
      } else if (c->getBalance() == -1){
        AVLNode<Key, Value>* g = c->getLeft(); 
        rotateRight(c); 
        rotateLeft(node); 
        if(g->getBalance() == -1){
          node->setBalance(0); 
          c->setBalance(1); 
          g->setBalance(0); 
        } else if (g->getBalance() == 0){
          node->setBalance(0); 
          c->setBalance(0); 
          g->setBalance(0); 
        } else if (g->getBalance() == 1){
          node->setBalance(-1); 
          c->setBalance(0); 
          g->setBalance(0); 
        }
        removeFix(p, ndiff); 
      }
    } else if (node->getBalance() + diff == 1){
      node->setBalance(1); 
    } else if (node->getBalance() + diff == 0){
      node->setBalance(0); 
      removeFix(p, ndiff); 
    }
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
