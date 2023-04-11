#hw.c
#include <linux/list.h>  // 包含內核鏈表的相關函數和符號
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/slab.h>

struct birthday {
        int day;
        int month;
        int year;
        struct list_head list;
};

static LIST_HEAD(birthday_list);  // 定義鏈表頭

int __init my_module_init(void)
{
        int i;
        struct birthday *person;

        // 創建五個 struct_birthday 元素
        for (i = 0; i < 5; i++) {
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        person->day = i + 1;
        person->month = 3;
        person->year = 2000 + i;
            INIT_LIST_HEAD(&person->list);  // 初始化元素的鏈表頭
            list_add_tail(&person->list, &birthday_list);  // 將元素添加到鏈表中
        }

        // 遍歷鏈表，輸出每個元素的內容到內核日誌緩衝區
        struct birthday *ptr;
        list_for_each_entry(ptr, &birthday_list, list) {
            printk(KERN_INFO "Birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
    }
    return 0;
}

void __exit my_module_exit(void)
{
    struct birthday *ptr, *next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
        printk(KERN_INFO "Removing birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
        list_del(&ptr->list);  // 從鏈表中刪除元素
        kfree(ptr);  // 釋放元素的內存空間
    }
}

module_init(my_module_init);
module_exit(my_module_exit);