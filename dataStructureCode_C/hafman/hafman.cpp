/**
 * @file hafman.cpp
 * @author your name (you@domain.com)
 * @brief
 * 我们希望压缩一个10万个字符的数据文件，下图给出文件中所出现的字符和他们的出现频率。
 * a - 45%, b - 13%, c - 12%, d - 16%, e - 9%, f - 5%
 * 使用哈夫曼编码为二进制编码，每个字符用一个唯一的二进制串表示，给出压缩率最好的编码方式，也即字符的最优二进制表示。
 * @version 0.1
 * @date 2022-12-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;

struct Node {
  char ch;
  int freq;
  Node *left, *right;

// 树结节点的构造函数
  Node(char ch, int freq) {
    this->ch = ch;
    this->freq = freq;
    this->left = this->right = NULL;
  }
};

struct compare {
  bool operator()(Node *l, Node *r) { return l->freq > r->freq; }
};

void printCodes(Node *root, string str) {
  if (root == NULL) return;
  if (root->ch != '$') cout << root->ch << ": " << str << endl;
//   左孩子为0，右孩子为1
  printCodes(root->left, str + "0");
  printCodes(root->right, str + "1");
}

void Huffman(char *chars, int *freqs, int n) {
    /**priority_queue是一个优先队列，具有三个类型参数，按照频率的大小排列，频率小的在前面，每次取频率最小的两个点合并*/
  priority_queue<Node *, vector<Node *>, compare> pq;
  for (int i = 0; i < n; i++) pq.push(new Node(chars[i], freqs[i]));
  /**当pq.size() == 1时，说明只剩下一个点，即根节点，此时结束循环*/
  while (pq.size() != 1) {
    Node *x = pq.top();
    pq.pop();
    Node *y = pq.top();
    pq.pop();
    Node *z = new Node('$', x->freq + y->freq);
    z->left = x;
    z->right = y;
    pq.push(z);
  }
  printCodes(pq.top(), "");
}

int main() {
  char chars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
  int freqs[] = {45, 13, 12, 16, 9, 5}; // 单位为%
  int n = sizeof(chars) / sizeof(chars[0]);
  Huffman(chars, freqs, n);
  return 0;
}