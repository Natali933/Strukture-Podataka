#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "../headeri/racuni.h"
#include "../headeri/strukture.h"
#define BUFFER_SIZE 256

/** Èitanje svih raèuna **/
void read_racuni(const char* filename, DateHead* dateHead) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Nemoguæe otvoriti datoteku %s\n", filename);
        return;
    }//otvara racuni.txt

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = '\0'; //uzima po ro red i stavlja nul terminator na kraj u svakom slucaju 

        char fullpath[BUFFER_SIZE];
        snprintf(fullpath, sizeof(fullpath), "popis/%s", buffer); //radi path za taj file koji je u popisu 

        FILE* racun = fopen(fullpath, "r");
        if (!racun) {
            printf("Nemoguæe otvoriti raèun %s\n", fullpath);
            continue;
        } // otvara taj file

        RacunHead novi_racun = malloc(sizeof(Racun)); //nova lista 
        if (!novi_racun) {
            printf("pogresna alokacija novog racuna");
            fclose(racun); //i zatvara file
            continue;
        }
        memset(novi_racun, 0, sizeof(Racun)); //postavlja sve na nulu

        if (!fgets(buffer, sizeof(buffer), racun)) {
            fclose(racun);
            free(novi_racun);
            continue;
        } //ako ne uspije pokupit zatvara file i oslobada listu

        buffer[strcspn(buffer, "\n")] = '\0'; //vraca index prvog znaka "/n" i zamijenjuje ga sa "/0"
        strncpy(novi_racun->datum, buffer, DATE_LENGTH - 1); //kopira u datum novog racuna za jedan manje zanak
        novi_racun->datum[DATE_LENGTH - 1] = '\0'; //stavlja "/0" na kraj

        while (fgets(buffer, sizeof(buffer), racun)) { //kupi artikle
            char naziv[30];
            int kolicina;
            double cijena;

            if (sscanf(buffer, "%29s %d %lf", naziv, &kolicina, &cijena) == 3) { //cita pojedini artikl i sprema
                ArtiklHead novi_artikl = malloc(sizeof(Artikl)); //nova lista za artikle tog datuma
                if (!novi_artikl) {
                    printf("nije pokupio odredeni artikl");
                    continue;
                }

                memset(novi_artikl, 0, sizeof(Artikl)); //postavlja sve na nulu
                snprintf(novi_artikl->naziv, sizeof(novi_artikl->naziv), "%s", naziv);
                novi_artikl->kolicina = kolicina;
                novi_artikl->cijena = cijena; //stavlja u listu 

                insert_artikl_sorted(&novi_racun->anext, novi_artikl); //sortira u listu
            }
        }

        fclose(racun);
        insert_racun_in_date(dateHead, novi_racun); //kad zavrsi sa racunom pojedinog datuma sprema u tu listu sa datumima
    }

    fclose(fp);
}

/*** Ubacivanje raèuna sortirano po datumima unutar dateNode ***/
void insert_racun_sorted(RacunHead* head, RacunHead novi) {
    RacunHead* curr = head;
    while (*curr && compare_dates((*curr)->datum, novi->datum) < 0)
        curr = &(*curr)->rnext;
    novi->rnext = *curr;
    *curr = novi;
}


/*** Ubacivanje raèuna u listu datuma ***/
void insert_racun_in_date(DateHead* head, RacunHead racun) {
    int y, m, d;
    if (sscanf(racun->datum, "%d-%d-%d", &y, &m, &d) != 3) {
        printf("Greška: format datuma nije ispravan (%s)\n", racun->datum);
        return;
    }

    DateHead* curr = head;
    while (*curr && compareDateNodes(*curr, y, m, d) < 0)
        curr = &(*curr)->next;

    if (*curr && compareDateNodes(*curr, y, m, d) == 0) {
        insert_racun_sorted(&(*curr)->racuni, racun);
    }
    else {
        DateHead novi = malloc(sizeof(DateNode));
        if (!novi) {
            printf("Greška: Ne mogu alocirati memoriju za novi datum!\n");
            return;
        }

        memset(novi, 0, sizeof(DateNode));
        novi->year = y;
        novi->month = m;
        novi->day = d;
        novi->racuni = NULL;
        insert_racun_sorted(&novi->racuni, racun);

        novi->next = *curr;
        *curr = novi;
    }
}