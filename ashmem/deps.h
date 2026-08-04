#ifndef DEPS_H
#define DEPS_H

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 18, 0)
static inline void vma_set_anonymous(struct vm_area_struct *vma)
{
	vma->vm_ops = NULL;
}
#endif

/* vma->vm_flags became read-only in 6.3 (commit bc292ab00f6c, "mm:
 * introduce vma->vm_flags wrapper functions"). Older kernels don't have
 * vm_flags_clear() at all, so provide it ourselves there. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 3, 0)
static inline void vm_flags_clear(struct vm_area_struct *vma, unsigned long flags)
{
	vma->vm_flags &= ~flags;
}
#endif

/* mm->get_unmapped_area (a function pointer member on struct mm_struct)
 * was replaced by the free function mm_get_unmapped_area() as part of the
 * same era of mm rework. Verified present with this exact signature on
 * 6.17; NOT independently verified on which earlier release first
 * introduced it, so this boundary is a best-effort estimate grouped with
 * the vm_flags change above, not confirmed against kernel history the
 * way that one is - please adjust if CI/testing shows otherwise. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 3, 0)
static inline unsigned long mm_get_unmapped_area(struct mm_struct *mm,
		struct file *file, unsigned long addr, unsigned long len,
		unsigned long pgoff, unsigned long flags)
{
	return mm->get_unmapped_area(file, addr, len, pgoff, flags);
}
#endif

#endif // DEPS_H
