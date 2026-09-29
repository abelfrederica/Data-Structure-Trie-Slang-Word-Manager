#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <ctype.h> 

void clear(){
    system("cls");
}

void toLower(char* str){
    for (int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

typedef struct Trie{
    struct Trie* children[26]; 
    bool EndWord;
    char description[100];
}Trie;

Trie* createNode(){
    Trie* node = (Trie*)malloc(sizeof(Trie));
    if(node){
        node->EndWord = false;
        memset(node->children, 0, sizeof(node->children));
        memset(node->description, 0, sizeof(node->description));
    }
    return node;
}

void insert(Trie* root, const char* word, const char* desc){
    Trie* curr = root;
    for (int i = 0; word[i] != '\0'; i++){
        int index = word[i] - 'a';
        if (!curr->children[index]){
            curr->children[index] = createNode();
        }
        curr = curr->children[index];
    }
    curr->EndWord = true;
    strcpy(curr->description, desc);
}

bool search(Trie* root, const char* word, char* desc){
    Trie* curr = root;
    for (int i = 0; word[i] != '\0'; i++){
        int index = word[i] - 'a';
        if (!curr->children[index]){
            return false;
        }
        curr = curr->children[index];
    }
    if (curr && curr->EndWord){
        strcpy(desc, curr->description);
        return true;
    }
    return false;
}

void collectWords(Trie* node, char* current, char** words, int* count){
    if(node->EndWord){
        words[*count] = strdup(current); 
        (*count)++;
        if (*count >= 100) return; 
    }

    for(int i = 0; i < 26; i++){
        if (node->children[i]) {
            int len = strlen(current);
            current[len] = 'a' + i;
            current[len + 1] = '\0';
            collectWords(node->children[i], current, words, count);
            current[len] = '\0';
        }
    }
}

bool startsWith(Trie* root, const char* prefix, char** results, int* count){
    Trie* curr = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (!curr->children[index]) {
            return false;
        }
        curr = curr->children[index];
    }

    char current[100];
    strcpy(current, prefix);
    collectWords(curr, current, results, count);
    return *count > 0;
}


void collectAllWords(Trie* node, char* current, char** words, int* count){
    if (node->EndWord){
        words[*count] = strdup(current); 
        (*count)++;
        if (*count >= 100) return; 
    }

    for (int i = 0; i < 26; i++){
        if (node->children[i]) {
            int len = strlen(current);
            current[len] = 'a' + i;
            current[len + 1] = '\0';
            collectAllWords(node->children[i], current, words, count);
            current[len] = '\0';
        }
    }
}

char** getAllWords(Trie* root, int* count){
    char** words = (char**)malloc(100 * sizeof(char*));
    *count = 0;
    char current[100] = "";
    collectAllWords(root, current, words, count);
    return words;
}

bool isValidWord(const char* word){
    return strlen(word) > 1 && strchr(word, ' ') == NULL;
}

bool isValidDesc(const char* desc){
    int spaces = 0;
    for (int i = 0; desc[i] != '\0'; i++){
        if (desc[i] == ' ') spaces++;
    }
    return spaces >= 1;
}

void Enter(){
    printf("\nPress enter to continue...");
    getchar(); 
    clear(); 
}

int main(){
    Trie* root = createNode();
    int choice;

    while(1){
    	clear(); 
        printf("Boogle - Slang Word Manager\n");
        printf("1. Release a new slang word\n");
        printf("2. Search a slang word\n");
        printf("3. View all slang words starting with a prefix\n");
        printf("4. View all slang words\n");
        printf("5. Exit\n");
        printf("Choose menu [1-5]: ");
        scanf("%d", &choice);
        
        getchar(); 

        if (choice == 1){
            char word[100];
            char desc[200];

            do{
                printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
                fgets(word, sizeof(word), stdin);
                word[strcspn(word, "\n")] = 0;
                toLower(word); 
            } while (!isValidWord(word));

            do{
                printf("Input a new slang word description [Must be more than 2 words]: ");
                fgets(desc, sizeof(desc), stdin);
                desc[strcspn(desc, "\n")] = 0; 
                toLower(word); 
            } while (!isValidDesc(desc));

            char dummy[200];
            if (search(root, word, dummy)){
                insert(root, word, desc);
                printf("Successfully updated a slang word.\n");
            } 
			else{
                insert(root, word, desc);
                printf("Successfully released new slang word.\n");
            }
            Enter();
        } 
        
		else if (choice == 2){
            char word[100];
            char desc[200];

            do{
                printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
                fgets(word, sizeof(word), stdin);
                word[strcspn(word, "\n")] = 0; 
                toLower(word); 
            } while (!isValidWord(word));

            if (search(root, word, desc)){
                printf("Slang word	: %s\nDescription	: %s\n", word, desc);
            } 
			else{
                printf("There is no word \"%s\" in the dictionary.\n", word);
            }
            Enter();
        } 
		
		else if (choice == 3){
            char prefix[100];
            printf("Input a prefix to be searched: ");
            fgets(prefix, sizeof(prefix), stdin);
            prefix[strcspn(prefix, "\n")] = 0; 
            toLower(prefix);

            char* found[100];
            int count = 0;
            if (startsWith(root, prefix, found, &count)){
                printf("Words starts with \"%s\":\n", prefix);
                for (int i = 0; i < count; i++){
                    printf("%d. %s\n", i + 1, found[i]);
                    free(found[i]);
                }
            } 
			else{
                printf("There is no prefix \"%s\" in the dictionary.\n", prefix);
            }
            Enter();
        } 
		
		else if(choice == 4){
            int count = 0;
            char** all = getAllWords(root, &count);
            if (count == 0){
                printf("There is no slang word yet in the dictionary.\n");
            } 
			else{
                printf("List of all slang words in the dictionary:\n");
                for (int i = 0; i < count; i++){
                    printf("%d. %s\n", i + 1, all[i]);
                    free(all[i]); 
                }
                free(all);
            }
            Enter();
        } 
		
		else if(choice == 5){
            printf("Thank you... Have a nice day :)\n");
            break;
        } 
		
		else{
            printf("Invalid input. Choose from 1 to 5.\n");
        }
    }

    return 0;
}

