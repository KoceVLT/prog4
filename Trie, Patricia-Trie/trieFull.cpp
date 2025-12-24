#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

void insert(TrieNode* root, string word) {
    TrieNode* curr = root;
    for (char c : word) {
        int index = c - 'a'; 
        
        if (index >= 0 && index < 26) {
            if (curr->children[index] == nullptr) {
                curr->children[index] = new TrieNode();
            }
            curr = curr->children[index];
        }
    }
    curr->isEndOfWord = true;
}


struct PatriciaNode {
    string text; 
    bool isEndOfWord;
    vector<PatriciaNode*> children; 

    PatriciaNode(string t, bool end) {
        text = t;
        isEndOfWord = end;
    }
};

void convertToPatricia(TrieNode* trieNode, PatriciaNode* patriciaNode) {
    for (int i = 0; i < 26; i++) {
        if (trieNode->children[i] != nullptr) {
            
            char firstChar = i + 'a';
            string path = "";
            path += firstChar;
            
            TrieNode* temp = trieNode->children[i];
            
            while (!temp->isEndOfWord) {
                int childCount = 0;
                int childIndex = -1;
                for(int j=0; j<26; j++) {
                    if(temp->children[j]) {
                        childCount++;
                        childIndex = j;
                    }
                }
                
                if (childCount == 1) {
                    path += (char)(childIndex + 'a');
                    temp = temp->children[childIndex];
                } else {
                    break; 
                }
            }
            
            PatriciaNode* newChild = new PatriciaNode(path, temp->isEndOfWord);
            patriciaNode->children.push_back(newChild);
            
            convertToPatricia(temp, newChild);
        }
    }
}

struct MkTrieNode {
    map<string, MkTrieNode*> children; 
    bool isEndOfWord;

    MkTrieNode() {
        isEndOfWord = false;
    }
};

void insertMacedonian(MkTrieNode* root, string word) {
    MkTrieNode* curr = root;
    
    for (int i = 0; i < word.length(); ) {
        string letter = "";
        unsigned char c = word[i];
        
        if ((c & 0xC0) == 0xC0) { 
            letter += word[i];
            letter += word[i+1];
            i += 2;
        } else {
            letter += word[i];
            i++;
        }
        
        if (curr->children.find(letter) == curr->children.end()) {
            curr->children[letter] = new MkTrieNode();
        }
        curr = curr->children[letter];
    }
    curr->isEndOfWord = true;
}