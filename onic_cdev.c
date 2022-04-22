#include <linux/version.h>
#include <linux/pci.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/netdevice.h>
#include <linux/miscdevice.h>
#include <linux/etherdevice.h>
#include <linux/limits.h>
#include <linux/vmalloc.h>

#include "qdma_access/qdma_register.h"
#include "onic.h"
#include "ioctl.h"

#define USERQID 9
#define DRV_NAME "ONIC"

inline static u16 onic_ring_get_real_count(struct onic_ring *ring)
{
	/* Valid writeback entry means one less count of descriptor entries */
	return (ring->wb) ? (ring->count - 1) : ring->count;
}

inline static bool onic_ring_full(struct onic_ring *ring)
{
	u16 real_count = onic_ring_get_real_count(ring);
	return ((ring->next_to_use + 1) % real_count) == ring->next_to_clean;
}

inline static void onic_ring_increment_head(struct onic_ring *ring)
{
	u16 real_count = onic_ring_get_real_count(ring);
	ring->next_to_use = (ring->next_to_use + 1) % real_count;
}

inline static void onic_ring_increment_tail(struct onic_ring *ring)
{
	u16 real_count = onic_ring_get_real_count(ring);
	ring->next_to_clean = (ring->next_to_clean + 1) % real_count;
}

static void onic_tx_clean(struct onic_tx_queue *q)
{
	struct onic_private *priv = netdev_priv(q->netdev);
	struct onic_ring *ring = &q->ring;
	struct qdma_wb_stat wb;
	int work, i;

	if (test_and_set_bit(0, q->state))
		return;

	qdma_unpack_wb_stat(&wb, ring->wb);

	if (wb.cidx == ring->next_to_clean) {
		clear_bit(0, q->state);
		return;
	}

	work = wb.cidx - ring->next_to_clean;
	if (work < 0)
		work += onic_ring_get_real_count(ring);

	for (i = 0; i < work; ++i) {
		struct onic_tx_buffer *buf = &q->buffer[ring->next_to_clean];
		struct sk_buff *skb = buf->skb;

		dma_unmap_single(&priv->pdev->dev, buf->dma_addr, buf->len,
				 DMA_TO_DEVICE);
		dev_kfree_skb_any(skb);

		onic_ring_increment_tail(ring);
	}

	clear_bit(0, q->state);
}

static void onic_clear_tx_queue(struct onic_private *priv, u16 qid)
{
	struct onic_tx_queue *q = priv->tx_queue[qid];
	struct onic_ring *ring;
	u32 size;
	int real_count;

	if (!q)
		return;

	onic_qdma_clear_tx_queue(priv->hw.qdma, qid);

	ring = &q->ring;
	real_count = ring->count - 1;
	size = QDMA_H2C_ST_DESC_SIZE * real_count + QDMA_WB_STAT_SIZE;
	size = ALIGN(size, PAGE_SIZE);

	if (ring->desc)
		dma_free_coherent(&priv->pdev->dev, size, ring->desc,
				  ring->dma_addr);
	kfree(q->buffer);
	kfree(q);
	priv->tx_queue[qid] = NULL;
}

