/*
Ben Wil\liams-Van Zant
cs430
project 5
backslash is click
and arrows to move
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/keyboard.h>
#include <linux/uaccess.h>
#include <linux/io.h>

struct input_dev *micky;
struct notifier_block nb;
int press_state = 0;

void key_map(short code){	
    if(code == 43){// backslash is click
		if (press_state == 0){
			press_state = 1;
		}
		else{
			press_state = 0;
		}
		input_report_key(micky, BTN_LEFT, press_state);
		input_sync(micky);
    }
    if(code == 103){//up arrow
		input_report_rel(micky, REL_Y, -5);
		input_sync(micky);
    }
    if(code == 105){//left arrow
		input_report_rel(micky, REL_X, -5);
		input_sync(micky);
    }
    if(code == 106){//right arrow
		input_report_rel(micky, REL_X, 5);
		input_sync(micky);
    }
    if(code == 108){//down arrow
		input_report_rel(micky, REL_Y, 5);
		input_sync(micky);
    }
}

int kb_notifier_fn(struct notifier_block * nb, unsigned long key_press, void * data){    
    struct keyboard_notifier_param * kp = (struct keyboard_notifier_param*)data;
    if(key_press == KBD_KEYCODE){
        if(kp->down){key_map(kp->value);}
				if(!(kp ->down) && kp->value == 43){key_map(kp->value);}
    }
    return 0;
}

int init_module(){
	nb.notifier_call = kb_notifier_fn;
	micky = input_allocate_device();
	micky->name = "micky";
	set_bit(EV_REL, micky->evbit);
	set_bit(REL_X, micky->relbit);
	set_bit(REL_Y, micky->relbit);
	set_bit(EV_KEY, micky->evbit);
	set_bit(BTN_LEFT, micky->keybit);
	input_register_device(micky);
	register_keyboard_notifier(&nb);
	return 0;
}

void cleanup_module(){
	unregister_keyboard_notifier(&nb);
}

