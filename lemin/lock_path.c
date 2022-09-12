
#include "../includes/lemin.h"

static int	len_check(t_room *pass, int indx)
{
	int	i;
	int	start;

	i = pass->info[LEN][pass->info[JUMP][indx]];
	i += pass->info[LEN][indx];
	start = indx;
	while (pass->info[NEXT][indx] != pass->end && pass->info[PATH][indx])
	{
		indx = pass->info[NEXT][indx];
		//ft_printf("pass->rooms[indx %d]\n", indx);
		if (pass->info[PATH][indx] == 2)
			return (TRUE);
		// if (pass->info[PATH][pass->info[JUMP][indx]] == 4)
		// 	return (FALSE);
		if (pass->info[JUMP][indx] && \
			pass->info[PATH][pass->info[JUMP][indx]] == 1 && \
			pass->info[LEN][pass->info[JUMP][indx]] + pass->info[LEN][indx] < i)
		{
			//ft_printf("pass->rooms[indx %d]\n", indx);
			return (FALSE);
		}
	}
	return (TRUE);
}

static void	locate_jump_out(t_room *pass, int *indx)
{
	//ft_printf("HERE6\n");
	if ((len_check(pass, *indx) && !pass->hold))
	{
		//ft_printf("HERE\n");
		pass->info[PREV][*indx] = pass->info[JUMP][*indx];
		pass->info[JUMP][*indx] = 0;
		pass->hold = 1;
	}
	else
	{
		//ft_printf("HERE2\n");
		pass->next = pass->info[NEXT][*indx];
		pass->info[PREV][*indx] = 0;
	}
}

static void	update_no_jump_out(t_room *pass, int *indx, int *for_now)
{
	//ft_printf("HERE3\n");
	pass->next = pass->info[NEXT][*indx];
	if (!pass->hold)
	{
		pass->info[PREV][*indx] = 0;
		pass->info[NEXT][*indx] = 0;
	}
	else
		pass->info[NEXT][*indx] = *for_now;
	pass->hold = 0;
}

static void	lock_conditions(t_room *pass, int *value, int *indx, int *for_now)
{
	//ft_printf("ENTER\n");
	if ((pass->info[PATH][*indx] == 3 && *value == 1) || \
		(pass->info[PATH][*indx] == 1 && *value == 3) || \
		pass->info[PATH][*indx] == 1)
	{
		//ft_printf("HERE4\n");
		pass->hold = 0;
		if (*value == 1 && pass->info[PATH][*indx] != 1)
			pass->next = pass->info[NEXT][*indx];
		pass->info[NEXT][*indx] = *for_now;
	}
	else if (pass->info[PATH][*indx] == 3 && *value == 3 && \
		pass->info[JUMP][*indx] && !pass->hold)
		locate_jump_out(pass, indx);
	else if (pass->info[PATH][*indx] == 3 && *value == 3 && \
		pass->info[JUMP][*indx] && pass->hold)
	{
		pass->next = pass->info[NEXT][*indx];
		pass->info[NEXT][*indx] = *for_now;
		pass->hold = 0;
		//ft_printf("HERE5\n");
	}
	else if (pass->info[PATH][*indx] == 3 && *value == 3 && \
		!pass->info[JUMP][*indx])
		update_no_jump_out(pass, indx, for_now);
	//ft_printf("EXIT\n");
}

void	lock_path(t_room *pass, int indx)
{
	int	for_now;
	int	value;

	pass->next = 0;
	pass->hold = 0;
	pass->info[NEXT][indx] = pass->end;
	//ft_printf("NEW-------\n");
	while (indx != 0)
	{
		for_now = indx;
		value = pass->info[PATH][indx];
		if (pass->info[PATH][indx] == 1)
			pass->info[PATH][indx] = 4;
		if (!pass->next && pass->info[PATH][pass->info[PREV][indx]] != 4)
			indx = pass->info[PREV][indx];
		else
			indx = pass->next;
		pass->next = 0;
		//ft_printf("pass->rooms: %s[%d] path: %d, prev: %d, next: %d, jump: %d, end: %d\n", pass->rooms[indx], indx, pass->info[PATH][indx], pass->info[PREV][indx], pass->info[NEXT][indx], pass->info[JUMP][indx], pass->end);
		
		lock_conditions(pass, &value, &indx, &for_now);
		if (pass->info[PREV][indx] != 0 && \
			pass->info[NEXT][indx] == pass->info[PREV][indx])
		{
			//ft_printf("ERROR: pass->rooms: %s[%d] path: %d, prev: %d, next: %d, jump: %d, end: %d\n", pass->rooms[indx], indx, pass->info[PATH][indx], pass->info[PREV][indx], pass->info[NEXT][indx], pass->info[JUMP][indx], pass->end);
			return ;
		}
		//ft_printf("pass->rooms: %s[%d] path: %d, prev: %d, next: %d, jump: %d, end: %d\n", pass->rooms[indx], indx, pass->info[PATH][indx], pass->info[PREV][indx], pass->info[NEXT][indx], pass->info[JUMP][indx], pass->end);
		// if (pass->info[PATH][indx] == 4)
		// 	exit(0);
	}
}
