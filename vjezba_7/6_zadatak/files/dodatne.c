#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "../headeri/dodatne.h"
#include "../headeri/strukture.h"

void najvise_zarada(DateHead head) {
    if (!head) {
        printf("Nema uèitanih raèuna.\n");
        return;
    }

    Artikl* suma = NULL;  // privremena lista da prati ukupnu zaradu po artiklu

    DateNode* d = head;
    while (d) {
        RacunHead r = d->racuni;
        while (r) {
            ArtiklHead a = r->anext;
            while (a) {
                // provjeri postoji li artikl veæ u listi suma
                Artikl* temp = suma;
                while (temp && strcmp(temp->naziv, a->naziv) != 0)
                    temp = temp->next;

                if (temp) {
                    // ako postoji dodaj ukupnu zaradu
                    temp->cijena += a->kolicina * a->cijena;
                }
                else {
                    //ako ne postoji stvori novi èvor
                    Artikl* novi = malloc(sizeof(Artikl));
                    if (!novi) {
                        printf("Nemoguæe alocirati memoriju.\n");
                        // oslobodi listu prije nego se vratis
                        Artikl* t = suma;
                        while (t) {
                            Artikl* tmp = t;
                            t = t->next;
                            free(tmp);
                        }
                        return;
                    }

                    strncpy(novi->naziv, a->naziv, sizeof(novi->naziv) - 1);
                    novi->naziv[sizeof(novi->naziv) - 1] = '\0';
                    novi->cijena = a->kolicina * a->cijena;  // ukupna zarada
                    novi->kolicina = 0;  // kolièina se ne koristi ovdje pa ne znan ocu li maknit
                    novi->next = suma;
                    suma = novi;
                }

                a = a->next;
            }
            r = r->rnext;
        }
        d = d->next;
    }

    if (!suma) {
        printf("Nema artikala.\n");
        return;
    }

   
    Artikl* max = suma;  // pronaði artikl s najveæom zaradom
    Artikl* temp = suma->next;
    while (temp) {
        if (temp->cijena > max->cijena)
            max = temp;
        temp = temp->next;
    }

    printf("\nNajvise zaradio artikl: %s, ukupno: %.2lf\n", max->naziv, max->cijena);

   
    temp = suma;
    while (temp) {
        Artikl* t = temp;
        temp = temp->next;
        free(t);  // oslobodi listu suma
    }
}

/* Najjeftiniji i najskuplji artikl */
void najjeftiniji_najskuplji(DateHead head) {
    if (!head) {
        printf("Nema uèitanih raèuna.\n");
        return;
    }

    ArtiklHead svi_artikli = NULL;
    DateHead curr = head;

    
    while (curr) { // Skupi sve razlicite artikle
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

    if (!svi_artikli) {
        printf("Nema artikala.\n");
        return;
    }

    
    ArtiklHead najjeftiniji = svi_artikli; // naði najjeftiniji i najskuplji
    ArtiklHead najskuplji = svi_artikli;
    ArtiklHead temp = svi_artikli->next;

    while (temp) {
        if (temp->cijena < najjeftiniji->cijena)
            najjeftiniji = temp;
        if (temp->cijena > najskuplji->cijena)
            najskuplji = temp;
        temp = temp->next;
    }

    printf("\nNajjeftiniji artikl: %s - %.2lf\n", najjeftiniji->naziv, najjeftiniji->cijena);
    printf("Najskuplji artikl: %s - %.2lf\n", najskuplji->naziv, najskuplji->cijena);

    
    while (svi_artikli) {
        ArtiklHead t = svi_artikli;
        svi_artikli = svi_artikli->next;
        free(t); // Oslobodi memoriju
    }
}

/* Artikl koji je najviše prodan po kolièini */
void najvise_prodan(DateHead head) {
    if (!head) {
        printf("Nema uèitanih raèuna.\n");
        return;
    }

    Artikl* suma = NULL;

    DateHead d = head;
    while (d) {
        RacunHead r = d->racuni;
        while (r) {
            ArtiklHead a = r->anext;
            while (a) {
                Artikl* temp = suma;
                while (temp && strcmp(temp->naziv, a->naziv) != 0)
                    temp = temp->next;

                if (temp) {
                    temp->kolicina += a->kolicina;
                }
                else {
                    Artikl* novi = malloc(sizeof(Artikl));
                    if (!novi) {
                        printf("Nemoguæe alocirati memoriju.\n");
                        Artikl* t = suma;
                        while (t) {
                            Artikl* tmp = t;
                            t = t->next;
                            free(tmp);
                        }
                        return;
                    }

                    strncpy(novi->naziv, a->naziv, sizeof(novi->naziv) - 1);
                    novi->naziv[sizeof(novi->naziv) - 1] = '\0';
                    novi->kolicina = a->kolicina;
                    novi->cijena = 0;
                    novi->next = suma;
                    suma = novi;
                }

                a = a->next;
            }
            r = r->rnext;
        }
        d = d->next;
    }

    if (!suma) {
        printf("Nema artikala.\n");
        return;
    }

    Artikl* max = suma;
    Artikl* temp = suma->next;
    while (temp) {
        if (temp->kolicina > max->kolicina)
            max = temp;
        temp = temp->next;
    }

    printf("\nNajvise prodan artikl: %s - %d komada\n", max->naziv, max->kolicina);

    temp = suma;
    while (temp) {
        Artikl* t = temp;
        temp = temp->next;
        free(t);
    }
}

/* Najskuplji raèun */
void najskuplji_racun(DateHead head) {
    if (!head) {
        printf("Nema uèitanih raèuna.\n");
        return;
    }

    double max_iznos = 0.0;
    char max_datum[DATE_LENGTH] = "";
    int ima_racuna = 0;

    DateHead d = head;
    while (d) {
        RacunHead r = d->racuni;
        while (r) {
            ima_racuna = 1;
            double iznos_racuna = 0.0;

            ArtiklHead a = r->anext;
            while (a) {
                iznos_racuna += a->kolicina * a->cijena;
                a = a->next;
            }

            if (iznos_racuna > max_iznos) {
                max_iznos = iznos_racuna;
                strncpy(max_datum, r->datum, DATE_LENGTH - 1);
                max_datum[DATE_LENGTH - 1] = '\0';
            }

            r = r->rnext;
        }
        d = d->next;
    }

    if (!ima_racuna) {
        printf("Nema raèuna.\n");
        return;
    }

    printf("\nNajskuplji racun: %s - %.2lf\n", max_datum, max_iznos);
}
