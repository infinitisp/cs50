#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{   
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy f infile outfile\n");
        return 1;
    }
    
    float f = atof(argv[1]);
    
    //ensure proper float input
    if (f <= 0 || f >= 100){
        printf("Must enter valid scale factor between 0 and 100, exclusive\n");
        return 1;
    }
        
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    
    //CONVERT HEADER W/H
    int origBiWidth = bi.biWidth;
    bi.biWidth = (int) (bi.biWidth * f);
    bi.biHeight = bi.biHeight > 0 ? (int) (bi.biHeight * f) : -1 * (int) (abs(bi.biHeight) * f); 
    
    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int origPadding =  (4 - (origBiWidth * sizeof(RGBTRIPLE)) % 4) % 4;
     
    //CONVERT HEADER SIZES 
    bi.biSizeImage = abs(bi.biHeight) * (bi.biWidth*3 + padding); // calculate biSizeImage inc. padding
    bf.bfSize = bi.biSizeImage + 54; 
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

      
        // iterate over outfiles's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            //get start position of line in infile
            int lineStart = 54 + ((int)(i / f))*(origBiWidth*sizeof(RGBTRIPLE) + origPadding);
                
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                
                // seek to proper source position
                fseek(inptr, lineStart + sizeof(RGBTRIPLE)*(int)(j/f), SEEK_SET);
                
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
            // add padding to outfile (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
