#ifndef DATUMI_H
#define DATUMI_H
#include "strukture.h"


int compare_dates(const char* date1, const char* date2);
int compareDateNodes(DateNode* a, int y, int m, int d);
void pregled_po_datumu(DateHead head, const char* naziv, const char* start_date, const char* end_date);
void free_all_dates(DateHead head);

#endif DATUMI_H