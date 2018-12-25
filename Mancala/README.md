# MiniMax algorithm
The player is maximizer, his opponent is minimizer. The search tree starts with the player -> Max, the next level his opponent -> Min and then again Max in the next level.
Now from a node we generate the successors(the possible moves) and traverse them till a leaf node is found(state where the game is finished). The terminal state or leaf returns a value. The node that is "Min" will select the minimum value it gets, the "Max" will select the maximum value it acquired. Then finally from the root node which is a "Max" will decide the child to traverse(move to make in the current circumstances).

Max-> the player would try to maximize his score
Min-> the opponent would try to minimize the effort of the player thus select min

# Alpha-Beta-Pruning
The whole game tree searching in MiniMax algorithm would take exponential time and memory thus require some optimizations to do. In alpha-beta pruning each node is assigned with two variables <b>alpha</b> and <b>beta</b>. alpha is the maximizer and beta is the minimizer. if a node in "Min" then it updates its "beta" when it gets any value returned by its children, for a "Max" it updates "alpha". The update is like this->

<b>for beta:</b>
x is acquired from children
best_val = min(best_val, x)
beta = min(beta, best_val)

the beta in a node is used to decide whether to explore and expand the next child of the current node or not(not means prune)

it returns best value to its parent that is the min value it acquired

<b>for alpha:</b>
x is acquired from children
best_val = max(best_val, x)
alpha = max(alpha, best_val)

the alpha in a node is used to decide whether to explore and expand the next child of the current node or not(not means prune)

it returns best value to its parent that is the max value it acquired

***in any node if alpha>=beta then we prune that portion

another point to be noted is, <b>the value "Min" returns is compared to alpha the maximizer and the value "Max" returns is compared to beta the minimizer</b>

It can be explained like this-> suppose a "Min" node returned value to "Max", "Max" compares it with alpha because <b>the value returned by "Min" nodes is the maximum a "Max" node can get</b>
same for the beta.

# how the pruning works
                 Z
                 |
     -----------------------	  
       |         |       |
       A         B       C
     -----     -----   -----
     | | |     | | |   | | |
     3 5 10    2 a b   2 7 3

Z->(-inf,inf)
A->(-inf,inf) -> min(3,5,10) = 3 so (alpha,beta) will be now (-inf,3) -> beta is updated as A is Min. 3 is returned

Z->(3,inf) as max(-inf,3)=3 and Z is Max
B-> (3,inf)
now we first get 2 from the first child of B. best_value=2, beta=2. now the other child a,b need not to be checked as whatever the value is, finally we will be returning the a value c that is <=2. The value will be compared with alpha of Z, but we can see that Z the maximizer already have alpha=3 which is greater than 2, so whatever value a,b gives, the value returned and beta at B will be <= 2 thus never needed. So we are not checking subtree of 'a' and 'b' thus pruned

https://www.hackerearth.com/blog/artificial-intelligence/minimax-algorithm-alpha-beta-pruning/
