#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIC_SIZE 64

int k, attempts, gameon = 1, valid = 0;
char *r, *p, *discovered, *buffer;
int *dictionary;
char **not_present;
int w;

/*          DICTIONARY
 *  minus -> 45                          ---> 0
 *  numbers -> 48 - 57                   ---> 1-10
 *  caps lock ASCII -> 65- 90            ---> 11-36
 *  underscore -> 95                     ---> 37
 *  lower case ASCII -> 97-122           ---> 38-63
 *  #symbols -> 64
 */

typedef struct {
    struct node *left;
    struct node *right;
    char *key;
    char v;
} node;

node *hash_table[DIC_SIZE];

unsigned int hash(char *key) {
    char c = key[0];
    if (c >= 'a') return c - 97 + 38;
    if (c == '_') return 37;
    if (c >= 'A') return c - 65 + 11;
    if (c >= '0') return c - 48 + 1;
    return 0;
}

void hash_init() {
    for (int i = 0; i < DIC_SIZE; i++) {
        hash_table[i] = malloc(sizeof(node));
        hash_table[i]->key = NULL;
    }
    return;
}

void *initialize(node *n) {
    n = malloc(sizeof(node));
    n->key = NULL;
    n->v = 'n';
    return n;
}

void *add(node *n) {
    if (n->key == NULL) {
        n->key = malloc(k);
        memcpy(n->key, buffer, k + 1);
        n->left = initialize((node *) n->left);
        n->right = initialize((node *) n->right);
        n->v = 'y';
        return 0;
    }
    if (memcmp(buffer, n->key, k) < 0) return add((node *) n->left);
    return add((node *) n->right);
}

void hash_insert() {
    int value = hash(buffer);
    if (hash_table[value]->key == NULL) {
        hash_table[value]->key = malloc(k);
        memcpy(hash_table[value]->key, buffer, k + 1);
        hash_table[value]->left = initialize((node *) hash_table[value]->left);
        hash_table[value]->right = initialize((node *) hash_table[value]->right);
        hash_table[value]->v = 'y';
        return;
    }
    add(hash_table[value]);
    return;
}

int exists(node *n) {
    if (n->key != NULL) {
        int value = memcmp(p, n->key, k);
        if (value == 0) return 0;
        if (value < 0) return exists((node *) n->left);
        return exists((node *) n->right);
    }
    return 1;
}

int hash_search() {
    int value = hash(p);
    if (hash_table[value]->key == NULL) return 1;
    return exists(hash_table[value]);
}

char d_convert(int n) {
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
        inorder_tree_walk(hash_table[hash(discovered)]);
        return;
    }
    for (int i = 0; i < DIC_SIZE; i++) {
        if (hash_table[i]->key == NULL) continue;
        inorder_tree_walk(hash_table[i]);
    }
}

char count_letters(char *c) {
    int count;
    char letter;
    for (int i = 0; i < DIC_SIZE; i++) {
        if (dictionary[i] == 0) continue;
        letter = d_convert(i);
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
        if (dictionary[i] > 0 && count < dictionary[i]) return 'n';
    }
    return 'y';
}

char check_presence(char *c) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (not_present[i][j] == '#') break;
            if (not_present[i][j] == c[i]) return 'n';
        }
    }
    return 'y';
}

void update_v(node *n) {
    char *c = n->key;
    if (c != NULL) {
        if (n->v == 'y') {
            for (int i = 0; i < k; i++) {
                if (discovered[i] != '#') {
                    if (c[i] != discovered[i]) {
                        n->v = 'n';
                        break;
                    }
                }
            }
            if (n->v == 'y') n->v = count_letters(c);
            if (n->v == 'y') n->v = check_presence(c);
        }
        update_v((node *) n->left);
        update_v((node *) n->right);
    }
    return;
}

void valid_all(node *n) {
    if (n->key != NULL) {
        valid_all((node *) n->left);
        n->v = 'y';
        valid_all((node *) n->right);
    }
}

void reset() {
    for (int i = 0; i < DIC_SIZE; i++) {
        valid_all(hash_table[i]);
    }
    memset(discovered, '#', k);
    memset(dictionary, 0, DIC_SIZE * sizeof(int));
    for (int i = 0; i < k; i++) memset(not_present[i], '#', k);
}

void command(char *string) {
    valid = 0;
    if (string[1] == 'n') {
        if (w != 0) w++;
        reset();
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
                    if (attempts == 0) {
                        w = scanf("%s", buffer);
                        if (buffer[0] == '+') command(buffer);
                        return;
                    }
                    gameon = 0;
                    for (int i = 0; i < DIC_SIZE; i++) {
                        if (hash_table[i] != NULL) update_v(hash_table[i]);
                    }
                    return;
                } else command(buffer);
            }
            hash_insert();
        }
    }
    print_table();
    return;
}
void d_update(char c, char bool, int count) {
    if (bool == 'n') {
        if (c >= 97) {
            dictionary[c - 97 + 38] = -1;
            return;
        }
        if (c == 95) {
            dictionary[37] = -1;
            return;
        }
        if (c >= 65) {
            dictionary[c - 65 + 11] = -1;
            return;
        }
        if (c >= 48) {
            dictionary[c - 48 + 1] = -1;
            return;
        }
        dictionary[0] = -1;
        return;
    }
    if (c >= 97) {
        if (count > dictionary[c - 97 + 38]) dictionary[c - 97 + 38] = count;
        return;
    }
    if (c == 95) {
        if (count > dictionary[37]) dictionary[37] = count;
        return;
    }
    if (c >= 65) {
        if (count > dictionary[c - 65 + 11]) dictionary[c - 65 + 11] = count;
        return;
    }
    if (c >= 48) {
        if (count > dictionary[c - 48 + 1]) dictionary[c - 48 + 1] = count;
        return;
    }
    if (count > dictionary[0]) dictionary[0] = count;
    return;
}

void pregame() {
    while (1) {
        w = scanf("%s", buffer);
        if (buffer[0] == '+') {
            command(buffer);
            return;
        }
        hash_insert();
    }
}

void valid_count(node *n) {
    if (n->key != NULL) {
        if (n->v == 'y') valid++;
        valid_count((node *) n->right);
        valid_count((node *) n->left);
    }
}

void game() {
    if (hash_search() == 1) {
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
        if (discovered[0] != '#') {
            update_v(hash_table[hash(discovered)]);
            valid = 0;
            valid_count(hash_table[hash(discovered)]);
        } else {
            for (int i = 0; i < DIC_SIZE; i++) {
                if (hash_table[i] != NULL) update_v(hash_table[i]);
            }
            valid = 0;
            for (int i = 0; i < DIC_SIZE; i++) {
                if (hash_table[i] != NULL) valid_count(hash_table[i]);
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
    hash_insert();
    pregame();
    if (gameon == -2 && w == EOF) printf("ERRORE");
    while (1) {
        if (attempts == 0) {
            printf("ko\n");
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
            printf("ok\n");
        }
        if (gameon == 0) {
            game();
        }
        if (gameon == 1) pregame();
    }
}
