//
//  SmGenerativeModelUtility.h
//  SkeletonMidiController
//
//  Created by Remediation on 7/31/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_SmGenerativeModelUtility_h
#define SkeletonModuleApplication_SmGenerativeModelUtility_h

#include <map>

namespace sm{
    
    
    typedef boost::bimap<int/*state#*/,int/*index*/> state_index_map;
    typedef state_index_map::value_type state_index_entry;
    typedef state_index_map::const_iterator state_index_iterator;
    
    bool generatePitchModelFromFile(char* file_name, boost::numeric::ublas::matrix<int>** state_matrix, state_index_map** state_indices);
    
    bool generateDurationPitchModelFromFile(char* file_name, boost::numeric::ublas::matrix<int>** state_matrix, state_index_map** pd_state_indices, state_index_map** p_state_indices);
    
    // This is a specialized binary search method for finding the the state in a row that corresponds to a given random value
    // i.e. for row (0, 0, .1, .2, .6 , .75, .75, 1) and val .74, returns 
    int binarySearchMatrixRow(boost::numeric::ublas::matrix<int>* state_matrix,int row, float val, int low_index, int high_index);
}
#endif
