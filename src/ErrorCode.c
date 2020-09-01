
#include "ErrorCode.h"

bool error_isSuccess(ErrorCode code) {
    if (code == ERROR_SUCCESS) {
        return true;
    }
    return false;
}

const char* error_getErrorMessage(ErrorCode code) {
    if (error_isSuccess(code)) {
    return "No error In the code";
    }
    return "Error in the code";
}