#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

int stop = 0;

void *prover9_thread_function ( void *filename ) {
    pid_t id = fork ( );

    if ( id ) {
        int status;
        waitpid( id, &status, 0 );
        if ( WIFEXITED ( status ) )
            printf( "Unknot\n" );
        stop = 1;
    }
    else {
        int devNull = open ( "/dev/null", O_WRONLY );
        dup2( devNull, STDOUT_FILENO );
        dup2( devNull, STDERR_FILENO );
        execl ( "/usr/local/bin/prover9", "prover9", "-f", ( char * ) filename, NULL );
    }

    return NULL;
}

void *mace4_thread_function ( void *filename ) {
    pid_t id = fork ( );

    if ( id ) {
        int status;
        waitpid( id, &status, 0 );
        if ( WIFEXITED ( status ) )
            printf( "Knot\n" );
        stop = 2;
    }
    else {
        int devNull = open ( "/dev/null", O_WRONLY );
        dup2( devNull, STDOUT_FILENO );
        dup2( devNull, STDERR_FILENO );
        execl ( "/usr/local/bin/mace4", "mace4", "-f", ( char * ) filename, NULL );
    }

    return NULL;
}

int main ( int argc, char **argv ) {

    if ( argc != 2 ) {
        printf( "Usage: ./determine [knot_file]\n");
        return -1;
    }

    if ( access ( argv[1], F_OK ) ) {
        printf ( "%s ", argv[1] );
        printf ( ": No such file found\n" );
        return -2;
    }

    pthread_t prover_thread, mace_thread;

    pthread_create ( &prover_thread, NULL, prover9_thread_function, argv[1] );
    pthread_create ( &mace_thread, NULL, mace4_thread_function, argv[1] );

    while( !stop );

    return 0;
}