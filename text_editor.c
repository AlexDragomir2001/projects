#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char string;
    struct node* next;
    struct node* prev;
    int linie;
    int coloana;
};

struct stack {
    char string[10];
    struct stack* next;
};

//functie care imi introduce elemente in stiva
void push(struct stack** top, char c[]) {
    struct stack* newNode = malloc(sizeof(struct stack));
    strcpy(newNode->string, c);
    newNode->next = *top;
    *top = newNode;
}

//functie care imi returneaza intr-un string ultimul element adaugat in stiva
void peek(struct stack** top, char string_to_undo[10]) {
    strcpy(string_to_undo, (*top)->string);
}

//functie care imi sterge din stiva ultimul element adaugat
void pop(struct stack** top) {
    struct stack* node = *top;

    if(*top == NULL) {
        return;
    }
    else {
        *top = (*top)->next;
        free(node);
    }
}

//functie care imi insereaza caracter la finalul unei liste duble inalntuite
void insertEnd(struct node** head, char c, int lin, int col) {
    struct node* new_node = malloc(sizeof(struct node));
    new_node->string = c;
    new_node->linie = lin;
    new_node->coloana = col;
    new_node->next = NULL;
    new_node->prev = *head;
    (*head)->next = new_node;
    *head = new_node;
}

//functie care imi adauga primul caracter intr-o lista dubla inalntuita goala
void addToEmpty(struct node** tail, struct node** head, char c, int lin, int col) {
    struct node* new_node = malloc(sizeof(struct node));
    new_node->string = c;
    new_node->linie = lin;
    new_node->coloana = col;
    new_node->next = NULL;
    new_node->prev = NULL;
    *tail = new_node;
    *head = new_node;
}

//functie care imi sterge nodul de la sfarsitul unei liste duble intalnuite
void delete_from_end(struct node** head) {
    struct node* temp;
        temp = *head;
        (*head)->prev->next = NULL;
        (*head) = (*head)->prev;
        free(temp);
}

//functie care imi sterge nodul de la inceputul unei liste duble inalntuite
void delete_from_beginning(struct node** tail) {
    struct node* temp;
        temp = (*tail);
        (*tail) = (*tail)->next;
        (*tail)->prev = NULL;
        free(temp);
}

//functie care imi sterge un nod oriunde in lista, dar care nu functioneaza pentru head si tail
void remove_node(struct node* node_to_remove) {
    if(node_to_remove->prev != NULL) {
        node_to_remove->prev->next = node_to_remove->next;
    }
    if(node_to_remove->next != NULL) {
        node_to_remove->next->prev = node_to_remove->prev;
    }
    free(node_to_remove);
}

//functie care imi printeaza lista dubla inalntuita
void print_list(struct node** tail) {
    struct node* curr = *tail;
    while(curr != NULL) {
        printf("%c", curr->string);
        curr = curr->next;
    }
}

//functie care imi sterge o linie pozitionata la indicele x, dar care nu functioneaza pentru prima si ultima linie
void delete_line(struct node** tail, int x) {
    struct node* curr = *tail;
    struct node* copy;
    while(curr != NULL) {
     if(curr->linie == x) {
         copy = curr->next;
         remove_node(curr);
         curr = copy;
     }
     else {
         curr = curr->next;
     }
    }
} 

//functie care imi sterge prima linie din lista dubla inalntuita
void delete_first_line(struct node** tail) {
    struct node* curr = *tail;
    struct node* copy;
    while(curr->linie == 1) {
        copy = (*tail)->next;
        delete_from_beginning(tail);
        curr = copy;
    }
}

//functia care dupa fiecare comanda de text executata, restabileste indicii de lini si coloana in urma modificarilor
void indici(struct node** tail) {
    int linie = 1, coloana = 1;
    struct node* curr = *tail;
    while(curr != NULL) {
        if(curr->string == '\n') {
            curr->linie = linie;
            curr->coloana = coloana;
            linie++;
            coloana = 1;
        }
        else {
            curr->linie = linie;
            curr->coloana = coloana;
            coloana++;
        }
        curr = curr->next;
    }
}

