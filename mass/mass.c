/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "mass.tab.h"
#include "mass.yy.h"
#include "mass.h"
#include "bingen.h"
#include "symbol.h"

const char *help_msg = 
    "This is " NAME " by " AUTHOR "\n"
    "Usage: mass [options] file\n"
    "Options:\n"
    "  -Bb                  Use a byte order of big endian\n"
    "  -Bl                  Use a byte order of little endian(default)\n"
    "  -D                   Debug mode, output binary to stdout\n"
    "  -h                   Display this help message\n"
    "  -o <file>            Place the output into <file>(the default is `a.out')\n"
    "  -v                   Show version infomation\n"
    "\n"
    "For detailed information, see the README file.\n"
    ;
const char *version_msg = 
    "This is " NAME " by " AUTHOR "\n"
    "release " RELEASE ", version " VERSION "\n"
    ;
const char *optstr = "-B:DWo:hv";

int option_byteorder = 0;
int option_output_binary = 0;
int option_warning = 1;
char *option_input_file = NULL;
char *option_output_file = NULL;


void errmsg(char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    return;
}

int yyerror(char *msg)
{
    errmsg("%s: %d: Error: %s at '%s'\n", option_input_file, yyget_lineno(), msg, yytext);
    return 0;
}

static void option_init(int argc, char **argv)
{
    int optchr;
    while ((optchr = getopt(argc, argv, optstr)) != -1)
    {
        switch (optchr)
        {
        case 1: /* those not an option, treating as input files */
            if (!option_input_file)
                option_input_file = optarg; /* lifetime of optarg would be long enough */
            else
            {
            errmsg(
                "mass: error: multiple input file specified\n"
                "Usage: mass [options] file\n"
                "Try `mass -h' for detailed help.\n"
                );
            exit(0);
            }
            break;
        case 'B': /* byte orders */ 
            if (!strcmp(optarg, "b"))
            {
                if (!option_byteorder)
                    option_byteorder = BYTEORDER_BE;
                else
                    option_byteorder = BAD_OPTION;
            }
            else if (!strcmp(optarg, "l"))
            {
                if (!option_byteorder)
                    option_byteorder = BYTEORDER_LE;
                else
                    option_byteorder = BAD_OPTION;
            }
            else
                goto HELP_MSG_AND_EXIT;
            if (option_byteorder == BAD_OPTION)
            {
                errmsg("mass: error: multiple byteorders specified\n");
                exit(0);
            }
            break;
        case 'D': /* show debugging message */
            option_output_binary = 1;
            break;
        case 'W': /* show warnings */
            option_warning = 1;
            break;
        case 'o': /* output file */
            option_output_file = optarg; /* lifetime of optarg would be long enough */
            break;
        case 'v': /* version info */
            fprintf(stdout, version_msg);
            exit(0);            
            break;
        case 'h': /* help message */
        case '?':
        HELP_MSG_AND_EXIT:
            fprintf(stdout, help_msg);
            exit(0);
            break;
        default:
            /* the execution shall not reach this line */
            break;
        }        
    }
    return;
}

static void mass_init(int argc, char **argv)
{
    option_init(argc, argv);
    
    if (!option_output_file)
        option_output_file = "a.out";
        
    if (!option_input_file)
    {
        errmsg("mass: error: no input file specified\n");
        exit(0);
    }
    
    FILE *tmp_in = fopen(option_input_file, "r");
    if (!tmp_in)
    {
        errmsg("mass: error: %s: %s\n", option_input_file, strerror(errno));
        exit(0);
    }
    yyset_in(tmp_in);

    symbol_table_init();
    buf_init();
    return;
}

static void mass_fini()
{
    write_back_label_addr_all();
    
    FILE *output = fopen(option_output_file, "wb");
    if (!output)
    {
        errmsg("mass: error: %s: %s\n", option_output_file, strerror(errno));
        exit(0);
    }
    
    buf_dump_to_file(output);
    fclose(output);
    symbol_table_fini();
    buf_fini();
    return;
}

int main(int argc, char **argv, char **env)
{
    mass_init(argc, argv);
    yyparse();
    mass_fini();
    return 0;
}
