#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char new[] = "nuova_partita";
char filter[] = "stampa_filtrate";
char begin[] = "inserisci_inizio";
char end[] = "inserisci_fine";

int k, attempts, gameon = 1;
char *r, *p, *discovered;
int dictionary[54] = {};

/*          DICTIONARY
 *  lower case ASCII -> 97-122           ---> 0-25
 *  caps lock ASCII -> 65- 90            ---> 26-51
 *  minus -> 45 . underscore -> 126      ---> 52-53
 *  #symbols -> 54
 */

typedef struct {
    char *key;
    struct node *father;
    struct node *left;
    struct node *right;
    char v;
}node;

node *root;

char d_convert(int n){
    if (n <=25) return 'a' + n;
    if (n == 52) return '-';
    if (n == 53) return '_';
    return 'A' + (n - 26);
}

void count_letters(node *n){
    int count = 0;
    for (int i = 0; i < 54; i++) {
        if (dictionary[i] > 0){
            for (int j = 0; j < k; j++) {
                if (n->key[j] == d_convert(i)) count++;
            }
            if (count != dictionary[i]){
                n->v = 'n';
                return;
            }
            count = 0;
        }
    }
}

void update_v(node *n){
    if (n != NULL){
        while (n->v == 'y') {
            for (int i = 0; i < k; i++) {
                if (n->key[i] != '#') {
                    if (n->key[i] != discovered[i]) {
                        n->v = 'n';
                    }
                }
            }
            count_letters(n);
        }
        update_v((node *) n->left);
        update_v((node *) n->right);
    }
}

void valid_all(node *n){
    if (n != NULL){
        valid_all((node *) n->left);
        n->v = 'y';
        valid_all((node *) n->right);
    }
    return;
}

void inorder_tree_walk(node *n){
    if (n != NULL){
        inorder_tree_walk((node *) n->left);
        if (n->v == 'y') printf("%s\n", n->key);
        inorder_tree_walk((node *) n->right);
    }
    return;
}

void add(char *word, node *n, node *father){
    if (n->key == NULL) {
        n = malloc(sizeof(node));
        n->key = word;
        n->left = NULL;
        n->right = NULL;
        n->father = (struct node *) father;
        n->v = 'y';
        return;
    }
    for(int i=0; i<k; i++){
        if (word[i] < n->key[i]) return add(word, (node *) n->left, n);
        if (word[i] > n->key[i]) return add(word, (node *) n->right, n);
    }
    printf("ERRORE");
    return;
}

void reset(){
    valid_all(root);
    for (int i = 0; i < k; i++) discovered[i] = '#';
    for (int i = 0; i < 54; i++) dictionary[i] = 0;
}

void command(){
    char buffer[k];
    scanf("%s", buffer);
    if (strcmp(buffer, new) == 0) {
        reset();
        for (int j = 0; j < k; j++) scanf("%c", r[j]);
        scanf("%d", &attempts);
        gameon = 0;
        return;
    }
    if (strcmp(buffer, begin) == 0) {
        while(1) {
            for (int j = 0; j < k; j++){
                scanf("%c", buffer[j]);
                if (buffer[0] == '+') {
                    scanf("%s", buffer);
                    if (strcmp(buffer, end) == 0) {
                        gameon =0;
                        return;
                    }
                    else{
                        printf("ERRORE");
                        return;
                    }
                }
            }
            add(buffer, root, NULL);
        }
    }
    if (strcmp(buffer, filter) == 0) {
        gameon = 0;
        update_v(root);
        inorder_tree_walk(root);
        return;
    }
    return;
}

void pregame() {
    char buffer[k];
    while (1) {
        if (gameon == -1) break;
        for (int i = 0; i < k; i++) {
            scanf("%c", buffer[i]);
            if (buffer[0] == '+') {
                command();
                return;
            }
        }
        add(buffer, root, NULL);
    }
    return;
}

int present (char *word){
    for (int i = 0; i<k; i++){
        if (r[i] == word[i]) return 0;
    }
    return 1;
}

void d_update(char c, char bool, int count){
    if (bool == 'n') {
        if (c == 126) {
            dictionary[53]--;
            return;
        }
        if (c == 45) {
            dictionary[52]--;
            return;
        }
        if (c >= 97) {
            dictionary[c - 97]--;
            return;
        }
        if (c <= 65) {
            dictionary[c - 39]--;
            return;
        }
    }
    if (c == 126) {
        dictionary[53] = count;
        return;
    }
    if (c == 45) {
        dictionary[52] = count;
        return;
    }
    if (c >= 97) {
        dictionary[c - 97] = count;
        return;
    }
    if (c <= 65) {
        dictionary[c - 39] = count;
        return;
    }
}

void game(){
    for (int i = 0; i<k; i++){
        if (r[i] == p[i]) {
            printf("+");
            discovered[i] = p[i];
        }
        else if (present(p) == 1) {
            d_update(p[i], 'n', 0);
            printf("/");
        }
        else {
            int recA = 0, recB =0, recP = 0;
            for (int j = 0; j <k; j++) {
                if (r[i] == r[j]) recA++;
                if (p[i] == p[j]) {
                    recB++;
                    if (j<i) recP++;
                }
            }
            if (recA >= recB || recB > i || recB - recP > 0 ) {
                printf("|");
            }
            else {
                d_update(p[i], 'y', recA);
                printf("/");
            }
        }
    }
}

int main() {
    char buffer[k];
    scanf("%d", &k);
    root = malloc(sizeof(node));
    root->father = NULL;
    root->v = 'y';
    for (int i = 0; i < k; i++) scanf("%c", buffer);
    root->key = buffer;
    pregame();
    p = malloc(sizeof(char)*k);
    discovered = malloc(sizeof(char)*k);
    for (int i = 0; i < k; i++) {
        discovered[i] = '#';
    }
    r = malloc(sizeof(char)*k);
    while (1) {
        if (gameon == -1) break;
        if (attempts == 0) {
            printf("ko");
            pregame();
        }
        for (int i = 0; i < k; i++) scanf("%c", p[i]);
        if (p[0] == '+') command();
        if (strcmp(r,p) == 0) {
            gameon = 1;
            printf("ok");
        }
        if (gameon == 0){
            game();
            printf("\n");
            attempts--;
        }
        if (gameon == 1) pregame();
    }
}
