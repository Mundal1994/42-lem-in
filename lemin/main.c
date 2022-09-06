/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molesen <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 16:11:31 by molesen           #+#    #+#             */
/*   Updated: 2022/07/19 12:42:13 by jdavis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/lemin.h"

/*	function checking room is already in use	*/

static int	create_used(t_room *pass)
{
	int	i;

	i = 0;
	pass->info = (int **) malloc(8 * sizeof(int *));
	if (!pass->info)
		return (ERROR);
	pass->info[PATH] = (int *) malloc(pass->total * sizeof(int));
	pass->info[PREV] = (int *) malloc(pass->total * sizeof(int));
	pass->info[LEN] = (int *) malloc(pass->total * sizeof(int));
	pass->info[CURRENT] = (int *) malloc(pass->total * sizeof(int));
	pass->info[NEXT] = (int *) malloc(pass->total * sizeof(int));
	pass->info[JUMP] = (int *) malloc(pass->total * sizeof(int));
	pass->info[LOCKED] = (int *) malloc(pass->total * sizeof(int));
	pass->info[MOVE] = (int *) malloc(pass->total * sizeof(int));
	if (!pass->info[PATH] || !pass->info[PREV] || !pass->info[LEN] || !pass->info[CURRENT] || !pass->info[NEXT] || !pass->info[JUMP] || !pass->info[LOCKED] || !pass->info[MOVE])
		return (ERROR);
	while (i < pass->total)
	{
		pass->info[PATH][i] = FALSE;
		pass->info[PREV][i] = FALSE;
		pass->info[LEN][i] = FALSE;
		pass->info[NEXT][i] = FALSE;
		pass->info[JUMP][i] = FALSE;
		pass->info[LOCKED][i] = FALSE;
		pass->info[MOVE][i] = FALSE;
		pass->info[CURRENT][i++] = FALSE;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_room	*pass;
	char	*input;
	t_input	*build;
	//int		max_paths;



	if (argc == 1 || (argc == 2 && ft_strcmp(argv[1], "-r") == 0))
	{
		input = NULL;
		build = NULL;
		pass = (t_room *) malloc(sizeof(t_room));
		if (!pass)
			return (0);
		if (argc == 2)
			pass->row = TRUE;
		else
			pass->row = FALSE;
		if (create(pass, &build, &input) == ERROR)
			return (0);
		//exit (0);
		if (create_used(pass) == ERROR)
			return (error_path(pass, input, TRUE));
		if (initialize_path_finder(pass, input) == ERROR)
			return (0);
		ft_printf("%s\n", build->input);
		solve(pass);
		error_path(pass, input, FALSE);
	}
	else if (argv)
		ft_printf("Usage: ./lem-in < maps/subject.map\n");
	return (0);
}
