/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:08:52 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/15 18:14:40 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	ft_atol_s(const char *nptr)
{
	unsigned long long	res;

	res = 0;
	while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
		nptr++;
	if (*nptr == '-')
		return (-1);
	if (*nptr == '+')
		nptr++;
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + *nptr - '0';
		if ((res > (unsigned long long)LONG_MAX))
			return (-1);
		nptr++;
	}
	if ((*nptr < '0' || *nptr > '9') && *nptr != '\0')
		return (-1);
	return ((long)res);
}

int	ft_error(const char *err_msg, int exit_code)
{
	size_t	len;

	len = 0;
	while (err_msg[len])
		len++;
	write(2, err_msg, len);
	return (exit_code);
}

long	ft_get_time(t_tc time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		return (ft_error("philo_bonus: gettimeofday failed\n", -1));
	else if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + tv.tv_usec / 1e3);
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	return (0);
}

void	ft_usleep(long sleep_time, t_data *data)
{
	const long	start_time = ft_get_time(MICROSECOND);
	const long	end_time = start_time + sleep_time;
	long		current_time;
	long		remaining_time;

	while (1)
	{
		current_time = ft_get_time(MICROSECOND);
		remaining_time = end_time - current_time;
		if (current_time < 0)
			ft_abort(data);
		if (remaining_time <= 0)
			break ;
		else if (remaining_time > 500)
			usleep(remaining_time - 500);
	}
}

void	ft_cleanup(t_data *data)
{
	const char	*prefix = "/philo_lock_";
	char		name[16];
	int			i;

	ft_sem_remove(data->forks_sem, "/philo_forks");
	ft_sem_remove(data->write_sem, "/philo_write");
	ft_sem_remove(data->stop_sem, "/philo_stop");
	if (data->philo)
	{
		i = -1;
		while (prefix[++i])
			name[i] = prefix[i];
		name[15] = '\0';
		i = -1;
		while (++i < data->philo_nbr)
		{
			name[12] = '0' + i / 100;
			name[13] = '0' + (i / 10) % 10;
			name[14] = '0' + i % 10;
			ft_sem_remove(data->philo[i].lock_sem, name);
		}
		free(data->philo);
	}
	if (data->pid)
		free(data->pid);
}
