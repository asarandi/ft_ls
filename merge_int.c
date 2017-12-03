#include <stdio.h>
#include <stdlib.h>

//
// inspiration: https://www.youtube.com/watch?v=TzeBrDU-JaY
//

void print_arr(int *arr, int size)
{
	int i = 0;
	while (i < size)
		printf("%d, ", arr[i++]);
	printf("\n");
}

int	*merge(int *left, int ln, int *right, int rn)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int	*merged;

	merged = (int *)malloc(ln + rn);
	while ((i < ln) && (j < rn))
	{
		if (left[i] <= right[j])
			merged[k++] = left[i++];
		else
			merged[k++] = right[j++];
	}
	while (i < ln)
		merged[k++] = left[i++];
	while (j < rn)
		merged[k++] = right[j++];
	i = 0;
	while (i < ln + rn)
	{
		left[i] = merged[i];
		i++;
	}

	free(merged);
	return (left);
}

int	*merge_sort(int *arr, int n)
{
	if (n < 2)
		return (arr);

	int ln = n / 2;
	int rn = n - ln;

	merge_sort(&arr[0], ln);
	merge_sort(&arr[ln], rn);
	merge(&arr[0], ln,  &arr[ln], rn);
	print_arr(arr, n);
	return (arr);
}

int main()
{
	int	arr[] = {2, 4, 1, 6, 8, 5, 3, 7, 9, 12, 99, 0, -1, 45};
	int size = sizeof (arr) / sizeof (int);

	print_arr(arr, size);
	merge_sort(arr, size);
	print_arr(arr, size);
	return (0);
}
