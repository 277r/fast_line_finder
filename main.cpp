#include <iostream>
#include <cstring>




// check if a certain size is in the bounds of an array 
bool in_bounds(int size, int bounds){
	if (size < bounds && size >= 0){
		return true;
	}
	return false;
}

int place_random_line(unsigned char **data, int bounds, int value){
	int a = rand() % bounds;
	int b = rand() % bounds;
	int length = rand() % bounds;
	// value in [-1, 0, 1], to define direction
	int diry = rand() % 3 - 1;
	int dirx = rand() % 3 - 1;

	for (int i = 0; in_bounds(a + i * diry, bounds) && in_bounds(b + i * dirx, bounds) && i < length; i++){
		data[a + i * diry][b + i * dirx] = value;
	}
	data[a][b] = value;
}


struct line {
	// starting position x and y
	int posx, posy;
	// length
	int length;
	// 0 = up, for every increment, go 45 degrees clockwise so 1 = pointing toward right top corner, 2 = pointing toward right, etc. 
	int direction;

	unsigned char value;
};

// can be optimized using referencing
line get_largest(line *data, int length){
	int size = 0;
	int index = 0;
	for (int i = 0; i < length; i++){
		if (data[i].length > size){
			size = data[i].length;
			index = i;
		}
	}
	return data[index];
}

void output2d(unsigned char **data, int size){
	for (int i = 0; i < size; i++){
		for (int ii = 0; ii < size; ii++){
			std::cout << (int) data[i][ii] << ", ";
		}
		std::cout << "\n";
	}
	return;
}

/**
	@param data 2 dimensional data array
	@param xpos the x position to start searching from
	@param ypos the y position to start searching from
	@param xoff the offset on the x axis for every iteration of the loop
	@param yoff the offset on the y axis for every iteration of the loop
*/
line find(unsigned char **data, int xpos, int ypos, int xoff, int yoff, int size){
	line current_longest;
	line tmp;
	int i = 0;
	// set values of current and temporary line to zero
	current_longest = {0, 0, 0, 0, 0};
	tmp = {0, 0, 0, 0, 0};

	// while the boundaries are fit
	while (in_bounds(xpos + i * xoff, size) && in_bounds(ypos + i * yoff, size)){
		
		
		if (data[ypos + i * yoff][xpos + i * xoff] == tmp.value && tmp.value != 0){
			tmp.length++;
		}
		else if (tmp.length > current_longest.length){
				current_longest = tmp;
		}
		else if (data[ypos + i * yoff][xpos + i * xoff] != 0){
			tmp.posx = xpos + i * xoff;
			tmp.posy = ypos + i * yoff;
			tmp.value = data[ypos + i * yoff][xpos + i * xoff];
			tmp.length = 1;
		}

		i++;
	}
	if (tmp.length > current_longest.length){
		return tmp;
	}
	return current_longest;

}

/*
	left:	find diagonal lines going toward right top corner, find lines from left to right
	top:	find diagonal lines going toward right bottom corner, find lines from top to bottom
	right:	find diagonal lines going toward left bottom corner
	bottom: find diagonal lines going toward left top corner

*/



// find lines from left bottom to left top, not in the specified order
line find_longest_left(unsigned char **data, int size){
	line *rdata = new line[size];
	// possible threadification, split diagonal and horizontal in 2 processes, would be more complex but useful
	for (int i = 0; i < size; i++){
		// threadify right here, level 3
		rdata[i] = find(data, 0, i, 1, -1, size);
	}
	line r = get_largest(rdata, size);
	r.direction = 0;
	return r;
}

// find lines from left top to right top, not in the specified order
line find_longest_top(unsigned char **data, int size){
	line *rdata = new line[size];

	for (int i = 0; i < size; i++){
		// threadify right here, level 3
		rdata[i] = find(data, 0, i, 1, 1, size);
	}
	return get_largest(rdata, size);
}

// find lines from right top to right bottom, not in the specified order
line find_longest_right(unsigned char **data, int size){
	line *rdata = new line[size];

	for (int i = 0; i < size; i++){
		// threadify right here, level 3
		rdata[i] = find(data, i, size - 1, -1, 1, size);
	}
	return get_largest(rdata, size);
}

// find lines from right bottom to left bottom, not in the specified order
line find_longest_bottom(unsigned char **data, int size){
	line *rdata = new line[size];

	for (int i = 0; i < size; i++){
		// threadify right here, level 3
		rdata[i] = find(data, size - 1, i, -1, -1, size);
	}
	return get_largest(rdata, size);
}

