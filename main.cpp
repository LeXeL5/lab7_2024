#include <iostream>
using namespace std;

struct Tree {
	struct Node {
		Node* left = nullptr;
		Node* right = nullptr;
		Node* up = nullptr;
		int value = 0;
		unsigned short depth = 1;
		unsigned short minDepth = 0;
	};
	enum Order { Prefix, Infix, Postfix };
	int size = 0;
	Node* root = nullptr;

	void depthFixer(Node* current) {
		while (current != nullptr) {
			int left = 0;
			int right = 0;
			int minLeft = 0;
			int minRight = 0;
			if (current->left != nullptr) {
				left = current->left->depth;
				minLeft = current->left->minDepth;
			}
			if (current->right != nullptr) {
				right = current->right->depth;
				minRight = current->right->depth;
			}
			current->depth = max(left, right) + 1;
			current->minDepth = (min(minLeft, minRight) + 1);
			current = current->up;
		}
	}
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
			//cout << current->value << " max: " << current->depth << " min: " << current->minDepth << endl;
			array[index] = current->value;
			index++;
		}
	}
	int* ToArray(Order order = Infix) {
		int* array = new int[count()];
		req(array, 0, order, root);
		return array;
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
		child->up = parent;
		current->up = child;
		if (parent == nullptr) {
			root = child;
		}
		else {
			if (parent->left == current) parent->left = child;
			else parent->right = child;
		}
		if (isLeft) {
			current->right = child->left;
			if (child->left != nullptr) child->left->up = current;
			child->left = current;

		}
		else {
			current->left = child->right;
			if (child->right != nullptr) child->right->up = current;
			child->right = current;
		}
		depthFixer(current);
		current = child;
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
		newNode->up = parent;
		newNode->value = value;
		size++;
		if (value > parent->value) {
			parent->right = newNode;
		}
		else {
			parent->left = newNode;
		}
		depthFixer(newNode);
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
		if (child != nullptr) child->up = parent;
		depthFixer(current); 
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
	cout << "Enter numbers to add (sequence end sign 0):" << endl;
	cin >> input;
	while (input) {
		tree.add(input);
		cin >> input;
	}

	//int* arr = tree.ToArray(tree.Postfix);
	//for (int i = 0; i < tree.count(); i++) {
	//	cout << arr[i] << " ";
	//}
	//cout << endl;

	//cout << "Enter numbers to remove (sequence end sign 0):" << endl;
	//cin >> input;
	//while (input) {
	//	tree.remove(input);
	//	cin >> input;
	//}

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