#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG

#include "../lib/dbg/dbg.h"
dbg_t debug = {0};

#endif

#include "../lib/str.h"
#include "../lib/csv.h"

const char* usage = "USAGE: %s file.csv\n\n"
                    "Flags:\n"
                    "   --silent        prints only the password to stdout\n\n";





void parse_csv_file(CSV *csv, bool flag)
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

    Row row = csv_get_row_from_line_number(csv, line_num);

    Header *header = &csv->header;
    assert(header);
        
    for (size_t i = 0; i < csv->header.header_count && flag == false; i++)
    {
        printf("[!] "STR_FMT": \t"STR_FMT"\n", 
                STR_ARG(header->header[i]), 
                STR_ARG(row.buffer[i]));
    }

    if (flag == true) {
        str_print(row.buffer[header->header_count - 1]);
        printf("\n");
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

    str_t csv_file = str_read_file_to_str(csv_file_path);

    CSV csv = csv_init(&csv_file);

    parse_csv_file(&csv, flag);




cleanup:

    csv_destroy(&csv);

#ifdef DEBUG

    dbg_destroy();

#endif //DEBUG

    return 0;
}

