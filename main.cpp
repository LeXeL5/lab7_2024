#include <iostream>
using namespace std;


struct Tree {
	struct Node {
		Node* left = nullptr;
		Node* right = nullptr;
		int value = 0;
	};
	struct Heightmap {
		Node* left = nullptr;
		Node* right = nullptr;
	};

	enum Order { Prefix, Infix, Postfix };
	int size = 0;
	Node* root = nullptr;

	int req(int* array, int index, Order order, Node* current) {
		if (current == nullptr) return index;
		if (order == Prefix) {
			array[index] = current->value;
			index++;
		}
		index = req(array, index, order, current->left);
		if (order == Infix) {
			array[index] = current->value;
			index++;
		}
		index = req(array, index, order, current->right);
		if (order == Postfix) {
			array[index] = current->value;
			index++;
		}
	}
	int* ToArray(Order order = Infix) {
		int* array = new int[count()];
		req(array, 0, order, root);
		return array;
	}

	int subtreeDepth(Node* current) {
		if (current == nullptr) return 0;
		int left = subtreeDepth(current->left);
		int right = subtreeDepth(current->right);
		return max(left, right) + 1;
	}
	int minLeafDepth(Node* current) {
		if (current == nullptr) return 0;
		int left = minLeafDepth(current->left);
		int right = minLeafDepth(current->right);
		return min(left, right) + 1;
	}

	void turn(Node*& current, Node* parent, bool isLeft) {
		if (current == nullptr) return;
		Node* child = nullptr;
		if (isLeft) {
			child = current->right;
		}
		else {
			child = current->left;
		}
		if (child == nullptr) return;
		if (parent == nullptr) {
			root = child;
		}
		else {
			if (parent->left == current) parent->left = child;
			else parent->right = child;
		}
		if (isLeft) {
			current->right = child->left;
			child->left = current;
		}
		else {
			current->left = child->right;
			child->right = current;
		}
		return;
	}
	void ToLeft(int value) {
		Node* parent = nullptr;
		Node* current = goTo(value, parent);
		turn(current, parent, true);
	}
	void ToRight(int value) {
		Node* parent = nullptr;
		Node* current = goTo(value, parent);
		turn(current, parent, false);
	}

	void Balance(Node* current = nullptr) {

	}
	Node* goTo(int value, Node*& parent) {
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				return current;
			}
			parent = current;
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return nullptr;
	}
	void add(int value) {
		if (!size) {
			Node* newNode = new Node;
			newNode->value = value;
			size++;
			root = newNode;
			return;
		}
		Node* parent = nullptr;
		if (goTo(value, parent) != nullptr) return;
		Node* newNode = new Node;
		newNode->value = value;
		size++;
		if (value > parent->value) {
			parent->right = newNode;
		}
		else {
			parent->left = newNode;
		}
	}
	void remove(int value) {
		Node* parent = nullptr;
		Node* current = goTo(value, parent);
		if (current == nullptr) return;
		if ((current->right != nullptr) && (current->left != nullptr)) {
			parent = current;
			Node* min = current->right;
			while (min->left != nullptr) {
				parent = min;
				min = min->left;
			}
			current->value = min->value;
			current = min;
		}
		Node* child = nullptr;
		if (current->left != nullptr) {
			child = current->left;
		}
		else {
			child = current->right;
		}
		if (current != root) {
			if (parent->left == current) {
				parent->left = child;
			}
			else if (parent->right == current) {
				parent->right = child;
			}
		}
		else {
			root = child;
		}
		delete current;
		size--;
	}
	void clear(Node* current) {
		if (current == nullptr) return;
		clear(current->left);
		clear(current->right);
		delete current;
		size--;
	}
	void clear() {
		clear(root);
		root = nullptr;
	}
	bool contains(int value) {
		Node* parent = nullptr;
		if (goTo(value, parent) != nullptr) {
			return true;
		}
		else {
			return false;
		}
	}
	int count() {
		return size;
	}
};
void main() {
	Tree tree;
	int input = 0;
	cout << "Enter sequence of numbers (sequence end sign 0):" << endl;
	cin >> input;
	while (input) {
		tree.add(input);
		cin >> input;
	}
	
	cout << "Enter numbers to search (sequence end sign 0):" << endl;
	cin >> input;
	while (input) {
		if (tree.contains(input)) {
			cout << "This number is on the tree" << endl;
		}
		else {
			cout << "This number is NOT on the tree" << endl;
		}
		cin >> input;
	}
	tree.clear();
}