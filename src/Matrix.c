#include "Matrix.h"

#include <stdlib.h>
#include <stdio.h>
typedef struct Matrix{
    int height;
    int width;
    double** values;
} Matrix;

/**
 * @brief Creates a new matrix of a given height an width,
 *  all values are initially zeroes.
 *
 * @param[out] matrix The address of a matrix pointer to receive
 *  the address of the created matrix.
 * @param[in] height Height of the matrix
 * @param[in] width Width of the matrix
 * @return ErrorCode
 */
ErrorCode matrix_create(PMatrix* matrix, uint32_t height, uint32_t width) {
    if (matrix == NULL) {
        return ERROR_FAILURE;
    }
    //dinamic allocation
	*matrix = (PMatrix)malloc(1 * sizeof(Matrix));
	//if the allocate failed
    PMatrix pm = *matrix;
	if (pm == NULL) {
		return ERROR_FAILURE;
	}

	//allocating
	pm->height = (int)height;
    pm->width = (int)width;
	pm->values = (double**)malloc(height * sizeof(double*));

	//if the allocate failed
    if (pm->values == NULL) {
        free(pm);
        return ERROR_FAILURE;
    }
    
    for (uint32_t i = 0; i < height; i++) {
        pm->values[i] = (double*)malloc(width * sizeof(double));
        //if the allocate failed
        if (pm->values[i] == NULL) {
            for (int k = 0; k < i; k++) {
                free(pm->values[k]);
            }
            free(pm->values);
            free(pm);
		    return ERROR_FAILURE;
        }
	}
        for (uint32_t i = 0; i < height; i++) {
            for (uint32_t j = 0; j < width; j++) {
                if (matrix_setValue(pm, i, j, 0) == ERROR_FAILURE) {
                    return ERROR_FAILURE;
                }
        }
    }
	return ERROR_SUCCESS;
}

/**
 * @brief Creates a new matrix from an old matrix.
 *
 * @param[out] matrix The address of a matrix pointer to receive
 *  the address of the copied matrix.
 * @param[in] source The matrix to copy.
 * @return ErrorCode
 */
ErrorCode matrix_copy(PMatrix* result, CPMatrix source) {
    if (source == NULL) {
        return ERROR_FAILURE;
    }
    ErrorCode e = matrix_create(result, source->height, source->width);
    if (!error_isSuccess(e)) {
        return e;
    }
    //put the values
    for (int i = 0; i < source->height; i++) {
        for (int j = 0; i < source->width; i++) {
            e = matrix_setValue(*result, i, j, source->values[i][j]);
            if (!error_isSuccess(e)) {
                return e;
            }
        }
    }
    return e;
}

/**
 * @brief Destroys a matrix.
 *
 * @param matrix the matrix to destroy.
 */
void matrix_destroy(PMatrix matrix) {
    if (matrix == NULL) {
        return;
    }
    for (int i = 0; i < matrix->height; i++) {
        free(matrix->values[i]);
    }
    free(matrix->values);
    free(matrix);
}

/**
 * @brief Returns the height of a give matrix.
 *
 * @param[in] matrix The matrix.
 * @param[out] result On output, contains the height of the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getHeight(CPMatrix matrix, uint32_t* result) {
        if (matrix == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
    *result = (uint32_t)matrix->height;
    return ERROR_SUCCESS;
}

/**
 * @brief Returns the width of a give matrix.
 *
 * @param[in] matrix The matrix.
 * @param[out] result On output, contains the height of the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getWidth(CPMatrix matrix, uint32_t* result) {
    if (matrix == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
    *result = (uint32_t)matrix->width;
    return ERROR_SUCCESS;
}

/**
 * @brief Sets a value to the matrix.
 *
 * @param[in, out] matrix The matrix to operate on.
 * @param[in] rowIndex Row index of the value to set.
 * @param[in] colIndex Column index of the value to setF.
 * @param[in] value Value to set.
 * @return ErrorCode
 */
