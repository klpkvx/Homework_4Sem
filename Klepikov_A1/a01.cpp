#include "query.h"
#include "enum.h"
#include "time.h"
#include <stdio.h>
int Solution1(FILE *istream_, FILE *ostream_, char *s, char *t, int *err);

int Solution1(FILE *istream_, FILE *ostream_, char *s, char *t, int *err)
{
	int task = 1;
	int count = 0;
	char curr_str[LEN];
	strcpy(curr_str, s);

	Query query;
	if (query.parse(task, s, t) != parse_results::SUCCESS)
	{
		printf("usage x: '<', '>', '=', '<=', '>=', '<>' \n");
		err++;
		return 0;
	}

	count = query.apply(istream_, ostream_);

	return count;
}

int main(int argc, char *argv[])
{
	int task = 1;
	double time = 0.;
	int err = 0;
	if (argc != 5)
	{
		printf("Usage: %s f_in f_out s t \n", argv[0]);
		return ARGS;
	}
	char *s = argv[3], *t = argv[4];
	char *input = argv[1], *output = argv[2];
	FILE *istream_ = fopen(input, "r"), *ostream_ = fopen(output, "w");
	if (istream_ == nullptr || ostream_ == nullptr)
	{
		if (ostream_)
			fclose(ostream_);
		if (istream_)
			fclose(istream_);
		printf("Cannot open!\n");
		return OPEN;
	}
	time = clock();
	int res = Solution1(istream_, ostream_, s, t, &err);
	if (err != 0)
	{
		printf("ERROR!\n");
		fclose(istream_);
		fclose(ostream_);
		return err;
	}
	time = (clock() - time) / CLOCKS_PER_SEC;

	printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, res, time);

	fclose(istream_);
	fclose(ostream_);
	return 0;
}
