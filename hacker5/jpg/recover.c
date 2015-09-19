/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */
 
 #include <stdio.h>
 #include <stdlib.h>


int main(void)
{
    FILE* raw = fopen("card.raw", "r");
    
        //find position of first jpeg start

        for (int k = 0; k != 1; ) {
            unsigned char j[512];
            fread(j, 512, 1, raw);
            for (int i = 0; i < 509; i++) //misses some but w/e
                if (j[i]== 255)
                    if (j[i+1] == 216 && j[i+2] == 255 && (j[i+3] == 224 || j[i+3] == 225)){
                            fseek(raw, -512 + i, SEEK_CUR);
                            k = 1;
                            break; 
                        }
        }
    
    printf("HI");
    
    for (int i = 0; i < 50; i++) {
    
        char fname[8];
        sprintf(fname, "%03d.jpg", i); 
        FILE* jpg = fopen(fname, "w");
        
        int j = 0;
        int s = 0;
        while (1) {
            unsigned char buff[512];
            fread(buff, 512, 1, raw);
            if (buff[0] == 255 && buff[1] == 216 && buff[2] == 255 && (buff[3] == 224 || buff[3] == 225)) j++;

            if (j > 1){
                fseek(raw, -512, SEEK_CUR);
                break;
            }
            
            fwrite(buff, 512, 1, jpg);
            if (++s > 1000) break;
            
        }
        
        fclose(jpg);
        printf("done %s\n", fname);
        
    }
    fclose(raw);
    
    printf("DONE!");
}
