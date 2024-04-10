#ifndef		PHILOSOPHERS_H
# define	PHILOSOPHERS_H

/*printf*/
# include <stdio.h>

/*usleep*/
# include <unistd.h>

/*gettimeofday*/
# include <sys/time.h>

/*bool*/
# include <stdbool.h>

/*malloc*/
# include <stdlib.h>

/*thread*/
# include <pthread.h>

typedef struct s_philosophers t_philosophers;
typedef struct s_data	t_data;

typedef struct s_philosophers
{
	unsigned int	number_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philosopher_must_eat;
	unsigned int	id_philosphers;
	long int		start_time;
	long int		end_time;
	long int		start_dead;
	pthread_t		thread_philo;
	pthread_mutex_t	my_fork;
	pthread_mutex_t	*next_fork;
	pthread_mutex_t	print;
	t_data			*data;
} t_philosophers;

typedef struct s_data
{
	int				dead_id;
	t_philosophers	*data_philo;
} t_data;

/*philosophers_utils.c*/
long int		ft_atoi(const char *nptr);
bool			its_integer(char *str);
bool			all_positiv_num(char **av);

/*free.c*/
void	free_split(int	**tab);
void	free_all(t_data *data);

/*init.c*/
void	init_philo(t_data *data, char **av);
void	init_mutex(t_data *data, char **av);

/*DEL AFTER -- test.c*/
void	print_test_init(t_data *data);

#endif 
