#pragma once
#include <linux/proc_fs.h>
#include <linux/types.h>

bool init_proc_entry(void);

void deinit_proc_entry(void);