//functia care imi copiaza in string_stack intreaga comanda, inclusiv cu indici daca acestia exista
void get_line(struct node** curr, char string_stack[]) {
    int contor = 0;
    while((*curr)->string != '\n') {
                string_stack[contor++] = (*curr)->string;
                (*curr) = (*curr)->next;
            }
    string_stack[contor] = '\0';
    (*curr) = (*curr)->next;
}

//functia care imi imi copiaza in conditie doar cocmanda fara indici(de exmplu doar dl, re, s, q), intrucat parcurc string_stack pana la primul spatiu
void get_command(char string_stack[], char conditie[]) {
    int contor = 0;
    int i = 0;
    for(i = 0; i < strlen(string_stack); i++) {
        if(string_stack[i] == ' ')
            break;
        conditie[contor++] = string_stack[i];
    }
    conditie[contor] = '\0';
}

//functia care imi extrage din string_stack doar indicii corespunzatori comenzii(de exmeplu pentru gc 3 6, imi retine 3 in linie si 6 in coloana)
void get_parametrii(char string_stack[], int* linie, int* coloana, char conditie[]) {
    int i = 0;
    int j = 0;
    if(strlen(string_stack) <= 4) {
        for(i = 0; i < strlen(string_stack); i++) {
            if(string_stack[i] == ' ')
                break;
        }
        if(string_stack[i] != '\0') {
            if((strcmp(conditie, "gc")) == 0) {
                    *coloana = string_stack[i+1] -'0';              // 0 inseamna ca nu mi trebuie in comanda
                    *linie = -1;                                     // -1 inseamna ca linia/colona o sa fie cea curenta cand parcurg
            }
            else {
                *linie = string_stack[i+1] - '0';
                *coloana = 0;
            }
        }
        else {
            *linie = -1;
            *coloana = 0;
        }
    }
    if(strlen(string_stack) > 4) {
        for(j = 0; j < strlen(string_stack); j++) {
            if(string_stack[j] == ' ')
                break;
        }
        *coloana = string_stack[j+1] - '0';
        *linie = string_stack[j+3] - '0';
    }
}

//functia care imi copiaza lista pe care fac modificari in fisierul editor.out, la comanda save (s)
void copy_to_file(struct node** tail, FILE* file_to_write) {
    struct node* curr = *tail;
    while(curr != NULL) {
        fputc(curr->string, file_to_write);
        curr = curr->next;
    }
}

//functia aferenta comenzii d, care sterge un anumit numar de caractere de dupa cursor
void delete_d(struct node** tail, int current_line, int current_column, int com_linie) {
    struct node* curr = *tail;
    struct node* copy;
    while(curr != NULL) {
        if(curr->linie == current_line && curr->coloana == current_column)
            break;
        else
            curr = curr->next;
    }
    int copie = com_linie;
    while(com_linie > 0) {
        curr = curr->next;
        com_linie--;
    }
    while(copie > 0) {
        copy = curr->next;
        remove_node(curr);
        curr = copy;
        copie--;
    }
}

//functia aferenta comenzii backspace, care sterge un caracter de dinaintea cursorului
void backspace(struct node** tail, int x, int y) {
    struct node* curr = *tail;
    while(curr != NULL) {
        if(curr->linie == x && curr->coloana == y) {
            remove_node(curr);
            break;
        }
        else
            curr = curr->next;
    }
}

//functia care imi copiaza din string_stack, cuvant pe care trebuie sa il inlocuiesc in text, si cuvantul nou cu care acesta din urma trebuie inlocuit
void replace_word(char string_stack[], char old_word[], char new_word[]) {
    int ok = 0;
    const char s[2] = " ";
    char *token;
    token = strtok(string_stack, s);
    while(token != NULL) {
        if(ok == 0) {
            ok++;
            token = strtok(NULL, s);
            continue;
        }
        if(ok == 1) {
            strcpy(old_word, token);
            ok++;
        }
        if(ok == 2) {
            strcpy(new_word, token);
        }
        token = strtok(NULL, s);
    }
}

