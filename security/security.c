 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
#include <linux/module.h>
/* Maximum number of letters for an LSM name string */
#define SECURITY_NAME_MAX	10
static __initdata char chosen_lsm[SECURITY_NAME_MAX + 1] =
	CONFIG_DEFAULT_SECURITY;
static void __init do_security_initcalls(void)
	initcall_t *call;
	call = __security_initcall_start;
	while (call < __security_initcall_end) {
		(*call) ();
		call++;
	pr_info("Security Framework initialized\n");
	 * Load minor LSMs, with the capability module always first.
	capability_add_hooks();
	yama_add_hooks();
	loadpin_add_hooks();
	 * Load all the remaining security modules.
	do_security_initcalls();
/* Save user chosen LSM */
static int __init choose_lsm(char *str)
	strncpy(chosen_lsm, str, SECURITY_NAME_MAX);
	return 1;
__setup("security=", choose_lsm);
 * security_module_enable - Load given security module on boot ?
 * @module: the name of the module
 * Each LSM must pass this method before registering its own operations
 * to avoid security registration races. This method may also be used
 * to check if your LSM is currently loaded during kernel initialization.
 * Return true if:
 *	-The passed LSM is the one chosen by user at boot time,
 *	-or the passed LSM is configured as the default and the user did not
 *	 choose an alternate LSM at boot time.
 * Otherwise, return false.
int __init security_module_enable(const char *module)
	return !strcmp(module, chosen_lsm);
		list_for_each_entry(P, &security_hook_heads.FUNC, list)	\
		list_for_each_entry(P, &security_hook_heads.FUNC, list) { \
int security_binder_set_context_mgr(struct task_struct *mgr)
int security_binder_transaction(struct task_struct *from,
				struct task_struct *to)
int security_binder_transfer_binder(struct task_struct *from,
				    struct task_struct *to)
int security_binder_transfer_file(struct task_struct *from,
				  struct task_struct *to, struct file *file)
}

		     kernel_cap_t *effective,
		     kernel_cap_t *inheritable,
		     kernel_cap_t *permitted)
				effective, inheritable, permitted);
				effective, inheritable, permitted);
int security_capable(const struct cred *cred, struct user_namespace *ns,
		     int cap)
	return call_int_hook(capable, 0, cred, ns, cap, SECURITY_CAP_AUDIT);
}

int security_capable_noaudit(const struct cred *cred, struct user_namespace *ns,
			     int cap)
{
	return call_int_hook(capable, 0, cred, ns, cap, SECURITY_CAP_NOAUDIT);
	list_for_each_entry(hp, &security_hook_heads.vm_enough_memory, list) {
int security_bprm_set_creds(struct linux_binprm *bprm)
	return call_int_hook(bprm_set_creds, 0, bprm);
int security_bprm_secureexec(struct linux_binprm *bprm)
	return call_int_hook(bprm_secureexec, 0, bprm);
	return call_int_hook(sb_alloc_security, 0, sb);
int security_sb_copy_data(char *orig, char *copy)
	return call_int_hook(sb_copy_data, 0, orig, copy);
EXPORT_SYMBOL(security_sb_copy_data);
int security_sb_remount(struct super_block *sb, void *data)
	return call_int_hook(sb_remount, 0, sb, data);
int security_sb_kern_mount(struct super_block *sb, int flags, void *data)
	return call_int_hook(sb_kern_mount, 0, sb, flags, data);
                       const char *type, unsigned long flags, void *data)
int security_sb_pivotroot(const struct path *old_path, const struct path *new_path)
				struct security_mnt_opts *opts,
				unsigned long kern_flags,
				unsigned long *set_kern_flags)
				opts->num_mnt_opts ? -EOPNOTSUPP : 0, sb,
				opts, kern_flags, set_kern_flags);
				struct super_block *newsb)
	return call_int_hook(sb_clone_mnt_opts, 0, oldsb, newsb);
int security_sb_parse_opts_str(char *options, struct security_mnt_opts *opts)
	return call_int_hook(sb_parse_opts_str, 0, options, opts);
EXPORT_SYMBOL(security_sb_parse_opts_str);
	inode->i_security = NULL;
	return call_int_hook(inode_alloc_security, 0, inode);
					const struct qstr *name, void **ctx,
					u32 *ctxlen)
	return call_int_hook(dentry_init_security, -EOPNOTSUPP, dentry, mode,
				name, ctx, ctxlen);
				name, old, new);
						&lsm_xattr->name,
						&lsm_xattr->value,
						&lsm_xattr->value_len);
