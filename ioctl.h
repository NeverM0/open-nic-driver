#ifndef ONICCTL_IOCTL_H
#define ONICCTL_IOCTL_H


#define ONICCTL_TYPE              'x'

/** \brief Retrieve information about an ONIC */
#define ONICCTL_INFO              _IOR(ONICCTL_TYPE, 0xe0,struct onicctl_info)

#define ONICCTL_RW                _IOWR(ONICCTL_TYPE, 0xe2,struct onicrw)
#define ONICCTL_H2C_DESC_SND      _IOWR(ONICCTL_TYPE, 0xe3,struct qdma_h2c_usr_desc)
#define ONICCTL_H2C_QUEUE_INIT   	_IOWR(ONICCTL_TYPE, 0xe4,int)

// DEBUG IOCTL
#define ONICCTL_READ_REG							_IOWR(ONICCTL_TYPE, 0xf0,struct reg_info)
#define ONICCTL_WRITE_REG            _IOWR(ONICCTL_TYPE, 0xf1,struct reg_info)

#define ONICCTL_GET_RING             _IOWR(ONICCTL_TYPE,0xf2,struct onic_ring_info2usr)

struct onic_ring_info2usr
{
	uint16_t count;		/* number of descriptors */

	uint16_t pidx;
	uint16_t cidx; 
	 
	 		/* descriptor writeback */
	uint32_t dma_addr;	/* DMA address for descriptors */

	uint16_t next_to_use;
	uint16_t next_to_clean;
	uint8_t color;
};


struct reg_info
{
	uint32_t offset;
	uint32_t value;
};


struct qdma_h2c_usr_desc
{ 
  uint32_t metadata;
  uint16_t len;
  uint8_t *h2c_usr_vaddr;

};


struct onicctl_info
{
	int info_1;
	int info_2;
	int info_3;
};

struct onicrw
{
	int readdata;
	int writedata;
};

#endif