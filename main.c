#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
};

// Trie structure
struct Trie
{
    struct TrieNode *root;
};

// Function to create a new Trie node
struct TrieNode *createNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node)
    {
        node->isEndOfWord = 0;
        for (int i = 0; i < ALPHABET_SIZE; ++i)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            return 0;
        }
        current = current->children[index];
    }
    return current->isEndOfWord;
}

// Deallocate the trie structure
void deallocateTrieNodes(struct TrieNode *node)
{
    if (node)
    {
        for (int i = 0; i < ALPHABET_SIZE; ++i)
        {
            deallocateTrieNodes(node->children[i]);
        }
        free(node);
    }
}

struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        deallocateTrieNodes(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    if (trie)
    {
        trie->root = createNode();
    }
    return trie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening dictionary file.\n");
        exit(1);
    }

    int count = 0;
    char word[100];

    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[count] = strdup(word);
        count++;
    }

    fclose(file);
    return count;
}

int main(void)
{
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL)
    {
        printf("Trie deallocated successfully.\n");
    }
    else
    {
        printf("There is an error in this program\n");
    }

    return 0;
}
