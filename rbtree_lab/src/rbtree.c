#include "rbtree.h"

#include <stdlib.h>


void right_rotate(rbtree* t, node_t* x);
void left_rotate(rbtree* t, node_t* x);
void insert_fixup(rbtree *t, node_t* z);
void transplant(rbtree* t, node_t *a, node_t* b);
void delete_fixup(rbtree* t, node_t* p);
void delete_node(rbtree *t, node_t* n);
node_t *subtree_min(const rbtree* t, node_t* n);
void inorder(const rbtree *t, key_t* arr, node_t* node, size_t n, size_t* i);

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    node_t* nil_node = (node_t*)calloc(1, sizeof(node_t));
    // node_t* root = (node_t*)calloc(1, sizeof(node_t));
    nil_node->color = RBTREE_BLACK;
    nil_node->parent = nil_node;
    nil_node->left = nil_node;
    nil_node->right = nil_node;

    p->nil = nil_node;
    p->root = p->nil;

    return p;
}

void delete_node(rbtree *t, node_t* n) {
  // TODO: reclaim the tree nodes's memory
  if(n==t->nil){return;}
  delete_node(t, n->left);
  delete_node(t, n->right);
  free(n);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t* node = (node_t*)calloc(1, sizeof(node_t));
    node->color = RBTREE_RED;
    node->parent = t->nil;
    node->left = t->nil;
    node->right = t->nil;
    node->key = key;

    if (t->root == t->nil){
      t->root = node;
      node->color = RBTREE_BLACK;
      return node;
    }

    node_t* cur = t->root;
    node_t* prev = NULL;

    while(cur!=t->nil){
      if(key<cur->key){
        prev = cur;
        cur = cur->left;
      }
      else {
        prev = cur;
        cur = cur->right;
      }
    }

    if(cur==t->nil){
      if(prev->key > key){
        prev->left = node;
        node->parent = prev;
      }
      else{
        prev->right = node;
        node->parent = prev;
      }
    }
  
    insert_fixup(t, node);
  
    return node;
}

