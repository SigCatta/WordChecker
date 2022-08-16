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
    return ((c >= '0') * (c <= '9') * (c - 48 + 1)) + ((c >= 'A') * (c <= 'Z') * (c - 65 + 11)) + ((c == '_') * 37) +
           ((c >= 'a') * (c <= 'z') * (c - 97 + 38));
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
    n->key = malloc(k + 1);
    memcpy(n->key, buffer, k + 1);
    n->left = n->right = NULL;
    n->v = 'y';
    return n;
}

void add(node **tree) {
    while ((*tree) != NULL) {
        if (memcmp(buffer, (*tree)->key, k) < 0) tree = (node **) &(*tree)->left;
        else tree = (node **) &(*tree)->right;
    }
    (*tree) = new_node();
}

int exists(node *n) {
    while (n != NULL) {
        int value = memcmp(p, n->key, k);
        if (value == 0) return 0;
        if (value < 0) n = (node *) n->left;
        else n = (node *) n->right;
    }
    return 1;
}

void inorder_tree_walk(node *n) {
    if (n != NULL) {
        inorder_tree_walk((node *) n->left);
        if (n->v == 'y') puts(n->key);;
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
        letter = (('a' + (i - 38)) * (i >= 38)) + ((i == 37) * '_') + ((i >= 11) * (i <= 36) * ('A' + (i - 11))) +
                 ((i >= 1) * (i <= 10) * ('0' + (i - 1))) + ((i == 0) * '-');
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

node *new_node_ins() {
    node *n = malloc(sizeof(node));
    n->key = malloc(k + 1);
    memcpy(n->key, buffer, k + 1);
    n->left = n->right = NULL;
    char *c = n->key;
    for (int i = 0; i < k; i++) {
        if (discovered[i] != '#') {
            if (c[i] != discovered[i])return n;
        }
        if (memchr(not_present[i], c[i], k) != NULL)return n;
    }
    if (count_letters(c) == 'n') return n;
    n->v = 'y';
    valid++;
    return n;
}

void add_ins(node **tree) {
    while ((*tree) != NULL) {
        if (memcmp(buffer, (*tree)->key, k) > 0) tree = (node **) &(*tree)->right;
        else tree = (node **) &(*tree)->left;
    }
    (*tree) = new_node_ins();
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
                if (memchr(not_present[i], n->key[i], k) != NULL) {
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
    print_table();
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
    int count;
    int LR, LV, LP;
    for (int i = 0; i < k; i++) {
        if (r[i] == p[i]) {
            printf("+");
            discovered[i] = p[i];
            if (LV > dictionary[hash(p[i])]) dictionary[hash(p[i])] = LV;
            continue;
        }
        if (memchr(r, p[i], k) == NULL) {
            dictionary[hash(p[i])] = -1;
            printf("/");
            continue;
        } else {
            count = 0, LP = 0, LR = 0, LV = 0;
            for (int j = 0; j < k; j++) {
                if (p[j] == p[i]) {
                    LP++;
                    if (j <= i) LV++;
                }
                if (r[j] == p[i]) LR++;
            }
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
                    if ((100 + LR) > dictionary[hash(p[i])]) dictionary[hash(p[i])] = 100 + LR;

                    continue;
                }
            }
            if (LV <= LR) {
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
            if ((100 + LR) > dictionary[hash(p[i])]) dictionary[hash(p[i])] = 100 + LR;
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
            } else {
                command(buffer);
                continue;
            }
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
        if (memcmp(r, p, k) == 0) {
            gameon = 1;
            puts("ok");
        }
        if (gameon == 0) game();
        if (gameon == 1) pregame();
    }
    return 0;
}
