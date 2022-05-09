#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 700
#define MIN 30

struct rgb_data {
    float r, g, b;
};

// bitmap file header (14 bytes)
struct bitmap_file_header{
    unsigned char   bitmap_type[2];     // 2 bytes
    int             file_size;          // 4 bytes
    short           reserved1;          // 2 bytes
    short           reserved2;          // 2 bytes
    unsigned int    offset_bits;        // 4 bytes
}bfh;

// bitmap image header (40 bytes)
struct bitmap_image_header{
    unsigned int    size_header;        // 4 bytes
    unsigned int    width;              // 4 bytes
    unsigned int    height;             // 4 bytes
    short int       planes;             // 2 bytes
    short int       bit_count;          // 2 bytes
    unsigned int    compression;        // 4 bytes
    unsigned int    image_size;         // 4 bytes
    unsigned int    ppm_x;              // 4 bytes
    unsigned int    ppm_y;              // 4 bytes
    unsigned int    clr_used;           // 4 bytes
    unsigned int    clr_important;      // 4 bytes
}bih;

void limits(int in){
    if(in < MIN || in > MAX){
        printf("Suurus peab olema vahemikus 30 - 700\n");
        exit(0);
    }
}

// Function to round an int to a multiple of 4
int round4(int x) {
    return x % 4 == 0 ? x : x - x % 4 + 4;
}

int main(int argc, char *argv[]){
    
    int width, height, mode;
    int dpi = 100;
    
    printf("Ruut 1, Nelinurk 2: ");
    scanf("%d", &mode);
    if(mode != 1 && mode != 2){
        printf("Sisestage 1 või 2\n");
        return 0;
    }
    printf("Laius: ");
    scanf("%d", &width);
    limits(width);
    printf("Pikkus: ");
    scanf("%d", &height);
    limits(height);

    int padded_width = round4(width * 3);
    int image_size = height * padded_width * 3;
    int file_size = 54 + 4 * image_size;
    int ppm = dpi * 39.375;

    memcpy(&bfh.bitmap_type, "BM", 2); // headeri algväärtustamine
    bfh.file_size       = file_size;
    bfh.reserved1       = 0;
    bfh.reserved2       = 0;
    bfh.offset_bits     = 0;

    bih.size_header     = sizeof(bih);
    bih.width           = width;
    bih.height          = height;
    bih.planes          = 1;
    bih.bit_count       = 24;
    bih.compression     = 0;
    bih.image_size      = file_size;
    bih.ppm_x           = ppm;
    bih.ppm_y           = ppm;
    bih.clr_used        = 0;
    bih.clr_important   = 0;

    FILE *f = fopen("bitmap.bmp", "wb");

    fwrite(&bfh, 1, 14, f);
    fwrite(&bih, 1, sizeof(bih), f); // kirjutan headeri
    int padding = 20;
    int maxsize;
    int startx = padding / 2;
    int starty = padding / 2;
    float change = 256.0 / (float) (height - padding); // kui palju pean värvi muutma iga reaga et jõuda 0 - 255

    if(mode == 1){ // kui on ruut pean muutma algust ja lõppu pikemal küljel
        if(width > height){
            maxsize = height - padding;
            startx = (width - maxsize) / 2;
        }
        else if(width < height){
            maxsize = width - padding;
            starty = (height - maxsize) / 2;
            change = 256.0 / (float) maxsize;
        }
    }

    char rgb[] = {255,0,0};
    float check = 0;
    char *bitmap = (char *) malloc(bih.image_size * sizeof(char));
    for (int i = 0; i < image_size; i++) bitmap[i] = 255;
    // For each pixel in the RGB image...
    for (int row = starty; row < height - starty; row++) {
        for (int col = startx; col < width - startx; col++) {
            // For R, G, and B...
            for (int color = 0; color < 3; color++) {

                // Get the index of the destination image
                int index = row * padded_width + col * 3 + color;

                // Set the destination to the value of the src at row, col.
                bitmap[index] = rgb[color];
            }
            
        }
        rgb[0] = 255 - round(check);
        rgb[2] = 0 + round(check);
        check += change;
        
    }
    fwrite(bitmap, bih.image_size * sizeof(char), 1, f);
    fclose(f);
    
    free(bitmap);
    return 0;
}
