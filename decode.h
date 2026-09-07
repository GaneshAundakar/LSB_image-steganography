#ifndef DECODE_H
#define DECODE_H

#include "types.h"       // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
 
typedef struct _DecodeInfo
{
    // decode File Info
    char *decode_fname;
    FILE *fptr_decode;
    char extn_decode_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_decode_file;

    // Stego Image Info
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[MAX_IMAGE_BUF_SIZE];
} DecodeInfo;

/* --------------- Decoding function prototypes ---------------  */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(int size, FILE *fptr_stego_image, FILE *fptr_dest_txt);

/* Decode secret file extension */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image, FILE *fptr_decode, DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode size from lsb */
Status decode_size_from_lsb(char *buffer, int *size);

#endif
