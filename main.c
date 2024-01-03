#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LOWERS  "abcdefghijklmnopqrstuvwxyz"
#define UPPERS  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "1234567890"
#define SYMBOLS "!\"#\\$%&'()*+,-./:;<=>?@[]^_`{|}~"

char* generate(int, char**);
int random(int, int);
void help(struct option[]);

int main(int argc, char** argv) {

    struct option long_options[] = {

		/* required parameters */
		{ "length", required_argument, NULL, 'l' },

        /* include/exclude */
        { "include", optional_argument, NULL, 'i' },
        { "exclude", optional_argument, NULL, 'e' },

		/* flags */
		{ "alpha-low", no_argument, NULL, 'a' },
        { "alpha-high", no_argument, NULL, 'A' }, 
		{ "numbers", no_argument, NULL, 'n' },
		{ "symbols", no_argument, NULL, 's' },

        { "help", no_argument, NULL, 'h' },
		
		{ NULL, 0, NULL, 0 }

	};

	/* v^v^v */
	unsigned int length = 0;
    char include[129];
    char exclude[129];
    bool use_lower = false;
    bool use_upper = false;
    bool use_numbers = false;
    bool use_symbols = false;
	
	/* process arguments */
	int opt;
	while ((opt = getopt_long(argc, argv, "l:i:e:aAnsh", long_options, NULL)) != -1) {
		switch (opt) {

            case 'l':
                length = atoi(optarg);
                break;

			case 'i':
                strcpy(include, optarg);
                break;

            case 'e':
                strcpy(exclude, optarg);
                break;
			
			case 'a':
                use_lower = true;
                break;
            
            case 'A':
                use_upper = true;
                break;

            case 'n':
                use_numbers = true;
                break;

            case 's':
                use_symbols = true;
                break;

			case 'h':
				help(long_options);
				exit(EXIT_SUCCESS);
			
			default:
				help(long_options);
				exit(EXIT_FAILURE);
		
		}
	}

    /* check arguments */
    if (length < 1) {
        printf("Length required.\n");
        help(long_options);
        exit(EXIT_FAILURE);
    } else if (use_lower + use_upper + use_numbers + use_symbols == 0) {
        printf("Must specify characters to use.\n");
        help(long_options);
        exit(EXIT_FAILURE);
    }

    char* choices[5] = { NULL, NULL, NULL, NULL, NULL }; int i = 0;
    if (use_lower)   { choices[i] = LOWERS; i++; }
    if (use_upper)   { choices[i] = UPPERS; i++; }
    if (use_numbers) { choices[i] = NUMBERS; i++; }
    if (use_symbols) { choices[i] = SYMBOLS; i++; }

    char* final = generate(length, choices);
    printf("%s", final);
    free(final);

    return 0;

}

char* generate(int l, char** strs) {
    
    // get the length of strs
    char** p = strs; while (*p) { p++; }
    int strs_len = p - strs;

    // make an array of each length of every str in strs
    int slens[strs_len];
    for (int i = 0; i < strs_len; i++) {
        slens[i] = strlen(strs[i]);
    }

    char* final = malloc(sizeof(char) * l+1);
    final[l] = '\0';
    for (int x = 0; x < l; x++) {
        int s = random(0, strs_len-1);
        int sl = slens[s];
        char* sv = strs[s];
        final[x] = sv[random(0, sl-1)];
    }

    return final;

}

int random(int min, int max) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand((unsigned) ts.tv_nsec);
    return (rand() % (max - min + 1)) + min;
}

void help(struct option opts[]) {
	printf("\n");
	printf("Parameters: \n");
	while (opts->name != NULL) {
		printf(" --%s/-%c", opts->name, opts->val);
		switch (opts->has_arg) {

			case required_argument:
				printf(" <arg>");
				break;

			case optional_argument:
				printf(" [arg]");
				break;

			default:
				break;

		}
		printf("\n");
		opts++;
	}
	printf("\n");
}
