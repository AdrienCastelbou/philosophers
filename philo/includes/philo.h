/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 10:58:08 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/31 10:58:10 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_philo {
	int				id;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*write;
	pthread_mutex_t	check_death;
	pthread_mutex_t	*check_end;
	long long int	t_die;
	long long int	t_eat;
	long long int	t_sleep;
	long long int	t_satiate;
	long long int	*t_start;
	int				*must_eat;
	int				t_must_eat;
	int				*finish;
	struct s_philo	*next;
	struct s_philo	*prev;
}				t_philo;

long int		ft_atolli(char *s);
t_philo			*set_philos2(t_philo *philo, t_philo *first_phil);
t_philo			*set_philos(t_philo *prev, int id,
					int nb, t_philo *first_phil);
t_philo			*set_first_philo3(t_philo *philo, int nb_params,
					char **params, int philos_nb);
t_philo			*set_first_philo2(t_philo *philo, int nb_params,
					char **params, int philos_nb);
t_philo			*set_first_philo(int nb_params, char **params, int	philos_nb);
void			free_philos(t_philo *philo, int philos_nb);
long long int	get_time(void);
void			ft_putchar(char c);
void			ft_putnbr(long long int nb);
int				ft_strlen(char *s);
void			ft_putstr(char *s);
int				check_end(t_philo *philo);
int				write_step(t_philo *philo, char *str);
void			ft_usleep(long long int delay);
void			philo_routine(t_philo *philo,
					pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
void			set_philo_finish_eat(t_philo *philo);
void			*run_philo(void *v_philo);
int				check_args(char **args);
int				check_params(t_philo *philo, int nb);
int				check_end_routine(t_philo *philo,
					long long int start, long long int time);
void			*monitoring(void	*v_philo);
void			*run_one_philo(void	*v_philo);
void			threads_init(t_philo *philo, pthread_t *threads,
					int philos_nb);
void			ft_free(t_philo *philo, pthread_t *threads, int philos_nb);
int				check_philos(t_philo *philo, int nb_philos);
long long int	time_to_usleep(t_philo *philo, long long int t_usleep);
#endif
