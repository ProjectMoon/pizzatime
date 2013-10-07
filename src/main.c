//http://static.msi.umn.edu/tutorial/scicomp/general/MPI/communicator.html

//two pizzas in two rooms = two mpi communicators

//guy room:
//1 party guy, who randomly gets call, opens door, provides drinks, or eats.
//   model with sleep calls
//X others, who only eat.
//once all have eaten all the pizza, they are done.

//girl room:
//X girls, who only eat.
//once all have eaten the pizza, they are done.

//eating the pizza:
//the pizza has x number of pieces
//each piece takes 7 bites to eat fully

//implementing exchange number requirement:
//when someone takes a piece, they tell others not to take it. (aka locking)
//salami: this is communication within the communicator
#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	int rank;

	//later replace this with argv and possibly
	//non-equal amounts of boys and girls
	int numPeople = 8;
	int boys[4] = { 0, 1, 2, 3 };
	int girls[4] = { 4, 5, 6, 7 };
	
	MPI_Init(&argc, &argv);
	//MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Group world, people;
	MPI_Comm_group(MPI_COMM_WORLD, &world);

	bool boyGroup = false;
	if (rank < numPeople / 2) {
		MPI_Group_incl(world, 4, boys, &people);
		boyGroup = true;
	}
	else {
		MPI_Group_incl(world, 4, girls, &people);
		boyGroup = false;
	}

	MPI_Comm room;
	MPI_Comm_create(MPI_COMM_WORLD, people, &room);

	int myrank;
	MPI_Comm_rank(room, &myrank);

	if (boyGroup == true) {
		printf("boys [%d] hi there\n", myrank);
	}
	else {
		printf("girls [%d] hi there\n", myrank);
	}

	MPI_Finalize();
}