int security_old_inode_init_security(struct inode *inode, struct inode *dir,
				     const struct qstr *qstr, const char **name,
				     void **value, size_t *len)
	if (unlikely(IS_PRIVATE(inode)))
		return -EOPNOTSUPP;
	return call_int_hook(inode_init_security, -EOPNOTSUPP, inode, dir,
			     qstr, name, value, len);
EXPORT_SYMBOL(security_old_inode_init_security);
int security_path_mknod(const struct path *dir, struct dentry *dentry, umode_t mode,
			unsigned int dev)
int security_path_mkdir(const struct path *dir, struct dentry *dentry, umode_t mode)
		     (d_is_positive(new_dentry) && IS_PRIVATE(d_backing_inode(new_dentry)))))
	if (flags & RENAME_EXCHANGE) {
		int err = call_int_hook(path_rename, 0, new_dir, new_dentry,
					old_dir, old_dentry);
		if (err)
			return err;
	}

				new_dentry);
	return call_int_hook(path_chown, 0, path, uid, gid);
}

#endif
int security_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode)
			 struct dentry *new_dentry)
			    const char *old_name)
int security_inode_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev)
			   struct inode *new_dir, struct dentry *new_dentry,
			   unsigned int flags)
        if (unlikely(IS_PRIVATE(d_backing_inode(old_dentry)) ||
            (d_is_positive(new_dentry) && IS_PRIVATE(d_backing_inode(new_dentry)))))
						     old_dir, old_dentry);
					   new_dir, new_dentry);
int security_inode_setattr(struct dentry *dentry, struct iattr *attr)
	return evm_inode_setattr(dentry, attr);
int security_inode_setxattr(struct dentry *dentry, const char *name,
	ret = call_int_hook(inode_setxattr, 1, dentry, name, value, size,
				flags);
	return evm_inode_setxattr(dentry, name, value, size);
int security_inode_removexattr(struct dentry *dentry, const char *name)
	ret = call_int_hook(inode_removexattr, 1, dentry, name);
		ret = cap_inode_removexattr(dentry, name);
	return evm_inode_removexattr(dentry, name);
int security_inode_killpriv(struct dentry *dentry)
	return call_int_hook(inode_killpriv, 0, dentry);
int security_inode_getsecurity(struct inode *inode, const char *name, void **buffer, bool alloc)
		return -EOPNOTSUPP;
	list_for_each_entry(hp, &security_hook_heads.inode_getsecurity, list) {
		rc = hp->hook.inode_getsecurity(inode, name, buffer, alloc);
		if (rc != -EOPNOTSUPP)
	return -EOPNOTSUPP;
int security_inode_setsecurity(struct inode *inode, const char *name, const void *value, size_t size, int flags)
		return -EOPNOTSUPP;
	list_for_each_entry(hp, &security_hook_heads.inode_setsecurity, list) {
								flags);
		if (rc != -EOPNOTSUPP)
	return -EOPNOTSUPP;
int security_inode_listsecurity(struct inode *inode, char *buffer, size_t buffer_size)
	return call_int_hook(inode_copy_up_xattr, -EOPNOTSUPP, name);
	return call_int_hook(file_alloc_security, 0, file);
			unsigned long flags)
	ret = call_int_hook(mmap_file, 0, file, prot,
					mmap_prot(file, prot), flags);
	return ima_file_mmap(file, prot);
			    unsigned long prot)
	return call_int_hook(file_mprotect, 0, vma, reqprot, prot);
				  struct fown_struct *fown, int sig)
int security_file_open(struct file *file, const struct cred *cred)
	ret = call_int_hook(file_open, 0, file, cred);
int security_task_create(unsigned long clone_flags)
	return call_int_hook(task_create, 0, clone_flags);
	return call_int_hook(cred_alloc_blank, 0, cred, gfp);
	return call_int_hook(cred_prepare, 0, new, old, gfp);
	return call_int_hook(kernel_module_request, 0, kmod_name);
int security_kernel_read_file(struct file *file, enum kernel_read_file_id id)
	ret = call_int_hook(kernel_read_file, 0, file, id);
	return ima_read_file(file, id);
void security_task_getsecid(struct task_struct *p, u32 *secid)
	call_void_hook(task_getsecid, p, secid);
EXPORT_SYMBOL(security_task_getsecid);
		struct rlimit *new_rlim)
int security_task_kill(struct task_struct *p, struct siginfo *info,
			int sig, u32 secid)
	return call_int_hook(task_kill, 0, p, info, sig, secid);
}

