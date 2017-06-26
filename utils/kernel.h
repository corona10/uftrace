#ifndef __UFTRACE_KERNEL_H__
#define __UFTRACE_KERNEL_H__

#define KERNEL_NOP_TRACER    "nop"
#define KERNEL_GRAPH_TRACER  "function_graph"

struct uftrace_kernel {
	int pid;
	int nr_cpus;
	int depth;
	bool skip_out;
	unsigned long bufsize;
	char *tracer;
	int *traces;
	int *fds;
	int64_t *offsets;
	int64_t *sizes;
	void **mmaps;
	struct kbuffer **kbufs;
	struct pevent *pevent;
	struct uftrace_record *rstacks;
	struct uftrace_rstack_list *rstack_list;
	bool *rstack_valid;
	bool *rstack_done;
	int *missed_events;
	int *tids;
	char *output_dir;
	struct list_head filters;
	struct list_head notrace;
	struct list_head patches;
	struct list_head nopatch;
	struct list_head events;
};

/* these functions will be used at record time */
int setup_kernel_tracing(struct uftrace_kernel *kernel, struct opts *opts);
int start_kernel_tracing(struct uftrace_kernel *kernel);
int record_kernel_tracing(struct uftrace_kernel *kernel);
int record_kernel_trace_pipe(struct uftrace_kernel *kernel, int cpu, int sock);
int stop_kernel_tracing(struct uftrace_kernel *kernel);
int finish_kernel_tracing(struct uftrace_kernel *kernel);
void list_kernel_events(void);

/* these functions will be used at replay time */
int setup_kernel_data(struct uftrace_kernel *kernel);
int read_kernel_stack(struct ftrace_file_handle *handle,
		      struct ftrace_task_handle **taskp);
int read_kernel_cpu_data(struct uftrace_kernel *kernel, int cpu);
void * read_kernel_event(struct uftrace_kernel *kernel, int cpu, int *psize);
int finish_kernel_data(struct uftrace_kernel *kernel);

static inline bool has_kernel_data(struct uftrace_kernel *kernel)
{
	return kernel && kernel->pevent != NULL;
}

static inline bool has_kernel_event(char *events)
{
	return events && strstr(events, "@kernel");
}

bool check_kernel_pid_filter(void);

#endif /* __UFTRACE_KERNEL_H__ */
