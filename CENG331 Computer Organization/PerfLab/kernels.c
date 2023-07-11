/********************************************************
 * Kernels to be optimized for the Metu Ceng Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Yenikale Birlik",                     /* Team name */

    "Ali KOMURCU",             /* First member full name */
    "e2380699",                 /* First member id */

    "Emre Can KOPARAL",                         /* Second member full name (leave blank if none) */
    "e2380673",                         /* Second member id (leave blank if none) */

    "",                         /* Third member full name (leave blank if none) */
    ""                          /* Third member id (leave blank if none) */
};

/****************
 * EXPOSURE FUSION KERNEL *
 ****************/

/*******************************************************
 * Your different versions of the exposure fusion kernel go here 
 *******************************************************/




/* 
 * naive_fusion - The naive baseline version of exposure fusion
 */
char naive_fusion_descr[] = "naive_fusion: Naive baseline exposure fusion";
void naive_fusion(int dim, int *img, int *w, int *dst) {
  
    int i, j, k; 
    for(k = 0; k < 4; k++){
        for(i = 0; i < dim; i++) {
            for(j = 0; j < dim; j++) {
                    dst[i*dim+j] += w[k*dim*dim+i*dim+j] * img[k*dim*dim+i*dim+j];
            }
        }
    }
}

/* 
 * fusion - Your current working version of fusion
 * IMPORTANT: This is the version you will be graded on
 */
