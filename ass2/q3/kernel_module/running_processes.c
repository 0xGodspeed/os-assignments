#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

MODULE_AUTHOR("gs");
MODULE_DESCRIPTION("Number of currently running processes");
MODULE_LICENSE("GPL");

static int __init custom_init(void) {
    struct task_struct *task;
    
   	int process_count = 0; 
		printk(KERN_NOTICE "Running processes:\n");
    rcu_read_lock(); // Lock the tasklist for reading
    for_each_process(task) {
				if (task->__state == 0)  {
					printk(KERN_INFO "PID: %d, Name: %s, State: %d\n", task->pid, task->comm, task->__state);
					process_count++;
				}

    }
   	printk(KERN_NOTICE "Running process count: %d", process_count);
    rcu_read_unlock(); // Unlock the tasklist
			    
    printk(KERN_INFO "Module loaded.");
    return 0;
}

static void __exit custom_exit(void) {
    printk(KERN_INFO "Exiting module...");
}

module_init(custom_init);
module_exit(custom_exit);

