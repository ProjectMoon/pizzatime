/*
 * Split 4 guys and 4 girls into 2 groups. 8 nodes required to run.
 * Each group will rely on their leader to tell them how long it is
 * taking them to eat pizza, and then they eventually check back to
 * see how in agreement they are.
 *
 * Boys - One party guy who does random longer tasks every other iteration.
 * But he also only tells the other guys the party overhead every other
 * iteration. They will in total take around 18-19 sec due to party guy's
 * extra overhead.
 *
 * Girls - Twice as chatty than the guys' room. Also apparently very adept
 * at speed-eating pizza. They will always take 10 seconds because they are
 * only eating pizza. The chief of the girls tells the others the current
 * overhead each iteration.
 *
 * At the end, all group members report their overhead findings and then
 * use MPI_Gather to send these findings back to the group's master proc.
 * The master process then asserts that they all agree (Which they always
 * do--perhaps drunkenness feature will be added in v2.0).
 */
#include "pizzatime.h"
#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
	int rank;

	int numPeople = 8;
	int boys[4] = { 0, 1, 2, 3 };
	int girls[4] = { 4, 5, 6, 7 };
	
	MPI_Init(&argc, &argv);
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
		pt_boys(room, people, myrank);
	}
	else {
		pt_girls(room, people, myrank);
	}

	MPI_Finalize();
}

void pt_boys(MPI_Comm room, MPI_Group boys, int rank) {
	int partyOverhead = 0;
	int buf = 0;

	srand(time(NULL));
	for (int c = 0; c < 10; c++) {
		int overhead = 0;
		if (rank == 0 && c % 2 == 0) {
			//party guy does random large task
			overhead = random(3, 5);
		}
		else {
			//party guy or everyone else eats a pizza.
			overhead = 1;
		}

		sleep(overhead);
		
		//party guy informs others of time taken and record our own
		//but only half as much as the girls
		if (c % 2 == 0) {
			if (rank == 0) buf = overhead;
			MPI_Bcast(&buf, 1, MPI_INT, 0, room);
			partyOverhead += buf;
		}
		
		MPI_Barrier(room);
	}
	
	MPI_Barrier(room);

	//all boys should agree on the total party overhead.
	printf("b%d - says total overhead is: %d\n", rank, partyOverhead);

	int results[4];
	MPI_Gather(&partyOverhead, 1, MPI_INT, results, 1, MPI_INT, 0, room);
	if (rank == 0) {
		pt_assert_agreement(results, 4);
	}
}

void pt_girls(MPI_Comm room, MPI_Group girls, int rank) {
	int partyOverhead = 0;
	int buf = 0;
	for (int c = 0; c < 10; c++) {
		//everyone eats a pizza.
		sleep(1);
		
	    //chief girl informs others of time taken and record our own
		//she is more organized and thus actually records her own time
		//instead of broadcoasting it loudly before recording it like
		//the boys.
		if (rank == 0) {
			partyOverhead++;
			buf = 1;
		}
		
		MPI_Bcast(&buf, 1, MPI_INT, 0, room);
		if (rank != 0) partyOverhead += buf;
		MPI_Barrier(room);
	}
	
	MPI_Barrier(room);
	
	printf("g%d - says total overhead is: %d\n", rank, partyOverhead);
	
	int results[4];
	MPI_Gather(&partyOverhead, 1, MPI_INT, results, 1, MPI_INT, 0, room);
	if (rank == 0) {
		pt_assert_agreement(results, 4);
	}
}

void pt_assert_agreement(int overheads[], int members) {
	bool agreement = true;
	for (int c = 0; c < members; c++) {
		for (int x = 0; x < members; x++) {
			if (overheads[c] != overheads[x]) {
				agreement = false;
				break;
			}
		}

		if (agreement == false) break;
	}

	if (agreement == true) {
		printf("members of group do agree on overheads!\n!");
	}
	else {
		printf("members of group do not agree on overheads!\n");
	}
}

int random(int min, int max) {
	//http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range-c
	int base_random = rand(); /* in [0, RAND_MAX] */
	if (RAND_MAX == base_random) return random(min, max);
  /* now guaranteed to be in [0, RAND_MAX) */
	int range       = max - min,
		remainder   = RAND_MAX % range,
		bucket      = RAND_MAX / range;
	/* There are range buckets, plus one smaller interval
	   within remainder of RAND_MAX */
	if (base_random < RAND_MAX - remainder) {
		return min + base_random/bucket;
	}
	else {
		return random(min, max);
	}
}
