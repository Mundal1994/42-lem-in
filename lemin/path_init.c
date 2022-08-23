/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molesen <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 09:38:32 by molesen           #+#    #+#             */
/*   Updated: 2022/07/15 13:30:59 by jdavis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/lemin.h"

/*	checks if end has been correctly sorted	*/

// static int	is_sorted(t_room *pass, int indx)
// {
// 	int	i;

// 	i = 0;
// 	if (pass->links[indx][i] == -1 || pass->links[indx][i + 1] == -1)
// 		return (TRUE);
// 	while (pass->links[indx][i + 1] != -1)
// 	{
// 		if (pass->distance[pass->links[indx][i]] > pass->distance[pass->links[indx][i + 1]])
// 			return (FALSE);
// 		++i;
// 	}
// 	return (TRUE);
// }

static int	is_sorted(t_room *pass, int indx)
{
	int	i;

	i = 0;
	if (pass->links[indx][i] == -1 || pass->links[indx][i + 1] == -1)
		return (TRUE);
	while (pass->links[indx][i + 1] != -1)
	{
		if (pass->distance[pass->links[indx][i]] < pass->distance[pass->links[indx][i + 1]] && pass->distance[pass->links[indx][i]] == 0)
			return (FALSE);
		if (pass->distance[pass->links[indx][i]] > pass->distance[pass->links[indx][i + 1]] && pass->distance[pass->links[indx][i + 1]] != 0)
			return (FALSE);
		++i;
	}
	return (TRUE);
}

static void set_to_minus_one(t_room *pass, int indx)
{
	//int	next;
	int	i;

	i = 0;
	while (pass->links[indx][i] != -1)
	{
		++i;
	}
	if (i == 2)
	{
		pass->distance[indx] = -1;
		i = 0;
		while (pass->links[indx][i] != -1)
		{
			if (pass->distance[pass->links[indx][i]] != -1)
			{
				set_to_minus_one(pass, pass->links[indx][i]);
			}
			++i;
		}
	}

}

/*	sorts the links of the end. Shortest paths in the beginning	*/

static void	sort_distance(t_room *pass)
{
	int	i;
	int	temp;
	int	indx;

	i = 0;
	indx = 1;
	// might remove is sorted from while loop and instead put in bottom - depends what is faster - check with bigger values
	i = 0;
	while (indx < pass->total)
	{
		i = 0;
		if (pass->links[indx][i] == -1 || (pass->links[indx][i + 1] == -1 && indx != pass->end && indx != 0))
		{
			if (pass->links[indx][i] != -1)
			{
				pass->distance[indx] = -1;
				set_to_minus_one(pass, pass->links[indx][i]);
			}
		}
		else
		{
			while (is_sorted(pass, indx) == FALSE)
			{
				i = 0;
				while (pass->links[indx][i + 1] != -1)
				{
					if (pass->distance[pass->links[indx][i]] > pass->distance[pass->links[indx][i + 1]] && pass->distance[pass->links[indx][i + 1]] != 0)
					{
						temp = pass->links[indx][i];
						pass->links[indx][i] = pass->links[indx][i + 1];
						pass->links[indx][i + 1] = temp;
					}
					else if (pass->distance[pass->links[indx][i]] < pass->distance[pass->links[indx][i + 1]] && pass->distance[pass->links[indx][i]] == 0)
					{
						temp = pass->links[indx][i];
						pass->links[indx][i] = pass->links[indx][i + 1];
						pass->links[indx][i + 1] = temp;
					}
					++i;
				}
			}
		}
		++indx;
	}
}

// static void	printf_struct(t_room *pass)
// {
// 	t_path *final;
// 	t_index *temp;
// 	int i;
// 	final = pass->final_head;
// 	i = 0;
// 	ft_printf("\n{green}after sort: finalS:{uncolor} \n");
// 	while (final)
// 	{
// 		temp = final->move_head;
// 		final->move = final->move_head;
// 		ft_printf("final\nnbr: %d	Len: %d	nbr of struct: %d\n", final->nbr, final->len, i);
// 		while (final->move)
// 		{
// 			ft_printf("room: %s\n", pass->rooms[final->move->index]);
// 			final->move = final->move->next;
// 		}
// 		final->move_head = temp;
// 		++i;
// 		final = final->next;
// 	}
// }

