#include <stdio.h>
#include <string.h>

    //functie prin care aflu distanta din alfabet raportata la litera "a" a literei din parola corespunzatoare indexului literei din cheie.
int gasire(int index, char parola[], char alfabet[])
{
        for(int k = 0; k < strlen(parola); k++)
            {
                for(int p = 0; p < strlen(alfabet); p++)
                    {
                        if((parola[index] == alfabet[p]) && (k == index))
                            {
                                index = p;
                                return index;
                            }
                    }
            }
}

    //functie prin care verific daca in cheie si parola exista doar litere
int verif_litere(char s[])
{
    int p; 
    p = strlen(s);
    for(int i = 0; i < p; i++)
        {
            if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
                continue;
            else
                return 0;
        }
    return 1;
}
    //functie prin care multplic astfel incat sa aiba acelasi nr. de litere cu parola, si copiez noua cheie in cheieV2
void multiplicare(char cheie[], char parola[], char cheieV2[])
{   
    int p = strlen(cheie), contor = 0, q = strlen(parola);
    for(int i = 0; i < p; i++)
        cheieV2[contor++] = cheie[i];
    int j = 0;
    while(p != q)
        {
            cheieV2[contor++] = cheieV2[j];
                j++;
                p++;
            if(j == strlen(cheie))
                j = 0;
        }
        
}
    //functie prin care micsorez numarul de litere al cheii, astfel incat sa aiba acelasi nr. de litere cu parola, si copiez noua cheie in cheieV2
void demultiplicare(char cheie[], char text[], char cheieV2[])
{
    int nrcuv_text = strlen(text);
    int ok = 0;
    int j = 0;
    while(ok < nrcuv_text)
        {
            cheieV2[ok++] = cheie[j];
            j++;
        }
}

int main(){
    int index = 0, mutare = 0;
    int contor = 0;
    char cheie[13500], parola[13500], cheieV2[13500], parola_criptata[13500];
    scanf("%s", cheie);
    scanf("%s", parola);
    char alfabet[100] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            //verific daca parola si cheia contin doar litere
    if((verif_litere(cheie) == 0) || (verif_litere(parola) == 0))
        {
            printf("INVALID\n");
            return 0;
        }
    else
        {    //compar nr. de litere si aplic functia necesara
            if(strlen(cheie) <= strlen(parola))
                multiplicare(cheie, parola, cheieV2);
            else
            {
                    demultiplicare(cheie, parola, cheieV2);
            }
            for(int i = 0; i < strlen(cheieV2); i++)    //parcurg cheia
            {
                for(int j = 0; j < strlen(alfabet); j++) //parcurg alfabetul
                {
                    if(cheieV2[i] == alfabet[j])
                    {   
                        //retin distanta literei din parola fata de "a"
                        mutare = gasire(i, parola, alfabet);
                        index = mutare + j; //distanta cu care voi "merge" in alfabet ca sa schimb litera veche din parola cu cea noua
                        if(index > 50)
                        {
                            //daca indexul depaseste pozitiile literelor din alfabet, fac modulo ca sa am certitudinea ca va lua o valoare corespunzatoare unui index din string-ul alfabet
                            index = index % 52; 
                            parola_criptata[contor++] = alfabet[index]; //introduc element cu element parola criptata intr-un nou string
                        }
                        else
                        {   
                            //stiu cu certitudine ca index o sa fie una dintre literele din alfabet fara sa mai am de facut vreo impartire
                            parola_criptata[contor++] = alfabet[index];
                        }
                    }
                else continue;
                }
            }
        
        parola_criptata[contor] = '\0';
            for(int i = 0; i < strlen(parola_criptata); i++)
                printf("%c", parola_criptata[i]);
        printf("\n");
    }
}
