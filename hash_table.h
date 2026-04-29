#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_LEN 997

typedef struct ht_entry {
    char *key;
    char *value;
    unsigned long hash;
    struct ht_entry *next;
} ht_entry;

typedef struct hash_table {
    ht_entry *buckets[HASH_LEN];
} hash_table_t;

// Initializes a hash table (zeros out the buckets)
void ht_init(hash_table_t *ht);

// Adds or updates a key-value pair in the hash table
void ht_add(hash_table_t *ht, const char* key, const char* value);

// Retrieves the value associated with a key, or NULL if not found
char* ht_get(hash_table_t *ht, const char* key);

// Frees all memory associated with the hash table
void ht_free(hash_table_t *ht);

// Returns the bucket index for a given key
int ht_bucket(const char* key);

// Prints the value associated with a key, or NULL if not found
void ht_print_value(hash_table_t *ht, const char* key);

// Debug function to print all entries in the hash table
void ht_debug_print(hash_table_t *ht);

#endif // HASH_TABLE_H
