#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "ll.h"
#include "huffman.h"

int main() {
    TreeNode root = generateFreqTable(); 
    TreeNode temp = root;
    while(temp->parent != NULL) {
            printf("%c : %d\n", temp->symbol, temp->freq);
            temp = temp->parent;
    }
    printf("\n");

    TreeNode master = createHuffmanTree(root);
    displayTree(master);
}

// Checks if a TreeNode has been created for the given symbol
// If it has, increment the TreeNodes `freq` field
int found(TreeNode t, char symbol) {
    TreeNode curr = t;
    while(curr->symbol != 0) {
        if(curr->symbol == symbol) {
            curr->freq += 1;
            return 1;
        }

        curr = curr->parent;
    }

    return 0;
}

// Helper function for sort that
// Returns true if the linked list is in descending order
int ordered(TreeNode t) {
    TreeNode curr = t;
    while(curr->parent != NULL) {
        if(curr->freq > curr->parent->freq) {
            return 0;
        }

        curr = curr->parent;
    }

    return 1;
}

void sort(TreeNode t) {
    if(t->parent == NULL) { return; }
    while(!ordered(t)) {
        TreeNode prev = t;
        TreeNode curr = t->parent;
        while(curr != NULL) {
            if(curr->freq < prev->freq) {
                // Copy TreeNode content
                int tempFreq = prev->freq;
                int tempChar = prev->symbol;
                TreeNode tempLeft = prev->left;
                TreeNode tempRight = prev->right;

                // Assign values from preceding TreeNode
                prev->freq = prev->parent->freq;
                prev->symbol = prev->parent->symbol;
                prev->left = prev->parent->left;
                prev->right = prev->parent->right;

                // Move values from current to preceding
                prev->parent->freq = tempFreq;
                prev->parent->symbol = tempChar;
                prev->parent->left = tempLeft;
                prev->parent->right = tempRight;
            }

            prev = curr;
            curr = prev->parent;
        }
    }
}

// Does not return a proper tree
// Instead, yields a chain of TreeNodes with character occurrences
// This string of nodes is not bidirectional
// Only the `parent` field is set
TreeNode generateFreqTable() {
    TreeNode parent = newTreeNode(0);

    char tempChar;
    while(scanf("%c", &tempChar) != EOF) {
        if(!found(parent, tempChar) && tempChar != 10) {
            TreeNode curr = newTreeNode(tempChar);
            curr->parent = parent;
            parent = curr;
        }
    }    

    // Ensure that list is in descending frequency
    sort(parent);

    // Free the frequency table's sentinel
    TreeNode temp = parent->parent;
    free(parent);

    return temp;
}

TreeNode createHuffmanTree(TreeNode t) {
    TreeNode curr = t;
    while(curr->parent != NULL) {
        TreeNode first = curr;
        TreeNode second = curr->parent;
        
        TreeNode join = newTreeNode(0);
        join->freq = first->freq + second->freq;
        join->parent = second->parent;
        join->left = first;
        join->right = second;
        first->parent = join;
        second->parent = join;   

        curr = join;
        sort(curr);
    }

    return curr;
}
