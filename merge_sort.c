/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 22:21:06 by asarandi          #+#    #+#             */
/*   Updated: 2017/12/03 12:15:56 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void	list_split(t_file *head, t_file **left, t_file **right)
{
	t_file	*slow;
	t_file	*fast;

	slow = head;
	if (head == NULL || head->next == NULL)
	{
		(*left) = head;
		(*right) = NULL;
	}
	else
	{
		fast = head->next;
		while (fast)
		{
			fast = fast->next;
			if (fast)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
	}
	(*left) = head;
	(*right) = slow->next;
	slow->next = NULL;
}

t_file	*merge_lists(t_file *left, t_file *right)
{
	static long long unsigned int i;
	ft_printf(1, "merge lists: %llu\n", i++);
	t_file	*merged;

	if (!left)
		return (right);
	if (!right)
		return (left);
	if (g_opt.sort_algo(left, right) <= 0)
	{
		merged = left;
		merged->next = merge_lists(left->next, right);
	}
	else
	{
		merged = right;
		merged->next = merge_lists(left, right->next);
	}
	return (merged);
}

void	sort_list(t_file **list)
{
	static long long unsigned int i;
	ft_printf(1, " sort lists: %llu\n", i++);

	t_file	*head;
	t_file	*left;
	t_file	*right;

	if (g_opt.sort == -1)
		return ;
	head = *list;
	if ((head == NULL) || (head->next == NULL))
		return ;
	list_split(head, &left, &right);
	sort_list(&left);
	sort_list(&right);
	(*list) = merge_lists(left, right);
}




/*
void	list_swap(t_file *f1, t_file *f2)
{
	char	*name;
	struct stat	st;

	name = f1->name;
	st = f1->st;
	f1->name = f2->name;
	f1->st = f2->st;
	f2->name = name;
	f2->st = st;
}

void	sort_list(t_file **list)
{
	t_file	*i;
	t_file	*j;
	int		k;

	if (*list == NULL)
		return ;
	k = 1;
	while (k > 0)
	{
		k = 0;
		i = *list;
		while (i->next)
		{
			j = i->next;
			if (g_opt.sort_algo(i, j) > 0)
			{
				k++;
				list_swap(i, j);
			}
			i = i->next;
		}
	}
}

*/

























