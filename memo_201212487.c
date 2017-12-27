#include <linux/fs.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>


static int meminfo_proc_show(struct seq_file *sf, void *v){
    struct sysinfo i;
    long Usada;    
    
    si_meminfo(&i);
    
    Usada = i.totalram - i.freeram;
        
    Usada = (Usada * 100) / i.totalram;
    
    seq_printf(sf, "Alan Bautista 201212487\nOS: openSuse\nMemoria Total: %8lu kb\nMemoria Libre: %8lu kb\nPorcentaje Utilizado: %8lu \%\n", i.totalram, i.freeram, Usada);
    
    return 0;
}
    static void __exit final(void) //Salida de modulo
    {   
        printk(KERN_INFO "Sistemas Operativos 1.\n");
        remove_proc_entry("memo_201212487", NULL);
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
        printk(KERN_INFO "201212487.\n");
        proc_create("memo_201212487", 0, NULL, &meminfo_proc_fops);
        return 0;
    }


    module_init(inicio);
    module_exit(final);

MODULE_AUTHOR("Alan Rene Bautista Barahona");
MODULE_DESCRIPTION("201212487");
MODULE_LICENSE("GPL");
