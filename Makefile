# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) -fsanitize=address -g #-std=c99 
INCLUDES = -Iinc -I$(LIBFT_DIR) -I$(MLX_DIR)
# MLX = -lmlx -framework OpenGL -framework AppKit
MLX =  -L$(MLX_DIR) -lmlx_Linux -L/usr/lib -lXext -lX11 -lm

# Output executable
NAME = miniRT

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
ORANGE = \033[0;38;5;166m

# Source files
SRCDIR = srcs/

SRCS_FIL = \
		math_utils/vector.c \
		math_utils/quaternion.c \
		math_utils/print_math.c \
		main.c\
		test.c \
		ray.c \
		mlx.c \

SRCS = $(addprefix $(SRCDIR), $(SRCS_FIL))

# Object files
OBJDIR = objs/
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))

# Libraries
LIBFT_DIR = libft/
LIBFT_A = $(LIBFT_DIR)libft.a
MLX_DIR = minilibx_linux/
MLX_A = $(MLX_DIR)libmlx_Linux.a


# Build targets
all: $(OBJDIR) $(NAME)

bonus: all

$(OBJDIR):
	@mkdir -p $(OBJDIR) $(addprefix $(OBJDIR), $(dir $(SRCS_FIL)))

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)
	@make -C $(MLX_DIR)
	@$(CC) $(LIBFT_A) $(MLX_A) $(CFLAGS) $(OBJS) -o $(NAME) $(MLX) && echo "$(GREEN)$(NAME) was created$(RESET)"

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ && echo "$(GREEN)object files were created$(RESET)"

# Cleanup
RM = rm -rf

clean:
	@$(RM) $(OBJDIR) && echo "$(ORANGE)object files were deleted$(RESET)"
	@make clean -C $(LIBFT_DIR) && echo "$(ORANGE)libft object files were deleted$(RESET)"
	@make clean -C $(MLX_DIR) && echo "$(ORANGE)MLX object files were deleted$(RESET)"

fclean: clean
	@$(RM) $(NAME) && echo "$(ORANGE)$(NAME) was deleted$(RESET)"
	@make fclean -C $(LIBFT_DIR) && echo "$(ORANGE)libft.a was deleted$(RESET)"
	@make clean -C $(MLX_DIR) && echo "$(ORANGE)MLX object files were deleted$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus
