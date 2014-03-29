Name:Ravikumar Jeyaraman
USC ID:9245816387

Compilation Details:

Extract the zip
type make in extracted directory

or use

g++ -o search main_assign1.cpp


Implementation Details:

Graph is created with following structure

Class Vertex {}
Class Edge {}
Class Graph {}

Graph class will have array of vertex objects and each vertex will have array of edges object.

For Greedy I have used priority queue to find next best move.

For Minimax and Pruning I have followed textbook psedocode with minor modifications.

All these functions are defined as memeber functions in Graph class.

Analysis:

Greedy gives us the best performance as number of iteration for greedy is very small which is equal to number of steps to finish the game.
But greedy wont guarantee the winning move and you may end up losing the game by following greedy method.

Minimax looks ahead more steps before making move to see best possible result. As it looks ahead number of iterations increases dramatically.It is the slowest of the three and running time increases with the depth.And also possibility of the winning move increases with the depth. So trade off between running time vs winning move 

Alpha-beta pruning basically works on Minimax except that it prunes unneccessary steps.It wont alter the output of Minimax. But Pruning helps to reduce number of steps drastically. So Alpha-beta pruning has same advantage as Minimax. More the depth more the winning possibilty but faster than minimax by pruning unwanted steps.

So at the end of the day my bet will be on Alpha beta pruning:).






