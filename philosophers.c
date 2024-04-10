/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:05:09 by natrijau          #+#    #+#             */
/*   Updated: 2024/04/10 18:10:43 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	check_arg(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 5 && ac != 6)
	{
		printf("Invalid number of arguments !\n");
		return (false);
	}
	while (av[i])
	{
		if (!its_integer(av[i]))
		{
			printf("One of the arguments is invalid !\n");
			return (false);			
		}
		i++;
	}	
	if (!all_positiv_num(av))
	{
		printf("One of the arguments is invalid !\n");
		return(false);
	}
	return(true);
}	

void	ft_eating(t_philosophers *philo)
{
	struct timeval my_time;
  	long int current_time;
	pthread_mutex_lock(&philo->my_fork);
	pthread_mutex_lock(philo->next_fork);
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	if (philo->time_to_die <= philo->time_to_eat)
	{
		printf("%ld\t %d take fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\t %d take second fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\t %d start eat \n",current_time - philo->start_time,  philo->id_philosphers);
		pthread_mutex_unlock(&philo->print);
		usleep(philo->time_to_die);
		philo->end_time += philo->time_to_die / 1000;
		pthread_mutex_lock(&philo->print);
		philo->data->dead_id += 1;
		if (philo->data->dead_id == 1)
			printf("%ld\t %d dead !\n", philo->end_time, philo->id_philosphers);
		pthread_mutex_unlock(&philo->print);
		pthread_mutex_unlock(&philo->my_fork);
		pthread_mutex_unlock(philo->next_fork);
		return ;
	}	
	if (philo->data->dead_id == 0)
	{
		printf("%ld\t %d take fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\t %d take second fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\t %d start eat \n",current_time - philo->start_time,  philo->id_philosphers);
		usleep(philo->time_to_eat);
	}
	philo->number_of_times_each_philosopher_must_eat--;
	philo->start_dead = philo->time_to_eat;
	pthread_mutex_unlock(&philo->print);
	pthread_mutex_unlock(&philo->my_fork);
	pthread_mutex_unlock(philo->next_fork);
}

void	sleeping(t_philosophers *philo)
{
	struct timeval my_time;
  	long int current_time;
	
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	if (philo->start_dead + philo->time_to_sleep >= philo->time_to_die)
	{
		philo->end_time += philo->time_to_die / 1000;
		philo->data->dead_id += 1;
		if (philo->data->dead_id == 1)
		{
			printf("%ld\t %d start sleep!\n",current_time - philo->start_time, philo->id_philosphers);
			printf("%ld\t %d dead!\n", philo->end_time, philo->id_philosphers);
		}
		pthread_mutex_unlock(&philo->print);
		return ;
	}
	if (philo->data->dead_id == 0)
	{
		printf("%ld\t %d start sleep !\n",current_time - philo->start_time, philo->id_philosphers);
	    usleep(philo->time_to_sleep);
	}
	philo->start_dead = current_time - philo->start_time;
	pthread_mutex_unlock(&philo->print);
}

void	thinking(t_philosophers *philo)
{
	struct timeval my_time;
  	long int current_time;
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	if (philo->data->dead_id == 0)
		printf("%ld\t %d think !\n",(current_time - philo->start_time),  philo->id_philosphers);
	pthread_mutex_unlock(&philo->print);
}

void	*round_table(void *arg)
{
	t_philosophers	*philo = (t_philosophers *) arg;

	if (philo->id_philosphers % 2 == 1)
	{
		thinking(philo);
		usleep(100);
	}	
	while (1)
	{
		ft_eating(philo);
		// printf("\tnumb eat %d ! \n",philo->number_of_times_each_philosopher_must_eat);
		if(philo->number_of_times_each_philosopher_must_eat == 0)
			break;
		sleeping(philo);
		thinking(philo);
		pthread_mutex_lock(&philo->print);
		if (philo->data->dead_id > 0)
		{
			pthread_mutex_unlock(&philo->print);
			break;
		}
		pthread_mutex_unlock(&philo->print);
	}
	
	pthread_exit(EXIT_SUCCESS);	
}

void	one_philo(t_data *data)
{
	printf("0\t Philo_ID 1 take fork!\n");
	printf("%d\t Philo_ID 1 dead!\n", data->data_philo->time_to_die / 1000);
}

int	get_thread(t_data *data)
{
	unsigned int	num_fork;
	struct timeval start;
	gettimeofday(&start, NULL);
	data->data_philo->start_time = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	num_fork = data->data_philo->number_of_philosophers;
	if (num_fork == 1)
	{
		one_philo(data);
		return (0);
	}
	while (num_fork > 0)
	{
		pthread_create(&data->data_philo[num_fork -1].thread_philo, NULL, round_table, &data->data_philo[num_fork - 1]);
		num_fork--;
	}
	while (num_fork < data->data_philo->number_of_philosophers)
	{
		pthread_join(data->data_philo[num_fork].thread_philo, NULL);
		num_fork++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data data;
	if (!check_arg(ac, av))
		return (1);
	init_philo(&data, av);
	// print_test_init(&data);
	get_thread(&data);
	// free_all(&data);
	return (0);
}