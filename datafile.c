#include "datafile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE* start_datafile(){
FILE* data;
do
    {
        data = fopen("index.dat", "r+");
        if (data == NULL)
        {
            data = fopen("index.dat  ", "w");      /* Se nao existir, cria o arquivo de dados */
            fclose(data);
        }
    } while (data == NULL);
return data;
}
long insert_register(RECORD* record,FILE* data)
{
long RNN;
fseek(data,0,SEEK_END);
RNN = ftell(data)/(sizeof(RECORD));//possivel erro 
fwrite(record,sizeof(RECORD),1,data);
free(record);
return RNN;
}
RECORD* search_record(long RNN,FILE* data){
    RECORD* record = malloc(sizeof(RECORD));
    fseek(data,RNN,SEEK_SET);
    fread(record,sizeof(RECORD),1,data);
    return record;
}
RECORD* create_record(){
    RECORD* record = malloc(sizeof(RECORD));
    
    printf("digite o Nusp: \n");
    scanf(" %d", &record->NUSP);
    
    printf("%s \ndigite o Nome: \n");
    scanf(" %[^\n]s", record->name);
    
    printf("digite o Sobrenome: \n");
    scanf(" %[^\n]s", record->surname);
    
    printf("digite o Curso: \n");
    scanf(" %[^\n]s", record->course);
    
    printf("digite a Nota \n");
    scanf(" %d", &record->grade);

    return record;
}
void print_record(RECORD* record){
fprintf("\nNUSP: %d\n",record->NUSP);
fprintf("NOME: %s\n",record->name);
fprintf("SOBRENOME: %s\n",record->surname);
fprintf("CURSO: %s\n",record->course);
fprintf("NOTA: %d\n",record->grade);
free(record);
}
void close_datafile(FILE* data){
    fclose(data);
}