/* dummy.c -- A simple dummy network interface kernel module */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Dummy Network Driver for ARMv7");
MODULE_VERSION("1.0");

static struct net_device *my_dummy_dev;

// 模拟网卡的初始化设置
static void my_dummy_setup(struct net_device *dev)
{
    ether_setup(dev); // 填充以太网的默认配置
    dev->flags |= IFF_NOARP; // 虚拟网卡不需要 ARP 协议
    dev->flags &= ~IFF_MULTICAST; // 关闭组播
}

// 模块加载时执行
static int __init dummy_init(void)
{
    // 创建一个名为 dummy0 的网络设备
    my_dummy_dev = alloc_netdev(0, "dummy0", NET_NAME_UNKNOWN, my_dummy_setup);
    if (!my_dummy_dev) {
        printk(KERN_ERR "dummy: Failed to allocate net device\n");
        return -ENOMEM;
    }

    // 注册到内核网络子系统中
    if (register_netdev(my_dummy_dev)) {
        printk(KERN_ERR "dummy: Failed to register net device\n");
        free_netdev(my_dummy_dev);
        return -EIO;
    }

    printk(KERN_INFO "dummy: Module loaded, dummy0 device created!\n");
    return 0;
}

// 模块卸载时执行
static void __exit dummy_exit(void)
{
    if (my_dummy_dev) {
        unregister_netdev(my_dummy_dev);
        free_netdev(my_dummy_dev);
        printk(KERN_INFO "dummy: Module unloaded, dummy0 device removed!\n");
    }
}

module_init(dummy_init);
module_exit(dummy_exit);
