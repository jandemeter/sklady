#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int wp = 0;
    int ip = 0;
    int np = 0;
    int ep = 0;
    int tp = 0;
    int pp = 0;
    int Wp = 0;

    char wparameter[MAX_NAME];
    char iparameter[MAX_NAME];
    double nparameter = 0.0;
    double eparameter = 0.0;
    double vzdialenost = 100.0;
    int poradie_skladu = 0;

    char tparameter[MAX_NAME];
    int pparameter = 0;
    
    int poradie = 1;
    int opt;
    char* optstring = ":w:i:n:e:t:p:W";
    
    //zistia sa zapnute spinace
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'w':
                strcpy(wparameter,optarg);
                wp++;
                break;
            case 'i':
                strcpy(iparameter,optarg);
                ip++;
                break;
            case 'n':
                np++;
                for(int i=0;i<strlen(optarg);i++){
                    char a = optarg[i];
                    if((a != '.') && (isdigit(a) == 0 )){
                        return 4;
                    }
                }
                sscanf(optarg, "%lf", &nparameter);
                break;
            case 'e':
                ep++;
                for(int i=0;i<strlen(optarg);i++){
                    char a = optarg[i];
                    if((a != '.') && (isdigit(a) == 0 )){
                        return 4;
                    }
                }
                sscanf(optarg, "%lf", &eparameter);
                break;
        
            case 't':
                tp++;
                strcpy(tparameter,optarg);
                break;
            case 'p':
                pp++;
                sscanf(optarg, "%d", &pparameter);
                break;
            case 'W':
                Wp++;
                break;
            case '?':
                return 1;
            case ':':
                return 2;

        }
    }
    
    //chybove situacie
    if(np==1 && ep==0 || np==0 && ep==1){
        return 3;
    }
    if(nparameter>90 || nparameter<-90 || eparameter>180 || eparameter<-180){
        return 4;
    }

    GPS vstupgps;
    vstupgps.lat = nparameter;
    vstupgps.lon = eparameter;
    for(int i = 0; i < DB_NUM;i++){
        double d = distance(db[i].gps,vstupgps);
        if (d<vzdialenost){
            vzdialenost = d;
            poradie_skladu = i;
        }
    } 
    
    
    //program podla zapnuteho spinaca alebo kombinacie zapnutych spinacov vykona vypis 
    if(Wp==1){   //W prepinac    
        if(wp==1){  //prepinac na mesto
                for(int i=0;i<DB_NUM;i++){
                    if(strcmp(db[i].name,wparameter)==0){
                        printf("%s %.3lf %.3lf %d :",db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                        for(int j=0;j<db[i].n;j++){
                            if(tp == 1){
                                if(strcmp(db[i].items[j].name,tparameter)== 0){
                                    printf("\n%d. %s %d",poradie, db[i].items[j].name, db[i].items[j].price);
                                }
                            }
                            else if(pp==1){
                                if(db[i].items[j].price<=pparameter){
                                printf("\n%d. %s %d",poradie, db[i].items[j].name, db[i].items[j].price);
                                poradie++;
                                }
                            }
                            else{
                                printf("\n%d. %s %d",poradie, db[i].items[j].name, db[i].items[j].price);   
                                poradie++; 
                            }
                        }
                    }
                }
        }
        else if (ip == 1){ //prepinac na sklad podla tovaru
                for(int i=0;i<DB_NUM;i++){
                    poradie = 1;
                    for(int j=0;j<db[i].n;j++){
                        if(strcmp(db[i].items[j].name,iparameter)== 0){
                            printf("%s %.3lf %.3lf %d :\n",db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                            for(int k=0; k<db[i].n;k++){
                                if(tp==1){
                                    if(strcmp(db[i].items[k].name,tparameter) == 0){
                                        printf("%d. %s %d\n",poradie, db[i].items[k].name, db[i].items[k].price);
                                    }
                                }
                                else if(pp==1){
                                    if(db[i].items[k].price<=pparameter){
                                    printf("%d. %s %d\n",poradie, db[i].items[k].name, db[i].items[k].price);
                                    poradie++;
                                    }
                                }
                                else{
                                    printf("%d. %s %d\n",poradie, db[i].items[k].name, db[i].items[k].price);
                                    poradie++;
                                }    
                            }     
                        }
                    }
                }
        }
        else if (np==1 && ep==1){  //prepinac na mesto podla suradnic 
                printf("%s %.3lf %.3lf %d :",db[poradie_skladu].name, db[poradie_skladu].gps.lat, db[poradie_skladu].gps.lon, db[poradie_skladu].n);
                for(int j=0;j<db[poradie_skladu].n;j++){
                    if(tp == 1){
                        if(strcmp(db[poradie_skladu].items[j].name,tparameter)== 0){
                            printf("\n%d. %s %d",poradie, db[poradie_skladu].items[j].name, db[poradie_skladu].items[j].price);
                        }
                    }
                    else if(pp==1){
                        if(db[poradie_skladu].items[j].price<=pparameter){
                        printf("\n%d. %s %d",poradie, db[poradie_skladu].items[j].name, db[poradie_skladu].items[j].price);
                        poradie++;
                        }
                    }
                    else{
                        printf("\n%d. %s %d",poradie, db[poradie_skladu].items[j].name, db[poradie_skladu].items[j].price);   
                        poradie++; 
                    }
                }       
        }

        
        else{ //ziadny filter mesta ale item name 
            for(int i=0;i<DB_NUM;i++){ 
                poradie =1;
                printf("%s %.3lf %.3lf %d :\n",db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                for(int j=0;j<db[i].n;j++){
                    if(tp == 1){
                        if(strcmp(db[i].items[j].name,tparameter)== 0){
                            printf("%d. %s %d\n",poradie, db[i].items[j].name, db[i].items[j].price);
                        }
                    }
                    else if(pp==1){
                        if(db[i].items[j].price<=pparameter){
                        printf("%d. %s %d\n",poradie, db[i].items[j].name, db[i].items[j].price);
                        poradie++;
                        }
                    }
                    else{
                        printf("%d. %s %d\n",poradie, db[i].items[j].name, db[i].items[j].price);
                        poradie++; 
                    }
                }  
            }
        }
        
    }
    
    if(Wp==0){   //bez prepinaca na utpravu vypisu     

        if(wp==1){  //prepinac na mesto
            for(int i=0;i<DB_NUM;i++){
                if(strcmp(db[i].name,wparameter)==0){
                    for(int j=0;j<db[i].n;j++){
                        if(tp == 1 ){
                            if(strcmp(db[i].items[j].name,tparameter)== 0)
                                printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[j].name, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                        }
                        else if(pp ==1 ){
                            if(db[i].items[j].price<=pparameter){
                            printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[j].name, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                            poradie++;
                            }
                        }    
                        else{
                            printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[j].name, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);  
                            poradie++;                  
                        
                        }                  
                    }
                }
            }
        }    

        else if (ip ==1){ //prepinac na sklad podla tovaru
                for(int i=0;i<DB_NUM;i++){
                    for(int j=0;j<db[i].n;j++){
                        if(strcmp(db[i].items[j].name,iparameter)== 0){
                            for(int k=0; k<db[i].n;k++){
                                if(tp == 1){
                                    if(strcmp(db[i].items[k].name,tparameter) == 0){
                                        printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[k].name, db[i].items[k].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                                        poradie++;
                                    }
                                }
                                else if (pp==1){
                                    if(db[i].items[k].price<=pparameter){
                                    printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[k].name, db[i].items[k].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                                    poradie++;
                                    }
                                }
                                else{
                                    printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[k].name, db[i].items[k].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                                    poradie++;
                                }    
                            }     
                        }
                    }
                }
        }
        else if(np == 1 && ep==1){  //prepinac na mesto
            for(int j=0;j<db[poradie_skladu].n;j++){
                if(tp == 1 ){
                    if(strcmp(db[poradie_skladu].items[j].name,tparameter)== 0)
                        printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[poradie_skladu].items[j].name, db[poradie_skladu].items[j].price, db[poradie_skladu].name, db[poradie_skladu].gps.lat, db[poradie_skladu].gps.lon, db[poradie_skladu].n);
                }
                else if(pp ==1 ){
                    if(db[poradie_skladu].items[j].price<=pparameter){
                    printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[poradie_skladu].items[j].name, db[poradie_skladu].items[j].price, db[poradie_skladu].name, db[poradie_skladu].gps.lat, db[poradie_skladu].gps.lon, db[poradie_skladu].n);
                    poradie++;
                    }
                }    
                else{
                    printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[poradie_skladu].items[j].name, db[poradie_skladu].items[j].price, db[poradie_skladu].name, db[poradie_skladu].gps.lat, db[poradie_skladu].gps.lon, db[poradie_skladu].n);  
                    poradie++;                  
                        
                }                  
            }
        }  
        else{
            for(int i=0;i<DB_NUM;i++){ 
                for(int j=0;j<db[i].n;j++){
                    if(tp==1){
                        if(strcmp(db[i].items[j].name,tparameter)== 0){
                            printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[j].name, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                            poradie++;
                        }  
                    }
                    else if(pp==1){
                        if(db[i].items[j].price<=pparameter){
                        printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[j].name, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                        poradie++;
                        }
                    }
                    else{
                        printf("%d. %s %d : %s %.3lf %.3lf %d\n",poradie, db[i].items[j].name, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                        poradie++;
                    } 
                }  
            }
        }
    }
        

    return 0;
}
