#include "stdint.h"
#include "cfwi/cfwi.h"

uint8_t perform_test();

void
main ()
{
	uint32_t time_before, time_after;

	fw_mc_send_printer('0');

	time_before = fw_kl_time_please();

	{
		uint8_t rc = perform_test();
		fw_mc_send_printer('1');
		fw_mc_send_printer(rc);
	}

	time_after = fw_kl_time_please();

	{
		uint32_t time_delta = time_after - time_before;
		fw_mc_send_printer(time_delta);
	}

	fw_mc_send_printer('2');
	fw_mc_wait_flyback();
	fw_mc_wait_flyback();
}

void rendezvous_point ()
{
}