static int onic_init_tx_queue(struct onic_private *priv, u16 qid)
{
	const u8 rngcnt_idx = 0;
	struct net_device *dev = priv->netdev;
	struct onic_tx_queue *q;
	struct onic_ring *ring;
	struct onic_qdma_h2c_param param;
	u16 vid;
	u32 size, real_count;
	int rv;
	bool debug = 0;

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	if (priv->tx_queue[qid]) {
		if (debug)
			netdev_info(dev, "Re-initializing TX queue %d", qid);
		onic_clear_tx_queue(priv, qid);
	}
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	q = kzalloc(sizeof(struct onic_tx_queue), GFP_KERNEL);
	if (!q)
		return -ENOMEM;

	/* evenly assign to TX queues available vectors */
	vid = qid % priv->num_q_vectors;

	q->netdev = dev;
	q->vector = priv->q_vector[vid];
	q->qid = qid;

	ring = &q->ring;
	ring->count = onic_ring_count(rngcnt_idx);
	real_count = ring->count - 1;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* allocate DMA memory for TX descriptor ring */
	size = QDMA_H2C_ST_DESC_SIZE * real_count + QDMA_WB_STAT_SIZE;
	size = ALIGN(size, PAGE_SIZE);
	ring->desc = dma_alloc_coherent(&priv->pdev->dev, size, &ring->dma_addr,
					GFP_KERNEL);
	if (!ring->desc) {
		rv = -ENOMEM;
		goto clear_tx_queue;
	}
	memset(ring->desc, 0, size);
	ring->wb = ring->desc + QDMA_H2C_ST_DESC_SIZE * real_count;
	ring->next_to_use = 0;
	ring->next_to_clean = 0;
	ring->color = 0;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* initialize TX buffers */
	q->buffer =
		kcalloc(real_count, sizeof(struct onic_tx_buffer), GFP_KERNEL);
	if (!q->buffer) {
		rv = -ENOMEM;
		goto clear_tx_queue;
	}
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* initialize QDMA H2C queue */
	param.rngcnt_idx = rngcnt_idx;
	param.dma_addr = ring->dma_addr;
	param.vid = vid;
	rv = onic_qdma_init_tx_queue(priv->hw.qdma, qid, &param);
	printk("%s %s line %d ，rv = %d\n", __FILE__, __FUNCTION__, __LINE__,rv);
	if (rv < 0)
		goto clear_tx_queue;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	priv->tx_queue[qid] = q;
	return 0;

clear_tx_queue:
	onic_clear_tx_queue(priv, qid);
	return rv;
}




// cdev structure ----------------------------------
static int onic_open(struct inode *inode, struct file *flip)
{
  struct onic_private *onic;
  struct onic_ctx *ctx;
	printk("enter the open func in kernel\r\n");

  onic = onic_find_by_minor(iminor(inode));
  if (onic == NULL)
  {
      pr_err("Failed to locate onic for minor = %u.\n", iminor(inode));
      return -ENODEV;
  }

  ctx = kmalloc(sizeof(struct onic_ctx),GFP_KERNEL);
  if(ctx == NULL){
    printk( "Failed to allocate onic_ctx.\n");
    return -ENOMEM;
  }
  //initialize the hw queue
	ctx->onic = onic;
  flip->private_data = ctx;
	printk("complete the open func\r\n");
  return 0;

}

static int onic_release(struct inode *inode, struct file *filp)
{
    struct onic_ctx *ctx = filp->private_data;
    // struct onic_private *onic = ctx->onic;

    // onic_clear_tx_queue(onic,USERQID);
    kfree(ctx);
    return 0;
}




