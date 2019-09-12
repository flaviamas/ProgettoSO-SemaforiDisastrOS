
# Progetto: Semafori Sistema Operativo DisastrOS

## WHAT:
Il progetto svolto è mirato all'implementazione delle funzioni che gestiscono i Semafori all'interno del sistema operativo DisastrOS.
I semafori sono valori interi utilizzati per inviare segnali tra processi. Possono essere effettuate solo tre operazioni su un semaforo: inizializzazione, incremento e decremento.
A questo proposito sono state implementate le funzioni di SemOpen e SemClose per la creazione e inizializzazione del semaforo e per la sua successiva deallocazione. Per quanto riguarda le operazioni di Incremento e decremento sono state implementate la semPost e la semWait. 

## HOW: 
Le *system call* implementate nel progetto per la gestione dei semafori sono state implementate nel seguente modo:

1.  **SemOpen**: La semOpen si occupa della creazione e dell'inizializzazione del semaforo. Prendendo in ingresso l'Id del semaforo e il suo valore di partenza si va ad allocare la nuova struttura dopo aver verificato che non sia già stata creata. Si inserisce all'interno di una lista globale di semafori e si allocano il descrittore e il puntatore ad esso relativo all'interno delle liste presenti nel processo attualmente in stato di Running.
2.  **SemClose**: La semClose si occupa della chiusura e della deallocazione di tutti i componenti relativi al semaforo che si sta chiudendo. In ingresso viene passato l'id del descrittore del semaforo che viene cercato all'interno della lista dei descrittori del processo attuale. In caso la ricerca vada a buon fine si eliminano il descrittore e il suo puntatore dalle relative liste e se il semaforo non ha più descrittori attivi viene rimosso dalla lista dei semafori.
3. **SemPost**:  La semPost si occupa di incrementare il valore del semaforo. Per fare ciò prende in ingresso l'id del descrittore e lo ricerca dentro la lista dei descrittori, se viene trovato incremento il valore. Nel caso in cui il valore del semaforo sia minore di uno il processo corrente viene messo in stato di ready ed inserito nella lista dei processi in attesa. Da questa lista si prende il primo processo in attesa che diventa il processo in fase di running. 
4. **SemWait** : La semWait effettua il decremento del semaforo, in ingresso prende l'id del descrittore e decrementa il valore del semaforo. Se il contatore è negativo il processo attuale viene messo in uno stato di waiting.

## HOW TO:
Per compilare si utilizza il comando `make` e per l'esecuzione si utilizza il comando `./disastrOS_test`

