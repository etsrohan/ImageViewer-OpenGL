//
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
int main(){
    char* buffer = 0;
    char* ret;
    int i = 0;
    long length = 0;
    FILE * f = fopen ("shaders/fragment.shader", "r");

    if (f)
    {
    fseek (f, 0, SEEK_END);
    length = ftell(f);
    printf("%d\n", length);
    fseek (f, 0, SEEK_SET);
    buffer = malloc(sizeof(char) * length);

    do
    {
        // Taking input single character at a time
        buffer[i] = fgetc(f);
 
        // Checking for end of file
        if (feof(f))
            break ;
 
        //printf("%c", buffer[i]);
        i++;
    }  while(1);

    ret = buffer;
    fclose(f);
    }

    free(buffer);
    printf(buffer);
    printf("\n\n");
    printf(ret);

    /*printf("%d\n", length);
    printf("%c\n", buffer[length]);
    printf("%c\n", buffer[length-1]);
    printf("%c\n", buffer[length-2]);
    printf("%c\n", buffer[length-3]);
    printf("%c\n", buffer[0]);
    printf("%c\n", buffer[1]);
    printf("%c\n", buffer[2]);
    printf("%c\n", buffer[3]);
    printf(buffer);
    printf("\n");*/
    return 0;
}