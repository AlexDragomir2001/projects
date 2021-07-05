#include <stdio.h>
#include <string.h>

void smooth(int N, int M, double a[][M], double b[][M])
{
    double c[3][3];
    int i = 0, j = 0;
    for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++) //creez matricea smooth, si pun pe fiecare pozitie 1, urmand sa inmultesc cu 1/9 la final, cand fac transformarea
                c[i][j] = 1;
        }
    for(i = 0; i < N; i++)
        {                               //parcurg matricea
            for(j = 0; j < M; j++)
                {
                    if((i >= 1 && i < N-1) && (j >= 1 && j < M-1)) //iau doar valorile corespunzatoare pozitiilor care au vecini in matrice
                        {
                        
                        b[i][j] = c[0][0]*a[i-1][j-1] + c[0][1]*a[i-1][j]+ c[0][2]*a[i-1][j+1]+
                        c[1][0]*a[i][j-1] + c[1][1]*a[i][j] + c[1][2]*a[i][j+1]+                // realizez tranformarea inmultind fiecare valoare din matricea c
                        c[2][0]*a[i+1][j-1] + c[2][1]*a[i+1][j] + c[2][2]*a[i+1][j+1];          // cu fiecare element din submatrice, submatricea reprezentand
                        b[i][j] = (1 / 9.0) * b[i][j];                                          // matricea vecinilor, evident respectand indicii pozitiilor la inmultire
                        }                                                                       // retin valoarile intr-o matrice b si nu uit sa inmultesc cu 1/9
                    else
                    {
                        b[i][j] = a[i][j];  // daca valorile nu sunt inconjurate de vecini, le retin pur si simplu in matricea b, fara sa le schimb
                    }
                    
                }

        }
    
}

void blur(int N, int M, double a[][M], double b[][M])
    {
        double c[3][3];
        int i = 0, j = 0;
        for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)  // creez matricea blur cu valorile in conformitate cu cele din cerinta
                    {
                        c[0][0] = 1;
                        c[0][2] = 1;
                        c[2][0] = 1;
                        c[2][2] = 1;
                        c[1][1] = 4;
                        c[0][1] = 2;
                        c[2][1] = 2;
                        c[1][2] = 2;
                        c[1][0] = 2;
                    }
            }
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
                {
                    if((i >= 1 && i < N-1) && (j >= 1 && j < M-1)) // pun conditia ca valorile pe care le iau sa fie inconjurate de vecini
                        {
                        
                        b[i][j] = c[0][0]*a[i-1][j-1] + c[0][1]*a[i-1][j]+ c[0][2]*a[i-1][j+1]+
                        c[1][0]*a[i][j-1] + c[1][1]*a[i][j] + c[1][2]*a[i][j+1]+                // rezlizez acelasi procedeu ca la functia smooth
                        c[2][0]*a[i+1][j-1] + c[2][1]*a[i+1][j] + c[2][2]*a[i+1][j+1];          // nu uit sa inmultesc cu 1/16 la final
                        b[i][j] = (1 / 16.0) * b[i][j];
                        
                        }
                    else
                    {
                        b[i][j] = a[i][j];  //daca elementul nu e inconjurat de vecini, il retin in matricea b fara sa-i schimb valoarea
                    }
                    
                }
        }
    }
int main(){
    int N, M, p, i = 0, j = 0;
    char s[10];
    scanf("%d %d", &N, &M); // citesc nr. de linii si de coloane
    double a[N][M], b[N][M];
    for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)   //citesc matricea pe care o voi transforma
                scanf("%lf", &a[i][j]);
        }
    scanf("%d", &p); // citesc numarul de tranformari pe care le voi face pe matrice
        while (p != 0)
            {
                scanf("%s", s); // citesc sirul de caractere, adica filtrul pe care il voi aplica, smooth sau blur
                if (strcmp(s, "smooth") == 0) // daca ce am introdus corespunde cu sirul "smooth"
                {
                    smooth(N, M, a, b); // aplic transformarea smooth
                    for(i = 0; i < N; i++)
                        {
                            for(j = 0; j < M; j++) // copiez matricea obtinuta dupa aplicarea filtrului smooth in matricea a, ca data viitoare cand mai intru
                                a[i][j] = b[i][j];     // ori in functia smooth, ori in functia blur, sa aplic filtrul pe matricea schimbata precedent
                        }
                    p--; //decrementez p-ul, intrucat am aplicat filtrul o data
                }
                else
                {
                    blur(N, M, a, b);   // aplic transformarea blur
                    for(i = 0; i < N; i++)
                        {
                            for(j = 0; j < M; j++) // copiez matricea obtinuta dupa aplicarea filtrului blur in matricea a, ca data viitoare cand mai intru
                                a[i][j] = b[i][j];     // ori in functia smooth, ori in functia blur, sa aplic filtrul pe matricea schimbata precedent 
                        }
                    p--; // decrementez p-ul, intrucat am aplicat filtrul o data
                }
                
            }
            for(i = 0; i < N; i++)
                {
                    for(j = 0; j < M; j++)      //parcurg matricea obtinuta dupa aplicarea ultimului filtru, si o afisez.
                        printf("%8.3lf", b[i][j]); 
                    printf("\n");
                }
    return 0;
}
