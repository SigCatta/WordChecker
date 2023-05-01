# Progetto di Algoritmi e Strutture Dati 2022
Votazione 30L / 30

## Funzionamento atteso
Il progetto consiste nell'implementazione efficiente e corretta di un sistema che controlli la corrispondenza tra le lettere di due parole di uguale lughezza secondo specifiche definite.
Le parole sono intese come sequenze di simboli che possono essere caratteri alfabetici minuscoli (a-z) o maiuscoli (A-Z), cifre numeriche(0-9): oppure i simboli - (trattino) e _ ("underscore").

Il sistema deve leggere in ordine:
- un valore k, che indica la lunghezza delle parole
- una sequenza (di lunghezza arbitraria) di parole, ognuna di lunghezza k, checostituisce l'insieme delle parole ammissibili (si assuma che non ci siano parole ripetute)
- in ultimo, viene letta da stdin una sequenza di "partite", in cui l'inizio di ogni nuova partita è marcato dal comando (letto sempre da stdin) +nuova_partita

Le sequenze di stringhe in input per ogni partita (successive al comando +nuova_partita) sono fatte nel seguente modo:
- parola di riferimento (di lunghezza k caratteri) presumibilmente appartenten alle parole ammissibili
- numero n massimo di parole da confrontare con la parola di riferimento (ovvero il numero massimo di tentativi)
- sequenza di parole (ognuna di k caratteri) da confrontare con la parola di riferimento
- inoltre, sia durante una partita, che tra una partita e l'altra, possono comparire i comandi +inserisci_inizio e +inserisci_fine che racchiudono tra di loro una sequenza di nuove parole (di lunghezza k) da aggiungere all'insieme delle parole ammissibili (anche per partite successive)
- è poi possibile che durante la partita si legga il comando +stampa_filtrate, il programma deve quini produrre in output, in **ordine lessicografico** (specificato dallo standard ASCII), l'insieme delle parole ammissibili che sono compatibili con i vincoli appresi fino a quel momento nella partita, scritte una per riga

si noti che i vincoli appresi riguardano, per ogni simbolo:
- se il simbolo non appartiene a r
- posti in cui quel simbolo deve comparire in r
- posti in cui quel simbolo non può comparire in r
- numero minimo di volte che il simbolo compare in r
- numero esatto di volte che il simbolo compare in r


Per ogni parola letta (che nel seguito indichiamo con p), da confrontare con la parola di riferimento (che nel seguito indichiamo con r) nel seguito, indichiamo con p[1], p[2], … p[k] i caratteri della parola p, con r[1], r[2], … r[k] quelli della parola r, e con res[1], res[2], … res[k] quelli della sequenza stampata.

Il programma scrive su standard output una sequenza di k caratteri fatta nella seguente maniera, per ogni 1 ≤ i ≤ k, si ha che:
- res[i] è il carattere + se l'i-esimo carattere di p è uguale all'i-esimo carattere di r
- cioè se vale che p[i] = r[i], quindi p[i] è "in posizione corretta"
- res[i] è il carattere / se l'i-esimo carattere di p non compare da nessuna parte in r
- res[i] è il carattere | se l'i-esimo carattere di p (indicato nel seguito come p[i]) compare in r,
ma non in posizione i-esima; tuttavia, se in r compaiono ni istanze di p[i], se ci è il numero di
istanze del carattere p[i] che sono in posizione corretta (chiaramente deve valere che ci ≤ ni) e
se ci sono prima del carattere i-esimo in p almeno ni-ci caratteri uguali a p[i] che sono in
posizione scorretta, allora res[i] deve essere / invece che |
Inoltre, dopo ogni confronto, il programma deve stampare in output il numero di parole ammissibili ancora compatibili con i vincoli appresi (tranne nel caso in cui la parola confrontata non sia ammissibile)


Il progetto deve essere svolto:
- Individualmente
- tramite linguaggio C11
- Usando esclusivamente libreria standard libc (nessuna libreria esterna)
- Non applicando alcun tipo di tecnica di parallelizzazione o multithreading
- Utilizzando input fornito in stdin e output da fornire in stdout

## Dettagli implementativi
La struttura dati utilizzata in questa implementazione consiste in una tabella 64x64 di BST. Ogni parola viene inizialmente inserita in uno dei 4096 alberi in base alle sue prime due lettere. Tramite questa implementaziono più la partita va avanti (quindi più dati sulla parola di riferimento si acquisiscono) meno nodi è necessario controllare, infatti ad ogni tentativo il numero di nodi (parole) controllati sarà sempre minore o uguale a quello precedente (salvo inserimento di nuove parole), ogni informazione acquisita sulle prime due lettere della parola di riferimento ridurrà infatti il numero di nodi controllati, si applica questo ragionamento sia al conteggio di parole ammissibile che alla stampa delle parole ancora ammissibili. L'inserimento delle parole è molto più veloce rispetto a quello che si avrebbe utilizzando un sibgolo BST, si possono infatti scartare 4095 alberi che contengono parole con iniziali diversi in un tempo costante (controllando le prime due lettere della parola da inserire).
Ai fini di questo progetto, la struttura dati utilizzata fornisce un miglioramento delle prestazioni di circa il 75% rispetto ad un singolo BST (il miglioramento varia a seconda dei casi test)
