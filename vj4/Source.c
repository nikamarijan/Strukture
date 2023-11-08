#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 50
#define MAX_LINE 1024
typedef struct _polinom polinom;
typedef polinom* pozicija;
struct _polinom {
	int koef;
	int pot;
	pozicija next;
};
int readFile(pozicija, char*);
int sortedInput(pozicija, int, int);
int unos(pozicija, int, int);
pozicija dodajHead();
int IspisPolinoma(pozicija);


int main() {

	pozicija head1 = NULL;
	pozicija head2 = NULL;
	head1 = dodajHead();
	head2 = dodajHead();
	char* imeDatoteke1 = "polinom.txt";
	char* imeDatoteke2 = "polinom1.txt";
	readFile(head1, imeDatoteke1);
	IspisPolinoma(head1);
	readFile(head2, imeDatoteke2);
	IspisPolinoma(head2);




	return 0;
}
int readFile(pozicija pol, char* imeDatoteke)
{

	FILE* filePointer = NULL;
	char buffer[MAX_LINE] = { 0 };
	char* buf;
	int koef = 0, pot = 0, br = 0;
	int status = 0;


	filePointer = fopen(imeDatoteke, "r");
	if (filePointer == NULL) {


		printf("nemogu se otvoriti datoteke");
		return -1;
	}
	fgets(buffer, MAX_LINE, filePointer);
	buf = buffer;
	while (!strlen(buf) == 0) {
		sscanf(buf, "%dx*%d %n", &koef, &pot, &br);
		unos(pol, koef, pot);
		buf = buf + br;

	}


	return 0;
}
pozicija dodajHead() {
	pozicija head = NULL;
	head = (pozicija)malloc(sizeof(polinom));
	if (head == NULL)
		printf("kriva alokacija memorije\n");
	head->next = NULL;
	head->koef = 0;
	head->pot = 0;
	return head;

}
int unos(pozicija head, int koef, int pot) {
	pozicija p = head;
	pozicija q = NULL;
	if (p->next != NULL) {
		while (p->next != NULL && p->next->pot >= pot) {
			p = p->next;
		}
		if (p->pot == pot) {
			p->koef += koef;
			return 0;
		}
	}
	q = (pozicija)malloc(sizeof(polinom));

	if (q == NULL) {
		printf("\nGreska u alociranju memorije!");
		return 0;
	}
	q->koef = koef;
	q->pot = pot;
	q->next = p->next;
	p->next = q;


	return 0;
}
int IspisPolinoma(pozicija head) {
	pozicija p = head->next;

	printf("\n");
	while (p != NULL) {

		if (p->pot == 0) {
			if (p->koef > 0) {
				printf(" ");
				printf("%d ", p->koef);
				p = p->next;
			}
			else if (p->koef == 0) {
				p = p->next;
			}
			else if (p->koef < 0) {
				printf("%d ", p->koef);
				p = p->next;
			}
			continue;
		}

		if (p->koef > 0 && p->pot != 0) {
			printf(" ");
			printf("%dX*%d ", p->koef, p->pot);
			p = p->next;
		}
		else if (p->koef == 0 && p->pot != 0) {
			p = p->next;
		}
		else if (p->koef < 0 && p->pot != 0) {
			printf("%dX*%d ", p->koef, p->pot);
			p = p->next;
		}
	}

	return 0;
}