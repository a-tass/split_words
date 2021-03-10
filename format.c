#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

int correctArg(char *s) //�������� ����������� ����������� ��������
{
	int len = strlen(s);
	int i, n;
	n = 0;
	for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
		n = 10 * n + (s[i] - '0');
	if (len != i) return (-1);
	else return(n);
}

void format(FILE *f1, FILE *f2, int words) //�������������� ������
{
	int length = 0;
	char *pointer = malloc(sizeof(char));
	char c;
	int count = 0;//���������� ��������� ����
	int ifWord = 0;//��� �� ������� ������ ������
	while ((c = getc(f1))!=EOF) //������� ������
	{
		length++;
		pointer = realloc(pointer, length * sizeof(char) + 1);
		if (c == ' ' || c == '\r' || c == '\t') {
			if (ifWord) count++;//����� �����
				ifWord = 0;
				pointer[length - 1] = c;
				continue;
		}
		if (c == '\n') { //���� ������ �����������, ������� ���� ������
			count = 0;
			ifWord = 0;
			pointer[length - 1] = c;
			continue;
		}
		if (count == words) {//����� ������
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

int main(int argc, char * argv[]) //argc ��������� ����� ���������� �������, argv - ������ ���������� �� �������
{
	int opt; //�����
	opterr = 0; //���������� ����������� ��������� ������
	int wArg = -1; //�������� ����� -w
	int ifOpt = 0; //��������� ����� ��� ���
	while ((opt = getopt(argc, argv, "w:")) != -1)
		switch (opt) {
		case 'w':
			if ((wArg = correctArg(optarg)) <= 0)
			{
				fprintf(stderr, "%s: invalid argument\n", argv[0]); //�������� �������� �����, argv[0] - ��� ���������
				exit(EXIT_FAILURE);
			}
			ifOpt = 1;
			break;
		default: //������
			fprintf(stderr, "%s is invalid\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	if (!ifOpt) {
		fprintf(stderr, "%s: option is required\n", argv[0]); //����� �����������
		exit(EXIT_FAILURE);
	}
	//���������� �������� ��������� � argv[optind] �� argv[argc-1], argv[argc] = null 
	FILE *inputFile, *outputFile;
	if (optind == argc) { // = null 
		inputFile = stdin;
		outputFile = stdout;
	}
	else if (optind == argc - 1) { //�������� ���� ��� �����
		inputFile = fopen(argv[optind], "r");
		outputFile = stdout;
	}
	else if (optind == argc - 2) { //�������� ��� ����� �����
		if (strcmp(argv[optind], argv[optind + 1]) == 0)  //����� ������ ���������
			inputFile = fopen(argv[optind], "r+");
		else {
			inputFile = fopen(argv[optind], "r");
			outputFile = fopen(argv[optind + 1], "w");
		}
	}
	else if (optind < argc - 2) { //�������� ������ ���� ���� ������
		fprintf(stderr, "%s: more than two files written\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (!inputFile || !outputFile) { //��� ������� � �����
		fprintf(stderr, "%s: file's not available\n", argv[0]);
		if (inputFile) fclose(inputFile);
		if (outputFile) fclose(outputFile);
		exit(EXIT_FAILURE);
	}
	format(inputFile, outputFile, wArg);
	if (inputFile) fclose(inputFile); //�������� ������
	if (outputFile) fclose(outputFile);	
}