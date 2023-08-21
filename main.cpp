#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    string meaning;
    bool isEnd;

    TrieNode() {
        for (int i = 0; i < 26; ++i)
            children[i] = nullptr;
        meaning = "";
        isEnd = false;
    }
};

class Dictionary {
private:
    TrieNode* root;

public:
    Dictionary() {
        root = new TrieNode();
    }

    void insert(const string& word, const string& meaning) {
        TrieNode* current = root;
        for (char c : word) {
            int index = c - 'a';
            if (!current->children[index])
                current->children[index] = new TrieNode();
            current = current->children[index];
        }
        current->meaning = meaning;
        current->isEnd = true;
    }

    string search(const string& word) {
        TrieNode* current = root;
        for (char c : word) {
            int index = c - 'a';
            if (!current->children[index])
                return "Word not found";
            current = current->children[index];
        }
        if (current->isEnd)
            return current->meaning;
        return "Word not found";
    }

    void deleteWord(const string& word) {
        deleteWordHelper(root, word, 0);
    }

    void deleteWordHelper(TrieNode* node, const string& word, int index) {
        if (index == word.length()) {
            if (node->isEnd) {
                node->isEnd = false;
                node->meaning = "";
            }
            return;
        }

        int charIndex = word[index] - 'a';
        if (!node->children[charIndex])
            return;

        deleteWordHelper(node->children[charIndex], word, index + 1);

        if (!node->children[charIndex]->isEnd) {
            for (int i = 0; i < 26; ++i) {
                if (node->children[charIndex]->children[i]) {
                    return;
                }
            }
            delete node->children[charIndex];
            node->children[charIndex] = nullptr;
        }
    }

    void viewAllWords(TrieNode* node, string currentWord, vector<string>& words) {
        if (node->isEnd) {
            words.push_back(currentWord + " - " + node->meaning);
        }

        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                viewAllWords(node->children[i], currentWord + char('a' + i), words);
            }
        }
    }

    void view() {
        vector<string> words;
        viewAllWords(root, "", words);

        if (words.empty()) {
            cout << "Dictionary is empty." << endl;
            return;
        }

        cout << "Words in the dictionary:" << endl;
        for (const string& word : words) {
            cout << word << endl;
        }
    }

    ~Dictionary() {
        deleteTrie(root);
    }

    void deleteTrie(TrieNode* node) {
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                deleteTrie(node->children[i]);
            }
        }
        delete node;
    }
};

int main() {
    Dictionary dict;

    ifstream inputFile("dictionary.txt");
    if (!inputFile) {
        cout << "Error opening dictionary.txt" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        size_t spacePos = line.find(' ');
        if (spacePos != string::npos) {
            string word = line.substr(0, spacePos);
            string meaning = line.substr(spacePos + 1);
            dict.insert(word, meaning);
        }
    }

    inputFile.close();

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Add a word\n";
        cout << "2. Search a word\n";
        cout << "3. Delete a word\n";
        cout << "4. View all words\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string word, meaning;
            cout << "Enter the word: ";
            cin >> word;
            cin.ignore();
            cout << "Enter the meaning: ";
            getline(cin, meaning);
            dict.insert(word, meaning);

            // Save the new word and meaning to the dictionary.txt file
            ofstream outputFile("dictionary.txt", ios::app);
            if (outputFile) {
                outputFile << word << " " << meaning << "\n";
                outputFile.close();
            } else {
                cout << "Error opening dictionary.txt for writing." << endl;
            }

            cout << "Word added successfully.\n";
            break;
        }
        case 2: {
            string word;
            cout << "Enter the word to search: ";
            cin >> word;
            cout << dict.search(word) << endl;
            break;
        }
        case 3: {
            string word;
            cout << "Enter the word to delete: ";
            cin >> word;
            dict.deleteWord(word);
            cout << "Word deleted if it existed.\n";
            break;
        }
        case 4:
            dict.view();
            break;
        case 5:
            cout << "Exiting the program...\n";
            break;
        default:
            cout << "Invalid choice. Please enter a valid option.\n";
            break;
        }
    } while (choice != 5);

    return 0;
}
