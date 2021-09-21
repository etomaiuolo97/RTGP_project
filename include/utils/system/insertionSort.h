// #pragma once

// #ifndef INSERTION_SORT
// #define INSERTION_SORT

// #include <vector>

// class InsertionSort {

// 	/**
// 	 * Sorts a list of particles so that the particles with the highest distance
// 	 * from the camera are first, and the particles with the shortest distance
// 	 * are last.
// 	 * 
// 	 * @param list
// 	 *            - the list of particles needing sorting.
// 	 */
// public:
    
//     static void sortHighToLow(vector<Particle> list) {
// 		for (int i = 1; i < list.size(); i++) {
// 			Particle item = list[i];
// 			if (item.getDistance() > list[i - 1].getDistance()) {
// 				sortUpHighToLow(list, i);
// 			}
// 		}
// 	}

// private:
//     static void sortUpHighToLow(vector<Particle> list, int i) {
// 		Particle item = list[i];
// 		int attemptPos = i - 1;
// 		while (attemptPos != 0 && list[attemptPos - 1].getDistance() < item.getDistance()) {
// 			attemptPos--;
// 		}
// 		// list.erase(list.at(i));
// 		// list.insert(attemptPos, item);
// 	}

// };

// #endif