int security_task_wait(struct task_struct *p)
{
	return call_int_hook(task_wait, 0, p);
			 unsigned long arg4, unsigned long arg5)
	int rc = -ENOSYS;
	list_for_each_entry(hp, &security_hook_heads.task_prctl, list) {
		if (thisrc != -ENOSYS) {
	return call_int_hook(msg_msg_alloc_security, 0, msg);
int security_msg_queue_alloc(struct msg_queue *msq)
	return call_int_hook(msg_queue_alloc_security, 0, msq);
void security_msg_queue_free(struct msg_queue *msq)
int security_msg_queue_associate(struct msg_queue *msq, int msqflg)
int security_msg_queue_msgctl(struct msg_queue *msq, int cmd)
int security_msg_queue_msgsnd(struct msg_queue *msq,
			       struct msg_msg *msg, int msqflg)
int security_msg_queue_msgrcv(struct msg_queue *msq, struct msg_msg *msg,
			       struct task_struct *target, long type, int mode)
int security_shm_alloc(struct shmid_kernel *shp)
	return call_int_hook(shm_alloc_security, 0, shp);
void security_shm_free(struct shmid_kernel *shp)
int security_shm_associate(struct shmid_kernel *shp, int shmflg)
int security_shm_shmctl(struct shmid_kernel *shp, int cmd)
int security_shm_shmat(struct shmid_kernel *shp, char __user *shmaddr, int shmflg)
int security_sem_alloc(struct sem_array *sma)
	return call_int_hook(sem_alloc_security, 0, sma);
void security_sem_free(struct sem_array *sma)
int security_sem_associate(struct sem_array *sma, int semflg)
int security_sem_semctl(struct sem_array *sma, int cmd)
int security_sem_semop(struct sem_array *sma, struct sembuf *sops,
			unsigned nsops, int alter)
int security_getprocattr(struct task_struct *p, char *name, char **value)
	return call_int_hook(getprocattr, -EINVAL, p, name, value);
int security_setprocattr(struct task_struct *p, char *name, void *value, size_t size)
{
	return call_int_hook(setprocattr, -EINVAL, p, name, value, size);
	return call_int_hook(secid_to_secctx, -EOPNOTSUPP, secid, secdata,
				seclen);

int security_unix_stream_connect(struct sock *sock, struct sock *other, struct sock *newsk)
						protocol, kern);
int security_socket_bind(struct socket *sock, struct sockaddr *address, int addrlen)
int security_socket_connect(struct socket *sock, struct sockaddr *address, int addrlen)
int security_socket_getpeersec_stream(struct socket *sock, char __user *optval,
				      int __user *optlen, unsigned len)
				optval, optlen, len);
int security_socket_getpeersec_dgram(struct socket *sock, struct sk_buff *skb, u32 *secid)
void security_sk_classify_flow(struct sock *sk, struct flowi *fl)
	call_void_hook(sk_getsecid, sk, &fl->flowi_secid);
void security_req_classify_flow(const struct request_sock *req, struct flowi *fl)
	call_void_hook(req_classify_flow, req, fl);
int security_inet_conn_request(struct sock *sk,
			struct sk_buff *skb, struct request_sock *req)
			const struct request_sock *req)
			struct sk_buff *skb)
	return call_int_hook(secmark_relabel_packet, 0, secid);
EXPORT_SYMBOL(security_secmark_relabel_packet);
void security_secmark_refcount_inc(void)
	call_void_hook(secmark_refcount_inc);
EXPORT_SYMBOL(security_secmark_refcount_inc);
void security_secmark_refcount_dec(void)
	call_void_hook(secmark_refcount_dec);
EXPORT_SYMBOL(security_secmark_refcount_dec);
int security_tun_dev_alloc_security(void **security)
	return call_int_hook(tun_dev_alloc_security, 0, security);
EXPORT_SYMBOL(security_tun_dev_alloc_security);
void security_tun_dev_free_security(void *security)
	call_void_hook(tun_dev_free_security, security);
EXPORT_SYMBOL(security_tun_dev_free_security);
int security_tun_dev_create(void)
	return call_int_hook(tun_dev_create, 0);
EXPORT_SYMBOL(security_tun_dev_create);
int security_tun_dev_attach_queue(void *security)
	return call_int_hook(tun_dev_attach_queue, 0, security);
EXPORT_SYMBOL(security_tun_dev_attach_queue);
int security_tun_dev_attach(struct sock *sk, void *security)
	return call_int_hook(tun_dev_attach, 0, sk, security);
