#include "Matrix.h"

#include <stdlib.h>
#include <stdio.h>
typedef struct Matrix{
    int height;
    int width;
    double** values;
} Matrix;

ErrorCode matrix_create(PMatrix* matrix, uint32_t height, uint32_t width) {
    if (matrix == NULL) {
        return ERROR_FAILURE;
    }
	*matrix = (PMatrix)malloc(1 * sizeof(Matrix));
    PMatrix pm = *matrix;
	if (pm == NULL) {
		return ERROR_FAILURE;
	}

    if ((int)height <= 0 || (int)width <= 0) {
        free(pm);
        return ERROR_FAILURE;
    }
    
	pm->height = (int)height;
    pm->width = (int)width;
	pm->values = (double**)malloc(height * sizeof(double*));

    if (pm->values == NULL) {
        free(pm);
        return ERROR_FAILURE;
    }
    
    for (uint32_t i = 0; i < height; i++) {
        pm->values[i] = (double*)malloc(width * sizeof(double));
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

ErrorCode matrix_copy(PMatrix* result, CPMatrix source) {
    if (source == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
    ErrorCode e = matrix_create(result, source->height, source->width);
    if (!error_isSuccess(e)) {
        return e;
    }
    for (int i = 0; i < source->height; i++) {
        for (int j = 0; j < source->width; j++) {
            e = matrix_setValue(*result, i, j, source->values[i][j]);
            if (!error_isSuccess(e)) {
                return e;
            }
        }
    }
    return e;
}

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

ErrorCode matrix_getHeight(CPMatrix matrix, uint32_t* result) {
        if (matrix == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
    *result = (uint32_t)matrix->height;
    return ERROR_SUCCESS;
}

ErrorCode matrix_getWidth(CPMatrix matrix, uint32_t* result) {
    if (matrix == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
    *result = (uint32_t)matrix->width;
    return ERROR_SUCCESS;
}

ErrorCode matrix_setValue(PMatrix matrix, uint32_t rowIndex, uint32_t colIndex, double value) {
if (matrix == NULL) {
    return ERROR_FAILURE;
}
if ((int)rowIndex < 0 || (int)rowIndex > matrix->height-1 || (int)colIndex < 0 || (int)colIndex > matrix->width-1) {
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

ErrorCode matrix_getValue(CPMatrix matrix, uint32_t rowIndex, uint32_t colIndex, double* value) {
    if (matrix == NULL || value == NULL) {
        return ERROR_FAILURE;
    }
    if ((int)rowIndex < 0 || (int)rowIndex > matrix->height-1 || (int)colIndex < 0 || (int)colIndex > matrix->width-1) {
        return ERROR_FAILURE;
    }
    
    *value = matrix->values[(int)rowIndex][(int)colIndex];
    return ERROR_SUCCESS;
}

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
        for (int j = 0; j < lhs->width; j++) {
            e = matrix_setValue(*result, i, j, lhs->values[i][j] + rhs->values[i][j]);
            if (!error_isSuccess(e)) {
                return e;
            }
        }
    }
    return e;
}

ErrorCode matrix_multiplyMatrices(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    if (lhs == NULL || rhs == NULL || result == NULL) {
        return ERROR_FAILURE;
    }
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

ErrorCode matrix_multiplyWithScalar(PMatrix matrix, double scalar) {
    if (matrix == NULL) {
        return ERROR_FAILURE;
    }
    for (int i = 0; i < matrix->height; i++) {
        for (int j = 0; j < matrix->width; j++) {
            ErrorCode e = matrix_setValue(matrix, i, j, scalar*matrix->values[i][j]);
            if (!error_isSuccess(e)) {
                return e;
            }
        }
    }
    return ERROR_SUCCESS;
}