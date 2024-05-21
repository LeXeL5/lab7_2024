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
	int getDepth(Node* current, bool isLeft) {
		if (isLeft) {
			if (current->left != nullptr) return current->left->depth;
		}
		else {
			if (current->right != nullptr) return current->right->depth;
		}
		return 0;
	}
	Node* doubleTurn(Node* current) {
		if (getDepth(current, true) - 1 > getDepth(current, false)) {
			if (getDepth(current->left, true) < getDepth(current->left, false)) {
				//current->left = 
				turn(current->left, true);
			}
			current = turn(current, false);
			current = current->right;
		}
		else if (getDepth(current, false) - 1 > getDepth(current, true)) {
			if (getDepth(current->right, false) < getDepth(current->right, true)) {
				//current->right =
				turn(current->right, false);
			}
			current = turn(current, true);
			current = current->left;
		}
		return current;
	}
	void Balance(Node* current = nullptr) {
		if (current == nullptr) current = root;

		if (current == nullptr) return;
		if (current->depth < 3) return;
		if (current->left != nullptr) Balance(current->left);
		if (current->right != nullptr) Balance(current->right);

		current = doubleTurn(current);
		if (current == nullptr) return;
		if (current->depth < 3) return;

		while (current->depth - current->minDepth > 1) {
			current = doubleTurn(current);
			if (current == nullptr) return;
			if (current->depth < 3) return;
		}
	}
	void depthFixer(Node* current) {
		while (current != nullptr) {
			unsigned short left = 0;
			unsigned short right = 0;
			unsigned short minLeft = 0;
			unsigned short minRight = 0;
			if (current->left != nullptr) {
				left = current->left->depth;
				minLeft = current->left->minDepth;
			}
			if (current->right != nullptr) {
				right = current->right->depth;
				minRight = current->right->minDepth;
			}
			current->depth = max(left, right) + 1;
			current->minDepth = (min(minLeft, minRight) + 1);
			current = current->up;
		}
	}
	void req(int* array, int& index, Order order, Node* current) {
		if (current == nullptr) return;
		if (order == Prefix) {
			cout << current->value << " max: " << current->depth << " min: " << current->minDepth << endl;
			array[index] = current->value;
			index++;
		}
		req(array, index, order, current->left);
		if (order == Infix) {
			cout << current->value << " max: " << current->depth << " min: " << current->minDepth << endl;
			array[index] = current->value;
			index++;
		}
		req(array, index, order, current->right);
		if (order == Postfix) {
			cout << current->value << " max: " << current->depth << " min: " << current->minDepth << endl;
			array[index] = current->value;
			index++;
		}
	}
	int* ToArray(Order order = Infix) {
		int* array = new int[count()];
		int index = 0;
		req(array, index, order, root);
		return array;
	}
	Node* turn(Node* current, bool isLeft) {
		if (current == nullptr) return current;
		Node* parent = current->up;
		Node* child = nullptr;
		Node* grandchild = nullptr;
		if (isLeft) {
			child = current->right;
			if (child == nullptr) return current;
			grandchild = child->left;
			child->left = current;
			current->right = grandchild;
		}
		else {
			child = current->left;
			if (child == nullptr) return current;
			grandchild = child->right;
			child->right = current;
			current->left = grandchild;
		}
		if (parent == nullptr) {
			root = child;
		}
		else {
			if (parent->left == current) {
				parent->left = child;
			}
			else {
				parent->right = child;
			}
		}
		if (grandchild != nullptr) grandchild->up = current;
		current->up = child;
		child->up = parent;
		depthFixer(current);
		return child;
	}
	Node* turnOld(Node* current, bool isLeft)   {
		if (current == nullptr) return current;
		Node* parent = current->up;
		Node* child = nullptr;
		if (isLeft) {
			child = current->right;
		}
		else {
			child = current->left;
		}
		if (child == nullptr) return current;
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
		return child;
	}
	void ToLeft(int value) {
		Node* current = goTo(value);
		turn(current, true);
	}
	void ToRight(int value) {
		Node* current = goTo(value);
		turn(current, false);
	}
	Node* goTo(int value) {
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				return current;
			}
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return nullptr;
	}
	Node* goToParent(int value) {
		Node* parent = nullptr;
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				return nullptr;
			}
			parent = current;
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return parent;
	}
	void add(int value) {
		if (!size) {
			Node* newNode = new Node;
			newNode->value = value;
			size++;
			root = newNode;
			return;
		}
		Node* parent = goToParent(value);
		if (parent == nullptr) return;
		Node* newNode = new Node;
		size++;
		newNode->up = parent;
		newNode->value = value;
		if (value > parent->value) {
			parent->right = newNode;
		}
		else {
			parent->left = newNode;
		}
		depthFixer(newNode);
	}
	void remove(int value) {
		Node* current = goTo(value);
		if (current == nullptr) return;
		Node* parent = current->up;
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
	}
	void clear() {
		clear(root);
		root = nullptr;
		size = 0;
	}
	bool contains(int value) {
		if (goTo(value) != nullptr) {
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
int main() {
	Tree tree;
	int input = 0;
	cout << "Enter numbers to add (sequence end sign 0):" << endl;
	cin >> input;
	while (input) {
		tree.add(input);
		cin >> input;
	}
	
	int* arr = tree.ToArray(tree.Postfix);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	
	//tree.ToRight(5);
	tree.Balance();

	int* arr2 = tree.ToArray(tree.Postfix);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr2[i] << " ";
	}
	cout << endl;
	
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
	return 0;
}