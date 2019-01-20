/*
跳表（skip list）的实现
*/
#include <iostream>
#include <cstdlib>

using namespace std;

const int maxLevel = 12;

struct skipListNode {
	int key;
	int value;
	skipListNode* next_nodes[1];	//后继指针数组
};

struct _skipList {
	int level;				//当前跳表内最大的层数或者高度
	skipListNode* head;
};

class SkipList {
public:
	//跳表的节点创建
	skipListNode* creatNode(int level, int key, int value) {
		skipListNode* node = (skipListNode*)malloc(sizeof(skipListNode) + level * sizeof(skipListNode*));
		node->key = key;
		node->value = value;
		
		return node;
	}

	//跳表初始化	skiplist的构造函数
	SkipList() {
		//srand((int)time_t(0));
		slist = (_skipList*)malloc(sizeof(_skipList));
		slist->head = creatNode(maxLevel - 1, 0, 0);
		slist->level = 0;
		for (int i = 0; i < maxLevel; i++) {
			slist->head->next_nodes[i] = NULL;
		}
	}

	//插入元素的时候元素所占有的层数完全是随机的，通过随机算法产生
	int randomLevel() {
		int k = 1;
		while (rand() % 2)
			k++;
		k = (k > maxLevel) ? maxLevel : k;

		return k;
	}
	
	void skipList_insert(int key, int value) {
		skipListNode* update[maxLevel];		// 前驱节点数组，插入的新节点的前面一个节点
		skipListNode* p = slist->head;
		skipListNode* q = NULL;
		int k = slist->level;

		//从最高层往下查找需要插入的位置	并更新update
		for (int i = k - 1; i >= 0; i--) {
			while (p->next_nodes[i] != NULL && p->next_nodes[i]->key < key) {
				p = p->next_nodes[i];
			}
			update[i] = p;
		}

		q = p->next_nodes[0];
		//不能插入相同的key
		if (q && q->key == key) {
			cout << "不能插入相同的key！" << endl;
			return;
		}

		//产生一个随机层数 k
		//新建一个待插入节点 q，一层一层插入
		int _k = randomLevel();
		//cout << _k << " ";
		//更新跳表的level
		if (_k > slist->level) {
			for (int i = slist->level; i < _k; i++) {
				update[i] = slist->head;
			}
			slist->level = _k;
		}

		q = creatNode(_k, key, value);

		for (int i = 0; i < _k; i++) {
			q->next_nodes[i] = update[i]->next_nodes[i];
			update[i]->next_nodes[i] = q;
		}
		//printList();
	}

	//搜索指定 key 的 value
	int skipList_search(int key) {
		skipListNode* p = slist->head;
		skipListNode* q = NULL;
		int k = slist->level - 1;

		for (int i = k; i >= 0; i--) {
			while (p->next_nodes[i] != NULL && p->next_nodes[i]->key <= key) {
				if (p->next_nodes[i]->key == key) {
					q = p->next_nodes[i];
					return q->value;
				}
				p = p->next_nodes[i];
			}
		}
		return NULL;
	}
	// 返回skipilist当前的高度
	int GetLevel() const {
		return slist->level;
	}

	// 如果关键字 key 大于 node结点中的关键字 key，则return true
	bool KeyIsGreaterThanNode(const int& key, skipListNode* node) const {
		return ((node != NULL) && (node->key < key));
	}

	// 返回大于或等于关键字 key 的结点中最小的一个结点；如果没有这样的结点，return NULL
	skipListNode* FindGreater_Equal(const int& key) const {
		skipListNode* tmp = slist->head;
		int level = GetLevel() - 1;

		while (true) {
			skipListNode* next = tmp->next_nodes[level];
			if (KeyIsGreaterThanNode(key, next)) {
				tmp = next;
			}
			else {
				if (level == 0) {
					return next;
				}
				else {
					level--;
				}
			}
		}
	}

	// 查找小于key关键字的结点中，最大的那一个结点
	skipListNode* FindLessThanNode(const int& key) const {
		skipListNode* node = slist->head;
		int level = GetLevel() - 1;
		while (true) {
			if (node != slist->head || node->key >= key) {
				return NULL;
			}
			skipListNode* next = node->next_nodes[level];
			if (next == NULL || next->key >= key) {
				if (level == 0) {
					return node;
				}
				else {
					level--;
				}
			}
			else {
				node = next;
			}
		}
	}

	// 返回skiplist跳表中的最后一个结点，如果跳表为空，则return head
	skipListNode* FindLastNode() const {
		skipListNode* node = slist->head;
		int level = GetLevel() - 1;
		while (true) {
			skipListNode* next = node->next_nodes[level];
			if (next == NULL) {
				if (level == 0) {
					return node;
				}
				else {
					level--;
				}
			}
			else {
				node = next;
			}
		}
	}

	void printList() {
		skipListNode* p;
		//skipListNode* q;
		int k = slist->level;
		for (int i = k-1; i >= 0; i--) {
			p = slist->head->next_nodes[i];
			cout << "Level[" << i << "]: ";
			while (p->next_nodes[i] != NULL) {
				cout << p->value << " ";
				p = p->next_nodes[i];
			}
			cout << p->value << endl;
		}
		cout << endl;
	}

private:
	_skipList* slist;
};

int main() {
	SkipList skiplist;
	for (int i = 1; i < 19; i++) {
		//skiplist.skipList_insert(i, 2 * i);
		skiplist.skipList_insert(i, i * 2);
	}

	cout << endl;

	skiplist.printList();

	cout << skiplist.skipList_search(6) << endl;

	return 0;
}