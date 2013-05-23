
#include <mpi.h>

#include <iostream>

#include "../game/board.hpp"
#include "../game/util.hpp"

int get_descendants(board* b,int depth,board* output){
  int offset = 0,i,imax;
  board children[TOTAL_FIELDS/2];
  if(depth==1){
    b->get_children(output,&offset);
    return offset;
  }
  else{
    b->get_children(&children,&imax);
    offset = 0;
    for(i=0;i<imax;i++){
      offset += get_descendants(children+i,depth-1,output+offset);
    }
    return offset;
  }
}



int main(int argc,char** argv){
  int error,process_count,process_id;
  board b,depth,*buffer;
  
  // Initialize MPI.
  error = MPI_Init(&argc,&argv);
  if(error != MPI_SUCCESS){
    std::cout << "Error with initializing MPI" << std::endl;
    exit(1);
  }
  
  // Get the number of processes.
  error = MPI_Comm_size(MPI_COMM_WORLD, &process_count);
  
  // Get the individual process ID.
  error = MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
  
  if(process_id == 0){
    if(argc!=3){
      std::cout << "I need exactly 2 arguments!\n";
      exit(1);
    }
    if(FIELD_SIZE != 8){
      std::cout << "FIELD_SIZE should be 8 \n";
      exit(1);
    }
    
    b = board(argv[1]);
    depth = argv[2];
  }
  
  buffer = new board[160000];
  get_descendants(&b,max(4,depth),buffer);
  
}
