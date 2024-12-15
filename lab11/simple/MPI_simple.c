#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "mpi.h"

#define HOSTNAME_MAX 255

int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag, i; 
  MPI_Status status;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  
  char hostname[HOSTNAME_MAX];
  gethostname(hostname, HOSTNAME_MAX);
  
  if(size>1){
    
    if( rank != 0 ){ dest=0; tag=0; 
      MPI_Send( hostname, HOSTNAME_MAX, MPI_CHAR, dest, tag, MPI_COMM_WORLD );      
    } else {
      char recvhostname[HOSTNAME_MAX];

      for( i=1; i<size; i++ ) { 
	      MPI_Recv( recvhostname, HOSTNAME_MAX, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        printf("Proces %d wysłał hostname: %s\n", status.MPI_SOURCE, recvhostname);
      }      
    }

  }
  else{
	printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  
  MPI_Finalize(); 
  
  return(0);

}

