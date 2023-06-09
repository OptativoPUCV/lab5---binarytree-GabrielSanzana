#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap *localTreeMap = (TreeMap *) malloc(sizeof(TreeMap));
    localTreeMap->lower_than = lower_than;
    localTreeMap->current = NULL;
    localTreeMap->root = NULL;
    return localTreeMap;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode *aux = tree->root;
  TreeNode *auxParent = NULL;
  while(aux != NULL)
  {
    if(is_equal(tree, aux->pair->key, key))
      return;
    auxParent = aux;
    
    if (tree->lower_than(aux->pair->key, key) == 1)
    {
      aux = aux->right;
    }
    else
      aux = aux->left;
  }
  aux = createTreeNode(key, value);
  aux->parent = auxParent; 
  if (tree->lower_than(aux->pair->key, auxParent->pair->key) == 1)
  {
    auxParent->left = aux;
  }
  else
    auxParent->right = aux;
  tree->current = aux;
}

TreeNode * minimum(TreeNode * x){
  while(x->left!=NULL)
    x = x->left;
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(node->left == NULL && node->right == NULL)
  {
    if(node->parent->left == node)
      node->parent->left=NULL;
    else
      node->parent->right=NULL;
  }
  else if(node->left == NULL && node->right !=NULL)
  {
    if(node->parent->left == node)
    {
      node->right->parent = node->parent;
      node->parent->left=node->right;
    }
    else
    {
      node->right->parent = node->parent;
      node->parent->right=node->right;
    }
  }
  else if(node->left != NULL && node->right == NULL)
  {
    if(node->parent->left == node)
    {
      node->left->parent = node->parent;
      node->parent->left=node->left;
    }
    else
    {
      node->left->parent = node->parent;
      node->parent->right=node->left;
    }
  }
  else 
  {
    TreeNode *aux = minimum(node->right);
    node->pair->key = aux->pair->key;
    node->pair->value = aux->pair->value;
    removeNode(tree, aux);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode *aux = tree->root;
  while(aux!=NULL)
  {
    if(is_equal(tree, aux->pair->key, key))
    {
      tree->current = aux;
      return aux->pair;
    }
    else
    {  
      if (tree->lower_than(aux->pair->key,key) == 1)
        aux = aux->right;
      else
       aux = aux->left;
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  tree->current = tree->root;
  TreeNode *aux=NULL;
  while(tree->current!=NULL)
  {
    if(is_equal(tree, tree->current->pair->key, key))
    {
      return tree->current->pair;
    }
    else
    {  
      if (tree->lower_than(tree->current->pair->key,key) == 1)
       tree->current = tree->current->right;
      else
      {
       aux=tree->current; 
       tree->current = tree->current->left;
      }
    }
  }
  if(aux!=NULL)
    return aux->pair;
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  return minimum(tree->root)->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *aux = tree->current;
  if(tree->current->right !=NULL)
  {
    aux = minimum(aux->right);
    tree->current = aux;
    return aux->pair;
  }
  
  TreeNode *padreAux = aux->parent;
  while (padreAux != NULL && padreAux->right == aux)
  {
    aux = aux->parent;
    padreAux = padreAux->parent;
  }
  
  if(padreAux==NULL)
    return NULL;
  else
  {
    tree->current = padreAux;
    return padreAux->pair;
  }
  
}
