void enable_irq()
{
	asm(
	"mrs r1, cpsr;"
 	"bic r1, r1, #0x80;"
  	"msr cpsr_c, r1;"
	);
}

void disable_irq()
{
	asm(
	"mrs r1, cpsr;"
 	"orr r1, r1, #0x80;"
  	"msr cpsr_c, r1;"
	);
}

void enable_fiq()
{
	asm(
	"mrs r1, cpsr;"
 	"bic r1, r1, #0x40;"
  	"msr cpsr_c, r1;"
	);
}

void disable_fiq()
{
	asm(
	"mrs r1, cpsr;"
 	"orr r1, r1, #0x40;"
  	"msr cpsr_c, r1;"
	);
}