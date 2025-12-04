#ifndef RACUNI_H
#define RACUNI_H
#include "strukture.h"

void read_racuni(const char* filename, DateHead* dateHead);
void insert_racun_sorted(RacunHead* head, RacunHead novi);
void insert_racun_in_date(DateHead* head, RacunHead racun);

#endif RACUNI_H