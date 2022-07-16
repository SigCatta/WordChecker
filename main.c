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
 *  lower case ASCII -> 97-122           ---> 0-25
 *  caps lock ASCII -> 65- 90            ---> 26-51
 *  minus -> 45 . underscore -> 126      ---> 52-53
 *  numbers -> 48 - 57                   ---> 54 -63
 *  #symbols -> 64
 */

typedef struct {
    struct node *left;
    struct node *right;
    char *key;
    char v;
} node;

node *root;

int exists(node *n) {
    if (n->key != NULL) {
        int value = memcmp(p, n->key, k);
        if (value == 0) return 0;
        if (value < 0) return exists((node *) n->left);
        return exists((node *) n->right);
    }
    return 1;
}

char d_convert(int n) {
    if (n <= 25) return 'a' + n;
    if (n >= 54) return '0' + (n - 54);
    if (n == 52) return '-';
    if (n == 53) return '_';
    return 'A' + (n - 26);
}

void inorder_tree_walk(node *n) {
    if (n->key != NULL) {
        inorder_tree_walk((node *) n->left);
        if (n->v == 'y') printf("%s\n", n->key);
        inorder_tree_walk((node *) n->right);
    }
}

char count_letters(node *n) {
    int count;
    char letter;
    for (int i = 0; i < DIC_SIZE; i++) {
        if (dictionary[i] == 0) continue;
        letter = d_convert(i);
        count = 0;
        if (dictionary[i] == -1) {
            if (memchr(n->key, letter, k) != NULL) return 'n';
        }
        for (int j = 0; j < k; j++) {
            if (n->key[j] == letter) count++;
        }
        if (dictionary[i] > 100) {
            if (count != dictionary[i] - 100) return 'n';
            continue;
        }
        if (dictionary[i] > 0 && count < dictionary[i]) return 'n';
    }
    return 'y';
}

char check_presence(node *n) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (not_present[i][j] == '#') break;
            if (not_present[i][j] == n->key[i]) return 'n';
        }
    }
    return 'y';
}

void update_v(node *n) {
    if (n->key != NULL) {
        if (n->v == 'y') {
            for (int i = 0; i < k; i++) {
                if (discovered[i] != '#') {
                    if (n->key[i] != discovered[i]) {
                        n->v = 'n';
                        break;
                    }
                }
            }
            if (n->v == 'y') n->v = count_letters(n);
            if (n->v == 'y') n->v = check_presence(n);
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
    return;
}

void reset() {
    valid_all(root);
    memset(discovered, '#', k);
    memset(dictionary, 0, DIC_SIZE * sizeof(int));
    for (int i = 0; i < k; i++) memset(not_present[i], '#', k);
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
                    update_v(root);
                    return;
                } else command(buffer);
            }
            add(root);
        }
    }
    inorder_tree_walk(root);
    return;
}

void pregame() {
    while (1) {
        w = scanf("%s", buffer);
        if (buffer[0] == '+') {
            command(buffer);
            return;
        }
        add(root);
    }
}

void d_update(char c, char bool, int count) {
    if (bool == 'n') {
        if (c >= 97 && c <= 122) {
            dictionary[c - 97] = -1;
            return;
        }
        if (c >= 65 && c <= 90) {
            dictionary[c - 39] = -1;
            return;
        }
        if (c >= 48 && c <= 57) {
            dictionary[c + 6] = -1;
            return;
        }
        if (c == 126) {
            dictionary[53] = -1;
            return;
        }
        dictionary[52] = -1;
        return;
    }
    if (c >= 97 && c <= 122) {
        if (count > dictionary[c - 97]) dictionary[c - 97] = count;
        return;
    }
    if (c >= 65 && c <= 90) {
        if (count > dictionary[c - 39]) dictionary[c - 39] = count;
        return;
    }
    if (c >= 48 && c <= 57) {
        if (count > dictionary[c + 6]) dictionary[c + 6] = count;
        return;
    }
    if (c == 126) {
        if (count > dictionary[53])dictionary[53] = count;
        return;
    }
    if (count > dictionary[52]) dictionary[52] = count;
    return;
}

void valid_count(node *n) {
    if (n->key != NULL) {
        if (n->v == 'y') valid++;
        valid_count((node *) n->right);
        valid_count((node *) n->left);
    }
}

void game() {
    if (exists(root) == 1) {
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
        update_v(root);
        valid = 0;
        valid_count(root);
    }
    printf("\n%d\n", valid);
    attempts--;
    return;
}

int main() {
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
    root = malloc(sizeof(node));
    root->key = malloc(k);
    root->v = 'y';
    w = scanf("%s", root->key);
    root->left = initialize((node *) root->left);
    root->right = initialize((node *) root->right);
    buffer = malloc(k);
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
