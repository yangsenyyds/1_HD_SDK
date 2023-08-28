#include "hci_dtm_test.h"
#include "btreg.h"


void hci_freq_test_end(void)
{
	HWRITE(mem_le_dtm_mode, 0);
	HWRITE(mem_freq_check_flag, 0);
	HWRITE(mem_le_hci_test, 0);
}

void hci_freq_tx_test()
{
	bt_set_freq_point(38);
	// HWRITE(mem_freq_test, 16);
	HWRITE(mem_le_dtm_swtich, 1);
	HWRITE(mem_le_dtm_mode, BT_CMD_FREQ_TEST);
	HWRITE(mem_freq_check_flag, 1);
	HWRITE(mem_le_hci_test, 1);
}

void bt_set_freq_point(uint8_t freq_point)
{
	HWRITE(mem_freq_point, freq_point);
}
