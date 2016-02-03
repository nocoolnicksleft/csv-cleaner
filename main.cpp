/* 
 * File:   main.cpp
 * Author: bseip
 *
 * Created on May 26, 2014, 4:51 PM
 */

#include "csv.h" 

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Intermediate data structure
 */
typedef struct {
    string ColA;
    string ColB;
    string ColC;
    string ColD;
    string ColE;
    string ColF;
    string ColG;
} columns;

/**
 * Current lexer data
 */
yylval_t yylval;

/**
 * Parameter and option data
 */
struct CSVREADERPARMS {
    bool optQuiet;
    bool optHelp;
    bool optVerbose;
    bool optExtendedStatistics;
    bool optAddLineNo;
    char Separator[2];
    char Sourcefile[FILENAME_MAX];
    char Outfile[FILENAME_MAX];

} Parm;

/**
 * Initialize Parm struct to default values
 */
void parminit() {
    Parm.optHelp = 0;
    Parm.optQuiet = 0;
    Parm.optVerbose = 0;
    Parm.optExtendedStatistics = 0;
    Parm.optAddLineNo = 0;
    strncpy(Parm.Separator,",",1);
    strncpy(Parm.Sourcefile, "data.csv", FILENAME_MAX);
    strncpy(Parm.Outfile, "data.cleaned.csv", FILENAME_MAX);
}

/**
 * Print syntax information to stdout
 */
void showsyntax(void) {
    printf("\ncsvreader1 csv file converter 1.0\n");
    printf("by Bjoern Seip (c) 2014\n\n");

    printf("Usage:    csvreader1 [options] sourcefile\n");

    printf("Options: -e        print extended statistics\n");
    printf("         -i [file] set input file name\n");
    printf("         -o [file] set output file name\n");
    printf("         -s [char] output field separator (quotes may be required)\n");
    printf("         -q        quiet operation\n");
    printf("         -v        print verbose information\n");
    printf("         -h        print this\n\n");

    printf("Default Operation:\n");
    printf("          Read [sourcefile], output cleaned to [sourcefile].cleaned.csv,\n");
    printf("          Print statistics to stdout\n");
}

/**
 * Check for additional parameter after option tag
 * 
 * @param argc
 * @param i
 * @param next
 * @param message
 * @return 
 */
int check2args(int argc, int i, const char *next, const char *message) {
    if (argc == (i + 1)) {
        if (!Parm.optQuiet) printf("%s\n", message);
        return (0);
    } else {
        if (next[0] == '-') {
            if (!Parm.optQuiet) printf("%s\n", message);
            return (0);
        }
    }
    return (1);
}

/**
 * Process command line options
 * 
 * @param argc
 * @param argv
 * @return 
 */
int processcmdline(int argc, char *argv[]) {
    int result = 1;

    parminit();

    for (int i = 1; i < argc; i++) {

        if ((argv[i][0] != '-') || (strlen(argv[i]) < 2)) {
            if (!Parm.optQuiet) printf("invalid argument %s\n", argv[i]);
            result = 0;
        } else

            switch (argv[i][1]) {

                case 'o':
                    if (check2args(argc, i, argv[i + 1], "-o must be followed by a file name")) {
                        strncpy(Parm.Outfile, argv[i + 1], FILENAME_MAX);
                        i++;
                    } else result = 0;
                    break;

                case 'i':
                    if (check2args(argc, i, argv[i + 1], "-i must be followed by a file name")) {
                        strncpy(Parm.Sourcefile, argv[i + 1], FILENAME_MAX);
                        i++;
                    } else result = 0;
                    break;
                    
                case 's':
                    if (check2args(argc, i, argv[i + 1], "-s must be followed by a single character (quotes may be required)")) {
                        Parm.Separator[0] = argv[i + 1][0];
                        Parm.Separator[1] = 0;
                        i++;
                    } else result = 0;
                    break;

                case 'e':
                    Parm.optExtendedStatistics = 1;
                    break;

                case 'q':
                    Parm.optQuiet = 1;
                    break;

                case 'v':
                    Parm.optVerbose = 1;
                    break;

                case 'h':
                    Parm.optHelp = 1;
                    break;

                case 'l':
                    Parm.optAddLineNo = 1;
                    break;
                    
                default:
                    if (!Parm.optQuiet) printf("invalid argument %s\n", argv[i]);
                    result = 0;

            }
    }

    return (result);
}

/*
 * Main routine
 */
int main(int argc, char** argv) {

    int current;

    int colno = 1;
    int linno = 1;

    char buf[MAX_STRING_LENGTH];

    processcmdline(argc, argv);

    if (Parm.optHelp) {
        showsyntax();
        return 0;
    }


    int colnomin = 1;
    int colnomax = 1;

    // vector<int> coltypes;

    vector< vector<string> > table;

    vector<string> * line = new vector<string>;

    vector<string>::const_iterator ix;
    vector< vector<string> >::const_iterator iy;

    yyin = fopen(Parm.Sourcefile, "r");

    if (!yyin) {
        printf("Input file %s not found.\n\n",Parm.Sourcefile);
        return 1;

    } else {

        while (current = yylex()) {
            switch (current) {
                case TOKEN_SEPARATOR:
                    printf("SEP ");
                    colno++;
                    break;
                case TOKEN_NEWLINE:
                    colnomin = min(colnomin, colno);
                    colnomax = max(colnomax, colno);
                    linno++;
                    printf("NL\n%d: ", linno);
                    colno = 1;
                    table.push_back(*line);
                    line = new vector<string>;
                    break;
                case TOKEN_STRING:
                    printf(" \"%s\" ", yylval.s);
                    line->push_back(*(new string(yylval.s)));
                    break;
                case TOKEN_UNQUOTED_STRING:
                    printf(" -%s - ", yylval.s);
                    line->push_back(*(new string(yylval.s)));
                    break;
                case TOKEN_INT:
                    printf("  %d  ", yylval.i);
                    sprintf(buf, "%d", yylval.i);
                    line->push_back(*(new string(buf)));
                    break;
                case TOKEN_FLOAT:
                    printf("  %f  ", yylval.f);
                    sprintf(buf, "%f", yylval.f);
                    line->push_back(*(new string(buf)));
                    break;
                default:
                    break;
            }
        }

        printf("\nColumns: %ld - %ld\n", colnomin, colnomax);
        
        linno = 1;
        for (iy=table.begin();iy!=table.end();++iy) {
            std::cout << linno << ';';
            for (ix=(*iy).begin();ix!=(*iy).end();++ix) {
                std::cout << '"' << (*ix) << '"' << Parm.Separator;
            }
            std::cout << std::endl;
            linno++;
        }
        

        return 0;
    }


}

