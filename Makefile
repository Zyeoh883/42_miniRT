# Compiler and flags
CC = gcc -o3 -march=native -flto -fno-strict-aliasing -ffast-math -funroll-loops -msse4 -std=gnu11
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) #-g -fsanitize=address #-std=c99 
INCLUDES = -Iinc -I$(LIBFT_DIR) -I$(MLX_DIR)
MLX = -Lminilibx_linux -lGL -lOpenCL -lX11 -lXext -lm

# Output executable
NAME = miniRT

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
ORANGE = \033[0;38;5;166m

# Source files
SRCDIR = srcs/

# create/oriented_bounding_box.c
# intersects/object_intercept.c
SRCS_FIL = \
		main.c\
		mlx_handlers/mlx.c \
		create/scene_objects.c \
		error_handling/memory_error.c \
		utils/utils.c \
		math_utils/vector.c \
		math_utils/quaternion.c \
		math_utils/print_math.c \
		mlx_handlers/input_translate.c \

SRCS = $(addprefix $(SRCDIR), $(SRCS_FIL))

# Object files
OBJDIR = objs/
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))

# Libraries
LIBFT_DIR = libft/
LIBFT_A = $(LIBFT_DIR)libft.a

MLX_DIR = minilibx_linux/
MLX_A = $(MLX_DIR)libmlx.a

# Build targets
all: $(OBJDIR) $(NAME)

bonus: all

$(OBJDIR):
	@mkdir -p $(OBJDIR) $(addprefix $(OBJDIR), $(dir $(SRCS_FIL)))

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)
	@make -C $(MLX_DIR)
	@$(CC) $(OBJS) $(CFLAGS) $(LIBFT_A) $(MLX_A) $(MLX) -o $(NAME) && echo "$(GREEN)$(NAME) was created$(RESET)"
	export DISPLAY=:0	

# export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0.0
	# @$(CC) $(LIBFT_A) $(MLX_A) $(CFLAGS) $(MLX) $(OBJS) -o $(NAME) && echo "$(GREEN)$(NAME) was created$(RESET)"

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
	@make clean -C $(MLX_DIR) && echo "$(ORANGE)libmlx.a was deleted$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus
