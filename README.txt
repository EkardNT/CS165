TEAM:
	Drake Tetreault, ID 35571095
	Maymunah Khafateh, ID 53332812
	
USAGE:
	The program takes one optional command line argument.
		- If no arguments provided, the random seed is set to the result of calling std::time().
		- If one integer argument is provided, the random seed is set to that value.
		- Non-integer arguments are rejected and the random seed is set to the result of calling std::time().

ALGORITHM/DATA STRUCTURE:
	We use a binary search tree to hold the indices of the k greatest elements. In addition to the standard root pointer, we also maintain a pointer to the least index. When the tree's size equals k, new elements are compared against the least element before being inserted, so that the tree only contains the k greatest elements.
	
	Once all indices have been inserted, we traverse the tree in reverse order, adding the indices at each node to the Best[] array.
	
	We initialized memoized our comparisons so that no two pair of indices was ever submitted to COMPARE() more than once. We later augmented this memoization technique with transitive proofs, where for example if we found that a > b and b > c, we then marked a > c without having to call COMPARE(). However, upon further analysis we realized that both our memoization and proof techniques experienced 0% hit rates due to how we implemented the main part of our algorithm, so we removed these techniques, subsequently greatly improving our algorithm's speed.
	
TIME ANALYSIS:
	It is immediately apparent that our algorithm is at least O(n), because we insert each of the n indices into our binary search tree. Normally each insertion into the binary search tree would be an O(log n) comparison operation, however our binary search tree is restricted in size to no greater than k, so each insertion is an O(log k) operation. After all indices have been added to the binary search tree, we traverse the entire tree in reverse order to add the greatest k elements to the Best array. Thus, the asymptotic time complexity is O(n * log(k) + k).

TIMING FUNCTION:
	One function that roughly models the time (in milliseconds) required for our algorithm to complete (per 1000 runs) is given below. We did not have access to curve fitting software that could handle two independent variables, so the constant factors in the equation below gave us the least error after much manual tweaking in MS Excel.
	
	time(n, k) = 0.0039 * n * log2(k) - 0.005 * k

SPACE ANALYSIS:
	Our solution requires Θ(k) space, both for the output Best array and the binary search tree. This is because the Best array and the binary search tree both end up with exactly k elements.