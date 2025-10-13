#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

static ssize_t nulo_read(struct file *filp, char __user *data, size_t s, loff_t *off) {
  printk(KERN_ALERT "Ni a ganchos leo todo lo que escribiste.\n");
  return 0;
}

static ssize_t nulo_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
  return s;
}

static struct file_operations nulo_operaciones = {
  .owner = THIS_MODULE,
  .read = nulo_read,
  .write = nulo_write,
};

static struct cdev nulo_device;
static dev_t major;
static struct class *nulo_class;

static int __init nulo_init(void) {

  char *name = "nulo";

  cdev_init(&nulo_device,  &nulo_operaciones);           //Inicializo como dispositivo
  
  alloc_chrdev_region(&major, 0, 1, name);              //Consigo major

  cdev_add(&nulo_device, major, 1);                      //asigno numeros

  nulo_class = class_create(THIS_MODULE, name);         //creo clase?
  device_create(nulo_class, NULL, major, NULL, name);   //asigno nodo?


  printk(KERN_ALERT "Cargando modulo nulo\n");
  return 0;
}

static void __exit nulo_exit(void) {
  
  unregister_chrdev_region(major, 1);                     //Elimino major
  
  cdev_del(&nulo_device);                                 //Cierro device?

  device_destroy(nulo_class, major);                      //Destruyo device?
  
  class_destroy(nulo_class);                              //Destruyo clase


  printk(KERN_ALERT "Descargando modulo nulo\n");
}




module_init(nulo_init);
module_exit(nulo_exit);



MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de '/dev/null'");
