#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"
 

int main(int argc, char **argv)
{
    //Validate

    printf("\nLets see what you have selected\n");

    if(check_operation_type(argv) == e_encode)
    {
        printf("\nCongrats!. you have selected encoding\n");
        EncodeInfo steganography;

        if (read_and_validate_encode_args(argv, &steganography) == e_success)
        {
            printf("\nInfo : Read and validated encode arguments successfully\n");

            printf("\n<----------------------- Let's start encoding -----------------------> \n");

            if(do_encoding(&steganography) == e_success)
            {
                printf("\nInfo : Completed encoding successfully !...\n");

                printf("\n<----------------------- Encoding process complete -----------------------> \n\n\n");
            }
            else
            {
                printf("Info : Failed to perform encoding !!! \n");
                return -1;
            }
        }
    }
    else if(check_operation_type(argv) == e_decode)  // decoding 
    {
        printf("\nInfo : Congrats!. you have selected decoding\n");

        DecodeInfo steganography;

        if (read_and_validate_decode_args(argv, &steganography) == e_success)
        {
            printf("\nInfo : Read and validated decode arguments successfully\n");

            printf("\n<----------------------- Let's start decoding -----------------------> \n\n");

            if(do_decoding(&steganography) == e_success)
            {
                printf("\nInfo : Completed decoding successfully !...\n");

                printf("\n<----------------------- Decoding process complete -----------------------> \n\n");
            }
            else
            {
                printf("Info : Failed to perform decoding !!! \n");
                return -1;
            }
        }

    }
    else
    {
        printf("\nError : Invalid input!!!\nPlease pass: \n");
        printf("\nConsider --> for encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp] \n");
        printf("\nConsider --> for decoding : ./a.out -d stego.bmp [output.txt] \n");
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if( strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;        
}

/* END */