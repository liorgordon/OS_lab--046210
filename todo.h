/*
 * todo.h
 *
 *  Created on: 27 бреб 2018
 *      Author: lior
 */

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#ifndef TODO_H_
#define TODO_H_

typedef struct TODO_st{
	struct list_head list;
	char* description;
	int description_len;
	time_t dead_line;
}todo_stack;

int sys_push_TODO(pid_t pid, const char* TODO_description, ssize_t description_size, time_t deadline);
int sys_peek_TODO(pid_t pid, char* TODO_description, ssize_t* description_size, time_t* deadline);
int sys_pop_TODO(pid_t pid);

#endif /* TODO_H_ */