EXPORT_SYMBOL(security_tun_dev_attach);
int security_tun_dev_open(void *security)
	return call_int_hook(tun_dev_open, 0, security);
EXPORT_SYMBOL(security_tun_dev_open);

#endif	/* CONFIG_SECURITY_NETWORK */

			      struct xfrm_sec_ctx **new_ctxp)
int security_xfrm_policy_lookup(struct xfrm_sec_ctx *ctx, u32 fl_secid, u8 dir)
	return call_int_hook(xfrm_policy_lookup, 0, ctx, fl_secid, dir);
				       const struct flowi *fl)
	int rc = 1;
	list_for_each_entry(hp, &security_hook_heads.xfrm_state_pol_flow_match,
				list) {
		rc = hp->hook.xfrm_state_pol_flow_match(x, xp, fl);
void security_skb_classify_flow(struct sk_buff *skb, struct flowi *fl)
	int rc = call_int_hook(xfrm_decode_session, 0, skb, &fl->flowi_secid,
				0);


int security_key_permission(key_ref_t key_ref,
			    const struct cred *cred, unsigned perm)
	return call_int_hook(key_permission, 0, key_ref, cred, perm);
int security_key_getsecurity(struct key *key, char **_buffer)
	*_buffer = NULL;
	return call_int_hook(key_getsecurity, 0, key, _buffer);


int security_audit_rule_match(u32 secid, u32 field, u32 op, void *lsmrule,
			      struct audit_context *actx)
	return call_int_hook(audit_rule_match, 0, secid, field, op, lsmrule,
				actx);
struct security_hook_heads security_hook_heads = {
	.binder_set_context_mgr =
		LIST_HEAD_INIT(security_hook_heads.binder_set_context_mgr),
	.binder_transaction =
		LIST_HEAD_INIT(security_hook_heads.binder_transaction),
	.binder_transfer_binder =
		LIST_HEAD_INIT(security_hook_heads.binder_transfer_binder),
	.binder_transfer_file =
		LIST_HEAD_INIT(security_hook_heads.binder_transfer_file),

	.ptrace_access_check =
		LIST_HEAD_INIT(security_hook_heads.ptrace_access_check),
	.ptrace_traceme =
		LIST_HEAD_INIT(security_hook_heads.ptrace_traceme),
	.capget =	LIST_HEAD_INIT(security_hook_heads.capget),
	.capset =	LIST_HEAD_INIT(security_hook_heads.capset),
	.capable =	LIST_HEAD_INIT(security_hook_heads.capable),
	.quotactl =	LIST_HEAD_INIT(security_hook_heads.quotactl),
	.quota_on =	LIST_HEAD_INIT(security_hook_heads.quota_on),
	.syslog =	LIST_HEAD_INIT(security_hook_heads.syslog),
	.settime =	LIST_HEAD_INIT(security_hook_heads.settime),
	.vm_enough_memory =
		LIST_HEAD_INIT(security_hook_heads.vm_enough_memory),
	.bprm_set_creds =
		LIST_HEAD_INIT(security_hook_heads.bprm_set_creds),
	.bprm_check_security =
		LIST_HEAD_INIT(security_hook_heads.bprm_check_security),
	.bprm_secureexec =
		LIST_HEAD_INIT(security_hook_heads.bprm_secureexec),
	.bprm_committing_creds =
		LIST_HEAD_INIT(security_hook_heads.bprm_committing_creds),
	.bprm_committed_creds =
		LIST_HEAD_INIT(security_hook_heads.bprm_committed_creds),
	.sb_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.sb_alloc_security),
	.sb_free_security =
		LIST_HEAD_INIT(security_hook_heads.sb_free_security),
	.sb_copy_data =	LIST_HEAD_INIT(security_hook_heads.sb_copy_data),
	.sb_remount =	LIST_HEAD_INIT(security_hook_heads.sb_remount),
	.sb_kern_mount =
		LIST_HEAD_INIT(security_hook_heads.sb_kern_mount),
	.sb_show_options =
		LIST_HEAD_INIT(security_hook_heads.sb_show_options),
	.sb_statfs =	LIST_HEAD_INIT(security_hook_heads.sb_statfs),
	.sb_mount =	LIST_HEAD_INIT(security_hook_heads.sb_mount),
	.sb_umount =	LIST_HEAD_INIT(security_hook_heads.sb_umount),
	.sb_pivotroot =	LIST_HEAD_INIT(security_hook_heads.sb_pivotroot),
	.sb_set_mnt_opts =
		LIST_HEAD_INIT(security_hook_heads.sb_set_mnt_opts),
	.sb_clone_mnt_opts =
		LIST_HEAD_INIT(security_hook_heads.sb_clone_mnt_opts),
	.sb_parse_opts_str =
		LIST_HEAD_INIT(security_hook_heads.sb_parse_opts_str),
	.dentry_init_security =
		LIST_HEAD_INIT(security_hook_heads.dentry_init_security),
	.dentry_create_files_as =
		LIST_HEAD_INIT(security_hook_heads.dentry_create_files_as),
#ifdef CONFIG_SECURITY_PATH
	.path_unlink =	LIST_HEAD_INIT(security_hook_heads.path_unlink),
	.path_mkdir =	LIST_HEAD_INIT(security_hook_heads.path_mkdir),
	.path_rmdir =	LIST_HEAD_INIT(security_hook_heads.path_rmdir),
	.path_mknod =	LIST_HEAD_INIT(security_hook_heads.path_mknod),
	.path_truncate =
		LIST_HEAD_INIT(security_hook_heads.path_truncate),
	.path_symlink =	LIST_HEAD_INIT(security_hook_heads.path_symlink),
	.path_link =	LIST_HEAD_INIT(security_hook_heads.path_link),
	.path_rename =	LIST_HEAD_INIT(security_hook_heads.path_rename),
	.path_chmod =	LIST_HEAD_INIT(security_hook_heads.path_chmod),
	.path_chown =	LIST_HEAD_INIT(security_hook_heads.path_chown),
	.path_chroot =	LIST_HEAD_INIT(security_hook_heads.path_chroot),
#endif
	.inode_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.inode_alloc_security),
	.inode_free_security =
		LIST_HEAD_INIT(security_hook_heads.inode_free_security),
	.inode_init_security =
		LIST_HEAD_INIT(security_hook_heads.inode_init_security),
	.inode_create =	LIST_HEAD_INIT(security_hook_heads.inode_create),
	.inode_link =	LIST_HEAD_INIT(security_hook_heads.inode_link),
	.inode_unlink =	LIST_HEAD_INIT(security_hook_heads.inode_unlink),
	.inode_symlink =
		LIST_HEAD_INIT(security_hook_heads.inode_symlink),
	.inode_mkdir =	LIST_HEAD_INIT(security_hook_heads.inode_mkdir),
	.inode_rmdir =	LIST_HEAD_INIT(security_hook_heads.inode_rmdir),
	.inode_mknod =	LIST_HEAD_INIT(security_hook_heads.inode_mknod),
	.inode_rename =	LIST_HEAD_INIT(security_hook_heads.inode_rename),
	.inode_readlink =
		LIST_HEAD_INIT(security_hook_heads.inode_readlink),
	.inode_follow_link =
		LIST_HEAD_INIT(security_hook_heads.inode_follow_link),
	.inode_permission =
		LIST_HEAD_INIT(security_hook_heads.inode_permission),
	.inode_setattr =
		LIST_HEAD_INIT(security_hook_heads.inode_setattr),
	.inode_getattr =
		LIST_HEAD_INIT(security_hook_heads.inode_getattr),
	.inode_setxattr =
		LIST_HEAD_INIT(security_hook_heads.inode_setxattr),
	.inode_post_setxattr =
		LIST_HEAD_INIT(security_hook_heads.inode_post_setxattr),
	.inode_getxattr =
		LIST_HEAD_INIT(security_hook_heads.inode_getxattr),
	.inode_listxattr =
		LIST_HEAD_INIT(security_hook_heads.inode_listxattr),
	.inode_removexattr =
		LIST_HEAD_INIT(security_hook_heads.inode_removexattr),
	.inode_need_killpriv =
		LIST_HEAD_INIT(security_hook_heads.inode_need_killpriv),
	.inode_killpriv =
		LIST_HEAD_INIT(security_hook_heads.inode_killpriv),
	.inode_getsecurity =
		LIST_HEAD_INIT(security_hook_heads.inode_getsecurity),
	.inode_setsecurity =
		LIST_HEAD_INIT(security_hook_heads.inode_setsecurity),
	.inode_listsecurity =
		LIST_HEAD_INIT(security_hook_heads.inode_listsecurity),
	.inode_getsecid =
		LIST_HEAD_INIT(security_hook_heads.inode_getsecid),
	.inode_copy_up =
		LIST_HEAD_INIT(security_hook_heads.inode_copy_up),
	.inode_copy_up_xattr =
		LIST_HEAD_INIT(security_hook_heads.inode_copy_up_xattr),
	.file_permission =
		LIST_HEAD_INIT(security_hook_heads.file_permission),
	.file_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.file_alloc_security),
	.file_free_security =
		LIST_HEAD_INIT(security_hook_heads.file_free_security),
	.file_ioctl =	LIST_HEAD_INIT(security_hook_heads.file_ioctl),
	.mmap_addr =	LIST_HEAD_INIT(security_hook_heads.mmap_addr),
	.mmap_file =	LIST_HEAD_INIT(security_hook_heads.mmap_file),
	.file_mprotect =
		LIST_HEAD_INIT(security_hook_heads.file_mprotect),
	.file_lock =	LIST_HEAD_INIT(security_hook_heads.file_lock),
	.file_fcntl =	LIST_HEAD_INIT(security_hook_heads.file_fcntl),
	.file_set_fowner =
		LIST_HEAD_INIT(security_hook_heads.file_set_fowner),
	.file_send_sigiotask =
		LIST_HEAD_INIT(security_hook_heads.file_send_sigiotask),
	.file_receive =	LIST_HEAD_INIT(security_hook_heads.file_receive),
	.file_open =	LIST_HEAD_INIT(security_hook_heads.file_open),
	.task_create =	LIST_HEAD_INIT(security_hook_heads.task_create),
	.task_free =	LIST_HEAD_INIT(security_hook_heads.task_free),
	.cred_alloc_blank =
		LIST_HEAD_INIT(security_hook_heads.cred_alloc_blank),
	.cred_free =	LIST_HEAD_INIT(security_hook_heads.cred_free),
	.cred_prepare =	LIST_HEAD_INIT(security_hook_heads.cred_prepare),
	.cred_transfer =
		LIST_HEAD_INIT(security_hook_heads.cred_transfer),
	.kernel_act_as =
		LIST_HEAD_INIT(security_hook_heads.kernel_act_as),
	.kernel_create_files_as =
		LIST_HEAD_INIT(security_hook_heads.kernel_create_files_as),
	.kernel_module_request =
		LIST_HEAD_INIT(security_hook_heads.kernel_module_request),
	.kernel_read_file =
		LIST_HEAD_INIT(security_hook_heads.kernel_read_file),
	.kernel_post_read_file =
		LIST_HEAD_INIT(security_hook_heads.kernel_post_read_file),
	.task_fix_setuid =
		LIST_HEAD_INIT(security_hook_heads.task_fix_setuid),
	.task_setpgid =	LIST_HEAD_INIT(security_hook_heads.task_setpgid),
	.task_getpgid =	LIST_HEAD_INIT(security_hook_heads.task_getpgid),
	.task_getsid =	LIST_HEAD_INIT(security_hook_heads.task_getsid),
	.task_getsecid =
		LIST_HEAD_INIT(security_hook_heads.task_getsecid),
	.task_setnice =	LIST_HEAD_INIT(security_hook_heads.task_setnice),
	.task_setioprio =
		LIST_HEAD_INIT(security_hook_heads.task_setioprio),
	.task_getioprio =
		LIST_HEAD_INIT(security_hook_heads.task_getioprio),
	.task_setrlimit =
		LIST_HEAD_INIT(security_hook_heads.task_setrlimit),
	.task_setscheduler =
		LIST_HEAD_INIT(security_hook_heads.task_setscheduler),
	.task_getscheduler =
		LIST_HEAD_INIT(security_hook_heads.task_getscheduler),
	.task_movememory =
		LIST_HEAD_INIT(security_hook_heads.task_movememory),
	.task_kill =	LIST_HEAD_INIT(security_hook_heads.task_kill),
	.task_wait =	LIST_HEAD_INIT(security_hook_heads.task_wait),
	.task_prctl =	LIST_HEAD_INIT(security_hook_heads.task_prctl),
	.task_to_inode =
		LIST_HEAD_INIT(security_hook_heads.task_to_inode),
	.ipc_permission =
		LIST_HEAD_INIT(security_hook_heads.ipc_permission),
	.ipc_getsecid =	LIST_HEAD_INIT(security_hook_heads.ipc_getsecid),
	.msg_msg_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.msg_msg_alloc_security),
	.msg_msg_free_security =
		LIST_HEAD_INIT(security_hook_heads.msg_msg_free_security),
	.msg_queue_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.msg_queue_alloc_security),
	.msg_queue_free_security =
		LIST_HEAD_INIT(security_hook_heads.msg_queue_free_security),
	.msg_queue_associate =
		LIST_HEAD_INIT(security_hook_heads.msg_queue_associate),
	.msg_queue_msgctl =
		LIST_HEAD_INIT(security_hook_heads.msg_queue_msgctl),
	.msg_queue_msgsnd =
		LIST_HEAD_INIT(security_hook_heads.msg_queue_msgsnd),
	.msg_queue_msgrcv =
		LIST_HEAD_INIT(security_hook_heads.msg_queue_msgrcv),
	.shm_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.shm_alloc_security),
	.shm_free_security =
		LIST_HEAD_INIT(security_hook_heads.shm_free_security),
	.shm_associate =
		LIST_HEAD_INIT(security_hook_heads.shm_associate),
	.shm_shmctl =	LIST_HEAD_INIT(security_hook_heads.shm_shmctl),
	.shm_shmat =	LIST_HEAD_INIT(security_hook_heads.shm_shmat),
	.sem_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.sem_alloc_security),
	.sem_free_security =
		LIST_HEAD_INIT(security_hook_heads.sem_free_security),
	.sem_associate =
		LIST_HEAD_INIT(security_hook_heads.sem_associate),
	.sem_semctl =	LIST_HEAD_INIT(security_hook_heads.sem_semctl),
	.sem_semop =	LIST_HEAD_INIT(security_hook_heads.sem_semop),
	.netlink_send =	LIST_HEAD_INIT(security_hook_heads.netlink_send),
	.d_instantiate =
		LIST_HEAD_INIT(security_hook_heads.d_instantiate),
	.getprocattr =	LIST_HEAD_INIT(security_hook_heads.getprocattr),
	.setprocattr =	LIST_HEAD_INIT(security_hook_heads.setprocattr),
	.ismaclabel =	LIST_HEAD_INIT(security_hook_heads.ismaclabel),
	.secid_to_secctx =
		LIST_HEAD_INIT(security_hook_heads.secid_to_secctx),
	.secctx_to_secid =
		LIST_HEAD_INIT(security_hook_heads.secctx_to_secid),
	.release_secctx =
		LIST_HEAD_INIT(security_hook_heads.release_secctx),
	.inode_invalidate_secctx =
		LIST_HEAD_INIT(security_hook_heads.inode_invalidate_secctx),
	.inode_notifysecctx =
		LIST_HEAD_INIT(security_hook_heads.inode_notifysecctx),
	.inode_setsecctx =
		LIST_HEAD_INIT(security_hook_heads.inode_setsecctx),
	.inode_getsecctx =
		LIST_HEAD_INIT(security_hook_heads.inode_getsecctx),
