WINDOWS = -D WIN
DEBUG   = -g  -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr 
SOURCE = src/attribute.cpp src/graph.cpp src/database.cpp src/buffer.cpp src/tree.cpp


all: lin

lin: 
	g++ -o akinator main.cpp $(SOURCE)

lin_debug:
	g++ $(DEBUG) -o akinator_debug main.cpp $(SOURCE)

win:
	g++ $(WINDOWS) -o akinator.exe main.cpp $(SOURCE)

win_debug:
	g++ $(WINDOWS) $(DEBUG) -o akinator_debug.exe main.cpp $(SOURCE)
