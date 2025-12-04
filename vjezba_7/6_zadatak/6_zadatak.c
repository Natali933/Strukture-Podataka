#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headeri/strukture.h"
#include "headeri/racuni.h"
#include "headeri/artikli.h"
#include "headeri/datumi.h"
#include "headeri/dodatne.h"

int main() {
    DateHead dateHead = NULL;

    read_racuni("racuni.txt", &dateHead);

    char naziv[30];
    printf("Dostupni artikli:\n");
    ispis_artikli(dateHead);

    printf("\nUnesite naziv artikla: ");
    if (scanf(" %29s", naziv) != 1) {
        printf("Neispravan unos.\n");
        return 1;
    }

    char start_date[DATE_LENGTH];
    char end_date[DATE_LENGTH];

    printf("Unesite datum OD (YYYY-MM-DD): ");
    if (scanf(" %19s", start_date) != 1) {
        printf("Neispravan datum.\n");
        return 1;
    }

    printf("Unesite datum DO (YYYY-MM-DD): ");
    if (scanf(" %19s", end_date) != 1) {
        printf("Neispravan datum.\n");
        return 1;
    }

    pregled_po_datumu(dateHead, naziv, start_date, end_date);

    najvise_zarada(dateHead);
    najjeftiniji_najskuplji(dateHead);
    najvise_prodan(dateHead);
    najskuplji_racun(dateHead);

    free_all_dates(dateHead);
    return 0;
}

