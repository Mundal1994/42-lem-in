/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_initialize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molesen <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:31:10 by molesen           #+#    #+#             */
/*   Updated: 2022/07/19 10:31:12 by molesen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/lemin.h"

static void	initialize_path(t_room *pass, int i)
{
	pass->info[PATH][pass->links[pass->end][i]] = 1;
	pass->info[CURRENT][i] = pass->links[pass->end][i];
	pass->info[PREV][pass->links[pass->end][i]] = pass->end;
	pass->info[LEN][pass->links[pass->end][i]] = pass->info[LEN][pass->end] + 1;
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

static int	other_connections(t_room *pass, int indx)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (pass->links[indx][i] != -1)
	{
		if (pass->info[PREV][pass->links[indx][i]] == indx)
			++count;
		++i;
	}
	if (count > 1)
		return (TRUE);
	return (FALSE);
}

static void	breadth_first(t_room *pass, int indx, int i)
{
	int	j;
	int	k;

	j = 0;
	while (pass->links[indx][j] >= 0)
	{
		if (pass->links[indx][j] == 0)
		{
			pass->info[PATH][0] = 1;
			return ;
		}
		++j;
	}
	j = 0;
	while (pass->links[indx][j] >= 0)
	{
		if (pass->info[PATH][pass->links[indx][j]] == 0 && pass->info[PATH][pass->links[indx][j]] != 2)
		{
			if (pass->links[indx][j] == pass->end)
			{
				pass->info[CURRENT][i] = 0;
				break;
			}
			pass->info[PATH][pass->links[indx][j]] = pass->info[PATH][indx];
			pass->info[PREV][pass->links[indx][j]] = indx;
			pass->info[LEN][pass->links[indx][j]] = pass->info[LEN][indx] + 1;
			pass->info[CURRENT][i] = pass->links[indx][j];
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
		else if (pass->info[LEN][pass->info[PREV][pass->links[indx][j]]] == pass->info[LEN][indx])
		{
			if (other_connections(pass, pass->info[PREV][pass->links[indx][j]]) == TRUE)
				pass->info[PREV][pass->links[indx][j]] = indx;
		}
		++j;
	}
	if (pass->info[CURRENT][i] == indx)
		pass->info[CURRENT][i] = 0;
}

static void	lock_path(t_room *pass, int indx)
{
	int	i;
	int prev;

	i = 0;
	while (pass->links[0][i] >= 0)
	{
		if (pass->links[0][i] == indx)
		{
			prev = pass->info[PREV][pass->links[0][i]];
			pass->info[PATH][pass->links[0][i]] += 1;
			while (prev > 0 && prev != pass->end)
			{
				pass->info[PATH][prev]++;
				prev = pass->info[PREV][prev];
			}
			break ;
		}
		++i;
	}
}

static void	delete_non_found_paths(t_room *pass, int indx)
{
	int	i;

	lock_path(pass, indx);
	i = 0;
	while (i < pass->total)
	{
		if (pass->info[PATH][i] == 1)
			pass->info[PATH][i] = 0;
		pass->info[CURRENT][i] = FALSE;
		++i;
	}
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
	// i = 0;
	// ft_printf("\n\n------------\nrooms path nbr:\n");
	// while (i < pass->total)
	// {
	// 	ft_printf("[%s]: [%d] \n",pass->rooms[i], pass->info[PATH][i]);
	// 	++i;
	// }
}

void	path_finder(t_path **path, t_room *pass, int i)
{
	//int	i;

	i = 0;
	pass->info[LEN][pass->end] = 1;
	while (pass->links[pass->end][i] >= 0)
		initialize_path(pass, i++);
	while (!current_true(pass))
	{
		i = 0;
		while (i < pass->total)
		{
			if (pass->info[CURRENT][i] != 0)
			{
				breadth_first(pass, pass->info[CURRENT][i], i);
				if (pass->info[PATH][0] == 1)
				{
					break ;
				}
			}
			++i;
		}
		if (pass->info[PATH][0] == 1)
		{
			pass->info[PATH][0] = 0;
			ft_printf("LOCK pass->rooms: %s\n", pass->rooms[pass->info[CURRENT][i]]);
			delete_non_found_paths(pass, pass->info[CURRENT][i]);
			i = 0;
			while (pass->links[pass->end][i] >= 0)
			{
				if (pass->info[PATH][pass->links[pass->end][i]] == 0)
					initialize_path(pass, i);
				++i;
			}
		}
		ft_printf("\n\n\n--------NEW--------\n\n\n");
		print_output(pass);
	}
	if (path)
		++i;
}
