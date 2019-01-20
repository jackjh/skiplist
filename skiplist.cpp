/*
����skip list����ʵ��
*/
#include <iostream>
#include <cstdlib>

using namespace std;

const int maxLevel = 12;

struct skipListNode {
	int key;
	int value;
	skipListNode* next_nodes[1];	//���ָ������
};

struct _skipList {
	int level;				//��ǰ���������Ĳ������߸߶�
	skipListNode* head;
};

class SkipList {
public:
	//����Ľڵ㴴��
	skipListNode* creatNode(int level, int key, int value) {
		skipListNode* node = (skipListNode*)malloc(sizeof(skipListNode) + level * sizeof(skipListNode*));
		node->key = key;
		node->value = value;
		
		return node;
	}

	//�����ʼ��	skiplist�Ĺ��캯��
	SkipList() {
		//srand((int)time_t(0));
		slist = (_skipList*)malloc(sizeof(_skipList));
		slist->head = creatNode(maxLevel - 1, 0, 0);
		slist->level = 0;
		for (int i = 0; i < maxLevel; i++) {
			slist->head->next_nodes[i] = NULL;
		}
	}

	//����Ԫ�ص�ʱ��Ԫ����ռ�еĲ�����ȫ������ģ�ͨ������㷨����
	int randomLevel() {
		int k = 1;
		while (rand() % 2)
			k++;
		k = (k > maxLevel) ? maxLevel : k;

		return k;
	}
	
	void skipList_insert(int key, int value) {
		skipListNode* update[maxLevel];		// ǰ���ڵ����飬������½ڵ��ǰ��һ���ڵ�
		skipListNode* p = slist->head;
		skipListNode* q = NULL;
		int k = slist->level;

		//����߲����²�����Ҫ�����λ��	������update
		for (int i = k - 1; i >= 0; i--) {
			while (p->next_nodes[i] != NULL && p->next_nodes[i]->key < key) {
				p = p->next_nodes[i];
			}
			update[i] = p;
		}

		q = p->next_nodes[0];
		//���ܲ�����ͬ��key
		if (q && q->key == key) {
			cout << "���ܲ�����ͬ��key��" << endl;
			return;
		}

		//����һ��������� k
		//�½�һ��������ڵ� q��һ��һ�����
		int _k = randomLevel();
		//cout << _k << " ";
		//���������level
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

	//����ָ�� key �� value
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
	// ����skipilist��ǰ�ĸ߶�
	int GetLevel() const {
		return slist->level;
	}

	// ����ؼ��� key ���� node����еĹؼ��� key����return true
	bool KeyIsGreaterThanNode(const int& key, skipListNode* node) const {
		return ((node != NULL) && (node->key < key));
	}

	// ���ش��ڻ���ڹؼ��� key �Ľ������С��һ����㣻���û�������Ľ�㣬return NULL
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

	// ����С��key�ؼ��ֵĽ���У�������һ�����
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

	// ����skiplist�����е����һ����㣬�������Ϊ�գ���return head
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