#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DIC_SIZE 64

int k, attempts, gameon = 1;
char *r, *p, *discovered, *buffer;
int dictionary[DIC_SIZE] = {};
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
    char *key;
    struct node *left;
    struct node *right;
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
    if (n >= 54) return '0' + (n - 48);
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

void count_letters(node *n) {
    int count = 0;
    for (int i = 0; i < DIC_SIZE; i++) {
        if (dictionary[i] > 0) {
            for (int j = 0; j < k; j++) {
                if (n->key[j] == d_convert(i)) count++;
            }
            if (count != dictionary[i]) {
                n->v = 'n';
                return;
            }
            count = 0;
        }
    }
}

void update_v(node *n) {
    if (n->key != NULL && n->v == 'y') {
        for (int i = 0; i < k; i++) {
            if (discovered[i] != '#') {
                if (n->key[i] != discovered[i]) {
                    n->v = 'n';
                    break;
                }
            }
        }
        if (n->v == 'y') count_letters(n);
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
    memset(dictionary, 0, sizeof(dictionary));
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
    if (memcmp(buffer, n->key, k) > 0) return add((node *) n->right);
    return 0;
}

void command(char *string) {
    if (string[1] == 'n') {
        reset();
        fflush(stdin);
        fgets(r, k + 1, stdin);
        w = scanf("%d", &attempts);
        gameon = 0;
        return;
    }
    if (string[1] == 'i') {
        while (1) {
            fgets(buffer, k + 1, stdin);
            if (buffer[0] == '+') {
                if (buffer[1] == 'i') {
                    gameon = 0;
                    return;
                } else command(buffer);
            }
            add(root);
        }
    }
    if (string[1] == 's') {
        update_v(root);
        inorder_tree_walk(root);
        return;
    }
    return;
}

void pregame() {
    while (1) {
        fflush(stdin);
        fgets(buffer, k + 1, stdin);
        if (buffer[0] == '+') {
            command(buffer);
            return;
        }
        add(root);
    }
}

void d_update(char c, char bool, int count) {
    if (bool == 'n') {
        if (islower(c) != 0) {
            dictionary[c - 97]--;
            return;
        }
        if (isupper(c) != 0) {
            dictionary[c - 39]--;
            return;
        }
        if (isalnum(c) != 0) {
            dictionary[c + 6]--;
            return;
        }
        if (c == 126) {
            dictionary[53]--;
            return;
        }
        dictionary[52]--;
        return;
    }
    if (islower(c) != 0) {
        dictionary[c - 97] = count;
        return;
    }
    if (isupper(c) != 0) {
        dictionary[c - 39] = count;
        return;

    }
    if (isalnum(c) != 0) {
        dictionary[c + 6] = count;
        return;
    }
    if (c == 126) {
        dictionary[53] = count;
        return;
    }
    dictionary[52] = count;
    return;
}

void game() {
    if (exists(root) == 1) {
        printf("not_exists\n");
        return;
    }
    for (int i = 0; i < k; i++) {
        if (r[i] == p[i]) {
            printf("+");
            discovered[i] = p[i];
        } else if (memchr(r, p[i], k) == NULL) {
            d_update(p[i], 'n', 0);
            printf("/");
            strcat(not_present[i], &p[i]);
        } else {
            int recA = 0, recB = 0, recP = 0;
            for (int j = 0; j < k; j++) {
                if (r[i] == r[j]) recA++;
                if (p[i] == p[j]) {
                    recB++;
                    if (j < i) recP++;
                }
            }
            if (recA >= recB || recB > i || recB - recP > 0) {
                printf("|");
                strcat(not_present[i], &p[i]);
            } else {
                d_update(p[i], 'y', recA);
                printf("/");
                strcat(not_present[i], &p[i]);
            }
        }
    }
    printf(", %d\n", --attempts);
    return;
}

int main() {
    if (w == -1) w = 0;
    w = scanf("%d", &k);
    r = malloc(k);
    p = malloc(k);
    not_present = calloc(k, sizeof(char *));
    for (int i = 0; i < k; i++) not_present[i] = malloc(DIC_SIZE - 1);
    discovered = malloc(k);
    memset(discovered, '#', k);
    root = malloc(sizeof(node));
    root->v = 'y';
    fflush(stdin);
    root->key = malloc(k);
    fgets(root->key, k + 1, stdin);
    root->left = initialize((node *) root->left);
    root->right = initialize((node *) root->right);
    buffer = malloc(k);
    pregame();
    while (1) {
        if (gameon == -2) break;
        if (attempts == 0) {
            printf("ko");
            pregame();
        }
        gameon = 0;
        fflush(stdin);
        fgets(p, k + 1, stdin);
        if (p[0] == '+') {
            command(p);
            gameon = 2;
        }
        if (memcmp(r, p, k) == 0) {
            gameon = 1;
            printf("ok");
        }
        if (gameon == 0) {
            game();
        }
        if (gameon == 1) pregame();
    }
}
