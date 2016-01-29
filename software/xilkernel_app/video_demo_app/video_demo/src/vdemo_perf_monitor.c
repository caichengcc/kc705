
/***************************** Include Files ********************************/

#include "xmk.h"
#include "xaxipmon.h"
#include "xparameters.h"
#include "xstatus.h"
#include "stdio.h"
#include "vdma_data.h"

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define AXIPMON_DEVICE_ID 	XPAR_AXIPMON_0_DEVICE_ID


/**************************** Type Definitions ******************************/
#define Pmon_ReadReg(RegOffset) \
		(Xil_In32((AxiPmonInstPtr->Config.BaseAddress) + (RegOffset)))
#define Pmon_WriteReg(RegOffset, Data) \
		(Xil_Out32((AxiPmonInstPtr->Config.BaseAddress) + (RegOffset), (Data)))

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Function Prototypes *****************************/

/************************** Variable Definitions ****************************/

static XAxiPmon AxiPmonInst;      /* System Monitor driver instance */

int init_PerfMonitor(struct VDMA_PERF_MONITOR *perf)
{
	int Status;

	//xil_printf("==> init_perfmon \r\n");
	u16 AxiPmonDeviceId = AXIPMON_DEVICE_ID;

	XAxiPmon_Config *ConfigPtr;
	XAxiPmon *AxiPmonInstPtr = &AxiPmonInst;

	/*
	 * Initialize the AxiPmon driver.
	 */
	//xil_printf("  lookup cfg...");
	ConfigPtr = XAxiPmon_LookupConfig(AxiPmonDeviceId);
	if (ConfigPtr == NULL) {
		xil_printf("failed \r\n");
		return XST_FAILURE;
	}
	//xil_printf("success \r\n");

	//xil_printf("  cfg initialize \r\n");
	XAxiPmon_CfgInitialize(AxiPmonInstPtr, ConfigPtr,
				ConfigPtr->BaseAddress);

	/*
	 * Self Test the System Monitor/ADC device
	 */
	//xil_printf("  self test...");
	Status = XAxiPmon_SelfTest(AxiPmonInstPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("failed \r\n");
		return XST_FAILURE;
	}
	//xil_printf("success \r\n");

	// set the sample interval
	//xil_printf("  set sample interval: 200000000\r\n");
	XAxiPmon_SetSampleInterval(AxiPmonInstPtr, 0x00000000, 200000000);		// offset 0x24 = 200000000

	// set the load sample interval
	//xil_printf("  load sample interval counter\r\n");
	XAxiPmon_LoadSampleIntervalCounter(AxiPmonInstPtr);						// offset 0x28 = 0x2

	//xil_printf("  enable metric counter reset\r\n");
	XAxiPmon_EnableMetricCounterReset(AxiPmonInstPtr);						// offset 0x28 = 0x100

	//xil_printf("  enable sample interval counter \r\n");
	XAxiPmon_EnableSampleIntervalCounter(AxiPmonInstPtr);					// offset 0x28 = 0x1
	// HACK !! API doesnot support Enable the Metric Counter Reset
	Pmon_WriteReg(XAPM_SICR_OFFSET, Pmon_ReadReg(XAPM_SICR_OFFSET) | XAPM_SICR_MCNTR_RST_MASK);

	perf->read_perf = 0;
	perf->write_perf = 0;
	perf->perfIntrCnt = 0;
	perf->pollmode = 1;
	perf->dataReady = 0;

	return XST_SUCCESS;
}

int reset_PerfMonitor(struct VDMA_PERF_MONITOR *perf)
{
	//xil_printf("==> reset_perfmon \r\n");
	stop_PerfMonitor();
	return XST_SUCCESS;
}

int reset_PerfMonitor_stats(struct VDMA_PERF_MONITOR *perf)
{
	//xil_printf("==> reset_perfmon statistics\r\n");
	perf->read_perf = 0;
	perf->write_perf = 0;
	perf->perfIntrCnt = 0;
	perf->pollmode = 1;

	return 0;
}

void stop_PerfMonitor(void)
{
	XAxiPmon *AxiPmonInstPtr = &AxiPmonInst;

	//xil_printf("==> stop_perfmon \r\n");

	/*
	 * Disable Global Clock Counter Register.
	 */

	//xil_printf("  disable global clk counter \r\n");
	XAxiPmon_DisableGlobalClkCounter(AxiPmonInstPtr);

	/*
	 * Disable Metric Counters.
	 */
	//xil_printf("  disable metrixs counter \r\n");
	XAxiPmon_DisableMetricsCounter(AxiPmonInstPtr);

	//xil_printf("  stop event log\r\n");
	//XAxiPmon_StopEventLog(AxiPmonInstPtr);

	//xil_printf("  stop counters \r\n");
	XAxiPmon_StopCounters(AxiPmonInstPtr);
}

