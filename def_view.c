/*Documentation
Name        :T Simhadri
Date        :07/08/2025
Description :MP3 Tag Reader and Editor
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "header.h"

// Determine operation type: view or edit
Status check_operation_type(const char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)
    {
        return view;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        return edit;
    }
    else
    {
        return unsupported;
    }
}

// Validate file extension is .mp3
Status read_and_validate(const char *argv[])
{
    printf("🔍 Validating arguments...\n\n");

    if(argv[2] == NULL)
    {
        return 1;
    }
    const char *ch = strrchr(argv[2], '.');
    if (ch != NULL && strcmp(ch, ".mp3") == 0)
    {
        printf("✅ Source file is validated\n\n");
        printf("✅ Successfully validated all arguments\n\n");
        return success;
    }
    else
    {
        printf("❌ Source file must have a .mp3 extension\n");
        return failure;
    }
}

// Check file accessibility and verify ID3 tag
Status check_file(AudioFile *audiofile)
{
    printf("🔍 Validating file...\n\n");

    audiofile->fptr = fopen(audiofile->src_filename, "r");
    if (audiofile->fptr == NULL)
    {
        perror("❌ ERROR opening file");
        return failure;
    }

    char buffer[4];
    if (fread(buffer, 3, 1, audiofile->fptr) != 1)
    {
        perror("❌ ERROR reading header");
        fclose(audiofile->fptr);
        return failure;
    }

    buffer[3] = '\0'; // Null-terminate the ID3 tag buffer
    rewind(audiofile->fptr); // Reset file pointer

    printf("📦 Header buffer: %s\n", buffer);

    if (strcmp(buffer, "ID3") != 0)
    {
        printf("❌ Not a valid ID3-tagged MP3 file\n");
        fclose(audiofile->fptr);
        return failure;
    }

    printf("✅ Source has valid ID3 tag\n\n");
    printf("✅ Successfully validated file\n");
    return success;
}

// View and print metadata from MP3
Status view_data(AudioFile *audiofile)
{
    const char *arr[]  = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    const char *name[] = {
        "🎵 TITLE",
        "🎤 ARTIST",
        "💿 ALBUM",
        "📅 YEAR",
        "🎧 GENRE",
        "💬 COMMENT"
    };

    rewind(audiofile->fptr);
    fseek(audiofile->fptr, 10, SEEK_SET); // Skip ID3 header

    // Print metadata heading
    printf("\n===============================================================\n");
    printf("                     MP3 FILE METADATA                       \n");
    printf("===============================================================\n\n");


    printf(" %-20s     | %-s\n", "🏷️  Tag", "📄 Content");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < MAX_TAGS; i++)
    {
        char tag_buf[5] = {0};
        if (fread(tag_buf, 4, 1, audiofile->fptr) != 1)
        {
            printf("❌ Error: Unable to read tag identifier.\n");
            return failure;
        }
        tag_buf[4] = '\0';

        if (strcmp(arr[i], tag_buf) != 0)
        {
            printf("❌ Warning: Unexpected tag '%s'. Skipping...\n", tag_buf);
            return failure;
        }

        int size;
        if (fread(&size, 4, 1, audiofile->fptr) != 1)
        {
            printf("❌ Error: Failed to read tag size.\n");
            return failure;
        }
        convert_big_little((char *)&size);

        char data_buf[size];
        fseek(audiofile->fptr, 3, SEEK_CUR); // Skip flags

        if (fread(data_buf, size - 1, 1, audiofile->fptr) != 1)
        {
            printf("❌ Error: Failed to read tag content.\n");
            return failure;
        }
        data_buf[size - 1] = '\0'; // Null-terminate content

        // Replace non-printable characters
        for (int j = 0; j < size - 1; j++)
        {
            if (data_buf[j] < 32 || data_buf[j] > 126)
                data_buf[j] = '.';
        }

        printf(" %-20s | %s\n", name[i], data_buf);
    }

    // End of metadata display
    printf("\n===============================================================\n\n");
    fclose(audiofile->fptr);
    printf("✅ %s is closed successfully\n", audiofile->src_filename);
    return success;
}

// Convert 4-byte integer from big-endian to little-endian
Status convert_big_little(char *size)
{
    unsigned char temp;
    temp = size[0];
    size[0] = size[3];
    size[3] = temp;

    temp = size[1];
    size[1] = size[2];
    size[2] = temp;

    return success;
}

// Compare tag with known types and print name
Status compare_tag(char *buffer, int i)
{
    const char *arr[]  = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    const char *name[] = {"🎵 TITLE", "🎤 ARTIST", "💿 ALBUM", "📅 YEAR", "🎧 GENRE", "💬 COMMENT"};

    if (strcmp(arr[i], buffer) == 0)
    {
        printf(" %-18s", name[i]); // Print matched tag name with spacing
        return success;
    }

    return failure;
}

