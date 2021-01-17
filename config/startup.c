extern unsigned char __text_end__;
extern unsigned char __bss_start__;
extern unsigned char __bss_end__;
extern unsigned char __data_start__;
extern void main();

void startup()
{
	unsigned char length;
	unsigned int *s,*begin,*end;
	s = (unsigned int*)&__text_end__;
	begin = (unsigned int*)&__data_start__;
	end = (unsigned int*)&__bss_start__;
	while(begin < end)
		*begin++ = *s++;
	
	begin = (unsigned int*)&__bss_start__;
	end = (unsigned int*)&__bss_end__;
	while(begin < end)
		*begin++ = 0;
	main();
} 