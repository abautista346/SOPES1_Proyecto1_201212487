#include <linux/fs.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/syscalls.h>

#define for_each_process(p) \
for (p = &init_task ; (p = next_task(p)) != &init_task ; )


static int meminfo_proc_show(struct seq_file *m, void *v)
    {
        //STRUCT PROCESO
        struct task_struct *task;
        //CONTADOR PROCESOS PADRE
        int ContPadre = 0;
        //CONTADOR PROCESOS HIJOS
        int ContHijos = 0;
        //ESTADO DEL PROCESO
        char Estado;
        char EstadoHijo;
        unsigned int usuario;
        char user[16];

        //LISTA JERARQUICA HTML
        //seq_printf(m,"<h4><ul id=\"menu\">");
        //ITERAMOS PARA SACAR TODOS LOS PROCESOS PADRE
        for_each_process(task)
        {
            struct task_struct *task_child;
            struct list_head *list;
            //ESTADO DEL PROCESO
            if(task->state == TASK_RUNNING){
            	Estado = 'R';
            }else if(task->state == TASK_STOPPED){
            	Estado = 'T';
            }else if(task->state == TASK_INTERRUPTIBLE){
            	Estado = 'S';
            }else if(task->state == TASK_UNINTERRUPTIBLE){
            	Estado = 'D';
            }else if(task->state == EXIT_ZOMBIE){
            	Estado = 'Z';		
            }else{
            	Estado = 'X';
            }
            //USUARIO DEL PROCESO
            usuario = task->cred->uid.val;
            if( usuario == 0){
            	strcpy(user,"root");
            }else{
            	strcpy(user,"plata346");
            }

            //PROCESOS PADRE           
            seq_printf(m,"  ++PID: %d -> NOMBRE:%s %d -> ESTADO: %c -> USUARIO: %s \n" ,                        
            task->pid,
            task->comm,
            (int)task->state,
            Estado,
            user);

            //LISTA INTERIOR DE LOS PROCESOS HIJOS 
            //seq_printf(m,"      <ul class=\"interior\">");

            list_for_each(list,&task->children) {
                task_child=list_entry(list,struct task_struct,sibling);
                //ESTADO DEL PROCESO HIJO
                if(task_child->state == TASK_RUNNING){
	            	EstadoHijo = 'R';
	            }else if(task_child->state == TASK_STOPPED){
	            	EstadoHijo = 'T';
	            }else if(task_child->state == TASK_INTERRUPTIBLE){
	            	EstadoHijo = 'S';
	            }else if(task_child->state == TASK_UNINTERRUPTIBLE){
	            	EstadoHijo = 'D';
	            }else if(task_child->state == EXIT_ZOMBIE){
	            	EstadoHijo = 'Z';		
	            }else{
	            	EstadoHijo = 'X';
	            }
	            //USUARIO DEL PROCESO HIJO
	            usuario = task_child->cred->uid.val;
	            if( usuario == 0){
	            	strcpy(user,"root");
	            }else{
	            	strcpy(user,"plata346");
	            }

                //PROCESOS HIJOS 
                //seq_printf(m,"          <li id=\"l%d\" ><a class=\"hiper\" href=\"#r\">%d-%s-%d-%c-%s</a></li>\n" ,
		seq_printf(m,"  --PID: %d -> NOMBRE:%s %d -> ESTADO: %c -> USUARIO: %s \n" ,                        
            task->pid,
            task->comm,
            (int)task->state,
            Estado,
            user);

                ContHijos++;
            }
            
            ContPadre++;
        }
        

		seq_printf(m,"Total Procesos\n");
		seq_printf(m,"Padres:%d \nHijos:%d",
		ContPadre, ContHijos);

        return 0;
    }
    
    static void __exit final(void) //Salida de modulo
    {   
        printk(KERN_INFO "Sistemas Operativos 1.\n");
        remove_proc_entry("cpu_201212487", NULL);
    }

    static int meminfo_proc_open(struct inode *inode, struct file *file)
    {
        return single_open(file, meminfo_proc_show, NULL);
    }

    static const struct file_operations meminfo_proc_fops = {
        .open       = meminfo_proc_open,
        .read       = seq_read,
        .llseek     = seq_lseek,
        .release    = single_release,
    };

    static int __init inicio(void) //Escribe archivo en /proc
    {
        printk(KERN_INFO "Alan Bautista.\n");
        proc_create("cpu_201212487", 0, NULL, &meminfo_proc_fops);
        return 0;
    }


    module_init(inicio);
    module_exit(final);

MODULE_AUTHOR("Alan Rene Bautista Barahona");
MODULE_DESCRIPTION("201212487");
MODULE_LICENSE("GPL");
