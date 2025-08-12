/*Documentation
Name        :T Simhadri
Date        :07/08/2025
Description :MP3 Tag Reader and Editor
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "types.h"

int main(int argc, char *argv[])
{
    AudioFile audiofile;

    // Initialize audiofile fields to NULL
    audiofile.fptr = NULL;
    audiofile.new_fptr = NULL;
    audiofile.src_filename = NULL;
    audiofile.new_filename = NULL;
    audiofile.new_content = NULL;

    // Display help menu
    if(argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf("      📜 ------------------- HELP MENU -------------------\n\n");
            printf("🎧 1. View mode          : ./a.out -v <filename>\n");
            printf("✏️  2. Edit mode         : ./a.out -e <option> <text> <filename>\n");
            printf("    📝 2.1  -t  -> Edit song title\n");
            printf("    🎤 2.2  -a  -> Edit artist name\n");
            printf("    💽 2.3  -A  -> Edit album name\n");
            printf("    📅 2.4  -y  -> Edit year\n");
            printf("    📃 2.5  -m  -> Edit genre/content type\n");
            printf("    💬 2.6  -c  -> Edit comment\n\n");
            return 0;
        }
        else{
            printf("❗ Usage for viewing     : ./a.out -v <filename>\n");
            printf("❗ Usage for editing     : ./a.out -e -t/-a/-A/-m/-y/-c <changing_text> <filename>\n");
            printf("❓ For help              : ./a.out --help\n");
        }
    }

    // Check for insufficient arguments
    if (argc < 2)
    {
        printf("❗ Usage for viewing     : ./a.out -v <filename>\n");
        printf("❗ Usage for editing     : ./a.out -e -t/-a/-A/-m/-y/-c <changing_text> <filename>\n");
        printf("❓ For help              : ./a.out --help\n");
        return 1;
    }
    if (argc > 5)
    {
        // Warn user about multiple space arguments without quotes
        printf("❗ If your data contains multiple spaces, wrap it in double quotes.\n");
        return 1;
    }

    
    // Determine operation type
    OperationType op_type = check_operation_type((const char **)argv);

    // Handle view operation
    if (op_type == view)
    {
        printf("view\n");
        if (argc <= 3)
        {
            // Validate view arguments
            if (read_and_validate((const char **)argv) == failure)
            {
                printf("❌ Error : Validation failed\n\n");
                printf("❗ Usage for viewing     : ./a.out -v <filename>\n");
                printf("❗ Usage for editing     : ./a.out -e -t/-a/-A/-m/-y/-c <changing_text> <filename>\n");
                printf("❓ For help              : ./a.out --help\n");
                return 1;
            }
                
            // Copy source filename
            audiofile.src_filename = strdup(argv[2]);

            // Check if file is valid MP3
            if (check_file(&audiofile) == failure)
                return 1;

            // Display tag data
            if (view_data(&audiofile) == failure)
                return 1;

            printf("\n🎉 View operation completed successfully\n\n");
        }
        else
        {
            printf("🚫 Error: Invalid view command\n");
            printf("💡 Correct usage: ./a.out -v <filename>\n");
            return 1;
        }
    }
    // Handle edit operation
    else if (op_type == edit)
    {
        if (argc == 5)
        {
            // Validate edit arguments
            if (read_and_validate_edit_args(argv, &audiofile) == failure)
            {
                printf("❌ Error: Invalid edit arguments provided\n");
                return 1;
            }
            printf("✅ Edit arguments validated successfully\n");

            // Check if the file is valid
            if (check_file(&audiofile) == failure)
            {
                printf("❌ Error: '%s' is not a valid MP3 file\n", audiofile.src_filename);
                return 1;
            }

            printf("✅ MP3 validation successful: '%s'\n", audiofile.src_filename);

            // Perform editing
            if (do_editing(&audiofile) == failure)
            {
                printf("❌ Error: Failed to edit MP3 metadata\n");
                return 1;
            }

            // Reassign and display updated data
            audiofile.src_filename = audiofile.original_name;
            check_file(&audiofile);
            if (view_data(&audiofile) == failure)
            {
                printf("❌ Error: Failed to display updated metadata\n");
                return 1;
            }
        }
        else
        {
            printf("🚫 Error: Invalid edit command\n");
            printf("❗ Usage: ./a.out -e -t/-a/-A/-m/-y/-c <changing_text> <filename>\n");
            return 1;
        }

        printf("\n🎉 Successfully updated MP3 file: '%s'\n\n", audiofile.original_name);
    }
    else
    {
        // Unsupported option
        printf("❗ Error: Unsupported operation requested\n");
        printf("💡 Use './a.out --help' to see valid options\n");
        return 1;
    }

    return 0;
}
