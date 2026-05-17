/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:08:52 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/17 14:49:29 by gdosch           ###   ########.fr       */
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
	else if (time_code == MS)
		return ((tv.tv_sec * 1e3) + tv.tv_usec / 1e3);
	else if (time_code == US)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	return (0);
}

void	ft_usleep(long sleep_time, t_data *data)
{
	const long	start_time = ft_get_time(US);
	const long	end_time = start_time + sleep_time;
	long		current_time;
	long		remaining_time;

	if (start_time < 0)
		ft_abort(data);
	while (1)
	{
		current_time = ft_get_time(US);
		if (current_time < 0)
			ft_abort(data);
		remaining_time = end_time - current_time;
		if (remaining_time <= 0)
			break ;
		else if (remaining_time > 500)
			usleep(remaining_time - 500);
	}
}

void	ft_cleanup(t_data *data)
{
	ft_remove_sem(data->forks_sem, "/philo_forks");
	ft_remove_sem(data->diners_sem, "/philo_diners");
	ft_remove_sem(data->write_sem, "/philo_write");
	ft_remove_sem(data->done_sem, "/philo_done");
	ft_remove_sem(data->stop_sem, "/philo_stop");
	if (data->philo)
	{
		if (data->philo_sem_init)
			ft_remove_philo_sems(data->philo_nbr, data);
		free(data->philo);
	}
	if (data->pid)
		free(data->pid);
}
