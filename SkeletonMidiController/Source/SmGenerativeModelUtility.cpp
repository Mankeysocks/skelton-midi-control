//
//  SmGenerativeModelUtility,cpp.cpp
//  SkeletonMidiController
//
//  Created by Remediation on 7/31/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/bimap.hpp>


#include "SmGenerativeModelUtility.h"
#include "SmUmbrellaHeader.h"

namespace sm {
    
    ///////////////////////////////////////
    // Helper Methods
    ///////////////////////////////////////
    
    using namespace boost::numeric::ublas;
    using namespace std;
    
    // Requisites: The file format is as follows. The state identifier (number corresponding to the pitch) comes first, followed by a semi-colon. The follower states come after the semi-colon, and a space precedes each follower-state number
    bool generatePitchModelFromFile(char* file_name, matrix<int>** state_matrix, state_index_map** state_indices)
    {
        string line; // we line to store lines we read in...
        ifstream myfile (file_name); // from file @ file_name
        
        *state_indices = new state_index_map();
        int state_index = 0; // a temporary state index so we can place our values in the state index bimap
        list<list<int>* > countList; // a list of lists to keep track of which notes follow each particular note/state  *this needs to be cleaned up at end of f*
        
        
        // Check if file is open and we are able to read
        if (myfile.is_open())
        {
            // if so, we perform our parse operations
            while ( myfile.good() )
            {
                getline (myfile,line); // read in whole line at a time
                
                if(line.length() > 3) // according to protocol, each line would need at least 4 chars TODO note working must fix!!!!!
                {
                    boost::tokenizer<> tok(line); // tokenize the line (will automatically use space and "," delimiters)
                    boost::tokenizer<>::iterator it=tok.begin(); // the first token is the state
#ifdef __SM_DEBUG_MODE_2__
                    cout << "state " << state_index << " maps to " << *it << "\n"; // DEBUG print to verify the states map correctly
#endif
                    int i = boost::lexical_cast<int>(*it);
                    (*state_indices)->insert(state_index_entry(state_index,i)); // index,state#
                    countList.push_back(new list<int>); // make a new list for all the entries in this state's row
                    
                    ++state_index;
                    ++it; // start at first token following state identifier
                    for(; it!=tok.end();++it)
                    { // the following tokens are the next states that correspond to the current state #
                        cout << *it << " ";
                        countList.back()->push_back(boost::lexical_cast<int>(*it));
                    }
#ifdef __SM_DEBUG_PARSE__
                    cout << "\n" << endl;
#endif
                }
            }
            myfile.close();
        }
        else
        { // if we cannot open the file, print a message and exit
            cout << "Unable to open file";
            return false;
        }
        // Here we will create our matrix and increment its entries
        int matrix_size = state_index; // size of our square model matrix
        (*state_matrix) = new matrix<int>(matrix_size, matrix_size); // our matrix
        (*state_matrix)->clear();
#ifdef __SM_DEBUG_MODE_2__
        cout << (*state_matrix) << endl << endl;
#endif
        state_index = 0;
        list<list<int>*>::iterator state_it = countList.begin();
        
        try {
            for (; state_it != countList.end(); ++state_it)
            {
                list<int>::iterator count_it = (*state_it)->begin();
                for (; count_it != (*state_it)->end(); ++count_it)
                {
                    // update the entry in the matrix
                    (**state_matrix)(state_index, (*state_indices)->right.at(*count_it)) += 1;
                }
                ++state_index;
            }
            
        } catch (std::exception const& e) {
            printf("There was an error parsing a graph file.\nThis is probably due to a non-complete graph which can result from an improperly formatted file");
            return false;
        }
#ifdef __SM_DEBUG_PARSE__
        cout << (*state_matrix) << endl << endl;
#endif
        // Now we loop back through the entire matrix and update the rows so they reflect the probability function
        for (int r = 0; r < matrix_size; r++)
        {
            int sum = 0;
            for (int c = 0; c < matrix_size; c++)
            {
                (**state_matrix)(r,c) += sum;
                sum = (**state_matrix)(r,c);
            }
        }
        
        // Clean up memory allocations!!
        while(!countList.empty()) delete countList.front(), countList.pop_front();
#ifdef __SM_DEBUG_PARSE__
        std::cout << *state_matrix << std::endl;
#endif
        return true;
    }    
    
    
    // This is a specialized binary search method for finding the the state in a row that corresponds to a given random value
    // i.e. for row (0, 0, .1, .2, .6 , .75, .75, 1) and val .74, returns 
    int binarySearchMatrixRow(matrix<int>* state_matrix,int row, float val, int low_index, int high_index) // eventually implement range feature
    {
        if(low_index >= high_index)
        {
            cout << "Error in binary matrix search! low index cannot be equal to or higher than high index..." << endl;
            return 0;
        }
        int begin = low_index;
        int end = high_index;
        int middle = (begin + end) / 2;
        float target = val * ((*state_matrix)(row, end) - (*state_matrix)(row, begin));
        
        while (begin < end - 1) {
            if((*state_matrix)(row,middle) < target){
                begin = middle;   
                middle = (begin + end) / 2;
            }
            else{
                end = middle;
                middle = (begin + end) / 2; //TODO fix
            }
        }
        if ((*state_matrix)(row,middle) == (*state_matrix)(row,end)) {
            return middle;
        }
        else return end;
    }
    
}