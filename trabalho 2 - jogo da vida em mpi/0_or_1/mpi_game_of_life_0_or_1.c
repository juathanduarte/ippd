#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 20

void initialize_board(int board[][N]){
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      board[i][j] = rand() % 2;
    }
  }
}

int count_neighbors(int board[][N], int x, int y){
  int count = 0;
  for (int i = x - 1; i <= x + 1; i++){
    for (int j = y - 1; j <= y + 1; j++){
      if ((i != x || j != y) && i >= 0 && j >= 0 && i < N && j < N){
        count += board[(i + N) % N][(j + N) % N];
      }
    }
  }
  return count;
}

int evolve_cell(int board[][N], int x, int y){
  int neighbors = count_neighbors(board, x, y);
  if (board[x][y] == 1){
    if (neighbors < 2 || neighbors > 3){
      return 0;
    } else {
      return 1;
    }
  } else {
    if (neighbors == 3) {
      return 1;
    } else {
      return 0;
    }
  }
}

void evolve_generation(int local_board[][N], int rank, int size){
  int temp_board[N][N];

  memcpy(temp_board, local_board, sizeof(int) * N * N);

  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      temp_board[i][j] = evolve_cell(local_board, i, j);
    }
  }

  memcpy(local_board, temp_board, sizeof(int) * N * N);
}

void print_board(int board[][N]){
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      printf("%d ", board[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]){
  int rank, size;
  int local_board[N][N];
  int global_board[N][N];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
  }

  MPI_Scatter(global_board, N * N / size, MPI_INT, local_board, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

  initialize_board(local_board);

  int max_generations = 10;

  double start_time = MPI_Wtime();

  for (int generation = 0; generation < max_generations; generation++){
    evolve_generation(local_board, rank, size);

    MPI_Gather(local_board, N * N / size, MPI_INT, global_board, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0){
      printf("\nGeracao %d\n", generation + 1);
      print_board(global_board);
      usleep(300000);
    }
  }

  double end_time = MPI_Wtime();
  double elapsed_time = end_time - start_time;

  if (rank == 0){
    printf("Tempo de execucao: %f segundos\n", elapsed_time);
  }

  MPI_Finalize();
  return 0;
}
