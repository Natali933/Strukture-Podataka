#ifndef  STRUKTURE_H
#define STRUKTURE_H

#define DATE_LENGTH 20

/*** STRUKTURE ***/
typedef struct _artikl* ArtiklHead;
typedef struct _artikl {
    char naziv[30];
    int kolicina;
    double cijena;
    ArtiklHead next;
} Artikl;

typedef struct _racun* RacunHead;
typedef struct _racun {
    char datum[DATE_LENGTH];
    ArtiklHead anext;
    RacunHead rnext;
} Racun;

typedef struct _dateNode* DateHead;
typedef struct _dateNode {
    int year;
    int month;
    int day;
    RacunHead racuni;   // lista svih raèuna za taj datum
    DateHead next;
} DateNode;


#endif //  STRUKTURE_H
