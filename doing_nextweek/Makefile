LIBFT = ./src/libft/libft.a

all : ./src/libft/libft.a
	gcc -g3 -fsanitize=address ./src/*.c -I./include -L./src/libft -lft
ppm : all
	./a.out > test.ppm
run : all
	./a.out
fclean :
	rm -rf ./a.out
re : fclean all

$(LIBFT) :
	make -C ./src/libft all
