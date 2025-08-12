/*Documentation
Name        :T Simhadri
Date        :07/08/2025
Description :MP3 Tag Reader and Editor
*/
#include <stdio.h>
#include "header.h"
#include <string.h>
#include <stdlib.h>

Status read_and_validate_edit_args(char *argv[], AudioFile *audiofile)
{
    // Check for missing tag option
    if (argv[2] == NULL)
    {
        printf("❌ ERROR: Missing tag option (-t, -a, -A, etc.)\n");
        return failure;
    }

    // List of valid tag options
    const char *arr[] = {"-t", "-a", "-A", "-y", "-m", "-c"};
    int flag = 0;

    // Match the user tag option
    for (int i = 0; i < MAX_TAGS; i++)
    {
        if (strcmp(arr[i], argv[2]) == 0)
        {
            flag = 1;
            audiofile->tag_index = i;
        }
    }

    // Year-specific validation
    if (strcmp(argv[2], "-y") == 0)
    {
        int i;
        int size = strlen(argv[3]);
        char *buffer = argv[3];
        for (i = 0; buffer[i]; i++)
        {
            if (buffer[i] < '0' || buffer[i] > '9')
            {
                printf("❌ ERROR: Year must contain only digits.\n");
                return failure;
            }
        }
        if (i != 4)
        {
            printf("❌ ERROR: Year must contain exactly 4 digits.\n");
            return failure;
        }
    }

    // Invalid tag option
    if (!flag)
    {
        printf("❌ ERROR: Invalid tag option '%s'. Use -t/-a/-A/-y/-m/-c\n", argv[2]);
        return failure;
    }

    // Check for missing new content
    if (argv[3] == NULL)
    {
        printf("❌ ERROR: Missing new content for tag update.\n");
        return failure;
    }

    audiofile->new_content = argv[3];

    // Validate file extension
    char *ch = strstr(argv[4], ".mp3");
    if (ch == NULL || strcmp(ch, ".mp3") != 0)
    {
        printf("❌ ERROR: Invalid filename '%s'. Must be a .mp3 file\n", argv[4]);
        return failure;
    }

    audiofile->src_filename = argv[4];
    audiofile->original_name = argv[4];
    audiofile->new_filename = "new_file.mp3";

    return success;
}

Status do_editing(AudioFile *audiofile)
{
    // Step 1: Copy header
    if (copy_header(audiofile) == failure)
    {
        printf("❌ ERROR: Failed to copy MP3 header.\n");
        return failure;
    }
    printf("✅ Header copied successfully.\n");

    // Step 2: Edit tag
    if (edit_tag(audiofile) == failure)
    {
        printf("❌ ERROR: Failed to edit tag data.\n");
        return failure;
    }
    printf("✅ Tag data edited successfully.\n");

    // Step 3: Copy remaining data
    if (copy_remaining_data(audiofile->fptr, audiofile->new_fptr) == failure)
    {
        printf("❌ ERROR: Failed to copy remaining MP3 data.\n");
        return failure;
    }
    printf("✅ Remaining MP3 data copied successfully.\n");

    // Step 4: Replace original file
    if (remove(audiofile->src_filename) == 0 && rename(audiofile->new_filename, audiofile->original_name) == 0)
    {
        return success;
    }
    else
    {
        printf("❌ ERROR: Failed to replace original file with updated file.\n");
        return failure;
    }
}

Status copy_header(AudioFile *audiofile)
{
    rewind(audiofile->fptr); // Move to start of file

    audiofile->new_filename = "new.mp3";
    audiofile->new_fptr = fopen(audiofile->new_filename, "w");
    if (audiofile->new_fptr == NULL)
    {
        perror("fopen");
        printf("❌ ERROR: Unable to open '%s' for writing.\n", audiofile->new_filename);
        return failure;
    }

    // Copy first 10 bytes (ID3 header)
    char *buffer[10];
    if (fread(buffer, 10, 1, audiofile->fptr) != 1)
    {
        printf("❌ ERROR: Failed to read MP3 header from source.\n");
        return failure;
    }
    fwrite(buffer, 10, 1, audiofile->new_fptr);
    return success;
}

Status edit_tag(AudioFile *audiofile)
{
    for (int i = 0; i <= audiofile->tag_index; i++)
    {
        char buffer[4];

        // Read tag name
        if (fread(buffer, 4, 1, audiofile->fptr) != 1)
        {
            printf("❌ ERROR: Failed to read tag identifier.\n");
            return failure;
        }
        fwrite(buffer, 4, 1, audiofile->new_fptr);

        int size, new_size = 0;
        int s_flag = 0;

        // Read tag size and convert
        if (fread(&size, 4, 1, audiofile->fptr) != 1)
        {
            printf("❌ ERROR: Failed to read tag size.\n");
            return failure;
        }

        if (convert_big_little((char *)&size) == failure)
            return failure;

        // If current tag is the target tag to be edited
        if (i == audiofile->tag_index)
        {
            new_size = strlen(audiofile->new_content) + 1;
            int temp = new_size;

            if (convert_little_end((char *)&temp) == failure)
                return failure;

            fwrite(&temp, 4, 1, audiofile->new_fptr);
            s_flag = 1;
        }

        if (!s_flag)
        {
            int temp = size;
            convert_little_end((char *)&temp);
            fwrite(&temp, 4, 1, audiofile->new_fptr);
        }

        // Read and write flags (3 bytes)
        char flag[3];
        if (fread(flag, 3, 1, audiofile->fptr) != 1)
        {
            printf("❌ ERROR: Failed to read tag flags.\n");
            return failure;
        }
        fwrite(flag, 3, 1, audiofile->new_fptr);

        // Allocate buffer for tag data
        char *data_buf = malloc(size);
        if (!data_buf)
        {
            printf("❌ ERROR: Memory allocation failed for tag content.\n");
            return failure;
        }

        // Read tag data
        if (fread(data_buf, size - 1, 1, audiofile->fptr) != 1)
        {
            printf("❌ ERROR: Failed to read tag content.\n");
            free(data_buf);
            return failure;
        }

        // Write new or original tag content
        if (i == audiofile->tag_index)
        {
            fwrite(audiofile->new_content, new_size - 1, 1, audiofile->new_fptr);
        }
        else
        {
            fwrite(data_buf, size - 1, 1, audiofile->new_fptr);
        }

        free(data_buf);
    }

    return success;
}

Status convert_little_end(char *size)
{
    // Convert 4-byte integer to little-endian
    unsigned char temp;

    temp = size[0];
    size[0] = size[3];
    size[3] = temp;

    temp = size[1];
    size[1] = size[2];
    size[2] = temp;

    return success;
}

Status copy_remaining_data(FILE *fptr_src, FILE *fptr_dest)
{
    // Copy remaining bytes after the tag data
    char ch;
    while (fread(&ch, 1, 1, fptr_src) == 1)
    {
        if (fwrite(&ch, 1, 1, fptr_dest) != 1)
        {
            printf("❌ ERROR: Failed writing remaining byte to destination.\n");
            return failure;
        }
    }

    fclose(fptr_dest);
    fclose(fptr_src);
    return success;
}
