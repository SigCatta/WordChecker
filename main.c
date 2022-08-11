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
            hash_table[i][j] = malloc(sizeof(node));
            hash_table[i][j]->key = NULL;
        }
    }
    return;
}

void *initialize(node *n) {
    n = malloc(sizeof(node));
    n->key = NULL;
    return n;
}

void add(node *n) {
    while (n->key != NULL) {
        if (memcmp(buffer, n->key, k) < 0) n = (node *) n->left;
        else n = (node *) n->right;
    }
    n->key = malloc(k);
    memcpy(n->key, buffer, k + 1);
    n->left = initialize((node *) n->left);
    n->right = initialize((node *) n->right);
    n->v = 'y';
    return;
}

int exists(node *n) {
    while (n->key != NULL) {
        int value = memcmp(p, n->key, k);
        if (value == 0) return 0;
        if (value < 0) n = (node *) n->left;
        else n = (node *) n->right;
    }
    return 1;
}

char hash_invert(int n) {
    if (n >= 38) return 'a' + (n - 38);
    if (n == 37) return '_';
    if (n >= 11) return 'A' + (n - 11);
    if (n >= 1) return '0' + (n - 1);
    return '-';
}

void inorder_tree_walk(node *n) {
    if (n->key != NULL) {
        inorder_tree_walk((node *) n->left);
        if (n->v == 'y') printf("%s\n", n->key);
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
    return;
}

char count_letters(char *c) {
    int count;
    char letter;
    for (int i = 0; i < DIC_SIZE; i++) {
        if (dictionary[i] == 0) continue;
        letter = hash_invert(i);
        count = 0;
        if (dictionary[i] == -1) {
            if (memchr(c, letter, k) != NULL) return 'n';
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
    char *c = n->key;
    if (c != NULL) {
        update_v((node *) n->left);
        update_v((node *) n->right);
        if (n->v == 'y') {
            for (int i = 0; i < k; i++) {
                if (discovered[i] != '#') {
                    if (c[i] != discovered[i]) {
                        n->v = 'n';
                        return;
                    }
                }
            }
            for (int i = 0; i < k; i++) {
                if (memchr(not_present[i], c[i], k) != NULL) {
                    n->v = 'n';
                    return;
                }
            }
            if (count_letters(c) == 'n') {
                n->v = 'n';
                return;
            }
            valid++;
        }
    }
    return;
}

void add_ins(node *n) {
    while (n->key != NULL) {
        if (memcmp(buffer, n->key, k) < 0) n = (node *) n->left;
        else n = (node *) n->right;
    }
    n->key = malloc(k);
    memcpy(n->key, buffer, k + 1);
    n->left = initialize((node *) n->left);
    n->right = initialize((node *) n->right);
    char *c = n->key;
/* update_v for new words */
    for (int i = 0; i < k; i++) {
        if (discovered[i] != '#') {
            if (c[i] != discovered[i]) return;
        }
    }
    for (int i = 0; i < k; i++) {
        if (memchr(not_present[i], c[i], k) != NULL) return;
    }
    if (count_letters(c) == 'n') return;
    n->v = 'y';
    valid++;
    return;
}

void valid_all(node *n) {
    if (n->key != NULL) {
        valid_all((node *) n->left);
        n->v = 'y';
        valid_all((node *) n->right);
    }
    return;
}

void command(char *string) {
    if (string[1] == 'n') {
        for (int i = 0; i < DIC_SIZE; i++) {
            for (int j = 0; j < DIC_SIZE; j++) {
                valid_all(hash_table[i][j]);
            }
        }
        valid = 0;
        memset(discovered, '#', k);
        memset(dictionary, 0, DIC_SIZE * sizeof(int));
        for (int i = 0; i < k; i++) memset(not_present[i], '#', k);
/* rules rest done */
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
                } else command(buffer);
            }
            add_ins(hash_table[hash(buffer[0])][hash(buffer[1])]);
        }
    }
    print_table();
    return;
}

void d_update(char c, char bool, int count) {
    if (bool == 'n') {
        dictionary[hash(c)] = -1;
        return;
    }
    if (count > dictionary[hash(c)]) dictionary[hash(c)] = count;
    return;
}

void pregame() {
    while (1) {
        w = scanf("%s", buffer);
        if (buffer[0] == '+') {
            command(buffer);
            return;
        }
        add(hash_table[hash(buffer[0])][hash(buffer[1])]);
    }
}

void game() {
    if (exists(hash_table[hash(p[0])][hash(p[1])]) == 1) {
        printf("not_exists\n");
        return;
    }
    int count;
    int LR, LV, LP;
    for (int i = 0; i < k; i++) {
        count = 0, LP = 0, LR = 0, LV = 0;
        for (int j = 0; j < k; j++) {
            if (p[j] == p[i]) {
                LP++;
                if (j <= i) LV++;
            }
            if (r[j] == p[i]) LR++;
        }
        if (r[i] == p[i]) {
            printf("+");
            discovered[i] = p[i];
            d_update(p[i], 'y', LV);
            continue;
        }
        if (memchr(r, p[i], k) == NULL) {
            d_update(p[i], 'n', -1);
            printf("/");
            continue;
        } else {
            if (LP > LR) {
                for (int j = 0; j < k; j++) {
                    if (p[j] == p[i] && p[j] == r[j]) count++;
                }
                if (count == LR) {
                    printf("/");
                    for (int j = 0; j < k; j++) {
                        if (not_present[i][j] == p[i]) break;
                        if (not_present[i][j] == '#') {
                            not_present[i][j] = p[i];
                            break;
                        }
                    }
                    d_update(p[i], 'y', 100 + LR);
                    continue;
                }
            }
            if (LV <= LR) {
                printf("|");
                d_update(p[i], 'y', LV);
                for (int j = 0; j < k; j++) {
                    if (not_present[i][j] == p[i]) break;
                    if (not_present[i][j] == '#') {
                        not_present[i][j] = p[i];
                        break;
                    }
                }
                continue;
            }
            d_update(p[i], 'y', 100 + LR);
            printf("/");
            for (int j = 0; j < k; j++) {
                if (not_present[i][j] == p[i]) break;
                if (not_present[i][j] == '#') {
                    not_present[i][j] = p[i];
                    break;
                }
            }
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
    return;
}

int main() {
    hash_init();
    w = scanf("%d", &k);
    r = malloc(k);
    p = malloc(k);
    not_present = malloc(sizeof(char *) * k);
    for (int i = 0; i < k; i++) {
        not_present[i] = malloc(k);
        memset(not_present[i], '#', k);
    }
    dictionary = calloc(DIC_SIZE, sizeof(int));
    discovered = malloc(k);
    memset(discovered, '#', k);
    buffer = malloc(k);
    w = scanf("%s", buffer);
    add(hash_table[hash(buffer[0])][hash(buffer[1])]);
    pregame();
    if (gameon == -2) {
        if (w == EOF) printf("ERRORE");
    }
    while (1) {
        if (attempts == 0) {
            printf("ko\n");
            pregame();
        }
        gameon = 0;
        w = scanf("%s", p);
        if (p[0] == '+') {
            command(p);
            gameon = 2;
        }
        if (memcmp(r, p, k) == 0) {
            gameon = 1;
            printf("ok\n");
        }
        if (gameon == 0) game();
        if (gameon == 1) pregame();
        if (w == EOF) return 0;
    }
}
