/*
 * todo_api.h
 *
 *  Created on: 28 бреб 2018
 *      Author: lior
 */
#include <linux/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>


#ifndef TODO_API_H_
#define TODO_API_H_
int push_TODO(pid_t pid, const char *TODO_description, ssize_t description_size, time_t deadline){
	if(!TODO_description||description_size<0){
		errno=EINVAL;
		return -1;
	}

	int res;
	__asm__
	(
	"pushl %%eax;"
	"pushl %%ebx;"
	"pushl %%ecx;"
	"pushl %%edx;"
	"movl $243, %%eax;"
	"movl %1, %%ebx;"
	"movl %2, %%ecx;"
	"movl %3, %%edx;"
	"int $0x80;"
	"movl %%eax,%0;"
	"popl %%edx;"
	"popl %%ecx;"
	"popl %%ebx;"
	"popl %%eax;"
	: "=m" (res)
	: "m" (pid) ,"m" (TODO_description) ,"m"(description_size) ,"m"(deadline)
	);
	if (res >= (unsigned long)(-125))
	{
	errno = -res;
	res = -1;
	}
	return (int) res;

}

int peek_TODO(pid_t pid, char *TODO_description, ssize_t* description_size, time_t* deadline){
	if(!TODO_description||!description_size||!deadline){
		errno=EINVAL;
		return -1;
	}

	int res;
	__asm__
	(
	"pushl %%eax;"
	"pushl %%ebx;"
	"pushl %%ecx;"
	"pushl %%edx;"
	"movl $244, %%eax;"
	"movl %1, %%ebx;"
	"movl %2, %%ecx;"
	"movl %3, %%edx;"
	"int $0x80;"
	"movl %%eax,%0;"
	"popl %%edx;"
	"popl %%ecx;"
	"popl %%ebx;"
	"popl %%eax;"
	: "=m" (res)
	: "m" (pid) ,"m" (*TODO_description) ,"m"(description_size), "m"(deadline)
	);
	if (res >= (unsigned long)(-125))
	{
	errno = -res;
	res = -1;
	}
	return (int) res;

}

int pop_TODO(pid_t pid){
	int res;
	__asm__
	(
	"pushl %%eax;"
	"pushl %%ebx;"
	"pushl %%ecx;"
	"pushl %%edx;"
	"movl $245, %%eax;"
	"movl %1, %%ebx;"
	"movl %2, %%ecx;"
	"movl %3, %%edx;"
	"int $0x80;"
	"movl %%eax,%0;"
	"popl %%edx;"
	"popl %%ecx;"
	"popl %%ebx;"
	"popl %%eax;"
	: "=m" (res)
	: "m" (pid)
	);
	if (res >= (unsigned long)(-125))
	{
	errno = -res;
	res = -1;
	}
	return (int) res;

}



#endif /* TODO_API_H_ */
