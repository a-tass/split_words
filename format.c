#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

int correctArg(char *s) //проверка присутствия посторонних символов
{
	int len = strlen(s);
	int i, n;
	n = 0;
	for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
		n = 10 * n + (s[i] - '0');
	if (len != i) return (-1);
	else return(n);
}

void format(FILE *f1, FILE *f2, int words) //форматирование текста
{
	int length = 0;
	char *pointer = malloc(sizeof(char));
	char c;
	int count = 0;//количество считанных слов
	int ifWord = 0;//был ли прошлый символ словом
	while ((c = getc(f1))!=EOF) //считать символ
	{
		length++;
		pointer = realloc(pointer, length * sizeof(char) + 1);
		if (c == ' ' || c == '\r' || c == '\t') {
			if (ifWord) count++;//новое слово
				ifWord = 0;
				pointer[length - 1] = c;
				continue;
		}
		if (c == '\n') { //если строка закончилась, подсчет слов заново
			count = 0;
			ifWord = 0;
			pointer[length - 1] = c;
			continue;
		}
		if (count == words) {//новая строка
			pointer[length - 2] = '\n';
			count = 0;
			ifWord = 1;
			pointer[length - 1] = c;
			continue;
		}
		ifWord = 1;
		pointer[length - 1] = c;
	}
	pointer[length] = '\0';
	for (int i = 0; i < length + 1; i++)
		putc(pointer[i], f2);
	free(pointer);
}

int main(int argc, char * argv[]) //argc указывает число аргументов команды, argv - массив указателей на символы
{
	int opt; //опция
	opterr = 0; //отключение стандартной обработки ошибок
	int wArg = -1; //аргумент опции -w
	int ifOpt = 0; //считалась опция или нет
	while ((opt = getopt(argc, argv, "w:")) != -1)
		switch (opt) {
		case 'w':
			if ((wArg = correctArg(optarg)) <= 0)
			{
				fprintf(stderr, "%s: invalid argument\n", argv[0]); //неверный аргумент опции, argv[0] - имя программы
				exit(EXIT_FAILURE);
			}
			ifOpt = 1;
			break;
		default: //ошибка
			fprintf(stderr, "%s is invalid\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	if (!ifOpt) {
		fprintf(stderr, "%s: option is required\n", argv[0]); //опция отсутствует
		exit(EXIT_FAILURE);
	}
	//оставшиеся операнды находятся с argv[optind] по argv[argc-1], argv[argc] = null 
	FILE *inputFile, *outputFile;
	if (optind == argc) { // = null 
		inputFile = stdin;
		outputFile = stdout;
	}
	else if (optind == argc - 1) { //перадано одно имя файла
		inputFile = fopen(argv[optind], "r");
		outputFile = stdout;
	}
	else if (optind == argc - 2) { //переданы два имени файла
		if (strcmp(argv[optind], argv[optind + 1]) == 0)  //имена файлов совпадают
			inputFile = fopen(argv[optind], "r+");
		else {
			inputFile = fopen(argv[optind], "r");
			outputFile = fopen(argv[optind + 1], "w");
		}
	}
	else if (optind < argc - 2) { //перадано больше двух имен файлов
		fprintf(stderr, "%s: more than two files written\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (!inputFile || !outputFile) { //нет доступа к файлу
		fprintf(stderr, "%s: file's not available\n", argv[0]);
		if (inputFile) fclose(inputFile);
		if (outputFile) fclose(outputFile);
		exit(EXIT_FAILURE);
	}
	format(inputFile, outputFile, wArg);
	if (inputFile) fclose(inputFile); //закрытие файлов
	if (outputFile) fclose(outputFile);	
}