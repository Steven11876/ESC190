#include <stdio.h>
#include <stdlib.h>
#include "seamcarving.h"

#include <math.h>
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    create_img(grad, im->height, im->width);
    for(int i = 0; i < im->height; i++){
        for(int j = 0; j < im->width; j++){
            double xunder, xover, yunder, yover;
            if (i==0){
                yunder=im->height-1;
            }
            else {
                yunder = i-1;
            }
            if (j==0){
                xunder = im->width-1;
            }
            else{
                xunder = j-1;
            }
            if (i==im->height-1){
                yover = 0;
            }
            else{
                yover = i+1;
            }
            if (j==im->width-1){
                xover=0;
            }
            else{
                xover=j+1;
            }
            double c = get_pixel(im, i, xover, 0)- get_pixel(im, i, xunder, 0);
            double cc = get_pixel(im, i, xover, 1)- get_pixel(im, i, xunder, 1);
            double ccc = get_pixel(im, i, xover, 2)- get_pixel(im, i, xunder, 2);
            double d = get_pixel(im, yover, j, 0)- get_pixel(im, yunder, j, 0);
            double dd = get_pixel(im, yover, j, 1)- get_pixel(im, yunder, j, 1);
            double ddd = get_pixel(im, yover, j, 2)- get_pixel(im, yunder, j, 2);
            double e = c*c+cc*cc+ccc*ccc+d*d+dd*dd+ddd*ddd;
            double f = sqrt(e)/10;
            uint8_t cha = f;

            set_pixel(*grad, i, j, cha, cha, cha);
            //printf("%d \n", get_pixel(*grad, i, j, 0));
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    *best_arr= (double *)malloc(grad->height*grad->width*sizeof(double));

    for(int i = 0; i < grad->height; i++){
        for(int j = 0; j < grad->width; j++){
            if (i==0){
                //printf("%d \n", get_pixel(grad, i, j, 0));
                (*best_arr)[j]= get_pixel(grad, i, j, 0);
                //printf("%lf", (*best_arr)[j]);
            }
            else if (j==0){
                (*best_arr)[i*grad->width+j]= MIN((*best_arr)[(i-1)*grad->width+j], (*best_arr)[(i-1)*grad->width+j+1])+get_pixel(grad, i, j, 0);
            }
            else if (j== grad->width-1){
                (*best_arr)[i*grad->width+j]= MIN((*best_arr)[(i-1)*grad->width+j], (*best_arr)[(i-1)*grad->width+j-1])+get_pixel(grad, i, j, 0);
            }
            else {
                (*best_arr)[i*grad->width+j]= MIN((*best_arr)[(i-1)*grad->width+j-1], MIN((*best_arr)[(i-1)*grad->width+j], (*best_arr)[(i-1)*grad->width+j+1]))+ get_pixel(grad, i, j, 0);
            }
            //printf("%lf \n", (*best_arr)[i*grad->width+j]);
        }
        //printf("hi");
    }
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int*)malloc(height*sizeof(int));
    int compar = INFINITY;
    int saved;
    for (int a = 0; a< width; a++){
        if (best[height*width-1-a]< compar){
            compar= best[height*width-1-a];
            (*path)[height-1]= width-1-a;
        }}
    //printf ("%d \n", (*path)[height-1]);
    for (int b= 0; b<height-1; b++){
            //printf("%d", (*path)[height-1-b]);
    int shit;
    shit = (*path)[height-1-b];
        if ((*path)[height-1-b]==0){
            if (best[height*width-(b+2)*width] < best[height*width+1-(b+2)*width]){
            (*path)[height-2-b] =0;
            }
            else{
                (*path)[height-2-b] =1;
            }
        }
        if ((*path)[height-1-b] == width-1){
            if (best[height*width-1-(b+1)*width] < best[height*width-2-(b+1)*width]){
            (*path)[height-2-b] = width-1;
            }
            else{
            (*path)[height-2-b] = width-2;
            }
        }
        else{
            if ((best[height*width-1+shit+1-(2+b)*width+1]<=(best[height*width-1+shit+1-(2+b)*width-1]))&&(best[height*width-1+shit+1-(2+b)*width+1]<=(best[height*width-1+shit+1-(2+b)*width]))){
                (*path)[height-2-b] = (*path)[height-1-b] +1;
            }
            else if ((best[height*width-1+shit+1-(2+b)*width-1]<=(best[height*width-1+shit+1-(2+b)*width+1]))&&(best[height*width-1+shit+1-(2+b)*width-1]<=(best[height*width-1+shit+1-(2+b)*width]))){
                (*path)[height-2-b] = (*path)[height-1-b] -1;
            }
            else{
                (*path)[height-2-b] = (*path)[height-1-b];
            }

        }

        //printf ("%int \n", (*path)[height-2-b]);
    }
    }


void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    create_img(dest, src->height, src->width-1);
    for(int i = 0; i < (*dest)->height; i++){
        int trigger = 0;
        for(int j = 0; j < src->width; j++){
            if (j != path[i] && trigger==0){
                set_pixel(*dest, i, j, get_pixel(src, i, j, 0), get_pixel(src, i, j, 1), get_pixel(src, i, j, 2));
            }
            if (j == path[i]){
                trigger=1;
            }
            if (j != path[i] && trigger==1){
                set_pixel(*dest, i, j-1, get_pixel(src, i, j, 0), get_pixel(src, i, j, 1), get_pixel(src, i, j, 2));
            }
        }

    }
}





/*
int main() {
    struct rgb_img *im;
    struct rgb_img *grad;
    double* best;
  read_in_img(&im, "pres.bin");
  printf("hi");
  calc_energy(im,  &grad);
    dynamic_seam(grad, &best);
}
struct rgb_img *location;
    char names[5][11] = {"image1.bin", "image2.bin", "image3.bin", "image4.bin", "image5.bin"};
    for(int inc = 0; inc < 5; inc++){
        read_in_img(&location, "pres.bin");
//        printf("%d",location->raster[9]);
        for(int i = 0; i < location->height; i++){
            for(int j = 0; j < location->width; j++){
//                printf("%d\t", i);
//                printf("%d\n", j);
                int r = (get_pixel(location, i, j, 0) * ((inc*0.2)+0.3))/1;
                if (r>255){
                    r = 255;
                }
//                printf("**r = %d\n", r);
                int g = (get_pixel(location, i, j, 1) * ((inc*0.2)+0.3))/1;
                if (g>255){
                    g = 255;
                }
//                printf("**g = %d\n", g);
                int b = (get_pixel(location, i, j, 2) * ((inc*0.2)+0.3))/1;
                if (b>255){
                    b = 255;
                }
//                printf("**b = %d\n", b);
                set_pixel(location, i, j, r, g, b);
            }
        }
        write_img(location, names[inc]);
    }
    return 0;
*/