char fusion_descr[] = "fusion: Current working version";
void fusion(int dim, int *img, int *w, int *dst) 
{
    
    int i, koffset2,dimdim;
    
    koffset2=0;
    dimdim=dim*dim;
        
        for(i = 0; i < dimdim; i+=32) {
            
            dst[i] += w[koffset2] * img[koffset2];
            dst[i+1] += w[koffset2+1] * img[koffset2+1];
            dst[i+2] += w[koffset2+2] * img[koffset2+2];
            dst[i+3] += w[koffset2+3] * img[koffset2+3];
            dst[i+4] += w[koffset2+4] * img[koffset2+4];
            dst[i+5] += w[koffset2+5] * img[koffset2+5];
            dst[i+6] += w[koffset2+6] * img[koffset2+6];
            dst[i+7] += w[koffset2+7] * img[koffset2+7];
            dst[i+8] += w[koffset2+8] * img[koffset2+8];
            dst[i+9] += w[koffset2+9] * img[koffset2+9];
            dst[i+10] += w[koffset2+10] * img[koffset2+10];
            dst[i+11] += w[koffset2+11] * img[koffset2+11];
            dst[i+12] += w[koffset2+12] * img[koffset2+12];
            dst[i+13] += w[koffset2+13] * img[koffset2+13];
            dst[i+14] += w[koffset2+14] * img[koffset2+14];
            dst[i+15] += w[koffset2+15] * img[koffset2+15];
            dst[i+16] += w[koffset2+16] * img[koffset2+16];
            dst[i+17] += w[koffset2+17] * img[koffset2+17];
            dst[i+18] += w[koffset2+18] * img[koffset2+18];
            dst[i+19] += w[koffset2+19] * img[koffset2+19];
            dst[i+20] += w[koffset2+20] * img[koffset2+20];
            dst[i+21] += w[koffset2+21] * img[koffset2+21];
            dst[i+22] += w[koffset2+22] * img[koffset2+22];
            dst[i+23] += w[koffset2+23] * img[koffset2+23];
            dst[i+24] += w[koffset2+24] * img[koffset2+24];
            dst[i+25] += w[koffset2+25] * img[koffset2+25];
            dst[i+26] += w[koffset2+26] * img[koffset2+26];
            dst[i+27] += w[koffset2+27] * img[koffset2+27];
            dst[i+28] += w[koffset2+28] * img[koffset2+28];
            dst[i+29] += w[koffset2+29] * img[koffset2+29];
            dst[i+30] += w[koffset2+30] * img[koffset2+30];
            dst[i+31] += w[koffset2+31] * img[koffset2+31];
            koffset2+=32;
        }

        for(i = 0; i < dimdim; i+=32) {
            
            dst[i] += w[koffset2] * img[koffset2];
            dst[i+1] += w[koffset2+1] * img[koffset2+1];
            dst[i+2] += w[koffset2+2] * img[koffset2+2];
            dst[i+3] += w[koffset2+3] * img[koffset2+3];
            dst[i+4] += w[koffset2+4] * img[koffset2+4];
            dst[i+5] += w[koffset2+5] * img[koffset2+5];
            dst[i+6] += w[koffset2+6] * img[koffset2+6];
            dst[i+7] += w[koffset2+7] * img[koffset2+7];
            dst[i+8] += w[koffset2+8] * img[koffset2+8];
            dst[i+9] += w[koffset2+9] * img[koffset2+9];
            dst[i+10] += w[koffset2+10] * img[koffset2+10];
            dst[i+11] += w[koffset2+11] * img[koffset2+11];
            dst[i+12] += w[koffset2+12] * img[koffset2+12];
            dst[i+13] += w[koffset2+13] * img[koffset2+13];
            dst[i+14] += w[koffset2+14] * img[koffset2+14];
            dst[i+15] += w[koffset2+15] * img[koffset2+15];
            dst[i+16] += w[koffset2+16] * img[koffset2+16];
            dst[i+17] += w[koffset2+17] * img[koffset2+17];
            dst[i+18] += w[koffset2+18] * img[koffset2+18];
            dst[i+19] += w[koffset2+19] * img[koffset2+19];
            dst[i+20] += w[koffset2+20] * img[koffset2+20];
            dst[i+21] += w[koffset2+21] * img[koffset2+21];
            dst[i+22] += w[koffset2+22] * img[koffset2+22];
            dst[i+23] += w[koffset2+23] * img[koffset2+23];
            dst[i+24] += w[koffset2+24] * img[koffset2+24];
            dst[i+25] += w[koffset2+25] * img[koffset2+25];
            dst[i+26] += w[koffset2+26] * img[koffset2+26];
            dst[i+27] += w[koffset2+27] * img[koffset2+27];
            dst[i+28] += w[koffset2+28] * img[koffset2+28];
            dst[i+29] += w[koffset2+29] * img[koffset2+29];
            dst[i+30] += w[koffset2+30] * img[koffset2+30];
            dst[i+31] += w[koffset2+31] * img[koffset2+31];
            koffset2+=32;
        }

        for(i = 0; i < dimdim; i+=32) {
            
            dst[i] += w[koffset2] * img[koffset2];
            dst[i+1] += w[koffset2+1] * img[koffset2+1];
            dst[i+2] += w[koffset2+2] * img[koffset2+2];
            dst[i+3] += w[koffset2+3] * img[koffset2+3];
            dst[i+4] += w[koffset2+4] * img[koffset2+4];
            dst[i+5] += w[koffset2+5] * img[koffset2+5];
            dst[i+6] += w[koffset2+6] * img[koffset2+6];
            dst[i+7] += w[koffset2+7] * img[koffset2+7];
            dst[i+8] += w[koffset2+8] * img[koffset2+8];
            dst[i+9] += w[koffset2+9] * img[koffset2+9];
            dst[i+10] += w[koffset2+10] * img[koffset2+10];
            dst[i+11] += w[koffset2+11] * img[koffset2+11];
            dst[i+12] += w[koffset2+12] * img[koffset2+12];
            dst[i+13] += w[koffset2+13] * img[koffset2+13];
            dst[i+14] += w[koffset2+14] * img[koffset2+14];
            dst[i+15] += w[koffset2+15] * img[koffset2+15];
            dst[i+16] += w[koffset2+16] * img[koffset2+16];
            dst[i+17] += w[koffset2+17] * img[koffset2+17];
            dst[i+18] += w[koffset2+18] * img[koffset2+18];
            dst[i+19] += w[koffset2+19] * img[koffset2+19];
            dst[i+20] += w[koffset2+20] * img[koffset2+20];
            dst[i+21] += w[koffset2+21] * img[koffset2+21];
            dst[i+22] += w[koffset2+22] * img[koffset2+22];
            dst[i+23] += w[koffset2+23] * img[koffset2+23];
            dst[i+24] += w[koffset2+24] * img[koffset2+24];
            dst[i+25] += w[koffset2+25] * img[koffset2+25];
            dst[i+26] += w[koffset2+26] * img[koffset2+26];
            dst[i+27] += w[koffset2+27] * img[koffset2+27];
            dst[i+28] += w[koffset2+28] * img[koffset2+28];
            dst[i+29] += w[koffset2+29] * img[koffset2+29];
            dst[i+30] += w[koffset2+30] * img[koffset2+30];
            dst[i+31] += w[koffset2+31] * img[koffset2+31];
            koffset2+=32;
        }

        for(i = 0; i < dimdim; i+=32) {
            
            dst[i] += w[koffset2] * img[koffset2];
            dst[i+1] += w[koffset2+1] * img[koffset2+1];
            dst[i+2] += w[koffset2+2] * img[koffset2+2];
            dst[i+3] += w[koffset2+3] * img[koffset2+3];
            dst[i+4] += w[koffset2+4] * img[koffset2+4];
            dst[i+5] += w[koffset2+5] * img[koffset2+5];
            dst[i+6] += w[koffset2+6] * img[koffset2+6];
            dst[i+7] += w[koffset2+7] * img[koffset2+7];
            dst[i+8] += w[koffset2+8] * img[koffset2+8];
            dst[i+9] += w[koffset2+9] * img[koffset2+9];
            dst[i+10] += w[koffset2+10] * img[koffset2+10];
            dst[i+11] += w[koffset2+11] * img[koffset2+11];
            dst[i+12] += w[koffset2+12] * img[koffset2+12];
            dst[i+13] += w[koffset2+13] * img[koffset2+13];
            dst[i+14] += w[koffset2+14] * img[koffset2+14];
            dst[i+15] += w[koffset2+15] * img[koffset2+15];
            dst[i+16] += w[koffset2+16] * img[koffset2+16];
            dst[i+17] += w[koffset2+17] * img[koffset2+17];
            dst[i+18] += w[koffset2+18] * img[koffset2+18];
            dst[i+19] += w[koffset2+19] * img[koffset2+19];
            dst[i+20] += w[koffset2+20] * img[koffset2+20];
            dst[i+21] += w[koffset2+21] * img[koffset2+21];
            dst[i+22] += w[koffset2+22] * img[koffset2+22];
            dst[i+23] += w[koffset2+23] * img[koffset2+23];
            dst[i+24] += w[koffset2+24] * img[koffset2+24];
            dst[i+25] += w[koffset2+25] * img[koffset2+25];
            dst[i+26] += w[koffset2+26] * img[koffset2+26];
            dst[i+27] += w[koffset2+27] * img[koffset2+27];
            dst[i+28] += w[koffset2+28] * img[koffset2+28];
            dst[i+29] += w[koffset2+29] * img[koffset2+29];
            dst[i+30] += w[koffset2+30] * img[koffset2+30];
            dst[i+31] += w[koffset2+31] * img[koffset2+31];
            koffset2+=32;
        }

            
   
}

