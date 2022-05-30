#ifndef __KNN__
#define __KNN__

typedef struct {
    double **mat;
    long n_rows;
    long n_cols;
} t_data;

typedef struct {
    t_data *data;
    long k;
} t_knn_classifier;

/**
 * Reads a comma-separated-value (CSV) file
 *
 * @param path Path to file
 * 
 */
t_data *read_csv(char *path);

/**
 * Trains a KNN-classifier model base on data and k
 *
 * @param data double-array of features and target variables (last column)
 * 
 */
t_knn_classifier *knn_fit(t_data *data, long k);

/**
 * Predicts the class of vec using KNN-classifier
 *
 * @param knn KNN-classifier model
 * @param knn array of features
 * 
 */
long knn_predict(t_knn_classifier *knn, double *vec);

/**
 * Predicts the class of vec using KNN-classifier
 *
 * @param y_true double-array with true labels
 * @param y_pred double-array with predicted labels
 * @param n size of the arrays
 * 
 */
double accuracy_score(long *y_true, long *y_pred, long n);

/**
 * Returns the very last column of data->mat (Target column)
 * casted to long
 *
 * @param data array of features and target variables
 * 
 */
long *get_target(t_data *data);

#endif
