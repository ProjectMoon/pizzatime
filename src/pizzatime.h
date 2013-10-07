#ifndef PIZZA_TIME_H
#define PIZZA_TIME_H

#include "mpi.h"

/*
 * boys - this is a very manly function.
 */
void pt_boys(MPI_Comm room, MPI_Group boys, int rank);

/*
 * girls - this is a strong, independent functin.
 */
void pt_girls(MPI_Comm room, MPI_Group girls, int rank);

/*
 * assert_agreement - do all members agree on the party overhead?
 */
void pt_assert_agreement(int overheads[], int members);
/*
 * random - Generate a random number.
 */
int random(int min, int max);
#endif
