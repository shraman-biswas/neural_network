#include "main.h"

int main(void)
{
	printf("[ neural network ]\n");

	int i, num, epochs, rows, cols;
	double rate, range;
	gsl_matrix *train=NULL, *target=NULL, *x=NULL, *result=NULL;
	neural_net_t *nn=NULL;

	/* neural network parameters */
	num = SIZE(layers);			/* number of layers */
	rate = 0.1;				/* learning rate */
	range = 2;				/* random number range */
	epochs = 10000;				/* training iterations */

	/* calculate parameters */
	num = SIZE(layers);
	cols = layers[0];
	rows = SIZE(train_arr) / cols;

	/* convert training inputs array to gsl matrix */
	train = arr_to_gslmat(train_arr, rows, cols);
	/* convert training targets array to gsl matrix */
	target = arr_to_gslmat(target_arr, rows, layers[num-1]);
	/* allocate memory for testing inputs matrix */
	x = gsl_matrix_alloc(1, train->size2);
	/* allocate memory for neural network prediction result */
	result = gsl_matrix_alloc(1, layers[num-1]);

	/* create neural network */
	nn = nn_create(layers, num, rate, range);

	/* train neural network */
	nn_train(nn, train, target, epochs);

	/* loop over testing inputs  */
	for (i=0; i < train->size1; ++i) {
		select_test(train, x, i);	/* select testing input */
		nn_predict(nn, x, result);	/* neural network prediction */
		disp_result(train, result, i);	/* display prediction result */
	}

	/* destroy neural network */
	nn_destroy(nn);

	return EXIT_SUCCESS;
}

/* display neural nertwork prediction result matrix */
static void disp_result(
	const gsl_matrix *const test,
	const gsl_matrix *const result,
	const int set_num)
{
	int i;
	printf("(");
	/* display neural network testing inputs */
	for (i=0; i < test->size2-1; ++i)
		printf("%f, ", gsl_matrix_get(test, set_num, i));
	printf("%f) -> ", gsl_matrix_get(test, set_num, i));
	/* display neural network prediction results */
	for (i=0; i < result->size2-1; ++i)
		printf("%f, ", gsl_matrix_get(result, 0, i));
	printf("%f\n", gsl_matrix_get(result, 0, i));
}

/* select neural network testing inputs matrix */
static void select_test(
	const gsl_matrix *const train,
		gsl_matrix *const x,
	const int set_num)
{
	/* select 1 testing input from testing set matrix */
	gsl_matrix_const_view tmp = gsl_matrix_const_submatrix(
		train, set_num, 0, 1, train->size2);
	/* set testing inputs matrix */
	gsl_matrix_memcpy(x, &tmp.matrix);
}
