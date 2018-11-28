/*
* todo.c
 *
 *  Created on: 28 бреб 2018
 *      Author: lior
 */
#include <linux/todo.h>
int sys_push_TODO(pid_t pid, const char* TODO_description, ssize_t description_size, time_t deadline){
//	allocating and copying data to a new TODO
	todo_stack* new_todo=(todo_stack*)kmalloc(sizeof(todo_stack));
	if(!new_todo){
//		allocation problem in kernel space
		return ENOMEM;
	}
	new_todo->description=(char*)kmalloc(sizeof(char)*description_size);
	if(!new_todo->description){
//		allocation problem in kernel space
		return ENOMEM;

	}
	if(copy_from_user(new_todo->description,TODO_description,sizeof(char)*description_size)!=0){
//	allcation problem from user space
		return EFALUT;
	}
	new_todo->description_len=description_size;
	new_todo->dead_line=deadline;

	INIT_LIST_HEAD(&new_todo->list);
	struct task_struct* tsk= find_task_by_pid(pid);
	if(!tsk){
//		no such pid
		return ESRCH;

	}
//	Checks if the pid checks out before adding
	int proc_pid= current->pid;
	while(proc_pid!=pid&&pid!=0){
		pid=getppid(pid);
	}
//	non descendant relationship
	if(proc_pid!=pid){
	  	return ESRCH;
	}
	list_add(&(new_todo->list),&(tsk->TODO_list->list));
	return 0;

}
int sys_peek_TODO(pid_t pid, char* TODO_description, ssize_t* description_size, time_t* deadline){

}
int sys_pop_TODO(pid_t pid){

}