#ifdef CONFIG_SECURITY_NETWORK
	.unix_stream_connect =
		LIST_HEAD_INIT(security_hook_heads.unix_stream_connect),
	.unix_may_send =
		LIST_HEAD_INIT(security_hook_heads.unix_may_send),
	.socket_create =
		LIST_HEAD_INIT(security_hook_heads.socket_create),
	.socket_post_create =
		LIST_HEAD_INIT(security_hook_heads.socket_post_create),
	.socket_bind =	LIST_HEAD_INIT(security_hook_heads.socket_bind),
	.socket_connect =
		LIST_HEAD_INIT(security_hook_heads.socket_connect),
	.socket_listen =
		LIST_HEAD_INIT(security_hook_heads.socket_listen),
	.socket_accept =
		LIST_HEAD_INIT(security_hook_heads.socket_accept),
	.socket_sendmsg =
		LIST_HEAD_INIT(security_hook_heads.socket_sendmsg),
	.socket_recvmsg =
		LIST_HEAD_INIT(security_hook_heads.socket_recvmsg),
	.socket_getsockname =
		LIST_HEAD_INIT(security_hook_heads.socket_getsockname),
	.socket_getpeername =
		LIST_HEAD_INIT(security_hook_heads.socket_getpeername),
	.socket_getsockopt =
		LIST_HEAD_INIT(security_hook_heads.socket_getsockopt),
	.socket_setsockopt =
		LIST_HEAD_INIT(security_hook_heads.socket_setsockopt),
	.socket_shutdown =
		LIST_HEAD_INIT(security_hook_heads.socket_shutdown),
	.socket_sock_rcv_skb =
		LIST_HEAD_INIT(security_hook_heads.socket_sock_rcv_skb),
	.socket_getpeersec_stream =
		LIST_HEAD_INIT(security_hook_heads.socket_getpeersec_stream),
	.socket_getpeersec_dgram =
		LIST_HEAD_INIT(security_hook_heads.socket_getpeersec_dgram),
	.sk_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.sk_alloc_security),
	.sk_free_security =
		LIST_HEAD_INIT(security_hook_heads.sk_free_security),
	.sk_clone_security =
		LIST_HEAD_INIT(security_hook_heads.sk_clone_security),
	.sk_getsecid =	LIST_HEAD_INIT(security_hook_heads.sk_getsecid),
	.sock_graft =	LIST_HEAD_INIT(security_hook_heads.sock_graft),
	.inet_conn_request =
		LIST_HEAD_INIT(security_hook_heads.inet_conn_request),
	.inet_csk_clone =
		LIST_HEAD_INIT(security_hook_heads.inet_csk_clone),
	.inet_conn_established =
		LIST_HEAD_INIT(security_hook_heads.inet_conn_established),
	.secmark_relabel_packet =
		LIST_HEAD_INIT(security_hook_heads.secmark_relabel_packet),
	.secmark_refcount_inc =
		LIST_HEAD_INIT(security_hook_heads.secmark_refcount_inc),
	.secmark_refcount_dec =
		LIST_HEAD_INIT(security_hook_heads.secmark_refcount_dec),
	.req_classify_flow =
		LIST_HEAD_INIT(security_hook_heads.req_classify_flow),
	.tun_dev_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.tun_dev_alloc_security),
	.tun_dev_free_security =
		LIST_HEAD_INIT(security_hook_heads.tun_dev_free_security),
	.tun_dev_create =
		LIST_HEAD_INIT(security_hook_heads.tun_dev_create),
	.tun_dev_attach_queue =
		LIST_HEAD_INIT(security_hook_heads.tun_dev_attach_queue),
	.tun_dev_attach =
		LIST_HEAD_INIT(security_hook_heads.tun_dev_attach),
	.tun_dev_open =	LIST_HEAD_INIT(security_hook_heads.tun_dev_open),
