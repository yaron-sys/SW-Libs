// colors
#define RED(x) 		"\033[31;1m" x "\033[0m"
#define GREEN(x) 	"\033[32;1m" x "\033[0m"
#define YELLOW(x) 	"\033[33;1m" x "\033[0m"
#define BLUE(x) 	"\033[34;1m" x "\033[0m"
#define PURPULE(x) 	"\033[35;1m" x "\033[0m"
#define CYAN(x) 	"\033[36;1m" x "\033[0m"

// nullptr
#if _cplusplus < 201103L
#define nullptr 0
#endif // _cplusplus < 201103L
