#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "../headeri/artikli.h"
#include "../headeri/strukture.h"

/** Ubacivanje artikla sortirano po nazivu **/
void insert_artikl_sorted(ArtiklHead* head, ArtiklHead novi) {
    ArtiklHead* curr = head; //pokazivac na pokazivac da se moze mijenjati glava liste
    while (*curr && strcmp((*curr)->naziv, novi->naziv) < 0)
        curr = &(*curr)->next; //pokazivac na pokazivac koji vodi na sljedeci cvor 

    if (*curr && strcmp((*curr)->naziv, novi->naziv) == 0) {
        (*curr)->kolicina += novi->kolicina;
        free(novi);
    }
    else {
        novi->next = *curr;
        *curr = novi;
    }
}

/* Ispis svih artikala */
void ispis_artikli(DateHead head) {
    ArtiklHead svi_artikli = NULL;
    DateHead curr = head;
    while (curr) {
        RacunHead r = curr->racuni;
        while (r) {
            ArtiklHead a = r->anext;
            while (a) {
                ArtiklHead temp = svi_artikli;
                int found = 0;
                while (temp) {
                    if (strcmp(temp->naziv, a->naziv) == 0) {
                        found = 1;
                        break;
                    }
                    temp = temp->next;
                }
                if (!found) {
                    ArtiklHead novi = malloc(sizeof(Artikl));
                    if (novi) {
                        memset(novi, 0, sizeof(Artikl));
                        strncpy(novi->naziv, a->naziv, sizeof(novi->naziv) - 1);
                        novi->naziv[sizeof(novi->naziv) - 1] = '\0';
                        novi->cijena = a->cijena;
                        novi->next = svi_artikli;
                        svi_artikli = novi;
                    }
                }
                a = a->next;
            }
            r = r->rnext;
        }
        curr = curr->next;
    }

    ArtiklHead temp = svi_artikli;
    while (temp) {
        printf("%-20s\t%.2lf\n", temp->naziv, temp->cijena);
        temp = temp->next;
    }

    while (svi_artikli) {
        ArtiklHead t = svi_artikli;
        svi_artikli = svi_artikli->next;
        free(t);
    }
}