#endif	/* CONFIG_SECURITY_NETWORK */
#ifdef CONFIG_SECURITY_NETWORK_XFRM
	.xfrm_policy_alloc_security =
		LIST_HEAD_INIT(security_hook_heads.xfrm_policy_alloc_security),
	.xfrm_policy_clone_security =
		LIST_HEAD_INIT(security_hook_heads.xfrm_policy_clone_security),
	.xfrm_policy_free_security =
		LIST_HEAD_INIT(security_hook_heads.xfrm_policy_free_security),
	.xfrm_policy_delete_security =
		LIST_HEAD_INIT(security_hook_heads.xfrm_policy_delete_security),
	.xfrm_state_alloc =
		LIST_HEAD_INIT(security_hook_heads.xfrm_state_alloc),
	.xfrm_state_alloc_acquire =
		LIST_HEAD_INIT(security_hook_heads.xfrm_state_alloc_acquire),
	.xfrm_state_free_security =
		LIST_HEAD_INIT(security_hook_heads.xfrm_state_free_security),
	.xfrm_state_delete_security =
		LIST_HEAD_INIT(security_hook_heads.xfrm_state_delete_security),
	.xfrm_policy_lookup =
		LIST_HEAD_INIT(security_hook_heads.xfrm_policy_lookup),
	.xfrm_state_pol_flow_match =
		LIST_HEAD_INIT(security_hook_heads.xfrm_state_pol_flow_match),
	.xfrm_decode_session =
		LIST_HEAD_INIT(security_hook_heads.xfrm_decode_session),
#endif	/* CONFIG_SECURITY_NETWORK_XFRM */
#ifdef CONFIG_KEYS
	.key_alloc =	LIST_HEAD_INIT(security_hook_heads.key_alloc),
	.key_free =	LIST_HEAD_INIT(security_hook_heads.key_free),
	.key_permission =
		LIST_HEAD_INIT(security_hook_heads.key_permission),
	.key_getsecurity =
		LIST_HEAD_INIT(security_hook_heads.key_getsecurity),
#endif	/* CONFIG_KEYS */
#ifdef CONFIG_AUDIT
	.audit_rule_init =
		LIST_HEAD_INIT(security_hook_heads.audit_rule_init),
	.audit_rule_known =
		LIST_HEAD_INIT(security_hook_heads.audit_rule_known),
	.audit_rule_match =
		LIST_HEAD_INIT(security_hook_heads.audit_rule_match),
	.audit_rule_free =
		LIST_HEAD_INIT(security_hook_heads.audit_rule_free),
#endif /* CONFIG_AUDIT */
};