//functie care imi copiaza in word_to_delete, cuvantul pe care trebuies sa-l sterg din text (pentru comanda dw)
void get_word_to_delete(char string_stack[], char word_to_delete[]) {
    int ok = 0;
    const char s[2] = " ";
    char* token;
    token = strtok(string_stack, s);
    while(token != NULL) {
        if(ok == 0) {
            ok++;
            token = strtok(NULL, s);
        }
        if(ok == 1) {
            strcpy(word_to_delete, token);
            break;
        }
        token = strtok(NULL, s);
    }
}

//functie care imi elibereaza memoria din lista dubla inalnuita
void deallocate_double_list(struct node** tail, struct node** head) {
    struct node* current = *tail;
    while(current->next != NULL) {
        current = current->next;
        free(current->prev);
    }
    free(current);

    *tail = NULL;
    *head = NULL;
}

//functia care imi elibereaza memoria din stack
void deallocate_stack(struct stack** top) {
    struct stack* current = *top;
    struct stack* copy;
    while(current != NULL) {
        copy = current;
        current = current->next;
        free(copy);
    }
    *top = NULL;
}

//functie care insereaza dupa un anumit nod din lista
void insert_after(struct node* node, char c) {
    struct node* newNode = malloc(sizeof(struct node));
    if(newNode == NULL) {
        return;
    }
    newNode->string = c;
    newNode->prev = node;
    newNode->next = node->next;
    node->next = newNode;
}

