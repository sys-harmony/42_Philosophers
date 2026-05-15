/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:34:45 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/15 18:24:51 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		return (ft_error("philo: arguments must be "
				"positive integers or zero\n", 2));
	if (!argv[5])
		data->max_meals = -1;
	data->think_time
		= (data->time_to_die - data->time_to_eat - data->time_to_sleep) / 3;
	if (!data->philo_nbr || !data->max_meals)
		return (ft_error("philo: simulation cannot proceed with zero "
				"philosophers or a maximum meal count of zero\n", 3));
	return (0);
}

static void	ft_assign_forks(t_philo *philo, t_fork *fork)
{
	if (philo->id % 2)
	{
		philo->first_fork = &fork[(philo->id + 1) % philo->data->philo_nbr];
		philo->second_fork = &fork[philo->id];
	}
	else
	{
		philo->first_fork = &fork[philo->id];
		philo->second_fork = &fork[(philo->id + 1) % philo->data->philo_nbr];
	}
}

static int	ft_init_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		data->fork[i].id = i;
		data->philo[i] = (t_philo){.id = i, .data = data};
		if (pthread_mutex_init(&data->fork[i].mutex, NULL))
		{
			ft_mutexes_destroy(i, data);
			return (ft_error("philo: mutex init failed\n", 1));
		}
		if (pthread_mutex_init(&data->philo[i].mutex, NULL))
		{
			pthread_mutex_destroy(&data->fork[i].mutex);
			ft_mutexes_destroy(i, data);
			return (ft_error("philo: mutex init failed\n", 1));
		}
		ft_assign_forks(&data->philo[i], data->fork);
	}
	return (0);
}

static int	ft_init_data(t_data *data)
{
	data->fork = malloc(sizeof(t_fork) * data->philo_nbr);
	data->philo = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->fork || !data->philo)
		return (ft_error("philo: malloc failed\n", 1));
	if (pthread_mutex_init(&data->state_mutex, NULL))
		return (ft_error("philo: mutex init failed\n", 1));
	if (pthread_mutex_init(&data->write_mutex, NULL))
	{
		pthread_mutex_destroy(&data->state_mutex);
		return (ft_error("philo: mutex init failed\n", 1));
	}
	if (ft_init_philos(data))
		return (1);
	data->mutex_init = 1;
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		exit_code;
	int		ret;

	data = (t_data){0};
	exit_code = EXIT_SUCCESS;
	if (argc < 5 || argc > 6)
		return (ft_error("Usage : ./philo number_of_philosophers "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]\n", 2));
	ret = ft_parse_input(&data, argv);
	if (ret == 2)
		exit_code = 2;
	else if (!ret)
	{
		if (ft_init_data(&data))
			exit_code = EXIT_FAILURE;
		else if (ft_sim(&data))
			exit_code = EXIT_FAILURE;
	}
	ft_cleanup(&data);
	return (exit_code);
}
