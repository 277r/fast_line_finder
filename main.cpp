#include <iostream>
#include <cstring>


// check if a certain size is in the bounds of an array 
bool in_bounds(int size, int bounds){
	if (size < bounds && size >= 0){
		return true;
	}
	return false;
}



struct line {
	// starting position x and y
	int posx, posy;
	// length
	int length;
	// 0 = up, for every increment, go 45 degrees clockwise so 1 = pointing toward right top corner, 2 = pointing toward right, etc. 
	int direction;
};

void output2d(unsigned char **data, int size){
	for (int i = 0; i < size; i++){
		for (int ii = 0; ii < size; ii++){
			std::cout << (int) data[i][ii] << ", ";
		}
		std::cout << "\n";
	}
	return;
}

line find(unsigned char **data, int xpos, int ypos, int size){
	// search in every direction
	// top, rgiht top, right, right bottom, etc.
	line t, rt, r, rb, b, lb, l, lt;

	

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

	for (int i = 0; i < size; i++){
		// threadify right here
		rdata[i] = find(data, i, 0, size);
	}
}

// find lines from left top to right top, not in the specified order
line find_longest_top(unsigned char **data, int size){
	line *rdata = new line[size];

	for (int i = 0; i < size; i++){
		// threadify right here
		rdata[i] = find(data, 0, i, size);
	}
}

// find lines from right top to right bottom, not in the specified order
line find_longest_right(unsigned char **data, int size){
	line *rdata = new line[size];

	for (int i = 0; i < size; i++){
		// threadify right here
		rdata[i] = find(data, i, size - 1, size);
	}
}

// find lines from right bottom to left bottom, not in the specified order
line find_longest_bottom(unsigned char **data, int size){
	line *rdata = new line[size];

	for (int i = 0; i < size; i++){
		// threadify right here
		rdata[i] = find(data, size - 1, i, size);
	}
}




line find_longest_nonzero_x(unsigned char **data, int size){
	// threadify these 4 
	line a = find_longest_top(data, size);
	line a = find_longest_right(data, size);
	line c = find_longest_bottom(data, size);
	line d = find_longest_left(data, size);
}












int main(){
	int size = 20;

	// make a 2d array of size * size bytes, set all to zero
	unsigned char **data = new unsigned char*[size];
	for (int i = 0; i < size; i++){
		data[i] = new unsigned char[size];
		memset(data[i], 0, size);
	}

	
	// place line
	for (int i = 0; i < 7; i++){
		data[i + 3][i+2] = 1;
	}
		output2d(data, size);


}


