// Michael J. Hogan
// CS 400 Homework 7

#include <iostream>
#include <vector>
#include <fstream>

typedef struct Node *NodePtr;

// Struct for Huffman tree node, containing links to right and left child nodes, a character, and its frequency
struct Node {
public:
	char ch;
	int frequency;

	Node* left_child;
	Node* right_child;

	Node(int frequency) : ch{ 0 }, frequency{ frequency }, left_child{ nullptr }, right_child{ nullptr } {};
	Node(char ch, int frequency) : ch{ ch }, frequency{ frequency }, left_child{ nullptr }, right_child{ nullptr } {};
};

// Function for file reading operation. Takes in a filename(string type) and returns a vector containing the characters and their respective frequencies.
std::vector<std::pair<char, int>> readFile(std::string file_name);

// Function for constructing the Huffman tree. Returns a pointer to the root of the tree.
NodePtr huffman_tree(std::vector<std::pair<char,int>> frequency);

// Function that takes in the root of a binary tree and an empty string, then prints and returns the Huffman encoding scheme.
std::vector<std::pair<char, std::string>> huffman_encoding(NodePtr root, std::string s);

//Function that takes in the encoding scheme and filename, then outputs encoded content to pride.huff
void huffman_output(std::vector<std::pair<char, std::string>> v, std::string file_name);

int main() {
	NodePtr root;
	std::vector<std::pair<char, std::string>> v;
	root = huffman_tree(readFile("Pride_and_Prejudice.txt"));
	v = huffman_encoding(root, "");
	huffman_output(v, "Pride_and_Prejudice.txt");

	int size = v.size();
	std::ofstream huff;
  	huff.open ("huff.sch");
  	for(int i = 0; i < size; i++) {
  		std::cout << "Outputting: " << v[i].second << ":" << v[i].first << std::endl;
  		huff << v[i].second << ":" << v[i].first << std::endl;
  	}
  	huff.close();
}

std::vector<std::pair<char, int>> readFile(std::string file_name) {
	char ch;
	std::vector<std::pair<char, int>> frequency_table;
	std::fstream fin(file_name, std::fstream::in);
	if(fin.is_open()){
		// If a new character is encountered, it is pushed to the back of the vector with a frequency of 1.
		// If a character that has already been pushed to the vector is encountered, its frequency is incremented by 1.
		while (fin >> std::noskipws >> ch) {
			int temp = frequency_table.size() - 1;
			bool isRepeat = false;
			while (!isRepeat && temp >= 0) {
				if(frequency_table[temp].first == ch) {
					isRepeat = true;
					frequency_table[temp].second++;
				}
				temp--;
			}
			if(!isRepeat) {
				frequency_table.push_back({ch, 1});
			}


		}
		return frequency_table;
	}
	std::cerr << "Unable to open " << file_name << ". Quitting." << std::endl;
	exit(1);
}

NodePtr huffman_tree(std::vector<std::pair<char,int>> frequency) {
	NodePtr head = nullptr;
	std::vector<Node> huff;
	int freq_size = frequency.size();

	// Pushes nodes with the character and frequencies to huff
	for(int i = 0; i < freq_size; i++) {
		head = new Node(frequency[i].first, frequency[i].second);
		huff.push_back(*head);
		delete head;
		head = nullptr;
	}

	int huff_size = huff.size();
	while(huff_size > 1) {
		int index1, index2;

		// index1 is assigned the index with the lower frequency of the first two elements, and index 2 is assigned the higher frequency
		if (huff[0].frequency > huff[1].frequency) {
			index1 = 1;
			index2 = 0;
		}
		else {
			index1 = 0;
			index2 = 1;
		}

		// Loops through huff, and assigns the smallest frequency to index1, and the second smallest to index2.
		for(int i = 2; i < huff_size; i++) {
			if (huff[i].frequency < huff[index1].frequency) {
				index2 = index1;
				index1 = i;
			}
			else if (huff[i].frequency < huff[index2].frequency) {
				index2 = i;
			}
		}
		
		// Creates a new node containing a NULL character and a frequency equal to the sum of the smallest frequencies.
		head = new Node(huff[index1].frequency + huff[index2].frequency);
		
		// Copies child information into a new node outside of the vector that is already linked to the parent
		head->left_child = new Node(huff[index1]);
		head->right_child = new Node(huff[index2]);

		// Erases vector elements that have already been copied and linked to a parent node.
		huff.push_back(*head);
		huff.erase(huff.begin() + index1);
		if(index1 < index2)
			huff.erase(huff.begin() + index2 - 1);
		else 
			huff.erase(huff.begin() + index2);

	// Updates the size of the vector.
		huff_size = huff.size();
	}
	


	if (!huff.empty()) {
		head = new Node(huff[0].frequency);
		head->left_child = huff[0].left_child;
		head->right_child = huff[0].right_child;
		huff.erase(huff.begin());
		return head;
	}
	std::cerr << "Something went wrong" << std::endl;
	exit(1);
	
}

std::vector<std::pair<char, std::string>> huffman_encoding(NodePtr root, std::string s) {
	std::vector<std::pair<char, std::string>> v, v2;
	// If root has no left or right child, then root->ch and s are printed and pushed to the back of the vector, and the vector returned.



	if(root->left_child == nullptr && root->right_child == nullptr) {
		v.push_back({root->ch, s});
		int temp = v.size();
		if (v[temp-1].first != '\n')
			std::cout << "'" << v[temp - 1].first << "'";
		else
			std::cout << "(new line)";
		std::cout << ": " << v[temp - 1].second << std::endl;
		return v;
	}
	// If root is nullptr, then an empty vector is returned.
	if(!root)
		return v;
	v2 = huffman_encoding(root->left_child, s + "0");
	v.insert( v.end(), v2.begin(), v2.end());
	v2 = huffman_encoding(root->right_child, s + "1");
	v.insert( v.end(), v2.begin(), v2.end());

	return v;
}

void huffman_output(std::vector<std::pair<char,std::string>> v, std::string file_name) {
	char ch;
	std::fstream fin(file_name, std::fstream::in);
	std::ofstream pride;
  	pride.open ("pride.huff");
	if(fin.is_open() && pride.is_open()){
		// As long as there is input from Pride_and_Prejudice.txt, the program will scan through, find the characters associated huffman code, and then output the encoded character to pride.huff.
		while (fin >> std::noskipws >> ch) {
			int i = v.size();
			while(i >= 0 && ch != v[i].first) 
				i--;
			if(v[i].first = ch)
				pride << v[i].second;
		}	
		pride.close();
	}
	else {
		std::cerr << "Something went wrong. Quitting." << std::endl;
		exit(1);
	}


}