/*********************************************************************
 * register_fusion_functions - Register all of your different versions
 *     of the fusion kernel with the driver by calling the
 *     add_fusion_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_fusion_functions() 
{
    
    add_fusion_function(&naive_fusion, naive_fusion_descr);  
    add_fusion_function(&fusion, fusion_descr);  
    /* ... Register additional test functions here */
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***************************
 * GAUSSIAN BLUR KERNEL *
 ***************************/

/******************************************************
 * Your different versions of the Gaussian blur functions go here
 ******************************************************/



/* 
 * naive_blur - The naive baseline version of Gussian blur
 */
char naive_blur_descr[] = "naive_blur The naive baseline version of Gaussian blur";
void naive_blur(int dim, float *img, float *flt, float *dst) {
  
    int i, j, k, l;

    for(i = 0; i < dim-5+1; i++){
        for(j = 0; j < dim-5+1; j++) {
            for(k = 0; k < 5; k++){
                for(l = 0; l < 5; l++) {
                    dst[i*dim+j] = dst[i*dim+j] + img[(i+k)*dim+j+l] * flt[k*dim+l];
                }
            }
        }
    }
}



/* 
 * blur - Your current working version of Gaussian blur
 * IMPORTANT: This is the version you will be graded on
 */
char blur_descr[] = "blur: Current working version";
void blur(int dim, float *img, float *flt, float *dst) 
{
    int i, j,idim, kdim, idoffset, dim2;
    int a,b,c;
    dim2 = dim - 4;
    float temp1 ,temp2,temp3,temp4,temp5, temp6,temp8,temp10,temp12,temp14;
    float temp1a,temp2a,temp3a,temp4a,temp5a, temp6a,temp8a,temp10a,temp12a,temp14a;
    float temp1b ,temp2b,temp3b,temp4b,temp5b, temp6b,temp8b,temp10b,temp12b,temp14b;
    float temp1c ,temp2c,temp3c,temp4c,temp5c, temp6c,temp8c,temp10c,temp12c,temp14c;
    float temp1d ,temp2d,temp3d,temp4d,temp5d, temp6d,temp8d,temp10d,temp12d,temp14d;
    idim = 0;
    float kd1, kd2, kd3, kd4, kd5, kd6, kd7, kd8, kd9, kd10, kd11, kd12, kd13, kd14, kd15, kd16, kd17, kd18, kd19, kd20, kd21, kd22, kd23, kd24, kd25;
    

    a=dim+dim;
    b=a+dim;
    c=b+dim;

    kd1 = flt[0];
    kd2 = flt[1];
    kd3 = flt[2];
    kd4 = flt[3];
    kd5 = flt[4];

    kd6 = flt[dim];
    kd7 = flt[dim+1];
    kd8 = flt[dim+2];
    kd9 = flt[dim+3];
    kd10 = flt[dim+4];
    
    kd11 = flt[a];
    kd12 = flt[a+1];
    kd13 = flt[a+2];
    kd14 = flt[a+3];
    kd15 = flt[a+4];
    kd16 = flt[b];
    kd17 = flt[b+1];
    kd18 = flt[b+2];
    kd19 = flt[b+3];
    kd20 = flt[b+4];
    
    kd21 = flt[c];
    kd22 = flt[c+1];
    kd23 = flt[c+2];
    kd24 = flt[c+3];
    kd25 = flt[c+4];
    for(i = 0; i < dim2; i+=1){
        for(j = 0; j < dim2; j+=1) {

            
            idoffset = idim + j;
            
                ////////////////////////////1
                temp6 = img[idoffset];
                temp1 = temp6*kd1;

                temp8 = img[1 + idoffset];
                temp2 =  temp8*kd2;

                temp10 = img[2 + idoffset];
                temp3 = temp1+temp10*kd3;

                temp12 = img[3 + idoffset];
                temp4 = temp2+temp12*kd4;

                temp14 = img[4 + idoffset];
                temp5 = temp3+temp4+temp14*kd5;
                
                ////////////////////////////2
                temp6a = img[dim + idoffset];
                temp1a = temp6a*kd6;

                temp8a = img[dim+1 + idoffset];
                temp2a = temp8a*kd7;

                temp10a = img[dim+2 + idoffset];
                
                temp3a = temp1a+temp10a*kd8;

                temp12a = img[dim+3 + idoffset];
                temp4a = temp2a+temp12a*kd9;

                temp14a = img[dim+4 + idoffset];
                temp5a = temp3a+temp4a+temp14a*kd10;
                kdim=dim+dim;
                ////////////////////////////3
                temp6b = img[kdim + idoffset];
                temp1b = temp6b*kd11;

                temp8b = img[kdim+1 + idoffset];
                temp2b = temp8b*kd12;

                temp10b = img[kdim+2 + idoffset];
                temp3b = temp1b+temp10b*kd13;

                temp12b = img[kdim+3 + idoffset];
                temp4b = temp2b+temp12b*kd14;

                temp14b = img[kdim+4 + idoffset];
                temp5b = temp3b+temp4b+temp14b*kd15;
                kdim += dim;
                ////////////////////////////4
                temp6c = img[kdim + idoffset];
                temp1c = temp6c*kd16;

                temp8c = img[kdim+1 + idoffset];
                temp2c = temp8c*kd17;

                temp10c = img[kdim+2 + idoffset];
                temp3c = temp1c+temp10c*kd18;

                temp12c = img[kdim+3 + idoffset];
                temp4c = temp2c+temp12c*kd19;

                temp14c = img[kdim+4 + idoffset];
                temp5c = temp3c+temp4c+temp14c*kd20;
                kdim += dim;
                ////////////////////////////5
                temp6d = img[kdim + idoffset];
                temp1d = temp6d*kd21;

                temp8d = img[kdim+1 + idoffset];
                temp2d = temp8d*kd22;

                temp10d = img[kdim+2 + idoffset];
                temp3d = temp1d+temp10d*kd23;

                temp12d = img[kdim+3 + idoffset];
                temp4d = temp2d+temp12d*kd24;

                temp14d = img[kdim+4 + idoffset];
                temp5d = temp3d+temp4d+temp14d*kd25;
                kdim += dim;
                

            
            dst[idoffset] = temp5+temp5a+temp5b+temp5c+temp5d;
        }
        idim += dim;
    }
}

/*********************************************************************
 * register_blur_functions - Register all of your different versions
 *     of the gaussian blur kernel with the driver by calling the
 *     add_blur_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_blur_functions() 
{
    add_blur_function(&naive_blur, naive_blur_descr); 
    add_blur_function(&blur, blur_descr);
    /* ... Register additional test functions here */
}
