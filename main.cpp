#include <iostream>
using namespace std;

struct Node {
	Node* left = nullptr;
	Node* right = nullptr;
	int value = 0;
};
struct Tree {
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
	int minLeafDepth(Node* current){
		if (current == nullptr) return 0;
		int left = minLeafDepth(current->left);
		int right = minLeafDepth(current->right);
		return min(left, right) + 1;
	}
	void ToLeft(int value) {
		Node* parent = nullptr;
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				break;
			}
			parent = current;
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		if (current == nullptr) return;
		Node* child = current->right;
		if (child == nullptr) return;
		if (parent == nullptr) {
			root = child;
		}
		else {
			if (parent->left == current) parent->left = child;
			else parent->right = child;
		}
		current->right = child->left;
		child->left = current;
	}
	void ToRight(int value) {
		Node* parent = nullptr;
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				break;
			}
			parent = current;
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		if (current == nullptr) return;
		Node* child = current->left;
		if (child == nullptr) return;
		if (parent == nullptr) {
			root = child;
		}
		else {
			if (parent->left == current) parent->left = child;
			else parent->right = child;
		}
		current->left = child->right;
		child->right = current;
	}
	void Balance(Node* current = nullptr) {
		if (current == nullptr) current = root;
		if (current->left != nullptr) Balance(current->left);
		if (current->right != nullptr) Balance(current->right);
		if (subtreeDepth(current) < 3) return;
		bool isLeft = true;
		if (subtreeDepth(current->left) - 1 > subtreeDepth(current->right)) {
			isLeft = true;
			if (subtreeDepth(current->left->left) >= subtreeDepth(current->left->right)) {
				//левая внешняя ветка самая глубокая
				ToRight(current->value);
			}
			else {
				//левая внутренняя ветка самая глубокая
				ToLeft(current->left->value);
				ToRight(current->value);
			}
		}
		else if (subtreeDepth(current->left) < subtreeDepth(current->right) - 1) {
			isLeft = false;
			if (subtreeDepth(current->right->right) >= subtreeDepth(current->right->left)) {
				//правая внешняя ветка самая глубокая
				ToLeft(current->value);
			}
			else {
				// правая внутренняя ветка самая глубокая
				ToRight(current->right->value);
				ToLeft(current->value);
			}
		}
		//int* arr = ToArray(Postfix);
		//for (int i = 0; i < count(); i++) {
		//	cout << arr[i] << " ";
		//}
		//cout << endl;
		while (subtreeDepth(current) > 3) {
			if ((subtreeDepth(current) - minLeafDepth(current)) > 1) {
				if (subtreeDepth(current->left) > subtreeDepth(current->right)) {
					ToRight(current->value);
				}
				else {
					ToLeft(current->value);
				}
			}
			if (isLeft) {
				current = current->right;
			}
			else {
				current = current->left;
			}
		}
	}
	/*Node* goTo2(int value, Node*& parent) {
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
		parent = nullptr;
		return nullptr;
	}*/
	Node* goTo(int value) {
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				return current;
			}
			else if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return nullptr;
	}
	void add(int value) {
		Node* newNode = new Node;
		newNode->value = value;
		size++;
		if (count() == 1) {
			root = newNode;
			return;
		}
		Node* current = root;
		while (true) {
			if (value < current->value) {
				if (current->left != nullptr) {
					current = current->left;
				}
				else {
					current->left = newNode;
					return;
				}
			}
			else if (value > current->value) {
				if (current->right != nullptr) {
					current = current->right;
				}
				else {
					current->right = newNode;
					return;
				}
			}
			else if (value == current->value) {
				delete newNode;
				size--;
				return;
			}
		}
	}
	void remove(int value) {
		Node* parent = nullptr;
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				break;
			}
			parent = current;
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
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
void main() {
	Tree tree;
	int input = 0;
	cout << "Enter sequence of numbers (sequence end sign 0):" << endl;
	cin >> input;
	while (input) {
		tree.add(input);
		cin >> input;
	}
	/*cout << tree.subtreeDepth(tree.root) << endl;
	cout << tree.minLeafDepth(tree.root) << endl;

	int* arr = tree.ToArray(Postfix);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

	tree.Balance();

	arr = tree.ToArray(Postfix);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	*/
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