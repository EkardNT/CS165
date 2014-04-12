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
	
	We also memoized our comparisons, so that no pair of elements are ever submitted to COMPARE() more than once.
	
TIME ANALYSIS:
	todo

TIMING FUNCTION:
	todo

SPACE ANALYSIS:
	todo