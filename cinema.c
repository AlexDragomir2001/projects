#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"

// functie de comparare a 2 stringuri aflabetic
int compare_c_str(const void * str1, const void * str2)
{

    return strcmp(*(char **)str1,*(char **)str2);
}


// functie de initializare a grafului
void init_list_graph(graph *graph, int nodes) {
	graph->nr_nodes = nodes;
    // initializez lista de adiacenta
    graph->neighbours = malloc (nodes * sizeof(node *));
    // initializez listele de vecini
    for (int i = 0; i < nodes; i++) {
        graph->neighbours[i] = malloc(sizeof(node));
        graph->neighbours[i]->head = NULL;
        graph->neighbours[i]->tail = NULL;
        graph->neighbours[i]->size = 0;
        graph->neighbours[i]->name[0] = '\0';
    }
}

// functie in care iterez prin noduri pentru a il gasi
// pe cel cu numele cautat
// daca nu exista returnez -1
int findActor(graph *graph, int nodes, char* actor) {
    for (int i = 0; i < nodes; i++) {
        if(strcmp(graph->neighbours[i]->name, actor) == 0)
            return i;
    }
    return -1;
}

// creez nodul cu numele actorului
void create_node(graph *graph, int nodes, char* actor) {
    strcpy(graph->neighbours[nodes]->name, actor);
}

