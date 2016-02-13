import numpy as np
cimport numpy as np
from optv.parameters cimport ControlParams

def convert_arr_pixel_to_metric(np.ndarray[ndim=2, dtype=np.float_t] input,
                                ControlParams control,
                                np.ndarray[ndim=2, dtype=np.float_t] out=None):
    '''
    Convert NumPy 2d, Nx2 array from pixel coordinates to metric coordinates.
    @param input: input Numpy ndarray of Nx2 shape.
    @param control: ControlParams object that holds parameters needed for conversion.
    @param out: OPTIONAL numpy ndarray, same shape as input.
    
    @return: if no array was passed for output returns a new numpy ndarray with converted coordinates
    '''
    return convert_generic(input, control, out, pixel_to_metric)
  
def convert_arr_metric_to_pixel(np.ndarray[ndim=2, dtype=np.float_t] input,
                                ControlParams control,
                                np.ndarray[ndim=2, dtype=np.float_t] out=None):
    '''
    Convert NumPy 2d, Nx2 array from metric coordinates to pixel coordinates.
    @param input: input Numpy ndarray of Nx2 shape.
    @param control: ControlParams object that holds parameters needed for conversion.
    @param out: OPTIONAL Numpy ndarray, same shape as input.
    
    @return: if no array was passed for output returns a new numpy ndarray with converted coordinates
    '''
    return convert_generic(input, control, out, metric_to_pixel)

cdef convert_generic(np.ndarray[ndim=2, dtype=np.float_t] input,
                        ControlParams control,
                        np.ndarray[ndim=2, dtype=np.float_t] out,
                        void convert_function(double * , double * , double, double , control_par *)):
    # Raise exception if received non Nx2 shaped ndarray
    # or if output and input arrays' shapes do not match.
    if input.shape[1] != 2 or (out != None and out.shape[1] != 2):
        raise TypeError("Only two-column matrices accepted for conversion.")
    if out != None:
        if not(input.shape[0] == out.shape[0] and input.shape[1] == out.shape[1]):
            raise TypeError("Unmatching shape of input and output arrays: (" 
                            + str(input.shape[0]) + "," + str(input.shape[1]) 
                            + ") != (" + str(out.shape[0]) + "," + str(out.shape[1]) + ")")
    else:
        # If no array for output was passed (out==None):
        # create new array for output with  same shape as input array 
        out = np.empty_like(input)

    for i in range(input.shape[0]):
        convert_function(< double *> np.PyArray_GETPTR2(out, i, 0)
                        , < double *> np.PyArray_GETPTR2(out, i, 1)
                        , (< double *> np.PyArray_GETPTR2(input, i, 0))[0]
                        , (< double *> np.PyArray_GETPTR2(input, i, 1))[0]
                        , control._control_par)
    return out
         