int main() {
    struct node* head_1 = NULL;
    struct node* tail_1 = NULL;
    struct node* head_2 = NULL;
    struct node* tail_2 = NULL;
    struct node* head_3 = NULL;
    struct node* tail_3 = NULL;
    struct stack* top_1 = NULL;
    struct stack* top_2 = NULL;

    FILE* file_to_read = fopen("editor.in", "r");
    FILE* file_to_write;

    if(file_to_read == NULL) {
        return 0;
    }
    struct node* tmp;
    struct node* pass;
    struct node* walk;
    struct node* copy;
    struct node* pas;
    struct node* copie_ptr;
    int lungime_3;
    int count = 0;
    int reminder = 0;
    int reminder_2 = 0;
    int nr = 0;
    int i = 0;
    int j = 0;
    int lungime_1;
    int lungime_2;
    int nr_elem_adaugate = 0;
    int ok = 0;
    int contor = 0;
    int linie = 1;
    int coloana = 1;
    int com_linie, com_coloana;
    int copy_column;
    char conditie[10];
    char word_to_delete[20];
    char string_to_undo[20];
    char string_to_redo[20];
    char old_word[20];
    char new_word[20];
    char added_text[20];
    char c;
    char string_stack[15];

    //introduc tot continutul din fisierul editor.in intr-o lista
    while((c = fgetc(file_to_read)) != EOF) {
        if(tail_1 == NULL) {
            addToEmpty(&tail_1, &head_1, c, linie, coloana);
        }
        else {
        insertEnd(&head_1, c, linie, coloana);
        }
    }
    fclose(file_to_read);

    //adaug textul din fisier pana la gasirea ':', adica pana la introducerea comenzii de "introduce comezni"
    addToEmpty(&tail_2, &head_2, tail_1->string, linie, coloana);
    coloana++;
    struct node* curr = tail_1;
    curr = curr->next;
    while(curr->string != ':') {
        if(curr->string == '\n') {
            insertEnd(&head_2, curr->string, linie, coloana);
            linie++;
            copy_column = coloana -1;
            coloana = 1;

            //in timp ce introduc in cea de-a doua lista tot textul pe care urmeaza sa aplic comenzi,
            //sterg acest text din prima lista pentru a-mi ramane in prima lista doar comenzile pe care voi urma sa le aplic
            delete_from_beginning(&tail_1);
        }
        else {
            insertEnd(&head_2, curr->string, linie, coloana);
            coloana++;

            //in timp ce introduc in cea de-a doua lista tot textul pe care urmeaza sa aplic comenzi,
            //sterg acest text din prima lista pentru a-mi ramane in prima lista doar comenzile pe care voi urma sa le aplic
            delete_from_beginning(&tail_1);
        }
        curr = curr->next;
    }
    delete_from_beginning(&tail_1);
    int current_line = linie - 1;
    int current_column = copy_column;
    curr = tail_1;

    //incep parcurgerea primei liste, unde mi-au ramas doar comenzile
    while(curr != NULL) {
        //cand ok = 0, stiu ca urmeaza sa intalnesc comenzi pe care sa le aplic pe text
        if(ok == 0) {

            //copiez prima comanda integral in string_stack
            get_line(&curr, string_stack);

            //copiez in conditie doar comanda fara indici/cuvinte de inlocuit (de exemplu dl, gl, dw, re)
            get_command(string_stack, conditie);

            //daca intru in acest if, stiu ca am comanda "::i"
            if(strlen(conditie) == 3){
                
                //am presupus ca utilizatorul nu va vrea sa faca undo pentru primul text introdus, asa ca prima oara nu introduc in stack "::i"
                if(reminder == 0) {
                    reminder = 1;
                    continue;
                }
                else {
                    push(&top_1, string_stack);
                    continue;
                }
            }

            //daca intru in acest if, stiu ca am o comanda din 2 caractere (dl, re, gl etc)
            if(strlen(conditie) == 2) {

                //retin indicii fiecare comenzi
                get_parametrii(string_stack, &com_linie, &com_coloana, conditie);

                //urmatoarele if-uri verifica ce comanda gasesc in fisier
                if((strcmp(conditie, "dl")) == 0) {

                    //valoarea de -1 pentru linie inseamna ca o voi considera pe cea curenta
                    //valoarea de 0 pentru coloana inseamna ca ea nu este de luat in calcul in cazul acestei comezni
                    if(com_linie == -1 && com_coloana == 0) {
                        delete_line(&tail_2, current_line);

                        //restabilesc indicii dupa stergerea liniei
                        indici(&tail_2);
                        push(&top_1, string_stack);
                    }
                    else {

                        //caz particular in care trebuie sa sterg prima linie
                        if(com_linie == 1) {
                        delete_first_line(&tail_2);
                        indici(&tail_2);
                        push(&top_1, string_stack);
                        }
                        
                        //cazul in care linie este oricare, dar nu prima
                        else {
                            delete_line(&tail_2, com_linie);
                            indici(&tail_2);
                            push(&top_1, string_stack);
                        }
                    }
                }
                if((strcmp(conditie, "gl")) == 0) {

                    //setez variabile care retine linia curenta sa aiba valoarea indicelui de dupa gl (de exemplu, pt gl 7 current line debine 7)
                    current_line = com_linie;
                    push(&top_1, string_stack);
                }
                if((strcmp(conditie, "gc")) == 0) {

                    //caz particular, daca linie e -1 inseamna ca trebuie considerata linia curenta
                    if(com_linie == -1) {
                        current_column = com_coloana;
                        push(&top_1, string_stack);
                    }

                    //cazul in care gc vine cu ambii indici (de exemplu, gc 3 2 inseamna coloana 3, linia 2)
                    else {
                        current_line = com_linie;
                        current_column = com_coloana;
                        push(&top_1, string_stack);
                    }
                }

                //pentru comanda re, am considerat doar cazul in care,
                //cuvantul pe care vrem sa-l inlocuim are acelasi nr. de caractere cu (,) cuvantul pe care vrem sa-l introducem
                if((strcmp(conditie, "re")) == 0) {
                    replace_word(string_stack, old_word, new_word);
                    pas = tail_2;
                    contor = 0;
                    lungime_1 = strlen(old_word);
                    while(pas != NULL) {

                        if(pas->string == old_word[0]) {
                            
                            //daca (,) cuvantul are lungimea 1 si l-am si gasit in lista, inlocuies in lista si dau break
                            if(strlen(old_word) == 1) {
                                pas->string = new_word[0];
                                break;
                            }
                            //daca (,) cuvantul are lungimea 2, iar si urmatorul nod din lista e caracterul din cuvant, inlocuiesc in lista si dau break
                            if(strlen(old_word) == 2) {
                                if(pas->next->string == old_word[1]) {
                                    pas->string = new_word[0];
                                    pas->next->string = new_word[1];
                                    break;
                                }
                            }
                            //daca (,) cuvantul are lungimea mai mare decat 2, si stiu ca am gasit si prima litera din old_word si in lista,
                            //am considerat ca daca le verific si pe urmatoarele 2, deci as gasi 3 caractere din old_word, la rand, si in lista,
                            //atunci sunt sigur ca am gasit cuvantul si incepand de la adresa unde am gasit primul caracter, parcurg si inlocuiesc cuvantul vechi cu cel nou
                            if(strlen(old_word) > 2) {
                                if((pas->next->string == old_word[1]) && (pas->next->next->string == old_word[2]))  {
                                    while(lungime_1 > 0) {
                                        pas->string = new_word[contor];
                                        pas = pas->next;
                                        lungime_1--;
                                        contor++;
                                    }
                                    break;
                                }
                            }
                            pas = pas->next;
                        }
                        else
                            pas = pas->next;
                    }
                }
                if((strcmp(conditie, "dw")) == 0) {
                    get_word_to_delete(string_stack, word_to_delete);  
                    pass = tail_2;
                    lungime_2 = strlen(word_to_delete);
                    while(pass != NULL) {
                        
                        //am folosit acelasi rationament explicat la comanda re
                        if(pass->string == word_to_delete[0]) {
                            if(strlen(word_to_delete) == 1) {
                                remove_node(pass);
                                indici(&tail_2);
                                break;
                            }
                            if(strlen(word_to_delete) >= 2) {
                                if((pass->next->string == word_to_delete[1]) && (pass->next->next->string == word_to_delete[2])) {
                                    while(lungime_2 > 0) {
                                            copy = pass->next;
                                            remove_node(pass);
                                            pass = copy;
                                            lungime_2--;
                                    }
                                }
                            }
                            indici(&tail_2);
                            pass = pass->next;
                        }
                        else
                            pass = pass->next;
                    }
                    indici(&tail_2);
                }
                if((strcmp(conditie, "ra")) == 0) {
                    replace_word(string_stack, old_word, new_word);
                    pas = tail_2;
                    contor = 0;
                    lungime_1 = strlen(old_word);
                    while(pas != NULL) {

                        if(pas->string == old_word[0]) {
                            
                            //daca (,) cuvantul are lungimea 1 si l-am si gasit in lista, inlocuies in lista, merg mai departe in lista, si dau continue,
                            //intrucat mai pot aparea aparitii ale cuvantului
                            if(strlen(old_word) == 1) {
                                pas->string = new_word[0];
                                pas = pas->next;
                                indici(&tail_2);
                                continue;
                            }
                            //daca (,) cuvantul are lungimea 2, iar si urmatorul nod din lista e caracterul din cuvant, inlocuiesc in lista si dau continue
                            if(strlen(old_word) == 2) {
                                if(pas->next->string == old_word[1]) {
                                    pas->string = new_word[0];
                                    pas->next->string = new_word[1];
                                    pas = pas->next;
                                    continue;
                                }
                            }
                            //rationamentul este acelasi ca la comanda re:
                            //daca (,) cuvantul are lungimea mai mare decat 2, si stiu ca am gasit si prima litera din old_word si in lista,
                            //am considerat ca daca le verific si pe urmatoarele 2, deci as gasi 3 caractere din old_word, la rand, si in lista,
                            //atunci sunt sigur ca am gasit cuvantul si incepand de la adresa unde am gasit primul caracter, parcurg si inlocuiesc cuvantul vechi cu cel nou
                            if(strlen(old_word) > 2) {
                                if((pas->next->string == old_word[1]) && (pas->next->next->string == old_word[2]))  {
                                    while(lungime_1 > 0) {
                                        pas->string = new_word[contor];
                                        pas = pas->next;
                                        lungime_1--;
                                        contor++;
                                    }
                                    pas = pas->next;
                                    contor = 0;
                                    continue;
                                }
                            }
                            pas = pas->next;
                        }
                        else
                            pas = pas->next;
                    }
                    indici(&tail_2);
                }
                if((strcmp(conditie, "da")) == 0) {
                    get_word_to_delete(string_stack, word_to_delete);  
                    pass = tail_2;
                    lungime_2 = strlen(word_to_delete);
                    while(pass != NULL) {
                        
                        //am folosit acelasi rationament explicat la comanda ra
                        if(pass->string == word_to_delete[0]) {
                            if(strlen(word_to_delete) == 1) {
                                copy = pass->next;
                                remove_node(pass);
                                pass = copy;
                                if(pass->string == '\n') {
                                    copy = pass->next;
                                    remove_node(pass);
                                    pass = copy;
                                }
                                indici(&tail_2);
                                continue;
                            }
                            if(strlen(word_to_delete) >= 2) {
                                if((pass->next->string == word_to_delete[1]) && (pass->next->next->string == word_to_delete[2])) {
                                    while(lungime_2 > 0) {
                                            copy = pass->next;
                                            remove_node(pass);
                                            pass = copy;
                                            lungime_2--;
                                    }
                                    pass = pass->next;
                                    indici(&tail_2);
                                    continue;
                                }
                            }
                            pass = pass->next;
                        }
                        else
                            pass = pass->next;
                    }
                }       
            }
            if(strlen(conditie) == 1) {

                //daca am comanda s, copiez in editor.out ce s-a modificat in lista pana in acel moment
                if((strcmp(conditie, "s")) == 0) {
                    file_to_write = fopen("editor.out", "w");
                    copy_to_file(&tail_2, file_to_write);
                    push(&top_1, string_stack);
                    fclose(file_to_write);
                }
                //q inseamna ca dau break din while si programul s-a terminat
                if((strcmp(conditie, "q")) == 0) {
                    push(&top_1, string_stack);
                    break;
                }
                //daca am backspace
                if((strcmp(conditie, "b")) == 0) {
                    backspace(&tail_2, current_line, current_column);

                    //actualizez colana curenta dupa ce sterg
                    current_column = com_coloana -1;
                    indici(&tail_2);
                    push(&top_1, string_stack);
                }
                if((strcmp(conditie, "d")) == 0) {

                    //caz particular, daca primesc doar "d", sterg de la pozitia curenta doar un caracter de dupa cursor
                    if((strlen(string_stack)) == 1) {
                        delete_d(&tail_2, current_line, current_column, 1);
                        indici(&tail_2);
                        current_line = current_line - 1;
                        push(&top_1, string_stack);
                    }
                    else {
                        //daca nu, iau parametrul comenzii
                        com_linie = string_stack[2] - '0';
                        delete_d(&tail_2, current_line, current_column, com_linie);
                        indici(&tail_2);
                        //restabilesc indicele pentru current line dupa ce am sters oricate caractere
                        current_line = current_line - com_linie;
                        push(&top_1, string_stack);
                    }
                }
                if((strcmp(conditie, "u")) == 0) {
                    
                    //copiez in string_to_undo ultimul element adaugat in stiva
                    peek(&top_1, string_to_undo);
                    //daca este comanda "::i"
                    if((strcmp(string_to_undo, "::i")) == 0) {

                        //sterg ultimul text inserat
                        while(nr_elem_adaugate > 0) {
                            delete_from_end(&head_2);
                            nr_elem_adaugate--;
                        }
                        indici(&tail_2);
                        //introduc in stiva de undo comanda la care am dar redo
                        push(&top_2, string_to_undo);
                        //sterg din stiva de comenzi comanda la care am dat undo
                        pop(&top_1);
                    }
                }
                if((strcmp(conditie, "r")) == 0) {
                    //copiez in string_to_redo ce se afla in stiva de redo
                    peek(&top_2, string_to_redo);
                    //daca este comanda "::i"
                    if((strcmp(string_to_redo, "::i")) == 0) {
                        //adaug textul care a fost sters in urma comenzii de undo
                        for(i = 0; i < strlen(added_text); i++) {
                            insertEnd(&head_2, added_text[i], 1, 1);
                        }
                        indici(&tail_2);
                        //adaug in stiva de undo comanda
                        push(&top_1, string_to_redo);
                        //sterg din stiva de redo comanda
                        pop(&top_2);
                    }
                }
            }
            //daca in timpul parcurgerii, ajung la ":" stiu ca este comanda dupa care urmeaza sa fie inserat text
            //asadar, sar peste comanda si fac ok = 1, pentru a intra pe else
            if(curr->string == ':') {
                while(curr->string != '\n') {
                    curr = curr->next;
                }
                curr = curr->next;
                ok = 1;
            }
        }
        else {
            copie_ptr = curr;
            
            //copiez in added_text textul de inserat
            //rationamentul este ca daca se va insera textul la finalul listei reminder va deveni 1
            //daca reminder va deveni 1, se va intra pe while-ul cu reminder 1, unde se va insera text la o anumita pozitie
            //la o a treia intrare in else, se va decide valoare lui reminder: fie 0, daca se doreste inserare la finalul listei,
            //fie 1 daca se doreste inserarea la o anumita pozitie
            get_line(&curr, added_text);
            lungime_3 = strlen(added_text);
            curr = copie_ptr;
            if(reminder_2 == 1) {
                while(curr->string != ':')
                    curr = curr->next;
                if((current_line == linie -1) && (current_column == copy_column))
                    reminder_2 = 0;
                    else
                        reminder_2 = 1;
            }
            while(curr->string != ':') {
                if(reminder_2 == 0) {
                if((current_line == linie -1) && (current_column == copy_column)) {    
                insertEnd(&head_2, curr->string, 1, 1);
                if(count == 0) {
                    //introduc textul de inserat intr-o a treia lista, pentru ca in final sa-l introduc intr-un string pentru a stii nr. de caractere pe care le-am inserat
                    addToEmpty(&tail_3, &head_3, curr->string, 1, 1);
                    count = 1;
                    nr_elem_adaugate++;
                }
                else {
                    insertEnd(&head_3, curr->string, 1, 1);
                    nr_elem_adaugate++;
                }
                curr = curr->next;
                }
                else break;
                }
                else break;
            }
            walk = tail_2;
            while(walk != NULL) {
                if(reminder_2 == 1) {
                    if(walk->linie == current_line && walk->coloana == current_column) {
                        while(lungime_3 > 0) {
                            insert_after(walk, added_text[j]);
                            lungime_3--;
                            j++;
                            walk = walk->next;
                        }
                    }
                    else
                        walk = walk->next;
                }
                else
                    break;
            }
            indici(&tail_2);
            reminder_2 = 1;

            //ok devine 0, deoarece inserarea s-a terminat, iar se va trece din nou la executarea comenzilor, cand ok este 1
            ok = 0;
            tmp = tail_3;
            while(tmp != NULL) {
                added_text[nr++] = tmp->string;
                tmp = tmp->next;
            }
            added_text[nr] = '\0';
        }
    }

    //eliberez memoria din listele si stivele pe care le-am folosit
    deallocate_double_list(&tail_1, &head_1);
    deallocate_double_list(&tail_2, &head_2);
    deallocate_double_list(&tail_3, &head_3);
    deallocate_stack(&top_1);
    deallocate_stack(&top_2);
    return 0;
}               