// adaug un nod in lista de adiacenta
void add_nth_node(node *list, int n, int new_data, char* actor) {
    node *prev, *curr;
    node *new_node;

    if (list == NULL) {
        return;
    }

    /* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
    if (n > list->size) {
        n = list->size;
    } else if (n < 0) {
        return;
    }

    // ajung pana la pozitia unde vreau sa inserez
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    // creez nodul
    new_node = malloc(sizeof(node));
    if (new_node == NULL) {
        perror("Not enough memory to add element!");
        exit(-1);
    }

    // ii declar indexul si il leg la urmator
    new_node->val = new_data;
    new_node->next = curr;
    strcpy(new_node->name, actor);
    // daca e primul element atunci e capul
    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = new_node;
    } else {
        prev->next = new_node;
    }
    // daca e ultimul atunci e coada
    if (new_node->next == NULL) {
        list->tail = new_node;
    }
    // maresc numarul de vecini
    list->size++;
}

// functie in care verific daca muchia exista deja
// respectiv daca actorii deja au jucat impreuna in alt film
int has_edge_list_graph(graph *graph, int src, int dest) {
    int ok = 0;
    if (graph->neighbours[src] == NULL) {
        return 0;
    }
    // iau primul vecin si iterez prin ei
    // daca il gasesc returnez ca l am gasit
    node *current = graph->neighbours[src]->head;
    while (current != NULL) {
        int nr = current->val;
        if (nr == dest) {
            ok = 1;
            break;
        }
        current = current->next;
    }
    return ok;
}



// functie de adaugare a unei muchii
void add_edge_list_graph(graph *graph, int src, int *dest, char *name) {
    if (graph->neighbours[src] == NULL) {
        return;
    }
    // daca muchia nu exista atunci o adaugam
    if (!has_edge_list_graph(graph, src, *dest)) {
        add_nth_node(graph->neighbours[src], graph->neighbours[src]->size, *dest, name);
    }
}

// fucntie care imi da vecinii unui nod
node* get_neighbours_list_graph(graph *graph, int node) {
     if (graph->neighbours[node] == NULL) {
        return NULL;
    }
    return graph->neighbours[node];
}

// functie care sterg un nod din functie
// si il returnez
node* remove_nth_node(node *list, int n) {
    node *prev, *curr;

    if (list == NULL) {
        return NULL;
    }

    if (list->head == NULL) {
        return NULL;
    }

    if (n > list->size - 1) {
        n = list->size - 1;
    } else if (n < 0) {
        return NULL;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;

        if (prev->next == NULL) {
            list->tail = prev;
        }
    }

    list->size--;
    return curr;
}

// returnez dimensiunea unei liste
int get_size(node *list) {
    if (list == NULL) {
        return -1;
    }

    return list->size;
}

// eliberez o lista
void free_list(node **pp_list) {
    node *currNode;

    if (pp_list == NULL || *pp_list == NULL) {
        return;
    }

    // pana cand lista nu mai are noduri
    // eliberez cate un nod
    // si il sterg din lista
    while(get_size(*pp_list) > 0) {
        currNode = remove_nth_node(*pp_list, 0);
        free(currNode);
    }

    free(*pp_list);
    *pp_list = NULL;
}

// sterg o muchie dintr un graf
void remove_edge_list_graph(graph *graph, int src, int dest) {
    if (graph->neighbours[src] == NULL) {
        return;
    }
    node *current = graph->neighbours[src]->head;
    int cnt = 0;
    // ma plimb pana gasesc destinatia si elimin nodul
    // din lista de vecini
    while (current != NULL) {
        int nr = current->val;
        if (nr == dest) {
            remove_nth_node(graph->neighbours[src], cnt);
            break;
        }
        current = current->next;
        cnt++;
    }
}

// eliberez graful
void clear_list_graph(graph *graph) {
    for (int i = 0; i < graph->nr_nodes; i++) {
        free_list(&graph->neighbours[i]);
    }
    free(graph->neighbours);
}

// functie care imi realizeaza cautarea in adancime
void dfs(int *visited, int val, graph *lg, int *color, int culoare, int *colorCounter){
    // marchez nodul ca fiind vizitat
	visited[val] = 1;
    // cresc dimensiunea componentei conexe actuale
    colorCounter[culoare]++;
    // marchez culoarea nodului actual
    color[val] = culoare;
    // ma plimb in lista de vecini si verific
    // daca nodul curent are legatura la un nod
    // nevizitat inca si daca da atunci il adaug
    // si reapelz functia
	for (int i = 0; i < lg->nr_nodes; i++) {
        if (has_edge_list_graph(lg, val, i) == 1) {
        	if(!visited[i]){
        		dfs(visited, i, lg, color, culoare, colorCounter);
        	}
        }
    }
}

int main(int argc, char const *argv[])
{

    if(strcmp(argv[1], "-c1") == 0) {

        int nodes, movies, actors, i, j, k;
        int  visited[7000];
        int color[7000], colorCounter[7000], actorInd[7000];
        graph *lg = malloc(sizeof(graph));
        FILE *pFile, *wFile;
        char *buffer;
        // deschid fisierele
        pFile = fopen(argv[2], "r");
        wFile = fopen(argv[3], "w");
        // nr de filme
        fscanf(pFile, "%d\n", &movies);
        // initializez variabilele
        for (int i = 0; i < 7000; i++) {
            visited[i] = 0;
            color[i] = 0;
            colorCounter[i] = 0;
        }
        nodes = 0;
        // initializez graful cu un nr maxim de noduri
        init_list_graph(lg, 7000);
        size_t bufsize = 100;
        for (i = 0; i < movies; i++) {
            // citesc filmul
            buffer = (char *)malloc(bufsize * sizeof(char));

            // numele
            getline(&buffer,&bufsize,pFile);
            free(buffer);

            // nr de actori
            fscanf(pFile, "%d\n", &actors);
            // iterez prin actori
            for(j = 0; j < actors; j++) {
                // citesc un nume
                buffer = (char *)malloc(bufsize * sizeof(char));
                getline(&buffer,&bufsize,pFile);
                // sterg enterul de la final
                buffer[strlen(buffer) - 1] = '\0';
                // caut sa vad daca deja exista actorul
                int actor = findActor(lg, nodes, buffer);
                actorInd[j] = actor;
                // daca nu exista atunci il creez
                if(actor == -1) {
                    create_node(lg, nodes, buffer);
                    actorInd[j] = nodes;
                    nodes++;
                }
                free(buffer);
            }
            // iterez prin actorii din filmul curent
            // si adaug muchiile
            for(j = 0; j < actors; j++) {
                for (k = 0; k < actors; k++) {
                    if(k == j)
                        continue;
                    add_edge_list_graph(lg, actorInd[j], &actorInd[k], lg->neighbours[actorInd[k]]->name);
                }
            }
        }

        int culoare = 1;
        // fac componentele conexe
        for (int i = 0; i < nodes; i++) {
        	if(!visited[i]){
        		visited[i] = 1;
                color[i] = culoare;
                colorCounter[culoare]++;
        		dfs(visited, i, lg, color, culoare, colorCounter);
                culoare++;

        	}
        }
        // vad care componenta conexa are cele mai multe noduri
        int max, maxInd;
        max = colorCounter[i];
        maxInd = 0;
        for(i = 1; i < culoare; i++) {
            if(max < colorCounter[i]) {
                max = colorCounter[i];
                maxInd = i;
            }
        }
        j = 0;
        // iau numele din noduri si le pun intr-un vector de cuvinte
        char** actorsNames = (char**) malloc((max - 1) * sizeof(char*));

        for(i = 0; i < nodes; i++) {
            // daca se afla in componenta conexa mare
            // atunci il adaug in matrice
            if(color[i] == maxInd) {

                actorsNames[j] = (char*)calloc(100, sizeof(char));
                strcpy(actorsNames[j], lg->neighbours[i]->name);
                j++;
            }
        }
        // sortez alfabetic cu qsort
        qsort(actorsNames, max - 1, sizeof(actorsNames[0]), compare_c_str);
        // printez numarul de noduri
        fprintf(wFile, "%d\n", max - 1);
        for (int i = 0; i < max - 1; i++)
        {
            // printez numele actorilor
        	fprintf(wFile,"%s\n", actorsNames[i]);
        }

        // eliberez matricea
        for (i = 0; i < max - 1; i++) {
            free(actorsNames[i]);
        }
        free(actorsNames);
        // eliberez lista de adiacenta
        clear_list_graph(lg);
        // eliberez graful
        free(lg);
        // inchid fisierele
        fclose(pFile);
        fclose(wFile);
    }
}
