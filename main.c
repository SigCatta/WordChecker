#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DIC_SIZE 64

int k, attempts, gameon = 1, valid = 0;
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
    int count = 0;
    for (int i = 0; i < DIC_SIZE; i++) {
        if (dictionary[i] == -1) {
            for (int j = 0; j < k; j++) {
                if (memchr(n->key, d_convert(i), k) != NULL) return 'n';
            }
        }
        if (dictionary[i] > 0) {
            for (int j = 0; j < k; j++) {
                if (n->key[j] == d_convert(i)) count++;
            }
            if (count != dictionary[i]) return 'n';
        }
        count = 0;
    }
    return 'y';
}

char check_presence(node *n) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < DIC_SIZE; j++) {
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
        }
        if (n->v == 'y') n->v = count_letters(n);
        if (n->v == 'y') n->v = check_presence(n);
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
    for (int i = 0; i < k; i++) memset(not_present[i], '#', DIC_SIZE);
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
                    return;
                } else command(buffer);
            }
            add(root);
        }
    }
    if (string[1] == 's') {
        inorder_tree_walk(root);
        return;
    }
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
        if (islower(c) != 0) {
            dictionary[c - 97] = -1;
            return;
        }
        if (isupper(c) != 0) {
            dictionary[c - 39] = -1;
            return;
        }
        if (isalnum(c) != 0) {
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

void valid_count(node *n) {
    if (n->key != NULL) {
        if (n->v == 'y') valid = valid + 1;
        valid_count((node *) n->right);
        valid_count((node *) n->left);
    }
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
        } else {
            int recA = 0, recB = 0, recP = 0;
            for (int j = 0; j < k; j++) {
                if (r[i] == r[j]) recA++;
                if (p[i] == p[j]) {
                    recB++;
                    if (j < i) recP++;
                }
            }
            if (recA >= recB || recB > i || recA - recP > 0) {
                printf("|");
                if (memchr(not_present[i], p[i], DIC_SIZE) == NULL) {
                    for (int j = 0; j < DIC_SIZE; j++) {
                        if (not_present[i][j] == '#') {
                            not_present[i][j] = p[i];
                            break;
                        }
                    }
                }
                d_update(p[i], 'y', recA);
            } else {
                d_update(p[i], 'y', recA);
                printf("/");
                if (memchr(not_present[i], p[i], DIC_SIZE) == NULL) {
                    for (int j = 0; j < DIC_SIZE; j++) {
                        if (not_present[i][j] == '#') {
                            not_present[i][j] = p[i];
                            break;
                        }
                    }
                }
            }
        }
    }
    update_v(root);
    valid_count(root);
    printf("\n%d\n", valid);
    valid = 0;
    attempts--;
    return;
}

int main() {
    w = scanf("%d", &k);
    r = malloc(k);
    p = malloc(k);
    not_present = malloc(sizeof(char *) * k);
    for (int i = 0; i < k; i++) {
        not_present[i] = malloc(DIC_SIZE);
        memset(not_present[i], '#', DIC_SIZE);
    }
    discovered = malloc(k);
    memset(discovered, '#', k);
    root = malloc(sizeof(node));
    root->v = 'y';
    root->key = malloc(k);
    w = scanf("%s", root->key);
    root->left = initialize((node *) root->left);
    root->right = initialize((node *) root->right);
    buffer = malloc(k);
    pregame();
    while (1) {
        if (gameon == -2 && w == EOF) break;
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
