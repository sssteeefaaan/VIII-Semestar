#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int rank, size, *data_in, *data_out;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	data_in = (int*)malloc(sizeof(int) * size);
	data_out = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++)
		data_in[i] = i + rank * size;

	MPI_File fp;
	MPI_File_open(MPI_COMM_WORLD, "stefke", MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fp);
	MPI_File_seek(fp, sizeof(int) * size * rank, MPI_SEEK_SET);
	MPI_File_write_all(fp, data_in, size, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_close(&fp);

	free(data_in);
	
	MPI_Datatype tp;
	MPI_Type_vector(size, 1, size, MPI_INT, &tp);
	MPI_Type_commit(&tp);

	MPI_File_open(MPI_COMM_WORLD, "stefke", MPI_MODE_RDONLY, MPI_INFO_NULL, &fp);
	MPI_File_set_view(fp, sizeof(int) * rank, MPI_INT, tp, "native", MPI_INFO_NULL);
	MPI_File_read_all(fp, data_out, size, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_close(&fp);

	printf("P[%d]: data = |\t", rank);
	for (int i = 0; i < size; i++)
		printf("%d\t", data_out[i]);
	printf("|\n");
	fflush(stdout);

	free(data_out);

	MPI_Finalize();

	return 0;
}