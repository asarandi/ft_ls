#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <strings.h>


//
//inspiration: 
//https://www.youtube.com/watch?v=jOyYaBHyN28
//https://www.youtube.com/watch?v=iQzoVUU5-Ss
//
typedef struct		s_list
{
	int				value;
	struct s_list	*next;
}					t_list;

void print_list(t_list *list)
{
	while (list)
	{
		printf("%d, ", list->value);
		list = list->next;
	}
	printf("\n");
}

void	list_split(t_list *head, t_list **left, t_list **right)
{
	t_list	*start;
	t_list	*slow;
	t_list	*fast;
	if (head == NULL || head->next == NULL)
		return ;
	start = head;
	slow = head;
	fast = slow->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*left = start;
	*right = slow->next;
	slow->next = NULL;
	printf("list_split (left) : "); print_list(*left);
	printf("list_split (right): "); print_list(*right);
}


t_list	*merge_lists(t_list *left, t_list *right)
{
	t_list	*merged;

	if (left == NULL)
		return (right);
	if (right == NULL)
		return (left);

	if (left->value <= right->value)
	{
		merged = left;
		merged->next = merge_lists(left->next, right);
	}
	else
	{
		merged = right;
		merged->next = merge_lists(right->next, left);
	}
	printf("merge_lists : "); print_list(merged);
	return (merged);
}

void	sort_list(t_list **list)
{
	t_list	*head;
	t_list	*left;
	t_list	*right;

	head = *list;

	if ((head == NULL) || (head->next == NULL))
		return ;

	list_split(head, &left, &right);
	sort_list(&left);
	sort_list(&right);
	*list = merge_lists(left, right);
}

void destroy_list(t_list *list)
{
	t_list *next;
	while (list)
	{
		next = list->next;
		free(list);
		list = next;
	}
}

int	main()
{
	t_list *head;
	t_list *new;
	t_list *index;
	head = NULL;

	srand(time(NULL));

	int i = 0;
	while (i < 10)
	{
		new = malloc(sizeof(t_list));
		bzero(new, sizeof(t_list));
		if (head == NULL)
		{
			head = new;
			index = head;
		}
		else
		{
			index->next = new;
			index = index->next;
		}
		index->value = rand() % (99 + 1 - 0) + 0;
		i++;
	}

	printf("unsorted: ");	print_list(head);
	sort_list(&head);
	printf("  sorted: "); print_list(head);
	destroy_list(head);

	return (0);

}