ErrorCode matrix_setValue(PMatrix matrix, uint32_t rowIndex, uint32_t colIndex, double value) {
if (matrix == NULL) {
    return ERROR_FAILURE;
}
int row = (int)rowIndex;
int col = (int)colIndex;
if ((col > matrix->width-1 || col < 0) || (row > matrix->height-1 || row < 0)) {
    return ERROR_FAILURE;
}
matrix->values[row][col] = value;
    return ERROR_SUCCESS;
}

/**
 * @brief Gets a value from the matrix.
 *
 * @param[in] matrix The matrix to operate on.
 * @param[in] rowIndex Row index of the value to get.
 * @param[in] colIndex Column index of the value to get.
 * @param[out] value The address of a double variable to receive
 *  the value from the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getValue(CPMatrix matrix, uint32_t rowIndex, uint32_t colIndex, double* value) {
    if (matrix == NULL) {
        return ERROR_FAILURE;
    }
    *value = matrix->values[rowIndex][colIndex];
    return ERROR_SUCCESS;
}

/**
 * @brief Computes the addition of two matrices.
 *
 * @param[out] result The address of a matrix pointer to receive
 *  the address of the resulting matrix.
 * @param[in] lhs The left hand side of the addition operation.
 * @param[in] rhs The right hand side of the addition operation.
 * @return ErrorCode
 */
ErrorCode matrix_add(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    if (lhs == NULL || rhs == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
    if (lhs->height != rhs->height || lhs->width != rhs->width) {
        return ERROR_FAILURE;
    }
    ErrorCode e = matrix_create(result, lhs->height, lhs->width);
    if (!error_isSuccess(e)) {
        return e;
    }
    for (int i = 0; i < lhs->height; i++) {
        for (int j = 0; i < lhs->width; i++) {
            e = matrix_setValue(*result, i, j, lhs->values[i][j] + rhs->values[i][j]);
            if (!error_isSuccess(e)) {
                return e;
            }
        }
    }
    return e;
}

/**
 * @brief Computes the multiplication of two matrices.
 *
 * @param[out] result The address of a matrix pointer to receive
 *  the address of the resulting matrix.
 * @param[in] lhs The left hand side of the multiplication operation.
 * @param[in] rhs The right hand side of the multiplication operation.
 * @return ErrorCode
 */
ErrorCode matrix_multiplyMatrices(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    if (lhs == NULL || rhs == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
    //can multiply only if left columns is equal to right rows
    if (lhs->height != rhs->width) {
        return ERROR_FAILURE;
    }
    ErrorCode e = matrix_create(result, lhs->height, rhs->width);
    if (!error_isSuccess(e)) {
        return e;
    }
    for(int i = 0; i < lhs->height; ++i) {
		for(int j = 0; j < rhs->width; ++j) {
			for(int k = 0; k < lhs->width; ++k) {
                e = matrix_setValue(*result, i, j, lhs->values[i][k] * rhs->values[k][j]);
                if (!error_isSuccess(e)) {
                return e;
                }
		    }
        }
	}
    return ERROR_SUCCESS;
}

/**
 * @brief Multiplies a matrix with a scalar and stores the result in
 *  the given matrix.
 *
 * @param[in, out] matrix On input, the matrix to multiply with a scalar.
 *  On output, the result of the multiplication operation.
 * @param[in] scalar The scalar to multiply with.
 * @return ErrorCode
 */
ErrorCode matrix_multiplyWithScalar(PMatrix matrix, double scalar) {
    if (matrix == NULL) {
        return ERROR_FAILURE;
    }
    for (int i = 0; i < matrix->height; i++) {
        for (int j = 0; i < matrix->width; i++) {
            ErrorCode e = matrix_setValue(matrix, i, j, scalar*matrix->values[i][j]);
            if (!error_isSuccess(e)) {
                return e;
            }
        }
    }
    return ERROR_SUCCESS;
}