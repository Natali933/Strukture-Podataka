#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "../headeri/datumi.h"
#include "../headeri/strukture.h"

/*** Usporedba string datuma YYYY-MM-DD ***/
int compare_dates(const char* date1, const char* date2) {
    int y1, m1, d1, y2, m2, d2;
    if (sscanf(date1, "%d-%d-%d", &y1, &m1, &d1) != 3) return 0;
    if (sscanf(date2, "%d-%d-%d", &y2, &m2, &d2) != 3) return 0;

    if (y1 != y2) return y1 - y2;
    if (m1 != m2) return m1 - m2;
    return d1 - d2;
}

/*** Usporedba dateNode sa odreðenim datumom ***/
int compareDateNodes(DateNode* a, int y, int m, int d) {
    if (a->year != y) return a->year - y;
    if (a->month != m) return a->month - m;
    return a->day - d;
}

/*** Pregled po datumu ***/
void pregled_po_datumu(DateHead head, const char* naziv, const char* start_date, const char* end_date) {
    int y1, m1, d1, y2, m2, d2;
    if (sscanf(start_date, "%d-%d-%d", &y1, &m1, &d1) != 3) {
        printf("Greška: format datuma nije ispravan u funckiji pregled po datumu");
        return;
    }

    if (sscanf(end_date, "%d-%d-%d", &y2, &m2, &d2) != 3) {
        printf("Greška: format datuma nije ispravan u funckiji pregled po datumu");
        return;
    }

    DateHead curr = head;
    int total_quantity = 0;
    double total_amount = 0.0;

    while (curr) {
        if (compareDateNodes(curr, y1, m1, d1) >= 0 && compareDateNodes(curr, y2, m2, d2) <= 0) {
            RacunHead r = curr->racuni;
            while (r) {
                ArtiklHead a = r->anext;
                while (a) {
                    if (strcmp(a->naziv, naziv) == 0) {
                        total_quantity += a->kolicina;
                        total_amount += a->kolicina * a->cijena;
                    }
                    a = a->next;
                }
                r = r->rnext;
            }
        }
        curr = curr->next;
    }

    printf("\n===== REZULTAT =====\n");
    printf("Artikl: %s\n", naziv);
    printf("Razdoblje: %s do %s\n", start_date, end_date);
    printf("Ukupna kolièina: %d\n", total_quantity);
    printf("Ukupna zarada od %s: %.2lf\n",naziv, total_amount);
}



/*** Oslobaðanje memorije ***/
void free_all_dates(DateHead head) {
    while (head) {
        DateHead d = head;
        head = head->next;

        RacunHead r = d->racuni;
        while (r) {
            RacunHead t = r;
            r = r->rnext;

            ArtiklHead a = t->anext;
            while (a) {
                ArtiklHead ta = a;
                a = a->next;
                free(ta);
            }
            free(t);
        }
        free(d);
    }
}