static void	create_len(int *array, int **len)
{
	int	i;

	i = 0;
	while (array[i] != -1)
	{
		i++;
	}
	*len = (int *)malloc(sizeof(int) * (i + 1));
	i = 0;
	while (array[i] != -1)
	{
		(*len)[i] = 0;
		++i;
	}
	(*len)[i] = -1;
}

static void reset_len(int **len)
{
	int i;

	i = 0;
	while ((*len)[i] != -1)
	{
		(*len)[i] = 0;
		++i;
	}
}

static void	start_connect(t_room *pass, int *connect)
{
	while (pass->links[0][*connect] != -1)
	{
		(*connect)++;
	}
}
/*	copy pasted from path_finder	*/
static void	initialize_path(t_room *pass, int i)
{
	pass->info[PATH][pass->links[pass->end][i]] = 3;
	pass->info[CURRENT][i] = pass->links[pass->end][i];
	pass->info[PREV][pass->links[pass->end][i]] = pass->end;
	pass->info[LEN][pass->links[pass->end][i]] = pass->info[LEN][pass->end] + 1;
	pass->info[CONNECT][pass->links[pass->end][i]] = 1;
}

static int	current_true(t_room *pass)
{
	int i;

	i = 0;
	while (i < pass->total)
	{
		if (pass->info[CURRENT][i] != 0)
			return (FALSE);
		++i;
	}
	return (TRUE);
}

static int current_exist(t_room *pass, int indx)
{
	int j;

	j = 0;
	while (j < pass->total)
	{
		if (pass->info[CURRENT][j] == indx)
		{
			return (TRUE);
		}
		++j;
	}
	return (FALSE);
}

static void	breadth_first(t_room *pass, int indx, int i)
{
	int	j;
	int	k;
	int count;
	int	come_from_count;

	j = 0;
	count = 0;

	come_from_count = 0;
	while (pass->links[indx][j] >= 0)
	{
		if (pass->links[indx][j] == 0)
		{
			pass->info[CURRENT][i] = 0;
			return ;
		}
		if (pass->links[indx][j] == pass->end || pass->info[PATH][pass->links[indx][j]] == 0 || pass->info[PATH][pass->links[indx][j]] == 1)
		++come_from_count;
		++j;
	}
	j = 0;
	while (pass->links[indx][j] >= 0)
	{
		if (pass->info[PATH][pass->links[indx][j]] == 0 || (pass->info[PATH][pass->links[indx][j]] != 3 && pass->info[PATH][indx] == 3))
		{
			// check if it is one way path or one with more connections
			while (pass->links[indx][count] != -1)
			{
				++count;
			}
			
			if (pass->links[indx][j] == pass->end)
				pass->info[CURRENT][i] = 0;
			else if (count == 2)
			{
				if (pass->info[PATH][indx] == 3 && come_from_count == 2)
					pass->info[PATH][pass->links[indx][j]] = pass->info[PATH][indx];
				//pass->info[PREV][pass->links[indx][j]] = indx;
				pass->info[CONNECT][pass->links[indx][j]] += 1;
				if (current_exist(pass, pass->links[indx][j]) == FALSE)
					pass->info[CURRENT][i] = pass->links[indx][j];
				else
					pass->info[CURRENT][i] = 0;
				ft_printf("count 2 compare with: %s -> %s room [%d] [%d]\n", pass->rooms[indx], pass->rooms[pass->links[indx][j]], pass->info[CONNECT][pass->links[indx][j]], pass->info[PATH][pass->links[indx][j]]);
				k = 0;
				while (k < pass->total)
				{
					if (pass->info[CURRENT][k] == 0)
					{
						i = k;
						break ;
					}
					++k;
				}
			}
			else
			{
				ft_printf("path nnbr: %d, count: %d\n", pass->info[PATH][indx], come_from_count);
				if (pass->info[PATH][indx] == 3 && come_from_count == 2)// || pass->info[PREV][indx] == pass->end))
					pass->info[PATH][pass->links[indx][j]] = pass->info[PATH][indx];
				pass->info[CONNECT][pass->links[indx][j]] += 1;
				if (current_exist(pass, pass->links[indx][j]) == FALSE)
					pass->info[CURRENT][i] = pass->links[indx][j];
				else
					pass->info[CURRENT][i] = 0;
				ft_printf("count more compare with: %s -> %s room [%d] [%d]\n", pass->rooms[indx], pass->rooms[pass->links[indx][j]], pass->info[CONNECT][pass->links[indx][j]], pass->info[PATH][pass->links[indx][j]]);
				k = 0;
				while (k < pass->total)
				{
					if (pass->info[CURRENT][k] == 0)
					{
						i = k;
						break ;
					}
					++k;
				}
			}
	
		}
		++j;
	}
	if (pass->info[CURRENT][i] == indx)
		pass->info[CURRENT][i] = 0;
}

