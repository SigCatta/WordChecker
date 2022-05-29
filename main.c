#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define parole 20

char new[] = "nuova_partita";
/*char filter[] = "stampa_filtrate";*/
char begin[] = "inserisci_inizio";
char end[] = "inserisci_fine";

int k, ps, temp = 0;
char buff, *array, *buffer;

int exists(const char p[]){
    int c=0;
    for (int i=0; i<parole*(k+1); i++) {
        if (array[i] == p[i]) c++;
        else c=0;
        if (c == 5) return 0;
    }
    return 1;
}

void insert(){
    for (int i = ps; i<parole*(k); i++ ){
        temp++;
        scanf("%c", &buff);
        if (buff == '+') {
            scanf("%s", &buffer);
            if(strcmp(buffer, end)==0) {
                ps = i/k;
                break;
            }
            else return;
        }
        array[i] = buff;
        if (buff == 0 || buff == 32 || buff == '\n') i--;
    }
}

int main() {
    scanf("%d",&k);
    array = malloc((k)*parole);
    for (int i=0; i<parole*(k); i++) {
        temp++;
        scanf("%c", &buff);
        if (buff == '+') {
            scanf("%s", &buffer);
            if(strcmp(buffer, new)==0) {
                ps = i/k;
                break;
            }
            else return -1;
        }
        array[i] = buff;
        if (buff == 0 || buff == 32 || buff == '\n') i--;
    }
    char r[k];
    scanf("%s", r);
    int attempts;
    scanf("%d", &attempts);
    while(1){
        char p[k];
        scanf("%s", p);
        if (strcmp(p,r)==0) break;
        if (strcmp(p,begin)== 0) insert();
        if (exists(p)==0){
        }
        else printf("not_exists");
    }
}