line find_longest_horizontal(unsigned char **data, int size){
	line *rdata = new line[size];
	// possible threadification, split diagonal and horizontal in 2 processes, would be more complex but useful
	for (int i = 0; i < size; i++){
		// threadify right here, level 3
		rdata[i] = find(data, 0, i, 1, 0, size);
	}
	return get_largest(rdata, size);
}

// find lines from left top to right top, not in the specified order
line find_longest_vertical(unsigned char **data, int size){
	line *rdata = new line[size];

	for (int i = 0; i < size; i++){
		// threadify right here, level 3
		rdata[i] = find(data, i, 0, 0, 1, size);
	}
	return get_largest(rdata, size);
}


line find_longest_nonzero_x(unsigned char **data, int size){
	// threadify, level 1
	line lines[6];
	
	lines[0] = find_longest_top(data, size);
	lines[1] = find_longest_right(data, size);
	lines[2] = find_longest_bottom(data, size);
	lines[3] = find_longest_left(data, size);
	lines[4] = find_longest_horizontal(data, size);
	lines[5] = find_longest_vertical(data, size);
	
	return get_largest(lines, 6);
}



// create the largest line from the current point
line continue_line(unsigned char **data, int size, int xpos, int ypos, int xoff, int yoff){
	line tmp;
	tmp.value = data[ypos][xpos];
	tmp.length = 0;
	tmp.posx = xpos;
	tmp.posy = ypos;
	int i = 0;
	while (in_bounds(xpos + i * xoff, size) && in_bounds(ypos + i * yoff, size)){
		if (data[ypos + i * yoff][xpos + i * xoff] == tmp.value && tmp.value != 0){
			tmp.length++;
		}
		i++;
	}
	return tmp;
}



// my previous algorithm
line find_longest_line_bad(unsigned char **data, int size){
	line longest, tmp;
	// set longest line to zeros
	longest = {0, 0, 0, 0, 0};

	for (int i = 0; i < size; i++){
		for (int ii = 0; ii < size; ii++){
			// horizontal
			tmp = continue_line(data, size, ii, i, 1, 0);
			tmp.direction = 2;
			if (tmp.length > longest.length){
				longest = tmp;
			}

			// vertical
			tmp = continue_line(data, size, ii, i, 0, 1);
			tmp.direction = 0;
			if (tmp.length > longest.length){
				longest = tmp;
			}

			// diagonal leftbottom to righttop
			tmp = continue_line(data, size, ii, i, 1, -1);
			tmp.direction = 1;
			if (tmp.length > longest.length){
				longest = tmp;
			}

			// diagonal lefttop to right bottom
			tmp = continue_line(data, size, ii, i, 1, 1);
			tmp.direction = 3;
			if (tmp.length > longest.length){
				longest = tmp;
			}

		}
	}
	return longest;
}








// testing the algorithm in the main function
int main(){
	// initiate randomness
	srand(time(NULL));
	
	int size = 1;
	int testcount = 100;


	
	

	// execute each test
	for (int i = 0; i < testcount; i++){
		// make a 2d array of size * size bytes, set all to zero
		unsigned char **data = new unsigned char*[size];
		for (int i = 0; i < size; i++){
			data[i] = new unsigned char[size];
		}


		// set all to zero
		for (int i = 0; i < size; i++){
			memset(data[i], 0, size);
		}
		// place line
		place_random_line(data, size, 1);

		
		clock_t start, end;
		float time_1, time_2;
		line x, y;
		// test performance here
		{

			start = clock();
			x = find_longest_nonzero_x(data, size);
			end = clock();

			time_1 = end - start;

			start = clock();
			y = find_longest_line_bad(data, size);
			end = clock();
			time_2 = end - start;
		}
		// output
		std::cout << (int)x.value << ", " << x.posx << ", " << x.posy << ", " << x.length << ", " << x.direction << ", " << time_1 << "\n";
		std::cout << (int)y.value << ", " << y.posx << ", " << y.posy << ", " << y.length << ", " << y.direction << ", "<< time_2 << std::endl;

		// cleanup
		for (int i = 0; i < size; i++){
			delete[] data[i];
		}
		delete[] data;
		// double size for next run
		size *= 2;
		
	}
	
	
}