void left_rotate(rbtree* t, node_t* x){
    node_t* y = x->right;
    x->right = y->left;
    if(y->left != t->nil){
      y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == t->nil){
      t->root = y;
    }
    else if(x == x->parent->left){
      x->parent->left = y;
    }
    else{
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(rbtree* t, node_t* x){
  node_t* y = x->left;
  x->left = y->right;
  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else if(x == x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t* z){
  node_t* uncle=NULL;
  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->left){
      uncle = z->parent->parent->right;

      if (uncle->color == RBTREE_RED){
        uncle->color = RBTREE_BLACK;
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
     
      }
    }
    else{
      uncle = z->parent->parent->left;

      if(uncle->color == RBTREE_RED){
        uncle->color = RBTREE_BLACK;
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if(!t){return NULL;}
  node_t* cur = t->root;
  while(cur != t->nil){
    if(cur->key == key){
      return cur;
    }
    else if(cur->key > key){
      cur = cur->left;
    }
    else{
      cur = cur->right;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (!t){return NULL;}
  node_t* cur = t->root;
  while(cur->left != t->nil){
    cur=cur->left;
  }
  // t->root = cur;
  return cur;
}

node_t *subtree_min(const rbtree* t, node_t* n){
  while(n->left != t->nil){
    n = n->left;
  }
  return n;
}

node_t *rbtree_max(const rbtree* t) {
  // TODO: implement find
  if (t == NULL){return NULL;}
  
  node_t* cur = t->root;
  while(cur->right != t->nil){

    cur=cur->right;

  }
  return cur;
}

void transplant(rbtree* t, node_t *a, node_t* b){ //a가 삭제할 노드, b가 자식 노드
  // 일단 노드 a의 parent가 t->root일때
  if(a->parent == t->nil){
    t->root = b;
  }
  else if(a == a->parent->left){
    a->parent->left = b;
  }
  else{
    a->parent->right = b;
  }
  b->parent = a->parent;
  
}

void delete_fixup(rbtree* t, node_t* p){
  node_t* brother;
  while(p != t->root && p->color == RBTREE_BLACK){
    if(p == p->parent->left){
      brother = p->parent->right;
      // case 1
      if(brother->color == RBTREE_RED){
        brother->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        left_rotate(t, p->parent);
        brother = p->parent->right;
      }
      // case 2
      if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        p = p->parent;
      }
      else{
      // case 3
        if(brother->color == RBTREE_BLACK && brother->left->color == RBTREE_RED && brother->right->color == RBTREE_BLACK){
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          right_rotate(t, brother);
          brother = p->parent->right;
        }
        // case 4
        brother->color = p->parent->color;
        brother->right->color = RBTREE_BLACK;
        p->parent->color = RBTREE_BLACK;
        left_rotate(t, p->parent);
        p=t->root; //while 루프 종료
      }
    }
    else{
      brother = p->parent->left;
      //case 1
      if(brother->color == RBTREE_RED){
        brother->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        right_rotate(t, p->parent);
        brother = p->parent->left;
      }
      //case2
      if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        p = p->parent;
      }
      else{
      //case3
        if(brother->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_RED){
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          left_rotate(t, brother);
          brother = p->parent->left;
        }
        //case4
        brother->color = p->parent->color;
        brother->left->color = RBTREE_BLACK;
        p->parent->color = RBTREE_BLACK;
        right_rotate(t, p->parent);
        p=t->root;
      }
    }
  }
  p->color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *p) {
    // TODO: implement erase

    if(rbtree_find(t, p->key) == NULL){return -1;}

    node_t* y = p;
    color_t original_color = y->color;
    node_t* x;
    // 자식이 하나 이하로 있는 경우(right)
    if(p->left == t->nil){
      x = p->right;
      transplant(t, p, p->right);
      
    }
    // 자식이 하나 이하로 있는 경우(left)
    else if(p->right == t->nil){
      x = p->left;
      transplant(t, p, p->left);
      
    }
    // 자식이 2명
    else{
      y = subtree_min(t, p->right); // 오른 쪽에서 제일 작은 값(p와 제일 가까우면서 큰값) 저장
      original_color = y->color; // 찾은 값의 색상 저장
      x = y->right; // x는 y의 오른쪽 자식
      if(y!=p->right){ // y가 p의 오른쪽 자식이 아닐 경우
        transplant(t, y, y->right); // 기존 y 위치를 y의 자식에 연결
        y->right = p->right; // y의 오른쪽에 P의 오른쪽 전부 빼앗음
        y->right->parent = y; // 빼앗은 오른쪽의 부모를 y로 선언
      }
      else{
        x->parent = y; // y와 x의 값을 확실히 이어준다.
      }
      transplant(t,p,y); // p 대신 오른쪽 자식인 y를 이식
      y->left = p->left; // y의 왼쪽에 P의 왼쪽을 이식
      y->left->parent = y; // 방금 이식한 기존 p의 왼쪽 부모를 y로 변경
      y->color = p->color; // 변경한 y의 색을 기존 색으로 변경
      // free(p);
      
    }
    if(original_color == RBTREE_BLACK){
      delete_fixup(t,x);
    }
    free(p);
    return 0;
}

void inorder(const rbtree *t, key_t* arr, node_t* node, size_t n, size_t* i){
  if(node == t->nil || *i == n){return;}
  inorder(t, arr, node->left, n, i);

  if(*i < n){
    arr[*i] = node->key;
    (*i)++;
  }

  if(*i < n){
    inorder(t, arr, node->right, n, i);
  }

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  size_t i = 0;
  if(n == 0){return 0;}
  if(t->root == t->nil){return 0;}
  inorder(t, arr, t->root, n, &i);

  return 0;
}
