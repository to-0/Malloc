// zadanie1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define MAX 100
#include <stdio.h>
char* pam;
void* memory_alloc(unsigned int size) {
	int* p = pam;
	int* prev = NULL;
	unsigned int size_block = *p >> 1;
	int free = *p & 1;
	//najdem prvy volny blok
	while (free != 0 && p != NULL) {
		printf("\nP:%p\n", p);
		size_block = *p >> 1;
		printf("Size block:%d\n", size_block);
		p = ((char *)p+(4+size_block)); //tu to musim pretypovat na char aby som sa posuval po 1B, keby to bol int skacem +1 o 4B, posledna +1 lebo potrebujem vyskocit este z toho ukazovatela na int
		if (*p == 0) return NULL;
		printf("hladam som na:%p\n", p);
		free = *p & 1;
	}
	//ak je moc maly hladam pokial nenajdem alebo pokial neskoncim na konci
	size_block = *p >> 1;
	while ((free || size_block < size)&& p != NULL) {
		prev = p;
		 if (*(p + 1) == NULL) return NULL;
		p = *(p + 1);
		//if (p == NULL) return NULL;
		if (*p == 0) return NULL;
		size_block = *p >> 1;
		free = *p & 1;
	}
	//ak som na konci dovi
	if (p == NULL) return NULL;
	int new_size = size_block - size-4;
	if (new_size <= 0) { //ak by sa neoplatilo ani sekat ten blok na dalsi free lebo by bolmco maly, dam mu vacsiu velkost ked tak
		*p = size_block << 1 + 1;
		if (prev != NULL) *(prev + 1) = *(p+1);
		*(p + 1) = 0; //vymazem ten pointer odtial na next_fb
		return p + 1;
	} 
	int* next_fb = *(p + 1);
	*p = (size<<1)+1; //zakodujem velkost a ze je obsadene cize si velkost posuniem o 1 bit
	
	int* new_fb = ((char *)p + (4 + size));
	printf("\nNew fb adress:%p", new_fb);
	*new_fb = new_size << 1; //neviem ci to este cele netreba pretypovat na int ale hadam uz ne...
	*(new_fb + 1) = next_fb;
	printf("\nNext_fb %p\n", next_fb);
	if (prev != NULL) {
		*(prev + 1) = new_fb;
	}
	return p;
}
int memory_free(void* valid_ptr) {

}
int memory_check(void* ptr) {

}
void memory_init(void* ptr, unsigned int size) {
	int real_size = size - sizeof(int) - sizeof(int*);
	*(int*)ptr = (size - sizeof(int)-sizeof(int *))<< 1;
	int *foot = ((char*)ptr + 4 + real_size);
	*((int*)ptr + 1) =foot;
	char *test = ((char*)ptr + 4 + real_size); //+8 lebo 4B ma integer ktory urcuje velkost bloku,po nom ide pointer na dalsi novy blok, ten je na zaciatku pointer na patu celeho bloku
	*foot = NULL;
	printf("Koncim init: foot: %p next_free: %p\n",foot,*((int *)ptr+1));
}

int main()
{
	char pamat[100];
	char* k = pamat;
	int* k2 = pamat;
	printf("k:%p k2:%p, k+4:%p k2+1:%p\n",k,k2,(k+4),(k2+1));
	for (int i = 4; i < 10; i++) {
		pamat[i] = 99;
	}
	int* ptr = pamat;
	int* test;
	char* test2;
	printf("\n%d %d", sizeof(test), sizeof(test2));
	*ptr = 513;
	int free = *ptr & 1;
	int size = *ptr & ~1;
	printf("size: %d free: %d\n", size, free);
	for (int i = 0; i < 4; i++) {
		printf("%d", pamat[i]);
	}
	putchar('\n');
	pamat[10] = '\0';
	printf("%s\n", pamat);
	int* p = pamat;
	pam = pamat;
	printf("%d %p", *p,p);
	printf("\n%d\n", sizeof(ptr));
	printf("%p\n", pamat);
	memory_init(pamat, 100);
	printf("%p\n",pamat);
	printf("Po init haha %d %p\n", pamat[96],&pamat[96]);
	printf("%d", *p);
	printf("\nsize:%d free:%d",*p>>1,*p&1);

	char* p1 = (char*)memory_alloc(8);
	printf("\nsize:%d free:%d, %p", *(int *)p1 >> 1, *(int*)p1 & 1,p1);
	
	char* p2 = (char*)memory_alloc(20);

	printf("\nsize:%d free:%d, %p", *(int*)p2 >> 1, *(int*)p2 & 1, p2);
	printf("\n%p\n", &pamat[99]);
	char* p3 = (char*)memory_alloc(150);
	printf("\np3:%p", p3);
	return 0;
}

