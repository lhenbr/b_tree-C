#ifndef DATAFILE_H
#define DATAFILE_H


 typedef struct record {
    int NUSP;
    char  name[20];
    char  surname[20];
    char course[20];
    int grade;
} RECORD; 


FILE* start_datafile();
long insert_register(RECORD* record,FILE* data);
RECORD* search_record(long RNN,FILE* data);
RECORD* create_record();
void print_record(RECORD* record);
void close_datafile(FILE* data);
#endif