#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fuz
{
    int magicId; // FUZE
    int version; // Current version is 1
    unsigned int lipSize; // If this is NULL, then no lip is contained
};

static void printHelp()
{
    printf("fuzmanager\n\nUsage:\nfuzmanager.exe -help\nfuzmanager.exe -u \"fuzPath\"\nfuzmanager.exe -p \"fuzSavePath\" \"lipPath\" \"audioPath\"\n");
}

int main(int argc, char *argv[])
{
    char unpackArg[3] = "-u"; // Unpacking fuz
    char packArg[3] = "-p"; // Packing fuz

    if (argc >= 2)
    {
        if (strcmp(unpackArg, argv[1]) == 0)
        {
            if (argc == 3)
            {
                FILE *fuzFile = fopen(argv[2], "rb+");
                struct fuz input;
                unsigned int fuzSize;

                // Getting size of fuz
                fseek(fuzFile, 0L, SEEK_END);
                fuzSize = ftell(fuzFile);

                // Going back to the beginning
                rewind(fuzFile);

                // Checking if file exists
                if (fuzFile == NULL)
                {
                    fprintf(stderr, "\nError opening fuz\n");
                    exit (1);
                }

                // Reading header
                fread(&input, sizeof(struct fuz), 1, fuzFile);

                // Reading lip
                char lip[input.lipSize];
                fread(&lip, 1, input.lipSize, fuzFile);

                // Reading audio
                unsigned int audioSize = fuzSize - input.lipSize - sizeof(input);
                char audio[audioSize];
                fread(&audio, 1, audioSize, fuzFile);

                // Writing lip
                argv[2][strlen(argv[2]) - 3] = '\0';
                char lipExt[] = "lip";
                strncat(argv[2], &lipExt, 3);
                fwrite(&lip, 1, sizeof(lip), fopen(argv[2], "wb+"));

                // Writing audio, does not check what format audio is
                argv[2][strlen(argv[2]) - 3] = '\0';
                char audioExt[] = "audio";
                strncat(argv[2], &audioExt, 5);
                fwrite(&audio, 1, sizeof(audio), fopen(argv[2], "wb+"));
            }
            else
            {
                printf("Invalid argument count.");
            }
        }
        else if (strcmp(packArg, argv[1]) == 0)
        {
            if (argc == 5)
            {
                struct fuz output;
                output.magicId = 1163547974; // FUZE
                output.version = 1;

                // Reading lip
                FILE* lipFile = fopen(argv[3], "rb+");

                // Checking if lip exists
                if (lipFile == NULL)
                {
                    fprintf(stderr, "\nError opening lip\n");
                    exit (1);
                }

                // Getting lip size and setting variable in fuz struct
                fseek(lipFile, 0, SEEK_END);
                int fileSize = ftell(lipFile);
                output.lipSize = fileSize;
                fseek(lipFile, 0, SEEK_SET);

                char lip[fileSize];
                fread(&lip, 1, fileSize, lipFile);

                // Reading audio
                FILE* audioFile = fopen(argv[4], "rb+");

                // Checking if audio exists
                if (audioFile == NULL)
                {
                    fprintf(stderr, "\nError opening audio\n");
                    exit (1);
                }

                // Getting audio size
                fseek(audioFile, 0, SEEK_END);
                fileSize = ftell(audioFile);
                fseek(audioFile, 0, SEEK_SET);

                char audio[fileSize];
                fread(&audio, 1, fileSize, audioFile);

                // Writing fuz
                FILE *fuzFile = fopen(argv[2], "wb+");
                fwrite(&output, sizeof(output), 1, fuzFile);
                fwrite(&lip, sizeof(lip), 1, fuzFile);
                fwrite(&audio, sizeof(audio), 1, fuzFile);
            }
            else
            {
                printf("Invalid argument count.");
            }
        }
    }
    else
    {
        printHelp();
    }

    return 0;
}
