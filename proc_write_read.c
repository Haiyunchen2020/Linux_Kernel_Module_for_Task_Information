//procwr2-It works
/**
 * Kernel module that communicates with /proc file system.
 *
 * This provides the base logic for Project 2 - displaying task information
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>   /* not #include <asm/uaccess.h> */

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

/* the current pid */
static long l_pid;
char buffer[BUFFER_SIZE];

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
        .write = proc_write,
};


int proc_init(void)
{
    proc_create(PROC_NAME,0666,NULL,&proc_ops);
    printk(KERN_INFO "proc/pid created!\n");
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME,NULL);
    printk(KERN_INFO "proc/pid removed!\n");
}


/**
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 */


static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        //char buffer[BUFFER_SIZE];
        static int completed = 0;
        struct task_struct *tsk = NULL;

        if (completed) {
                completed = 0;
                return 0;
        }
        /*
        if (pid_task(find_vpid(l_pid),PIDTYPE_PID)!=NULL){
          	tsk=pid_task(find_vpid(l_pid),PIDTYPE_PID);
          }
          else {
          	printk(KERN_INFO "Error finding pid\n");
          	return 0;
          } */
        
        tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);
        
        rv = sprintf(buffer,"command = [%s], pid = [%ld], state = [%ld]\n",tsk->comm,l_pid,tsk->state);
        completed = 1;

        // copies the contents of kernel buffer to userspace usr_buf 
       
        if (copy_to_user(usr_buf, buffer, rv)) {
                rv = -1;
        } 
        // copy_to_user(usr_buf,buffer,rv);
        return rv;
}




/**
 * This function is called each time we write to the /proc file system.
 */

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
        char *k_mem;

        // allocate kernel memory
        k_mem = kmalloc(count, GFP_KERNEL);

        /* copies user space usr_buf to kernel buffer */
        if (copy_from_user(k_mem, usr_buf, count)) {
		printk( KERN_INFO "Error copying from user\n");
                return -1;
        }
	/**
 	 * kstrol() will not work because the strings are not guaranteed
	 * to be null-terminated.
	 * 
	 * sscanf() must be used instead.
	 */

        //char buffer[BUFFER_SIZE];
        sscanf(k_mem,"%s",buffer);
        kstrtol(buffer,10,&l_pid);
      

        kfree(k_mem);

        return count;  
        
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("write to and read from proc/pid");
MODULE_AUTHOR("Haiyun");
