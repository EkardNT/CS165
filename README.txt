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
	The worst case number of comparisons our algorithm takes is:
		max_comparisons(n, k) = ∑[i = 1 -> k-1](i) + (n - k) * k;
		
	The first term comes from adding the first k elements to the binary search tree before it is full. The second term comes from adding the remaining (n - k) elements to the binary search tree with exactly k elements. In both terms we assume adding an element to the binary search tree is an O(k) operation because we use a simple non-self balancing version of the binary seach tree.
	
	In practice, adding an element to the binary search tree is O(log(k)) because elements are inserted in random shuffled order.

TIMING FUNCTION:
	One function that roughly models the time (in milliseconds) required for our algorithm to complete (per 1000 runs) is given below. We did not have access to curve fitting software that could handle two independent variables, so the constant factors in the equation below gave us the least error after much manual tweaking in MS Excel.
	
	time(n, k) = 0.0039 * n * log2(k) - 0.005 * k
	
SPACE ANALYSIS:
	Our solution requires Θ(k) space, both for the output Best array and the binary search tree. This is because the Best array and the binary search tree both end up with exactly k elements.