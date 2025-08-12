#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include "types.h"

#define MAX_TAGS 6

// Struct representing the audio file and related metadata
typedef struct 
{
    FILE *fptr;
    const char *src_filename;

    FILE *new_fptr;
    const char *new_filename;

    const char *original_name;

    int tag_index;

    const char *new_content;

} AudioFile;

Status check_operation_type(const char *argv[]);

Status read_and_validate(const char *argv[]);

Status read_and_validate_edit_args(char *argv[], AudioFile *audiofile);

Status check_file(AudioFile *audiofile);

Status view_data(AudioFile *audiofile);

Status compare_tag(char *buffer, int i);

Status read_data(AudioFile *audiofile);

Status do_editing(AudioFile *audiofile);

Status copy_header(AudioFile *audiofile);

Status edit_tag(AudioFile *audiofile);

Status copy_remaining_data(FILE *fptr_src, FILE *fptr_dest);

Status convert_big_little(char *size);

Status convert_little_end(char *size);

#endif 