static long onic_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int rv;
  struct onic_ctx *ctx = filp->private_data;
  struct onic_private *onic = ctx->onic;
	const int onicInfo1 = 0x11223344;
	const int onicInfo2 = 0x12123434;
	const int onicInfo3 = 0x22334411;

	if (_IOC_TYPE(cmd) != ONICCTL_TYPE)
		return -ENOTTY;

	switch (cmd)
	{
		case ONICCTL_INFO:
		{
			struct onicctl_info ctl;
			ctl.info_1 = onicInfo1;
			ctl.info_2 = onicInfo2;
			ctl.info_3 = onicInfo3;
			if (copy_to_user((void *)arg, &ctl, sizeof(ctl)) != 0)
				return -EFAULT;
			return 0;
		} 

		case ONICCTL_RW:
		{	
			struct onicrw ctl;
			if (copy_from_user(&ctl, (void *)arg, sizeof(ctl)) != 0)
				return -EFAULT;
			printk ("read data from user space for ioctl is 0x%08x\r\n",ctl.readdata);
			return 0;
		}

		case ONICCTL_READ_REG:
		{
			struct reg_info ctl;
			struct qdma_dev *qdev = (struct qdma_dev *)onic->hw.qdma;
			if (copy_from_user(&ctl, (void *)arg, sizeof(ctl)) != 0)
				return -EFAULT;

			ctl.value = qdma_read_reg(qdev,ctl.offset);	
			if (copy_to_user((void *)arg, &ctl, sizeof(ctl)) != 0)
				return -EFAULT;

			return 0;
		}

		case ONICCTL_WRITE_REG:
		{
			struct reg_info ctl;
			struct qdma_dev *qdev = (struct qdma_dev *)onic->hw.qdma;
			if (copy_from_user(&ctl, (void *)arg, sizeof(ctl)) != 0)
				return -EFAULT;

			qdma_write_reg(qdev,ctl.offset,ctl.value);	
			return 0;
		}

		case ONICCTL_H2C_QUEUE_INIT:
		{
			printk("now the num of queue alloc for net : %d\r\n",onic->num_tx_queues);
			if(onic->num_tx_queues >= 63)
			{
				printk("fail to get the tx_queue 64\r\n");
				onic_clear_tx_queue(onic,USERQID);
				return -1;
			}

			rv = onic_init_tx_queue(onic,USERQID);
			printk("get rv = %d\r\n",rv);
			printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
			// if(!rv)
			// {
			// 	printk("onic_init_tx_queue %d, err = %d", USERQID, rv);
			// 	// onic_clear_tx_queue(onic,USERQID);
			// 	return -1;
			// } 

			printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
			return 0;
		}

    case ONICCTL_H2C_DESC_SND:
    {
      struct qdma_h2c_usr_desc ctl;
      struct qdma_h2c_st_desc desc;
      struct onic_ring *ring;
      struct onic_tx_queue *q;
      u16 qid = USERQID;
			u8 *desc_ptr;
			dma_addr_t dma_addr;
			//debug
			// int i;
			// unsigned int *test_array;
      //
			q = onic->tx_queue[qid];
      ring = &q->ring;
      
      onic_tx_clean(q);

      if (onic_ring_full(ring)) {
        printk("ring is full\r\n");
        return -1;
      }

			if (copy_from_user(&ctl, (void *)arg, sizeof(ctl)) != 0)
				return -EFAULT;

      dma_addr = dma_map_single(&onic->pdev->dev,ctl.h2c_usr_vaddr,ctl.len,DMA_TO_DEVICE);
			// test_array = (unsigned int *)ctl.h2c_usr_vaddr;
			// for(i=0;i<1024;i++)
			// {
			// 	printk("get data from user space is %d\t",test_array[i]);
			// }
      desc_ptr = ring->desc + QDMA_H2C_ST_DESC_SIZE * ring->next_to_use;
      desc.len = ctl.len;
      desc.src_addr = dma_addr;
      desc.metadata = ctl.metadata;
      qdma_pack_h2c_st_desc(desc_ptr, &desc);

	    onic_ring_increment_head(ring);

      wmb();
      onic_set_tx_head(onic->hw.qdma, qid, ring->next_to_use);

			printk("send desc compelete \r\n");
			return 0;
    }
		
		case ONICCTL_GET_RING:
		{
			struct onic_ring_info2usr ctl;
			struct onic_tx_queue *q;
			struct onic_ring *ring;
      struct qdma_wb_stat wb;
			q = onic->tx_queue[USERQID];
			ring = &q->ring;
			qdma_unpack_wb_stat(&wb, ring->wb);

			ctl.count = ring->count;
			ctl.dma_addr = ring->dma_addr;
			ctl.next_to_use = ring->next_to_use;
			ctl.next_to_clean = ring->next_to_clean;
			ctl.cidx = wb.cidx;
			ctl.pidx = wb.pidx;
			
			if (copy_to_user((void *)arg, &ctl, sizeof(ctl)) != 0)
				return -EFAULT;

			return 0;
		}

	}
	return 0;
}

// static int onic_mmap(struct file *filp, struct vm_area_struct *vma)
// {
//   struct onic_ctx *ctx = filp->private_data;
//   struct onic_private *onic;

// 	unsigned long phy = virt_to_phys(kernel_buf);

// 	/* 设置属性: cache, buffer */
// 	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

// 	/* map */
// 	if (remap_pfn_range(vma, vma->vm_start, phy >> PAGE_SHIFT,
// 			    vma->vm_end - vma->vm_start, vma->vm_page_prot)) {
// 		printk("mmap remap_pfn_range failed\n");
// 		return -ENOBUFS;
// 	}

// 	return 0;


// }

struct file_operations onic_fops = {
    .owner          = THIS_MODULE,
    .open           = onic_open,
    .release        = onic_release,
    .unlocked_ioctl = onic_ioctl,
};