static void	print_output(t_room *pass)
{
	int	i;
	int	nbr;
	int	prev;
	int new;

	new = 0;
	i = 0;
	while (pass->links[0][i] >= 0)
	{
		nbr = pass->info[PATH][pass->links[0][i]];
		if (nbr != 0)
		{
			ft_printf("{green}PATH [%d] LEN[%d]{uncolor}\n", new, pass->info[LEN][pass->links[0][i]]);
			prev = pass->info[PREV][pass->links[0][i]];
			ft_printf("%s\n", pass->rooms[pass->links[0][i]]);
			while (prev > 0)
			{
				ft_printf("%s\n", pass->rooms[prev]);
				prev = pass->info[PREV][prev];
			}
			new++;
		}
		++i;
	}
	i = 0;
	ft_printf("\n\n------------\nrooms path nbr:\n");
	while (i < pass->total)
	{
		ft_printf("[%s]: [%d] CONNECT: [%d] CURRENT: [%d]\n",pass->rooms[i], pass->info[PATH][i], pass->info[CONNECT][i], pass->info[CURRENT][i]);
		++i;
	}
}

static void set_to_one(t_room *pass)
{
	int j;

	j = 0;
	while (j < pass->total)
	{
		if (pass->info[PATH][j] == 0 && pass->info[CONNECT][j] != 0)
		{
			pass->info[PATH][j] = 1;
		}
		++j;
	}
}

static int len_of(t_room *pass, int *array)
{
	int i = pass->total - 1;
	ft_printf("array: %d\n", array[i]);
	while (i >= 0)
	{
		ft_printf("array: %d\n", array[i]);
		if (array[i] != 0)
			return (i);
		--i;
	}
	return (i);
}

static void	make_connect(t_room *pass)
{
	int i;

	i = 0;
	int len;
	pass->info[LEN][pass->end] = 1;
	while (pass->links[pass->end][i] >= 0)
		initialize_path(pass, i++);
	while (!current_true(pass))
	{
		i = 0;
		len = len_of(pass, pass->info[CURRENT]) + 1;
		ft_printf("len : %d\n", len);
		while (i < len)
		{
			if (pass->info[CURRENT][i] != 0)
			{
				breadth_first(pass, pass->info[CURRENT][i], i);
			}
			++i;
		}
		ft_printf("BEFOREn");
		print_output(pass);
		set_to_one(pass);
		ft_printf("AFTER\n");
		print_output(pass);
	}
	exit(0);
	print_output(pass);
	exit(0);
	
}

int	initialize_path_finder(t_room *pass, char *input)
{
	t_path	*path;
	t_path	*final;
	int	*len;
	int	i;
	int connect;

	path = NULL;
	final = NULL;
	len = NULL;
	sort_distance(pass);
	create_len(pass->links[0], &len);
	make_connect(pass);
	pass->final_head = NULL;
	i = 0;
	connect = 0;
	start_connect(pass, &connect);
	while (i < connect)
	{
		ft_printf("NUMBER OF TRIES %d\n", i);
		path_finder(&path, pass, i, &len);
		// if (pass->final_head && i != connect - 1)
		// {
		// 	del_path(&pass->final_head);
		// 	pass->final_head = NULL;
		// 	path = NULL;
		// }
		int k = 0;
		while (k < pass->total)
		{
			pass->info[PATH][k] = FALSE;
			pass->info[PREV][k] = FALSE;
			pass->info[LEN][k++] = FALSE;
		}
		reset_len(&len);
		++i;
	}
	if (!pass->final_head)
		return (error_path(pass, input, TRUE));
	final = pass->final_head;
	return (0);
}

/*

Fix one path deletes two paths problem
- idea 1: go through original copy and eliminate paths
- idea 2: reset where we start each time

smallest path... based on amount of ants

*/

/*

- when breath first finds beginning but want to find next shortest path - make sure it acts as we want it to

- decide between paths logic ex update struct or not

*/