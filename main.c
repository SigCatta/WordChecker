#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIC_SIZE 64

int k, attempts, gameon = 1, valid = 0;
char *r, *p, *discovered, *buffer;
int *dictionary;
char **not_present;
int w;

typedef struct {
    struct node *left;
    struct node *right;
    char *key;
    char v;
} node;

node *hash_table[DIC_SIZE][DIC_SIZE];

void *fastmemchr(char *src, char c) {
    for (int i = 0; i <= k; i++) {
        if (src[i] == '#') return NULL;
        if (src[i] == c) return &src[i];
    }
    return NULL;
}

int fastmemcmp(char *x, char *y) {
    for (int i = 0; i <= k; i++) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

int hash(char c) {
    if (c >= 'a') return c - 97 + 38;
    if (c == '_') return 37;
    if (c >= 'A') return c - 65 + 11;
    if (c >= '0') return c - 48 + 1;
    return 0;
}

void hash_init() {
    for (int i = 0; i < DIC_SIZE; i++) {
        for (int j = 0; j < DIC_SIZE; j++) {
            hash_table[i][j] = NULL;
        }
    }
}

node *new_node() {
    node *n = malloc(sizeof(node));
    n->key = buffer;
    buffer = malloc(k + 1);
    n->left = n->right = NULL;
    n->v = 'y';
    return n;
}

void add(node **tree) {
    while ((*tree) != NULL) {
        if (fastmemcmp(buffer, (*tree)->key) < 0) tree = (node **) &(*tree)->left;
        else tree = (node **) &(*tree)->right;
    }
    (*tree) = new_node();
}

int exists(node *n) {
    while (n != NULL) {
        int value = fastmemcmp(p, n->key);
        if (value == 0) return 0;
        if (value < 0) n = (node *) n->left;
        else n = (node *) n->right;
    }
    return 1;
}

void inorder_tree_walk(node *n) {
    if (n != NULL) {
        inorder_tree_walk((node *) n->left);
        if (n->v == 'y') puts(n->key);
        inorder_tree_walk((node *) n->right);
    }
}

void print_table() {
    if (discovered[0] != '#') {
        if (discovered[1] != '#') {
            inorder_tree_walk(hash_table[hash(discovered[0])][hash(discovered[1])]);
            return;
        }
        int first = hash(discovered[0]);
        for (int i = 0; i < DIC_SIZE; i++) {
            if (dictionary[i] == -1) continue;
            inorder_tree_walk(hash_table[first][i]);
        }
        return;
    }
    if (discovered[1] != '#') {
        int second = hash(discovered[1]);
        for (int i = 0; i < DIC_SIZE; i++) {
            if (dictionary[i] == -1) continue;
            inorder_tree_walk(hash_table[i][second]);
        }
        return;
    }
    for (int i = 0; i < DIC_SIZE; i++) {
        if (dictionary[i] == -1) continue;
        for (int j = 0; j < DIC_SIZE; j++) {
            if (dictionary[j] == -1) continue;
            inorder_tree_walk(hash_table[i][j]);
        }
    }
}

char count_letters(char *c) {
    int count;
    char letter;
    for (int i = 0; i < DIC_SIZE; i++) {
        if (dictionary[i] == 0) continue;
        if (i >= 38) letter = 'a' + (i - 38);
        else if (i == 37) letter = '_';
        else if (i >= 11) letter = 'A' + (i - 11);
        else if (i >= 1) letter = '0' + (i - 1);
        else letter = '-';
        count = 0;
        if (dictionary[i] == -1) {
            if (fastmemchr(c, letter) != NULL) return 'n';
            continue;
        }
        for (int j = 0; j < k; j++) {
            if (c[j] == letter) count++;
        }
        if (dictionary[i] > 100) {
            if (count != dictionary[i] - 100) return 'n';
            continue;
        }
        if (count < dictionary[i]) return 'n';
    }
    return 'y';
}

void update_v(node *n) {
    if (n != NULL) {
        update_v((node *) n->left);
        update_v((node *) n->right);
        if (n->v == 'y') {
            for (int i = 0; i < k; i++) {
                if (discovered[i] != '#') {
                    if (n->key[i] != discovered[i]) {
                        n->v = 'n';
                        return;
                    }
                }
                if (fastmemchr(not_present[i], n->key[i]) != NULL) {
                    n->v = 'n';
                    return;
                }
            }
            if (count_letters(n->key) == 'n') {
                n->v = 'n';
                return;
            }
            valid++;
        }
    }
}

node *new_node_ins() {
    node *n = malloc(sizeof(node));
    n->key = buffer;
    buffer = malloc(k + 1);
    n->left = n->right = NULL;
    for (int i = 0; i < k; i++) {
        if (discovered[i] != '#') {
            if (n->key[i] != discovered[i])return n;
        }
        if (fastmemchr(not_present[i], n->key[i]) != NULL)return n;
        if (dictionary[hash(n->key[i])] == -1) return n;
    }
    if (count_letters(n->key) == 'n') return n;
    n->v = 'y';
    valid++;
    return n;
}

void add_ins(node **tree) {
    while ((*tree) != NULL) {
        if (fastmemcmp(buffer, (*tree)->key) > 0) tree = (node **) &(*tree)->right;
        else tree = (node **) &(*tree)->left;
    }
    (*tree) = new_node_ins();
}

void valid_all(node *n) {
    if (n != NULL) {
        valid_all((node *) n->left);
        n->v = 'y';
        valid_all((node *) n->right);
    }
}

void command(char *string) {
    if (string[1] == 'n') {
        for (int i = 0; i < DIC_SIZE; i++) {
            for (int j = 0; j < DIC_SIZE; j++) {
                valid_all(hash_table[i][j]);
            }
        }
        memset(discovered, '#', k);
        memset(dictionary, 0, DIC_SIZE * sizeof(int));
        for (int i = 0; i < k; i++) memset(not_present[i], '#', k);
        valid = 0;
        w = scanf("%s", r);
        w = scanf("%d", &attempts);
        gameon = 0;
        return;
    }
    if (string[1] == 'i') {
        while (1) {
            w = scanf("%s", buffer);
            if (buffer[0] == '+') {
                if (buffer[1] == 'i') {
                    gameon = 0;
                    return;
                } else {
                    command(buffer);
                    return;
                }
            }
            add_ins(&hash_table[hash(buffer[0])][hash(buffer[1])]);
        }
    }
    if (valid != 1)print_table();
    else puts(r);
}

void pregame() {
    while (1) {
        w = scanf("%s", buffer);
        if (w == EOF) return;
        if (buffer[0] == '+') {
            if (buffer[1] == 'n') {
                command(buffer);
                return;
            } else {
                command(buffer);
                continue;
            }
        }
        add(&hash_table[hash(buffer[0])][hash(buffer[1])]);
    }
}

void game() {
    if (exists(hash_table[hash(p[0])][hash(p[1])]) == 1) {
        puts("not_exists");
        return;
    }
    int count, countV;
    int LR, LV;
    for (int i = 0; i < k; i++) {
        count = 0, countV = 0, LR = 0, LV = 0;
        for (int j = 0; j < k; j++) {
            if (p[j] == p[i]) {
                if (p[j] == r[j]) {
                    count++;
                    if (j <= i) countV++;
                }
                if (j <= i) LV++;
            }
            if (r[j] == p[i]) LR++;
        }
        if (r[i] == p[i]) {
            printf("+");
            discovered[i] = p[i];
            if (LV > dictionary[hash(p[i])]) dictionary[hash(p[i])] = LV;
            continue;
        }
        if (LR == 0) {
            dictionary[hash(p[i])] = -1;
            printf("/");
            continue;
        } else {
            if (LV - countV - 1 >= LR - count) {
                printf("/");
                for (int j = 0; j < k; j++) {
                    if (not_present[i][j] == p[i]) break;
                    if (not_present[i][j] == '#') {
                        not_present[i][j] = p[i];
                        break;
                    }
                }
                dictionary[hash(p[i])] = 100 + LR;
                continue;
            }
            printf("|");
            if (LV > dictionary[hash(p[i])]) dictionary[hash(p[i])] = LV;
            for (int j = 0; j < k; j++) {
                if (not_present[i][j] == p[i]) break;
                if (not_present[i][j] == '#') {
                    not_present[i][j] = p[i];
                    break;
                }
            }
            continue;
        }
    }
    if (valid != 1) {
        valid = 0;
        if (discovered[0] != '#') {
            if (discovered[1] != '#') {
                update_v(hash_table[hash(discovered[0])][hash(discovered[1])]);
            } else {
                int first = hash(discovered[0]);
                for (int i = 0; i < DIC_SIZE; i++) {
                    if (dictionary[i] == -1) continue;
                    update_v(hash_table[first][i]);
                }
            }
        } else if (discovered[1] != '#') {
            int second = hash(discovered[1]);
            for (int i = 0; i < DIC_SIZE; i++) {
                if (dictionary[i] == -1) continue;
                update_v(hash_table[i][second]);
            }
        } else {
            for (int i = 0; i < DIC_SIZE; i++) {
                if (dictionary[i] == -1) continue;
                for (int j = 0; j < DIC_SIZE; j++) {
                    if (dictionary[j] == -1) continue;
                    update_v(hash_table[i][j]);
                }
            }
        }
    }
    printf("\n%d\n", valid);
    attempts--;
}

void pregame_init() {
    while (1) {
        w = scanf("%s", buffer);
        if (buffer[0] == '+') {
            if (buffer[1] == 'n') {
                w = scanf("%s", r);
                w = scanf("%d", &attempts);
                gameon = 0;
                return;
            } else continue;
        }
        add(&hash_table[hash(buffer[0])][hash(buffer[1])]);
    }
}

int main() {
    hash_init();
    w = scanf("%d", &k);
    r = malloc(k + 1);
    p = malloc(k + 1);
    buffer = malloc(k + 1);
    not_present = malloc(sizeof(char *) * k);
    for (int i = 0; i < k; i++) {
        not_present[i] = malloc(k);
        memset(not_present[i], '#', k);
    }
    dictionary = calloc(DIC_SIZE, sizeof(int));
    discovered = malloc(k);
    memset(discovered, '#', k);
    pregame_init();
    while (w != EOF) {
        if (attempts == 0) {
            puts("ko");
            pregame();
        }
        gameon = 0;
        w = scanf("%s", p);
        if (w == EOF) return 0;
        if (p[0] == '+') {
            command(p);
            gameon = 2;
        }
        if (fastmemcmp(r, p) == 0) {
            gameon = 1;
            puts("ok");
        }
        if (gameon == 0) game();
        if (gameon == 1) pregame();
    }
    return 0;
}
