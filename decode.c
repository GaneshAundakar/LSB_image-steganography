#include <stdio.h>
#include "common.h"
#include <string.h>
#include "decode.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{

    // checking 4th arg is passed or not, if passed need to store with given name otherwise need to default with name output.txt

    if (argv[2] != NULL && strstr(argv[2], ".bmp") != NULL)
    {
        decInfo -> stego_image_fname = argv[2];
    } 
    else
    {
        fprintf(stderr, "ERROR: Input file must be a .bmp image\n");
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        decInfo -> decode_fname = argv[3];
    }
    else 
    {
        decInfo -> decode_fname = "output.txt";
    }
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "rb");
    
    if (decInfo -> fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }

    decInfo -> fptr_decode = fopen(decInfo -> decode_fname, "w");

    if (decInfo -> fptr_decode == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_fname);
        return e_failure;
    }
 
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);

    int magic_len = strlen(magic_string);
    char decoded_magic[magic_len + 1];
    memset(decoded_magic, 0, magic_len + 1);

    decode_data_from_image(decoded_magic, magic_len, decInfo -> fptr_stego_image, decInfo -> fptr_decode, decInfo);

    if (strcmp(decoded_magic, magic_string) == 0)
    {
        return e_success;
    }
    return e_failure;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image, FILE *fptr_decode, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        // Read 8 bytes of image data for each hidden character

        fread(decInfo -> image_data, 1, 8, fptr_stego_image);

        // Extract the original byte

        decode_byte_from_lsb( &data[i], decInfo -> image_data);
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch = ch | ((image_buffer[i] & 1) << (7 - i));
    }
    *data = ch;
    return e_success;
}

Status decode_secret_file_extn_size(int size, FILE *fptr_stego_image, FILE *fptr_dest_txt)
{
    char str[32];
    int extn_size = 0;

    fread(str, 1, 32, fptr_stego_image);

    decode_size_from_lsb(str, &extn_size);

    if (extn_size <= MAX_FILE_SUFFIX)
    {
        return e_success;
    }
    return e_failure;
}

Status decode_size_from_lsb(char *buffer, int *size)
{
    int num = 0;
    for (int i = 0; i < 32; i++)
    {
        num = num | ((buffer[i] & 1) << (31 - i));
    }
    *size = num;

    return e_success;
}

Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    decode_data_from_image(file_extn, 4, decInfo -> fptr_stego_image, decInfo -> fptr_decode, decInfo);
    file_extn[4] = '\0'; 
    return e_success;
}

Status decode_secret_file_size(long file_size, DecodeInfo *decInfo)
{
    char str[32];
    int parsed_size = 0;

    fread(str, 1, 32, decInfo -> fptr_stego_image);
    decode_size_from_lsb(str, &parsed_size);

    decInfo -> size_decode_file = (long)parsed_size;
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo -> size_decode_file; i++)
    {
        fread( decInfo -> image_data, 1, 8, decInfo -> fptr_stego_image);

        decode_byte_from_lsb(&ch, decInfo -> image_data);
        fputc(ch, decInfo -> fptr_decode);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_success)
    {
        printf("Opened required files in required mode successfully\n");

        if (decode_magic_string(MAGIC_STRING, decInfo) == e_success)
        {
            printf("\nInfo : Decoded magic string successfully\n");

            if (decode_secret_file_extn_size(4, decInfo -> fptr_stego_image, decInfo -> fptr_decode) == e_success)
            {
                printf("\nInfo : Decoded file extension size successfully\n");

                if (decode_secret_file_extn(decInfo -> extn_decode_file, decInfo) == e_success)
                {
                    printf("\nInfo : Decoded file extension successfully\n");

                    if (decode_secret_file_size(decInfo -> size_decode_file, decInfo) == e_success)
                    {
                        printf("\nInfo : Decoded secret file size successfully: %ld bytes\n", decInfo -> size_decode_file);
                        
                        if (decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("\nInfo : Decoded secret file data successfully\n");
                        }
                        else
                        {
                            printf("\nError : Failed to decode secret file data\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("\nError : Failed to decode secret file size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("\nError : Failed to decode file extension\n");
                    return e_failure;
                }
            }
            else
            {
                printf("\nError : Failed to decode file extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("\nError : Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("\nError : Failed to open files\n");
        return e_failure;
    }
    return e_success;
}

/* ------------------------------END of Functions------------------------------------------------------- */
