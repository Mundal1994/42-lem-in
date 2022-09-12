/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdavis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 13:20:42 by jdavis            #+#    #+#             */
/*   Updated: 2022/07/11 13:20:44 by jdavis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lemin.h"

int	by_line(char *input)
{
	int	flag;
	int	count;
	int	i;

	i = 0;
	count = 0;
	flag = 1;
	if (!ft_strncmp(&input[i], "##start", 7))
		return (5);
	else if (!ft_strncmp(&input[i], "##end", 5))
		return (6);
	else if (input[i] == '#')
		return (-1);
	if (!ft_strlchr(input, ' ', ft_strlen_stop(input, '\n')) && ft_strlchr(input, '-', ft_strlen_stop(input, '\n')))
		return (2);
	while (input[i] != '\n' && input[i] != '\0')
	{
		if (flag && input[i] != ' ')
		{
			 flag = 0;
			 ++count;
		}
		if (input[i++] == ' ')
			flag = 1;
		//++i;
	}
	return (count);
}

int	duplicated(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = i + 1;
		while (str[j])
		{
			if (!ft_strcmp(str[i], str[j]))
				return (-1);
			++j;
		}
		++i;
	}
	return (0);
}

static void	set_to_null(t_room *pass)
{
	int	i;

	i = 0;
	while (i < pass->total)
	{
		pass->rooms[i] = NULL;
		pass->links[i] = NULL;
		++i;
	}
	pass->rooms[i] = NULL;
}

int	create_connect(t_room *pass, int j)
{
	if (!pass->tmp_con)
	{
		pass->tmp_con = (t_connect *) malloc(sizeof(t_connect));
		if (!pass->tmp_con)
		{
			//clean
			return (-1);
		}
		pass->tmp_con->count = 1;
		pass->head_con = pass->tmp_con;
		pass->tmp_con->next = NULL;
	}
	else
	{
		pass->tmp_con->current_room = j;
		//ft_printf("-COUNT %i ROOM %s\n", pass->tmp_con->count, pass->rooms[pass->tmp_con->current_room]);
		pass->tmp_con->next = (t_connect *) malloc (sizeof(t_connect));
		if (!pass->tmp_con->next)
		{
			t_connect *temp;
			while (pass->head_con)
			{
				temp = pass->head_con->next;
				free(pass->head_con);
				pass->head_con = temp;
			}
			return (-1);
		}
		pass->tmp_con->next->next = NULL;
		pass->tmp_con->next->count = pass->tmp_con->count + 1;
		pass->tmp_con = pass->tmp_con->next;
		pass->tmp_con->current_room = -1;
	}
	return (1);
}

static void	connec_to_links(t_room *pass, int j)
{
	int	k;
	int	count;
	t_connect *temp;

	k = 0;
	count = pass->tmp_con->count;
	pass->tmp_con = NULL;
	while (k < count)
	{
		pass->links[j][k] = -1;
		if (pass->head_con->current_room != -1)
		{
			pass->links[j][k] = pass->head_con->current_room;
		}
		temp = pass->head_con->next;
		free(pass->head_con);
		pass->head_con = temp;
		k++;
	}
}

static int	create_links(t_room *pass, t_input **build, int i)
{
	int	j;
	//char **input = NULL; //just for error function

	//exit (0);
	if (duplicated(pass->rooms) == ERROR)
		return (error_free(pass, build, 0, FALSE));
	j = 0;
	while (pass->rooms[j])
	{
		if (create_connect(pass, 0) == -1)
			return (error_free(pass, build, j, FALSE)); //CHANGE INPUT TO FREE BUILD AND COTENTS
		if (count_in(j, &(((*build)->input)[i]), pass) == -1)
			return (error_free(pass, build, j, FALSE));
		pass->links[j] = (int *) malloc((pass->tmp_con->count) * sizeof(int));
		if (!pass->links[j])
			return (error_free(pass, build, j, FALSE));
		connec_to_links(pass, j);
		++j;
	}
	return (TRUE);
}

static int	start_and_end(t_room *pass,	int hold, t_input **build, int *i)
{
	if (hold == 5)
		hold = 0;
	else
		hold = pass->total - 1;
	if (pass->rooms[hold])
		return (error_free(pass, build, 0, FALSE));
	pass->rooms[hold] = ft_strnew(ft_strlen_stop(&(((*build)->input)[*i]), ' '));
	if (!pass->rooms[hold])
		return (error_free(pass, build, 0, FALSE));
	ft_strncat(pass->rooms[hold], &(((*build)->input)[*i]), ft_strlen_stop(&(((*build)->input)[*i]), ' '));
	while (((*build)->input)[*i] != '\n')
		++(*i);
	return (TRUE);
}

static void	set_val(t_room *pass, t_input **build, int *hold)
{
	*hold = ERROR;
	pass->rooms = NULL;
	pass->links = NULL;
	pass->tmp_con = NULL;
	pass->head_con = NULL;
	pass->info = NULL;
	pass->total = file_save(pass, build);
	pass->end = pass->total - 1;
}

int	create(t_room *pass, t_input **build)//char **input)
{
	int	i;
	int	j;
	int	hold;
	int	stop;

	i = 0;
	j = 1;
	set_val(pass, build, &hold);
	if (pass->total == ERROR)
		return (error_free(pass, build, 0, TRUE));
	if (pass->total > 0)
	{
		pass->rooms = (char **) malloc((pass->total + 1) * sizeof(char *));
		pass->links = (int **) malloc(pass->total * sizeof(int *));
		if (!pass->rooms || !pass->links)
		{
			return (error_free(pass, build, 0, FALSE)); //add BUILD
		}
		set_to_null(pass);
		while (((*build)->input)[i] != '\0')
		{
			if (hold == 5 || hold == 6)
			{
				if (start_and_end(pass, hold, build, &i) == -1)
					return (-1);
			}
			hold = by_line(&(((*build)->input)[i]));
			if (hold < 2 || hold == 5 || hold == 6)
			{
				while (((*build)->input)[i] != '\n')
					++i;
			}
			if (hold == 3)
			{
				stop = ft_strlen_stop(&(((*build)->input)[i]), ' ');
				pass->rooms[j] = ft_strnew(stop);
				if (!pass->rooms[j])
					return (error_free(pass, build, 0, FALSE));
				ft_strncat(pass->rooms[j++], &(((*build)->input)[i]), stop);
				while (((*build)->input)[i] != '\n')
					++i;
			}
			else if (hold == 2)
			{
				return (create_links(pass, build, i));
			}
			++i;
		}
	}
	return (1); //not sure what to return here
}
