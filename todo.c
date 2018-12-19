/*
* todo.c
 *
 *  Created on: 28 ���� 2018
 *      Author: lior
 */
#include <linux/todo.h>

/*find_queue: finds valid queue by PID and copies pointer to que.
	Returns 0 for success, ESRCH for failure*/

int find_queue(pid_t pid, struct task_struct* tsk)
{
    tsk = find_task_by_pid(pid);
    if (tsk == NULL){
        return ESRCH;
    }
    struct task_struct* tmp_task = tsk;
    pid_t ppid = current->pid;
    pid_t search_pid = tmp_task->pid;
    int i = 0;
    while (tmp_task != &init_task && search_pid != ppid)
    {
        tmp_task = tmp_task->p_pptr;
        search_pid = tmp_task->pid;
    }
    if (search_pid != ppid){
        return ESRCH;
    }
    return 0;
}

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
    struct task_struct* tsk = NULL;
    int result = find_queue(pid, tsk);
    if (result) {
        return -result;
    }
    tsk = find_task_by_pid(pid);
    struct todo_struct* todo = tsk->todo_head;
    if (todo == NULL){
        return -EINVAL;
    }
    if (description_size < todo->td_desc_size) {
        return -EINVAL;
    }
    else if (todo->td_desc_size < description_size){
        description_size = todo->td_desc_size;
    }
    if (copy_to_user(TODO_description, todo->td_description, description_size)){
        return -EFAULT;
    }
    *deadline = tmp_todo->dead_line;
    return 0;
}
int sys_pop_TODO(pid_t pid){

    struct task_struct* tsk = NULL;
    int result = find_queue(pid, tsk);
    if (result) {
        return -result;
    }
    tsk = find_task_by_pid(pid);
    struct todo_struct* todo = tsk->todo_head;
    if (todo == NULL){
        return -EINVAL;
    }
    list_del(&todo->todo_list);
    if (todo->td_description != NULL){
        kfree(todo->td_description);
    }
    kfree(todo);
    return 0;
}

