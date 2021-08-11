#define PORT 0x3f8	  // COM1
#define NULL 0

#include <kernel/serial.h>
/*
 * write a bytes
 * */
void outportb(uint16_t port, uint8_t val)
{
	asm volatile("outb %1, %0"
				 :
				 : "dN"(port), "a"(val));
}

/*
 * read a byte
 * */
uint8_t inportb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0"
				 : "=a"(ret)
				 : "Nd"(port));
	return ret;
}

/*
 * Read 2 bytes
 * */
uint16_t inports(uint16_t _port)
{
	uint16_t rv;
	asm volatile("inw %1, %0"
				 : "=a"(rv)
				 : "dN"(_port));
	return rv;
}

/*
 * Write 2 bytes
 * */
void outports(uint16_t _port, uint16_t _data)
{
	asm volatile("outw %1, %0"
				 :
				 : "dN"(_port), "a"(_data));
}

/*
 * Readt 4 bytes
 * */
uint32_t inportl(uint16_t _port)
{
	uint32_t rv;
	asm volatile("inl %%dx, %%eax"
				 : "=a"(rv)
				 : "dN"(_port));
	return rv;
}

/*
 * Write 4 bytes
 * */
void outportl(uint16_t _port, uint32_t _data)
{
	asm volatile("outl %%eax, %%dx"
				 :
				 : "dN"(_port), "a"(_data));
}

// Functions needed for QEMU
void vsprintf(char * str, void (*putchar)(char), const char * format, va_list arg)
{
	uint32_t pos = 0;
	vsprintf_helper(str, putchar, format, &pos, arg);
}
void itoa(char * buf, unsigned long int n, int base)
{
	unsigned long int tmp;
	int				  i, j;

	tmp = n;
	i	= 0;

	do {
		tmp		 = n % base;
		buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
	} while (n /= base);
	buf[i--] = 0;

	for (j = 0; j < i; j++, i--) {
		tmp	   = buf[j];
		buf[j] = buf[i];
		buf[i] = tmp;
	}
}
int strcpy(char * dst, const char * src)
{
	int i = 0;
	while ((*dst++ = *src++) != 0)
		i++;
	return i;
}

void vsprintf_helper(char * str, void (*putchar)(char), const char * format, uint32_t * pos, va_list arg)
{
	char	 c;
	int		 sign, ival, sys;
	char	 buf[512];
	char	 width_str[10];
	uint32_t uval;
	uint32_t size = 8;
	uint32_t i;
	int		 size_override = 0;
	memset(buf, 0, 512);

	while ((c = *format++) != 0) {
		sign = 0;

		if (c == '%') {
			c = *format++;
			switch (c) {
				// Handle calls like printf("%08x", 0xaa);
				case '0':
					/*   size_override = 1;
                    // Get the number between 0 and (x/d/p...)
                    i = 0;
                    c = *format;
                   // while(!is_format_letter(c)) {
                   //     width_str[i++] = c;
                   //     format++;
                   //     c = *format;
                   // }
                    width_str[i] = 0;
                    format++;
                    // Convert to a number
                    size = atoi(width_str);*/
				case 'd':
				case 'u':
				case 'x':
				case 'p':
					if (c == 'd' || c == 'u')
						sys = 10;
					else
						sys = 16;

					uval = ival = va_arg(arg, int);
					if (c == 'd' && ival < 0) {
						sign = 1;
						uval = -ival;
					}
					itoa(buf, uval, sys);
					uint32_t len = strlen(buf);
					// If use did not specify width, then just use len = width
					if (!size_override)
						size = len;
					if ((c == 'x' || c == 'p' || c == 'd') && len < size) {
						for (i = 0; i < len; i++) {
							buf[size - 1 - i] = buf[len - 1 - i];
						}
						for (i = 0; i < size - len; i++) {
							buf[i] = '0';
						}
					}
					if (c == 'd' && sign) {
						if (str) {
							*(str + *pos) = '-';
							*pos		  = *pos + 1;
						} else
							(*putchar)('-');
					}
					if (str) {
						strcpy(str + *pos, buf);
						*pos = *pos + strlen(buf);
					} else {
						char * t = buf;
						while (*t) {
							putchar(*t);
							t++;
						}
					}
					break;
				case 'c':
					if (str) {
						*(str + *pos) = (char)va_arg(arg, int);
						*pos		  = *pos + 1;
					} else {
						(*putchar)((char)va_arg(arg, int));
					}
					break;
				case 's':
					if (str) {
						char * t = (char *)va_arg(arg, int);
						strcpy(str + (*pos), t);
						*pos = *pos + strlen(t);
					} else {
						char * t = (char *)va_arg(arg, int);
						while (*t) {
							putchar(*t);
							t++;
						}
					}
					break;
				default:
					break;
			}
			continue;
		}
		if (str) {
			*(str + *pos) = c;
			*pos		  = *pos + 1;
		} else {
			(*putchar)(c);
		}
	}
}


// Receiving Data
int serial_received()
{
	return inportb(PORT + 5) & 1;
}

char read_serial()
{
	while (serial_received() == 0)
		;

	return inportb(PORT);
}

// Sending Data
int is_transmit_empty()
{
	return inportb(PORT + 5) & 0x20;
}

void write_serial(char a)
{
	while (is_transmit_empty() == 0)
		;

	outportb(PORT, a);
}

// Print to QEMU
void qemu_printf(const char * s, ...)
{
	va_list(ap);
	va_start(ap, s);
	vsprintf(NULL, write_serial, s, ap);
	va_end(ap);
}

void serial_init(void)
{
	outportb(PORT + 1, 0x00);
	outportb(PORT + 3, 0x80);
	outportb(PORT + 0, 0x03);
	outportb(PORT + 1, 0x00);
	outportb(PORT + 3, 0x03);
	outportb(PORT + 2, 0xC7);
	outportb(PORT + 4, 0x0B);
}
