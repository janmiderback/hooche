#ifndef IO_H_
#define IO_H_

t_bool io_input_available(void);
char*  io_getln(void);

void io_tell_gui(const char* fmt, ...);
void io_print(const char* fmt, ...);
void io_println(const char* fmt, ...);
void io_prompt();


#endif /*IO_H_*/
