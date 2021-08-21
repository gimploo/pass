#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG

#include "../lib/dbg/dbg.h"
dbg_t debug = {0};

#endif

#include "../lib/str.h"
#include "../lib/csv.h"

const char* usage = 
"USAGE: %s file.csv\n\n"
"Flags:\n"
"   --silent        prints only the password to stdout\n"
"   --first         prints all occurance to stdout\n"
"\n";



void parse_csv_get_first_occurance(CSV *csv, bool flag)
{
    if (csv == NULL) return ;

    char user_input[KB];
    memset(user_input, 0, KB);
    printf("[?] Enter website name: ");
    scanf("%s", user_input);

    if (strcmp(user_input, "/quit") == 0) return ;

    str_t find = new_str(user_input);

    size_t line_num = csv_get_line_num_of_string_restricted_to_a_header_field(csv, 1, &find);
    if (line_num == 0) return ;

    Row *row = csv_get_row_from_line_number(csv, line_num);
    assert(row);

    Header *header = &csv->header;
    assert(header);
        
    for (size_t i = 0; i < csv->header.header_count && flag == false; i++)
    {
        printf("[!] "STR_FMT": \t"STR_FMT"\n", 
                STR_ARG(header->header[i]), 
                STR_ARG(row->buffer[i]));
    }

    if (flag == true) {
        str_print(row->buffer[header->header_count - 1]);
        printf("\n");
    }

    csv_row_destroy(row);
}

void parse_csv_get_all_occurance(CSV *csv, bool flag)
{
    if (csv == NULL) return ;

    char user_input[KB];
    memset(user_input, 0, KB);
    printf("[?] Enter website name: ");
    scanf("%s", user_input);

    if (strcmp(user_input, "/quit") == 0) return ;

    str_t find = new_str(user_input);

    size_t line_num_array[csv->entries.entry_count];
    stack_t stack = stack_init((void **)line_num_array, csv->entries.entry_count);
    csv_get_all_line_nums_of_string_restricted_to_a_header_field(csv, &stack, 1, &find);


    size_t line_num = 0;
    for_i_in_stack(&stack)
    {
        line_num = line_num_array[i];

        Row *row = csv_get_row_from_line_number(csv, line_num);
        assert(row);

        Header *header = &csv->header;
        assert(header);
            
        printf("\n");
        for (size_t i = 0; i < csv->header.header_count && flag == false; i++)
        {
            printf("[!] "STR_FMT": \t"STR_FMT"\n", 
                    STR_ARG(header->header[i]), 
                    STR_ARG(row->buffer[i]));
        }

        if (flag == true) {
            str_print(row->buffer[header->header_count - 1]);
            printf("\n");
        }

        csv_row_destroy(row);
    }
}


int main(int argc, char *argv[])
{

#ifdef DEBUG

    dbg_init("debug.info");

#endif

    bool flag = false;

    if (argc < 2 ) {

        fprintf(stderr, usage, argv[0]);
        exit(1);

    } else if (argc == 3) {

        if (strcmp(argv[1], "--silent") == 0) {
            fprintf(stderr, usage, argv[0]);
            exit(1);
        } else if (strcmp(argv[2], "--silent") == 0) 
            flag = true;
    }

    char *csv_file_path = argv[1];
    str_t *csv_file     = str_read_file_to_str(csv_file_path);
    CSV csv             = csv_init(csv_file);

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--first") == 0) {
            parse_csv_get_first_occurance(&csv, flag);
            break;
        } else { 
            parse_csv_get_all_occurance(&csv, flag);
            break;
        }
    }

    csv_destroy(&csv);

#ifdef DEBUG

    dbg_destroy();

#endif //DEBUG

    return 0;
}

