/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:34:45 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/15 18:25:41 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_parse_input(t_data *data, char *argv[])
{
	data->philo_nbr = ft_atol_s(argv[1]);
	data->time_to_die = ft_atol_s(argv[2]) * 1e3;
	data->time_to_eat = ft_atol_s(argv[3]) * 1e3;
	data->time_to_sleep = ft_atol_s(argv[4]) * 1e3;
	if (argv[5])
		data->max_meals = ft_atol_s(argv[5]);
	if (data->philo_nbr < 0 || data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0 || data->max_meals < 0)
		return (ft_error("philo_bonus: arguments must be "
				"positive integers or zero\n", 2));
	if (!argv[5])
		data->max_meals = -1;
	data->think_time
		= (data->time_to_die - data->time_to_eat - data->time_to_sleep) / 3;
	if (!data->philo_nbr || !data->max_meals)
		return (ft_error("philo_bonus: simulation cannot proceed with zero "
				"philosophers or a maximum meal count of zero\n", 3));
	return (0);
}

static int	ft_sem_init(t_data *data)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_stop");
	data->forks_sem = sem_open("/philo_forks", O_CREAT, 0644, data->philo_nbr);
	data->write_sem = sem_open("/philo_write", O_CREAT, 0644, 1);
	data->stop_sem = sem_open("/philo_stop", O_CREAT, 0644, 0);
	if (data->forks_sem == SEM_FAILED
		|| data->write_sem == SEM_FAILED
		|| data->stop_sem == SEM_FAILED)
		return (ft_error("philo_bonus: sem_open failed\n", 1));
	return (0);
}

static int	ft_philo_init(t_data *data)
{
	const char	*prefix = "/philo_lock_";
	char		name[16];
	int			i;

	i = -1;
	while (prefix[++i])
		name[i] = prefix[i];
	name[15] = '\0';
	i = -1;
	while (++i < data->philo_nbr)
	{
		data->philo[i] = (t_philo){.id = i, .data = data};
		name[12] = '0' + i / 100;
		name[13] = '0' + (i / 10) % 10;
		name[14] = '0' + i % 10;
		sem_unlink(name);
		data->philo[i].lock_sem = sem_open(name, O_CREAT, 0644, 1);
		if (data->philo[i].lock_sem == SEM_FAILED)
			return (ft_error("philo_bonus: sem_open failed\n", 1));
	}
	return (0);
}

static int	ft_data_init(t_data *data)
{
	data->pid = malloc(sizeof(pid_t) * data->philo_nbr);
	data->philo = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->pid || !data->philo)
		return (ft_error("philo_bonus: malloc failed\n", 1));
	if (ft_sem_init(data) || ft_philo_init(data))
		return (1);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		exit_code;
	int		ret;

	if (argc < 5 || argc > 6)
		return (ft_error("Usage : ./philo_bonus number_of_philosophers "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]\n", 2));
	data = (t_data){0};
	exit_code = EXIT_SUCCESS;
	ret = ft_parse_input(&data, argv);
	if (ret == 2)
		exit_code = 2;
	else if (!ret)
	{
		if (ft_data_init(&data))
			exit_code = EXIT_FAILURE;
		else if (ft_sim(&data))
			exit_code = EXIT_FAILURE;
	}
	ft_cleanup(&data);
	return (exit_code);
}
