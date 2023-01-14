#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double frac;

long power(long base, long exponent){
    long powerValue = 1;
    for(long i=0; i < exponent; i++){
        powerValue = powerValue * base;
    }
    return powerValue;
}

long bitsNeeded(long num){
    long x = 1;
    while (power(2,x) <= num){
        x++;
    } 
    long size = x;
    return size;
}

void reverse(long arr[], long n){
    long aux[n];
 
    for (int i = 0; i < n; i++) {
        aux[n - 1 - i] = arr[i];
    }
 
    for (int i = 0; i < n; i++) {
        arr[i] = aux[i];
    }
}

void decimalToBinary(long a[],long length, long num){
    for(long i=0; i<length; i++){
        a[i] = num%2;
        num = num/2;
    }
}

double fractionBinary(double fraction){   //this code takes in a decimal fraction and turns it in to binary(as a double). For example, 0.625 = 0.101000
    double current = fraction * 2.0;
    double remainder = ((int)current) % 2;

    if(current == 1 || current == frac){
        return remainder / 10;
    }

    return fractionBinary(current - remainder)/10 + remainder/10;
}

void fractionToBinary(long a[], double num){  //takes the binary representation of a double and puts it in an array
    double binary = fractionBinary(num);
    for(int e=0; e<6; e++){
        binary = binary * 10;
        a[e] = (long)binary % 10; //now the binary of the number to the right of the decimal point is stored in int array[6]
    }
}

double binaryDecimal(long array[], int fracBits){
    double fracDecimal = 0;
    double twos = 2;
    for(int i=0;i<fracBits;i++){
        fracDecimal += array[i]/twos;
        twos *= 2.0;
    }
    return fracDecimal;
}

