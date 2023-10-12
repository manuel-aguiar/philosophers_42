## BUILD DETAILS

##CHANGE COMPILER AND MAKE

NAME 		=		philo_bonus

CC 			= 		cc -g

## -Wall Wextra Werror
FLAGS 		= 		-Wall -Werror -Wextra
LIBS		=		-lpthread

## FOLDER PATH ##
INC_PATH	=		incs
SRC_PATH	=		srcs
OBJ_PATH	=		objs

SRC_FILES	=		philo_bonus.c		\
					set_the_table.c		\
					the_life.c			\
					the_routine.c		\
					the_wait.c			\
					time_and_print.c	\
					mini_libft.c		

SRCS		:=		$(addprefix $(SRC_PATH)/,$(SRC_FILES)) \

OBJS 		=		$(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o,$(SRCS))

INCS		= 		-I$(INC_PATH)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) $(INCS) -o $(NAME) $(LIBS)
	@echo Program $(NAME) ready!!

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(INCS) -c $< -o $@ $(LIBS)

clean:
	@if [ -d "$(OBJ_PATH)" ]; then \
        rm -rf $(OBJ_PATH); \
    fi
	@echo Objects successfully deleted!

fclean: clean
	@if [ -e "$(NAME)" ]; then \
        rm $(NAME); \
    fi
	@echo Done!!

re: fclean all

git: fclean
	@git add . ; git commit -m "commiting" ; git push ; make

.PHONY: clean fclean re git