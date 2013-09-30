//two pizzas in two rooms = two mpi communicators
//question: "Guys/ladies exchange number of salami/piece" does this mean only
//          in that specific communicator? or do we need intercommunicators

//guy room:
//1 party guy, who randomly gets call, opens door, provides drinks, or eats.
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
//salami? need clarification