void start_PerfMonitor(void)
{
	XAxiPmon *AxiPmonInstPtr = &AxiPmonInst;
	u8 SlotId = 0x1;
	u8 Metrics, CounterNum;
	u32 RegValue, CurReg;

	//xil_printf("==> start_perfmon \r\n");

	/*------------------------------------------------------------*/
	/*  Configure the Clocks and Controls                         */
	/*------------------------------------------------------------*/
	//xil_printf("  set sample interval: 200000000\r\n");
	XAxiPmon_SetSampleInterval(AxiPmonInstPtr, 0x00000000, 200000000);		// offset 0x24 = 200000000

	//xil_printf("  load sample interval counter\r\n");
	XAxiPmon_LoadSampleIntervalCounter(AxiPmonInstPtr);						// offset 0x28 = 0x2

	//xil_printf("  enable metric counter reset\r\n");
	XAxiPmon_EnableMetricCounterReset(AxiPmonInstPtr);						// offset 0x28 = 0x100

	//xil_printf("  enable sample interval counter \r\n");
	XAxiPmon_EnableSampleIntervalCounter(AxiPmonInstPtr);					// offset 0x28 = 0x1

	/*---------------------------------------------------------------*/
	/* Configure the Agents and required set of Metrics for counters */
	/*---------------------------------------------------------------*/
	#if 0		// NO NEED
	SlotId=0x1;
	Metrics = XAPM_METRIC_SET_3;
	CounterNum = XAPM_METRIC_COUNTER_2;
	#if 0
	XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, Metrics, CounterNum);
	#else
	// HACK!! API has problem, need to fix in driver
	RegValue= (Metrics << (CounterNum * 8)) | (SlotId << (CounterNum * 8 + 5));
	CurReg = Pmon_ReadReg(XAPM_MSR0_OFFSET);
	CurReg |= RegValue;
	//xil_printf("  curReg=0x%x, ORing=0x%x, newVal=0x%x \r\n", Pmon_ReadReg(XAPM_MSR0_OFFSET), RegValue, CurReg);
	Pmon_WriteReg(XAPM_MSR0_OFFSET, CurReg);
	#endif
	#endif

	#if 0		// NO NEED
	SlotId=0x1;
	Metrics = XAPM_METRIC_SET_2;
	CounterNum = XAPM_METRIC_COUNTER_3;
	#if 0
	XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, Metrics, CounterNum);
	#else
	// HACK!! API has problem, need to fix in driver
	RegValue= (Metrics << (CounterNum * 8)) | (SlotId << (CounterNum * 8 + 5));
	CurReg = Pmon_ReadReg(XAPM_MSR0_OFFSET);
	CurReg |= RegValue;
	//xil_printf("  curReg=0x%x, ORing=0x%x, newVal=0x%x \r\n", Pmon_ReadReg(XAPM_MSR0_OFFSET), RegValue, CurReg);
	Pmon_WriteReg(XAPM_MSR0_OFFSET, CurReg);
	#endif
	#endif

	SlotId=0x0;
	Metrics = XAPM_METRIC_SET_3;
	CounterNum = XAPM_METRIC_COUNTER_0;
	#if 0
	XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, Metrics, CounterNum);
	#else
	// HACK!! API has problem, need to fix in driver
	RegValue= (Metrics << (CounterNum * 8)) | (SlotId << (CounterNum * 8 + 5));
	CurReg = Pmon_ReadReg(XAPM_MSR0_OFFSET);
	CurReg |= RegValue;
	//xil_printf("  curReg=0x%x, ORing=0x%x, newVal=0x%x \r\n", Pmon_ReadReg(XAPM_MSR0_OFFSET), RegValue, CurReg);
	Pmon_WriteReg(XAPM_MSR0_OFFSET, CurReg);
	#endif

	SlotId=0x0;
	Metrics = XAPM_METRIC_SET_2;
	CounterNum = XAPM_METRIC_COUNTER_1;
	#if 0
	XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, Metrics, CounterNum);
	#else
	// HACK!! API has problem, need to fix in driver
	RegValue= (Metrics << (CounterNum * 8)) | (SlotId << (CounterNum * 8 + 5));
	CurReg = Pmon_ReadReg(XAPM_MSR0_OFFSET);
	CurReg |= RegValue;
	//xil_printf("  curReg=0x%x, ORing=0x%x, newVal=0x%x \r\n", Pmon_ReadReg(XAPM_MSR0_OFFSET), RegValue, CurReg);
	Pmon_WriteReg(XAPM_MSR0_OFFSET, CurReg);
	#endif

	/*------------------------------------------------------------*/
	/* Reset the Counters, Enable the counting                    */
	/*------------------------------------------------------------*/
	//xil_printf("  reset metric counter \r\n");
	XAxiPmon_ResetMetricCounter(AxiPmonInstPtr);		// 0x002
	Pmon_WriteReg(XAPM_CTL_OFFSET, (Pmon_ReadReg(XAPM_CTL_OFFSET) & ~(XAPM_CR_MCNTR_RESET_MASK)));

	//xil_printf("  enable event log \r\n");
	//XAxiPmon_EnableEventLog(AxiPmonInstPtr);			// 0x100

	//xil_printf("  enable metrics counter \r\n");
	XAxiPmon_EnableMetricsCounter(AxiPmonInstPtr);		// 0x001

	//xil_printf("  enable external trigger \r\n");
	//XAxiPmon_EnableExternalTrigger(AxiPmonInstPtr);

	//xil_printf("  enable global clk counter \r\n");
	XAxiPmon_EnableGlobalClkCounter(AxiPmonInstPtr);

	////////////////////////////////////////
	#if 0
	xil_printf("  PMON reg dump \r\n");
	xil_printf("  offset: 0x024, value: 0x%x \r\n", Pmon_ReadReg(0x024));
	xil_printf("  offset: 0x028, value: 0x%x \r\n", Pmon_ReadReg(0x028));
	xil_printf("  offset: 0x044, value: 0x%x \r\n", Pmon_ReadReg(0x044));
	xil_printf("  offset: 0x300, value: 0x%x \r\n", Pmon_ReadReg(0x300));
	xil_printf("  offset: 0x038, value: 0x%x \r\n", Pmon_ReadReg(0x038));
	xil_printf("  offset: 0x210, value: 0x%x (wr_cnt) \r\n", Pmon_ReadReg(0x210));
	xil_printf("  offset: 0x200, value: 0x%x (rd_cnt) \r\n", Pmon_ReadReg(0x200));
	#endif

	return;
}