void IEEE(double number, long totalBits, long exponentBits, long fractionBits){
    long n;
    int signbit;                                  // ONE
    if(number < 0){
        n = (long)(-1*number);
       signbit = 1;
    }else{
        n = (long)number;
        signbit = 0;
    }
    long size = bitsNeeded(n); // this is the minimum number of bits we need to represent n in binary
    long arr[size];
    decimalToBinary(arr, size, n);  
    reverse(arr, size); // the left side of decimal point is now stored in arr[size]


    if(number>0){
        frac = number - (double)n;
    }else{
        frac = (-1*number) - (double)n;
    }
    //frac = number - (double)n; // this gets us just the fraction part of the double (whatever is on the right side of the decimal point)

    long array[6];
    fractionToBinary(array, frac); //now the binary representation of frac is stored inside array[6]

    long bias;
    long smallE;
    long valueofBigE;
    if(n > 0){
        smallE = size -1;
    }else{
        int index = 0;
        for(int c=0; c<6; c++){
            if(array[c]==1){
                index = c;
                break; 
            }
        }
        smallE = -(index + 1);
    }
    bias = (power(2, exponentBits - 1)) - 1;
    //printf("bias is %li\n", bias);
    //printf("small e is %li\n", smallE);
    valueofBigE = smallE + bias;
    //printf("big E is %li\n", valueofBigE);
    
    //long sizeE = bitsNeeded(valueofBigE);
    long binaryRepresentationForE [exponentBits];            //  TWO
    decimalToBinary(binaryRepresentationForE, exponentBits, valueofBigE);
    reverse(binaryRepresentationForE, exponentBits);
    long total = size + 6;
    long entire[total];
    for(int en=0;en<size;en++){
        entire[en]=arr[en];
    }
    for(int el=0;el<6;el++){
        entire[el+size] = array[el];
    }
    // for(int d=0;d<total;d++){
    //     printf("%li", entire[d]);
    // }
    printf("%d", signbit);
    for(int i=0;i<exponentBits;i++){
        printf("%li", binaryRepresentationForE[i]);
    }

    int id = 0;
    for(int di=0;di<total;di++){   // id now holds the index for the first occurance of a 1 entire[]
        if(entire[di]==1){
            id = di;
            break;
        }
    }
    //printf("%d", id);
    
    long rest = fractionBits - (total - (id + 1));
    if(fractionBits > (total-1)){
        //int remainder = fractionBits - (total-1);
        for(int st = id+1; st<total; st++){
            printf("%li", entire[st]);
        }
        for(int re=0;re<rest;re++){
            printf("0");
        }
    }else if(fractionBits == (total-1)){
        // for(int tt=id+1;tt<fractionBits+1;tt++){
        //     printf("%li", entire[tt]);
        // }
        for(int tt=id+1;tt<total;tt++){
            printf("%li", entire[tt]);
        }
        for(int ty=0;ty<rest;ty++){
            printf("%li", entire[ty]);
        }
    }
    else if(fractionBits < (total-1)){     //array[6] holds the binary of the fraction
        long fb[fractionBits];
        int rem = total - (id+1);
        if(rem<fractionBits){
            for(int i=0; i<rem; i++){
                fb[i] = entire[i + (id + 1)];
            }
            for(int p = rem; p < fractionBits; p++){
                fb[p] = 0;
            }
        }else{
            for(int ff=0; ff<fractionBits; ff++){
                fb[ff] = entire[id + 1 + ff];  //fb stores the first fractionBits of the decimal im the canonical version  (v1)
            }
        }
        // for(int ff=0; ff<fractionBits; ff++){
        //     fb[ff] = entire[id + 1 + ff];  //fb stores the first fractionBits of the decimal im the canonical version  (v1)
        // }
        // for(int oop=0; oop<fractionBits; oop++){
        //     printf("%li", fb[oop]);
        // }

        double v1 = binaryDecimal(fb, fractionBits);  //v1 will always be smaller than v2
        double v2 = v1 + (1/(double)power(2,fractionBits));
        double mp = (v1 + v2)/2;
        //printf("%lf %lf %lf", v1, v2, mp);

        long v2Array[6];
        long v2whole[fractionBits];
        //double vee2 = v2-1.0;
        if(v2 >= 1){
            //decimalToBinary(v2whole, fractionBits, (long)v2);
            //reverse(v2whole, fractionBits);
            for(int jh = 0; jh< fractionBits; jh++){
                v2whole[jh] = 0;
            }
        }else{
            fractionToBinary(v2Array, v2);  // the binary representation of v2;
            // for(int hhe=0; hhe<6; hhe++){
            //     printf("%li", v2Array[hhe]);
            // }
        }
        // fractionToBinary(v2Array, v2);  // the binary representation of v2;
        // for(int hhe=0; hhe<6; hhe++){
        //     printf("%li", v2Array[hhe]);
        // }
      
        if((v2 - mp) < (mp - v1)){
            if(v2>=1){
                for(int zr=0; zr<fractionBits; zr++){
                    printf("%li", v2whole[zr]);
                }
            }
            for(int zz=0;zz<fractionBits;zz++){
                printf("%li", v2Array[zz]);
            }
        }else if((mp - v1) < (v2 - mp)){
            for(int nn=0;nn<fractionBits;nn++){
                printf("%li", fb[nn]);
            }
        }else if((v2 - mp) == (mp - v1)){
            // if(v2whole[fractionBits-1]==0){
            //     for(int hh=0; hh<fractionBits; hh++){
            //         printf("%li", v2whole[hh]);
            //     }
            
            for(int jj=0; jj<fractionBits; jj++){
                printf("%li", fb[jj]);
            }
            
            // if(v2 >= 1){
            //     for(int u=0;u<fractionBits;u++){
            //         printf("%li", fb[u]);
            //     }
            // }else if(v2whole[fractionBits-1]==0){
            //     for(int d=0;d<fractionBits;d++){
            //         printf("%li", v2whole[d]);
            //     }
            // }
            // for(int kk=0;kk<fractionBits;kk++){
            //     printf("%li", fb[kk]);
            // }
            
        }
    }
    printf("\n");
}

int main(int argc, char* argv[argc+1]){
    double number;
    long totalBits;
    long exponentBits;
    long fractionBits;
    FILE* fp = fopen(argv[1], "r");
    while(fscanf(fp, "%lf %li %li %li\n", &number, &totalBits, &exponentBits, &fractionBits) != EOF){
        IEEE(number, totalBits, exponentBits, fractionBits);
    }
    fclose(fp);
    return 0;
}
