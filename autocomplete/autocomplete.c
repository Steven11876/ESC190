#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
};

int compare(const void *s1, const void *s2){
    struct term *e1 = (struct term *)s1;
    struct term *e2 = (struct term *)s2;
    return strcmp(e1->term, e2->term);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    int lines;
    double weight;
    char text[200];
    fscanf(fp, "%d\n", &lines);
    *pnterms = lines;
    *terms = (struct term *) calloc(lines, sizeof(struct term));

    for(int i = 0; i < lines; i++) {
        fscanf(fp, "%lf %[^\n]s", &weight, text);
        (*terms)[i].weight = weight;
        strcpy((*terms)[i].term, text);
    }
    qsort(*terms, lines, sizeof(struct term), compare);

    fclose(fp);
}


int mstrcmp(char str1[], char str2[]) {
    if (!*str1) {
        return 0;
    }
    if(*str1 && (*str1 == *str2)) {
        return mstrcmp(str1+1, str2+1);
    }
    return *(const unsigned char*)str1-*(const unsigned char*)str2;
}

int down (struct term *terms, int mid, char *substr){
    if (mid <= 0 || mstrcmp(substr, terms[mid-1].term) != 0){

        return mid;

    }
    else{
        return down(terms, mid-1, substr);
    }
}

int up (struct term *terms, int mid, char *substr, int nterms){
    if (mid >= nterms || mstrcmp(substr, terms[mid+1].term) != 0){

        return mid;
    }
    else{
        return up(terms, mid+1, substr, nterms);
    }
}

int reals(struct term *terms, int starter, int nterms, char *substr){
    if (starter==nterms || starter+1==nterms){
        if (mstrcmp(substr, terms[starter].term) == 0){
            return starter;}
        if (mstrcmp(substr, terms[starter+1].term) == 0){
            return starter+1;}
        else{
            return -1;
    }}
    if (starter <= nterms){
        int mid;
        mid = (nterms + starter+1)/2;

        if (mstrcmp(substr, terms[mid].term) == 0){
            return up(terms, mid, substr, nterms);
        }
        if (mstrcmp(substr, terms[mid].term) < 0){
            return reals(terms, starter, mid, substr);
        }
        if (mstrcmp(substr, terms[mid].term) > 0){
            return reals(terms, mid, nterms, substr);
        }
    }
    else{
        return -1;
    }
}

int real(struct term *terms, int starter, int nterms, char *substr){

    if (starter==nterms || starter+1==nterms){
        if (mstrcmp(substr, terms[starter].term) == 0){
            return starter;}
        if (mstrcmp(substr, terms[starter+1].term) == 0){
            return starter+1;}
        else{
            return -1;
    }}
        if (starter <= nterms){
        int mid;
        mid = (nterms + starter+1)/2;
        if (mstrcmp(substr, terms[mid].term) == 0){
            return down(terms, mid, substr);
        }
        if (mstrcmp(substr, terms[mid].term) < 0){

            return real(terms, starter, mid, substr);
        }
        if (mstrcmp(substr, terms[mid].term) > 0){

            return real(terms, mid, nterms, substr);
        }
    }
    else{
        return -1;
    }
}


int lowest_match(struct term *terms, int nterms, char *substr){
    int a= 0;
    return real(terms, a, (nterms-1), substr);
}

int highest_match(struct term *terms, int nterms, char *substr){
    int a= 0;
    return reals(terms, a, (nterms-1), substr);
}

int compares(const void *s1, const void *s2){
    if (((((struct term *)s2) -> weight) - (((struct term *)s1) -> weight))>=0){
        return 1;}
    if (((((struct term *)s2) -> weight) - (((struct term *)s1) -> weight))<=0){
        return -1;
    }
    else{
        return 1;
    }
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){

    int a, b, c;
    a = lowest_match(terms, nterms, substr);
    b = highest_match(terms, nterms, substr);
    if ((a==b) & (a==-1)){
        c = 0;
        *n_answer=0;
    }
    else{
        *n_answer= b-a+1;
        c = b-a+1;
    }
    *answer = (struct term *) malloc(sizeof(struct term)* (c));
    for (int i =0; i < c; i++) {
        (*answer)[i].weight = (terms)[a+i].weight;
        strcpy((*answer)[i].term, terms[a+i].term);}
    qsort(*answer, c, sizeof(struct term), compares);
}

int main(void)
{
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities.txt");
    lowest_match(terms, nterms, "Tor");
    highest_match(terms, nterms, "Tor");
    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "Tor");
    //free allocated blocks here -- not required for the project, but good practice
    return 0;
}
