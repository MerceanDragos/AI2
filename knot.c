#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define TREFOIL_KNOT { 1, 2, 3, 1, 2, 3, 1 }
#define CINQUEFOIL_KNOT { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1 }
#define SEPTAFOIL_KNOT { 1, 2, 3, 4, 5, 6, 7, 1, 2 ,3, 4, 5, 6, 7, 1 }

int validate_knot ( int knot[], int knot_arc_no ) {

    if ( knot_arc_no < 0 )
        return 1;
    else if ( knot_arc_no == 0 )
        return 0;

    int knot_length = knot_arc_no * 2 + 1;

    if ( knot[0] != knot[knot_length - 1] )
        return 1;


    int *arcs = ( int * ) calloc ( knot_arc_no, sizeof ( int ) );
    for ( int i = 0; i*2 < knot_length - 1; i++ ) {
        arcs[i] = knot[i*2];
        
        for ( int j = i - 1; j >= 0; j-- )
            if ( arcs[i] == arcs[j] ) {
                free ( arcs );
                return 2;
            }
    }

    for ( int i = 0; i * 2 + 1 < knot_length; i++ ) {

        int arc = knot[i * 2 + 1];
        
        int j = 0;
        for ( ; j < knot_arc_no; j++ )
            if ( arc == arcs[j] )
                break;

        if ( j == knot_arc_no ) {
            free ( arcs );
            return 3;
        }
    }

    free ( arcs );
    return 0;
}

int write_knot_to_file ( int file_descriptor, int knot[], int knot_arc_no ) {

    for ( int i = 0; i < knot_arc_no * 2 - 1; i += 2 ) {

        int length = 4 + 3 + 3 + 2;

        char statement[50] = "";
        char arc1[10];
        char arc2[10];
        char arc3[10];

        length += sprintf ( arc1, "%d", knot[i] );
        length += sprintf ( arc2, "%d", knot[i+1] );
        length += sprintf ( arc3, "%d", knot[i+2] );

        strcat ( statement, "    " );
        strcat ( statement, arc1 );
        strcat ( statement, " * " );
        strcat ( statement, arc2 );
        strcat ( statement, " = " );
        strcat ( statement, arc3 );
        strcat ( statement, ".\n" );

        if ( length != write ( file_descriptor, statement, length ) )
            return 1;
    }
    
    return 0;
}

int write_goal_to_file ( int file_descriptor, int knot[], int knot_arc_no ) {

    for ( int i = 0; i < knot_arc_no * 2 - 1; i += 2 ) {

        char goal[50] = "";
        int length = 4 + 2 + 3 + 2;
        char arc1[10];
        char arc2[10];

        length += sprintf ( arc1, "%d", knot[i] );
        length += sprintf ( arc2, "%d", knot[i + 2] );

        strcat ( goal, "    " );
        strcat ( goal, "( " );
        strcat ( goal, arc1 );
        strcat ( goal, " = " );
        strcat ( goal, arc2 );
        strcat ( goal, " )" );

        if ( i == knot_arc_no * 2 - 2 ) {
            length += 2;
            strcat ( goal, ".\n");
        }
        else {
            length += 3;
            strcat ( goal, " &\n");
        }

        if ( length != write ( file_descriptor, goal, length ) )
            return 1;
    }

    return 0;
}

unsigned int generate_knot_file ( char path[], int knot[], int knot_arc_no ) {

    unsigned int status = 0;

    if ( validate_knot ( knot, knot_arc_no ) ) {
        fprintf ( stderr, "Error: Invalid knot!\n" );
        return status = -1;
    }

    int file_descriptor = open ( path, O_WRONLY | O_CREAT, 0666 );

    char head[] =
        "formulas ( assumptions ).\n"
        "\n"
        "    x * x = x.\n"
        "    ( x * y ) * y = x.\n"
        "    ( x * z ) * ( y * z ) = ( x * y ) * z.\n"
        "\n";

    char body[] =
        "\n"
        "end_of_list.\n"
        "\n"
        "formulas(goals).\n"
        "\n";

    char tail[] =
        "\n"
        "end_of_list.\n";

    int head_size = sizeof ( head ) - 1;
    int body_size = sizeof ( body ) - 1;
    int tail_size = sizeof ( tail ) - 1;

    if ( head_size != write ( file_descriptor, head, head_size ) ) {
        fprintf ( stderr, "Failed writing head of knot file:\n");
        perror ( "    Error: " );
        status |= 1;
    }

    if ( write_knot_to_file ( file_descriptor, knot, knot_arc_no ) ) {
        fprintf ( stderr, "Failed to write statements!\n" );
        status |= 2;
    }

    if ( body_size != write ( file_descriptor, body, body_size ) ) {
        fprintf ( stderr, "Failed writing body of knot file:\n");
        perror ( "    Error: " );
        status |= 4;
    }

    if ( write_goal_to_file ( file_descriptor, knot, knot_arc_no ) ) {
        fprintf ( stderr, "Failed to write goal!\n" );
        status |= 8;
    }

    if ( tail_size != write ( file_descriptor, tail, tail_size ) ) {
        fprintf ( stderr, "Failed writing tail of knot file:\n");
        perror ( "    Error: " );
        status |= 16;
    }

    close ( file_descriptor );
    return status;

}

int main ( ) {

    int knot[] = TREFOIL_KNOT;
    int knot_arc_no = ( sizeof ( knot ) / sizeof ( int ) - 1 ) / 2;

    return generate_knot_file ( "file.in", knot, knot_arc_no );
}