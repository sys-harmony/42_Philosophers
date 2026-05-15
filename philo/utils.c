/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:48:14 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/15 18:05:31 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

long	ft_get_time(t_tc time_code, t_data *data)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		ft_mutex_set(&data->state_mutex, &data->end_sim, 1);
		return (ft_error("philo: gettimeofday failed\n", -1));
	}
	else if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + tv.tv_usec / 1e3);
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	return (0);
}

void	ft_usleep(long sleep_time, t_data *data)
{
	const long	start_time = ft_get_time(MICROSECOND, data);
	const long	end_time = start_time + sleep_time;
	long		current_time;
	long		remaining_time;

	while (1)
	{
		current_time = ft_get_time(MICROSECOND, data);
		remaining_time = end_time - current_time;
		if (ft_sim_is_over(data) || remaining_time <= 0)
			break ;
		else if (remaining_time > 500)
			usleep(remaining_time - 500);
	}
}

void	ft_cleanup(t_data *data)
{
	if (data->mutex_init)
		ft_mutexes_destroy(data->philo_nbr, data);
	if (data->fork)
		free(data->fork);
	if (data->philo)
		free(data->philo);
}
