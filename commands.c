#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100 // Size of the hash table bucket array
#define MAX_INPUT 256  // Max length for command inputs

// Node structure for hash table chaining (handles collisions)
typedef struct Entry {
    char *key;
    char *value;
    struct Entry *next;
} Entry;

// Key-Value Store structure
typedef struct {
    Entry *buckets[TABLE_SIZE];
} KeyValueStore;

// Hash function (djb2 algorithm) to convert a string key into an index
unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % TABLE_SIZE;
}

// Initialize the store
KeyValueStore* create_store() {
    KeyValueStore *store = malloc(sizeof(KeyValueStore));
    for (int i = 0; i < TABLE_SIZE; i++) {
        store->buckets[i] = NULL;
    }
    return store;
}

// SET: Insert or update a key-value pair
void store_set(KeyValueStore *store, const char *key, const char *value) {
    unsigned long index = hash(key);
    Entry *current = store->buckets[index];

    // Check if key already exists, if so, update its value
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = strdup(value);
            printf("OK (Updated)\n");
            return;
        }
        current = current->next;
    }

    // Otherwise, create a new entry and insert it at the head of the bucket list
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = strdup(value);
    new_entry->next = store->buckets[index];
    store->buckets[index] = new_entry;
    printf("OK (Inserted)\n");
}

// GET: Retrieve a value by its key
void store_get(KeyValueStore *store, const char *key) {
    unsigned long index = hash(key);
    Entry *current = store->buckets[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            printf(">> %s\n", current->value);
            return;
        }
        current = current->next;
    }
    printf("(nil) - Key not found\n");
}

// DEL: Delete a key-value pair
void store_del(KeyValueStore *store, const char *key) {
    unsigned long index = hash(key);
    Entry *current = store->buckets[index];
    Entry *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                store->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            printf("OK (Deleted)\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("(nil) - Key not found\n");
}

// NEW COMMAND 1: KEYS - List all stored keys
void store_keys(KeyValueStore *store) {
    int count = 0;
    printf("--- Stored Keys ---\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *current = store->buckets[i];
        while (current != NULL) {
            count++;
            printf("%d) %s\n", count, current->key);
            current = current->next;
        }
    }
    if (count == 0) {
        printf("(empty store)\n");
    }
}

// NEW COMMAND 2: FLUSH - Delete all keys and reset the store
void store_flush(KeyValueStore *store) {
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *current = store->buckets[i];
        while (current != NULL) {
            Entry *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
            count++;
        }
        store->buckets[i] = NULL;
    }
    printf("OK (Flushed %d keys)\n", count);
}

// Free all allocated memory on exit
void free_store(KeyValueStore *store) {
    store_flush(store);
    free(store);
}

// Main interactive loop
int main() {
    KeyValueStore *store = create_store();
    char input[MAX_INPUT];
    char cmd[16], key[64], value[128];

    printf("Commands:\n");
    printf("  SET <key> <value>\n");
    printf("  GET <key>\n");
    printf("  DEL <key>\n");
    printf("  KEYS\n");
    printf("  FLUSH\n");
    printf("  EXIT\n\n");

    while (1) {
        printf("kv> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // Remove trailing newline
        input[strcspn(input, "\r\n")] = 0;
        if (strlen(input) == 0) continue;

        // Parse command and arguments
        int args = sscanf(input, "%15s %63s %127[^\n]", cmd, key, value);

        if (strcmp(cmd, "EXIT") == 0) {
            break;
        } else if (strcmp(cmd, "SET") == 0 && args == 3) {
            store_set(store, key, value);
        } else if (strcmp(cmd, "GET") == 0 && args >= 2) {
            store_get(store, key);
        } else if (strcmp(cmd, "DEL") == 0 && args >= 2) {
            store_del(store, key);
        } else if (strcmp(cmd, "KEYS") == 0) {
            store_keys(store);
        } else if (strcmp(cmd, "FLUSH") == 0) {
            store_flush(store);
        } else {
            printf("Invalid command or syntax.\n");
        }
    }

    free_store(store);
    printf("Exiting... Memory freed successfully.\n");
    return 0;
}