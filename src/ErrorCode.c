
#include "ErrorCode.h"


/**
 * @brief Checks if a given error code indicates a success or not.
 *
 * @param[in] code the error code.
 * @return whether the error code indicates a success or not.
 */
bool error_isSuccess(ErrorCode code) {
    if (code == ERROR_SUCCESS) {
        return true;
    }
    return false;
}

/**
 * @brief gets a textual error message for a given error code.
 *
 * @param[in] code the error code.
 * @return const char* the textual representation of the error code.
 */
const char* error_getErrorMessage(ErrorCode code) {
    if (error_isSuccess(code)) {
    return "No error In the code";
    }
    return "Error in the code";
}