void PerfMon_pollHandler(struct VDMA_PERF_MONITOR *perf)
{
	XAxiPmon *AxiPmonInstPtr = &AxiPmonInst;
	u32 IntrStatus;
	volatile unsigned int wr_cnt, rd_cnt;
	double p;
	double one_mb = (double) (1024*1024);

	//xil_printf("==> poll_perfmon \r\n");

	perf->dataReady = 0;
	IntrStatus = XAxiPmon_IntrGetStatus(AxiPmonInstPtr);
	//xil_printf("  intr status: 0x%x \r\n", IntrStatus);

	//XAxiPmon_IntrClear(AxiPmonInstPtr, IntrStatus);
	while( (IntrStatus & 0x2) == 0)
	{
		IntrStatus = XAxiPmon_IntrGetStatus(AxiPmonInstPtr);
		//return;
	}
	//xil_printf("  got intr, status: 0x%x \r\n", IntrStatus);
	wr_cnt = XAxiPmon_GetSampledMetricCounter(AxiPmonInstPtr, 1);
	rd_cnt = XAxiPmon_GetSampledMetricCounter(AxiPmonInstPtr, 0);

	#if 0
	xil_printf("  PMON reg dump \r\n");
	xil_printf("  offset: 0x024, value: 0x%x \r\n", Pmon_ReadReg(0x024));
	xil_printf("  offset: 0x028, value: 0x%x \r\n", Pmon_ReadReg(0x028));
	xil_printf("  offset: 0x044, value: 0x%x \r\n", Pmon_ReadReg(0x044));
	xil_printf("  offset: 0x300, value: 0x%x \r\n", Pmon_ReadReg(0x300));
	xil_printf("  offset: 0x038, value: 0x%x \r\n", Pmon_ReadReg(0x038));
	xil_printf("  offset: 0x210, value: 0x%x (wr_cnt) \r\n", Pmon_ReadReg(0x210));
	xil_printf("  offset: 0x200, value: 0x%x (rd_cnt) \r\n", Pmon_ReadReg(0x200));
	#endif

	//p = (double)wr_cnt/(double)8;
	//p = (double)p / (double)one_mb;
	//p = (double)p * (double)512;
	p = (double)wr_cnt / (double)one_mb;
	p = (double)p / (double)1024;
	p = (double)p * (double)8;
	perf->write_perf = p;
	//xil_printf("  wr:0x%x  ", p);

	//p = (double)rd_cnt/(double)8;
	//p = (double)p / (double)one_mb;
	//p = (double)p * (double)512;
	p = (double)rd_cnt / (double)one_mb;
	p = (double)p / (double)1024;
	p = (double)p * (double)8;
	perf->read_perf = p;
	//xil_printf("  rd:0x%x  \r\n", p);

	perf->dataReady = 1;

	//start_PerfMonitor();
	XAxiPmon_IntrClear(AxiPmonInstPtr, IntrStatus);
	XAxiPmon_ResetMetricCounter(AxiPmonInstPtr);		// 0x002
	//XAxiPmon_EnableEventLog(AxiPmonInstPtr);			// 0x100
	XAxiPmon_EnableMetricsCounter(AxiPmonInstPtr);		// 0x001
	//XAxiPmon_EnableGlobalClkCounter(AxiPmonInstPtr);

	return;

}
