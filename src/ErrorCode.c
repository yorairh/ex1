
#include "ErrorCode.h"

bool error_isSuccess(ErrorCode code) {
    return code == ERROR_SUCCESS;
}

const char* error_getErrorMessage(const ErrorCode code) {
    if (error_isSuccess(code)) {
        return "No error In the code";
    } else if (code == ERROR_FAILURE_NULL) {
        return "Error in code - null variable";
    } else if (code == ERROR_FAILURE_ALLOCATE) {
        return "Error in code - could not allocate memory";
    } else if (code == ERROR_FAILURE_CONDITION) {
        return "Error in code - failed in a condition (out of boundary probably)";
    } else {
        return "Error in code - failed to set a value in a new matrix";  
    }
}