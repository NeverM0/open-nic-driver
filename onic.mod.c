#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xb3753869, "module_layout" },
	{ 0xc08ffb53, "netdev_info" },
	{ 0x35216b26, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xaccda578, "pci_free_irq_vectors" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x5ab5b891, "param_ops_int" },
	{ 0xb1c0a7a6, "napi_disable" },
	{ 0x71fded2d, "napi_schedule_prep" },
	{ 0xfcb1e873, "__napi_schedule_irqoff" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0xd8fd6a9a, "dma_set_mask" },
	{ 0x69790abd, "pcie_set_readrq" },
	{ 0xae630ce8, "pci_disable_device" },
	{ 0xecaf5c8a, "netif_carrier_on" },
	{ 0x9d9b5eb4, "netif_carrier_off" },
	{ 0x56470118, "__warn_printk" },
	{ 0x34c36bf5, "__dev_kfree_skb_any" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x436bd4c3, "pcie_capability_clear_and_set_word" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0x999e8297, "vfree" },
	{ 0x2b54d1d5, "dma_free_attrs" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x11e66d2, "__alloc_pages_nodemask" },
	{ 0x36e58bcd, "pv_ops" },
	{ 0x32ffdcca, "dma_set_coherent_mask" },
	{ 0xc66cf97f, "netif_napi_del" },
	{ 0x368a4bc3, "__dynamic_netdev_dbg" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xfee56dca, "pci_set_master" },
	{ 0xe582fa0, "misc_register" },
	{ 0x48e065be, "pci_alloc_irq_vectors_affinity" },
	{ 0x7e526bfa, "__x86_indirect_thunk_r10" },
	{ 0xfb578fc5, "memset" },
	{ 0x4c0d835c, "netif_tx_stop_all_queues" },
	{ 0x4681df01, "pci_iounmap" },
	{ 0xc5850110, "printk" },
	{ 0x6ef7ab52, "ethtool_op_get_link" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0xaadc6c4, "free_netdev" },
	{ 0x6700be79, "register_netdev" },
	{ 0x5792f848, "strlcpy" },
	{ 0x37805d7b, "dma_direct_map_page" },
	{ 0xfda0bdf3, "dma_alloc_attrs" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0x1fa62540, "netif_set_real_num_rx_queues" },
	{ 0xc893a848, "netif_set_real_num_tx_queues" },
	{ 0xd9a7e9c7, "netif_napi_add" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0x45997b1d, "softnet_data" },
	{ 0xe6257e29, "_dev_err" },
	{ 0xa76d7c72, "pci_iomap_range" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xe912db23, "pci_select_bars" },
	{ 0x9e163a88, "dma_direct_unmap_page" },
	{ 0x8104bca3, "napi_gro_receive" },
	{ 0x9409627e, "_dev_info" },
	{ 0x618911fc, "numa_node" },
	{ 0xa916b694, "strnlen" },
	{ 0x7478c9a1, "__napi_schedule" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x9cb986f2, "vmalloc_base" },
	{ 0xd1562ed1, "napi_complete_done" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0x98fcf627, "eth_type_trans" },
	{ 0x38c0115f, "dev_driver_string" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x60a8ac7a, "netdev_err" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x478f00e1, "pci_unregister_driver" },
	{ 0xf5cb25c8, "kmem_cache_alloc_trace" },
	{ 0xdbf17652, "_raw_spin_lock" },
	{ 0x2fa8953f, "pci_irq_vector" },
	{ 0x37a0cba, "kfree" },
	{ 0x69acdf38, "memcpy" },
	{ 0x24694125, "__pci_register_driver" },
	{ 0xc041bda5, "unregister_netdev" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x66374091, "pci_iomap" },
	{ 0x116c8d71, "consume_skb" },
	{ 0x252fb348, "pci_enable_device_mem" },
	{ 0x9be5a20f, "__napi_alloc_skb" },
	{ 0x4a453f53, "iowrite32" },
	{ 0x21d43104, "skb_put" },
	{ 0xa16b12ad, "pci_release_selected_regions" },
	{ 0xdf95b3d4, "pci_request_selected_regions" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xdd7bab0c, "dma_ops" },
	{ 0xe484e35f, "ioread32" },
	{ 0x763bb516, "__skb_pad" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x55842ec1, "pci_save_state" },
	{ 0xef5c5831, "alloc_etherdev_mqs" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v000010EEd00009031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009131sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009231sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009331sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009032sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009132sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009232sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009332sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009034sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009134sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009234sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009334sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009038sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009138sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009238sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009338sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd0000903Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd0000913Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd0000923Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd0000933Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006AA0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009041sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009141sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009241sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009341sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009042sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009142sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009242sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009342sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009044sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009144sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009244sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009344sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009048sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009148sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009248sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009348sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "4F7860B978F64AEFA1966B4");
