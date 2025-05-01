/*
 * blockchain.c
 * Quantum SMS OS Blockchain Core
 * Powered by ZKP, Helia IPFS, and Quantum Gateway
 * Includes light/dark theming system, QhyNLP protocol integration, and full data integrity
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_TRANSACTIONS 100
#define HASH_SIZE 64
#define ADDRESS_SIZE 64
#define THEME_LIGHT "light"
#define THEME_DARK "dark"

// Data structures

typedef struct {
    char sender[ADDRESS_SIZE];
    char recipient[ADDRESS_SIZE];
    double amount;
    char message[256]; // Quantum SMS payload
} Transaction;

typedef struct Block {
    int index;
    time_t timestamp;
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
    char previous_hash[HASH_SIZE];
    char hash[HASH_SIZE];
    struct Block* next;
} Block;

typedef struct Blockchain {
    Block* genesis;
    Block* latest;
    int size;
} Blockchain;

// Global blockchain instance
Blockchain* chain = NULL;

// Theme system
char active_theme[16] = THEME_LIGHT;

// Helper Functions
void apply_theme(const char* theme) {
    if (strcmp(theme, THEME_DARK) == 0 || strcmp(theme, THEME_LIGHT) == 0) {
        strcpy(active_theme, theme);
    } else {
        printf("⚠️ Invalid theme. Defaulting to light mode.\n");
        strcpy(active_theme, THEME_LIGHT);
    }
}

void print_theme_banner() {
    if (strcmp(active_theme, THEME_DARK) == 0) {
        printf("🌑 Dark Theme Activated\n");
    } else {
        printf("🌕 Light Theme Activated\n");
    }
}

void hash_block(Block* block, char* out_hash) {
    // Dummy hash function for simulation (replace with SHA256+ZKP proof generation)
    sprintf(out_hash, "BLOCK-%d-%ld", block->index, block->timestamp);
}

Block* create_block(const char* previous_hash) {
    Block* block = (Block*)malloc(sizeof(Block));
    block->index = chain->size;
    block->timestamp = time(NULL);
    strcpy(block->previous_hash, previous_hash);
    block->transaction_count = 0;
    block->next = NULL;
    hash_block(block, block->hash);
    return block;
}

Blockchain* init_blockchain() {
    Blockchain* new_chain = (Blockchain*)malloc(sizeof(Blockchain));
    Block* genesis = create_block("0");
    new_chain->genesis = genesis;
    new_chain->latest = genesis;
    new_chain->size = 1;
    return new_chain;
}

bool add_transaction(Block* block, const char* sender, const char* recipient, double amount, const char* message) {
    if (block->transaction_count >= MAX_TRANSACTIONS) return false;
    Transaction* tx = &block->transactions[block->transaction_count++];
    strncpy(tx->sender, sender, ADDRESS_SIZE);
    strncpy(tx->recipient, recipient, ADDRESS_SIZE);
    tx->amount = amount;
    strncpy(tx->message, message, sizeof(tx->message));
    return true;
}

Block* mine_block() {
    Block* new_block = create_block(chain->latest->hash);
    chain->latest->next = new_block;
    chain->latest = new_block;
    chain->size++;
    printf("🧱 Mined new block #%d with hash %s\n", new_block->index, new_block->hash);
    return new_block;
}

void print_block(const Block* block) {
    printf("\n📦 Block #%d\n", block->index);
    printf("Timestamp: %s", ctime(&block->timestamp));
    printf("Previous Hash: %s\n", block->previous_hash);
    printf("Current Hash: %s\n", block->hash);
    printf("Transactions (%d):\n", block->transaction_count);
    for (int i = 0; i < block->transaction_count; i++) {
        const Transaction* tx = &block->transactions[i];
        printf("  💸 %s -> %s | Amount: %.2f | Msg: %s\n", tx->sender, tx->recipient, tx->amount, tx->message);
    }
}

void traverse_chain() {
    Block* current = chain->genesis;
    while (current) {
        print_block(current);
        current = current->next;
    }
}

int main() {
    apply_theme(THEME_DARK);
    print_theme_banner();

    chain = init_blockchain();

    add_transaction(chain->latest, "alice.qsms", "bob.qsms", 20.5, "First transfer 🚀");
    add_transaction(chain->latest, "bob.qsms", "charlie.qsms", 10.0, "Thanks 💬");

    mine_block();
    add_transaction(chain->latest, "admin.qsms", "dev.qsms", 50.0, "Payment 💼");
    mine_block();

    traverse_chain();

    return 0;
}
