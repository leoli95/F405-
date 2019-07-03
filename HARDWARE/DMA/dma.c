#include "dma.h"																	   	  
#include "delay.h"		 

//DMAxµÄ¸÷Í¨µÀÅäÖÃ
//ÕâÀïµÄ´«ÊäÐÎÊ½ÊÇ¹Ì¶¨µÄ,ÕâµãÒª¸ù¾Ý²»Í¬µÄÇé¿öÀ´ÐÞ¸Ä
//´Ó´æ´¢Æ÷->ÍâÉèÄ£Ê½/8Î»Êý¾Ý¿í¶È/´æ´¢Æ÷ÔöÁ¿Ä£Ê½
//DMA_Streamx:DMAÊý¾ÝÁ÷,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAÍ¨µÀÑ¡Ôñ,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:ÍâÉèµØÖ·
//mar:´æ´¢Æ÷µØÖ·
//ndtr:Êý¾Ý´«ÊäÁ¿  
void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,uint32_t chx,uint32_t par,uint32_t mar,uint32_t dir,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//µÃµ½µ±Ç°streamÊÇÊôÓÚDMA2»¹ÊÇDMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2Ê±ÖÓÊ¹ÄÜ 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1Ê±ÖÓÊ¹ÄÜ 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//µÈ´ýDMA¿ÉÅäÖÃ 
	
  /* ÅäÖÃ DMA Stream */
  DMA_InitStructure.DMA_Channel 								= chx;  //Í¨µÀÑ¡Ôñ
  DMA_InitStructure.DMA_PeripheralBaseAddr 			= par;//DMAÍâÉèµØÖ·
  DMA_InitStructure.DMA_Memory0BaseAddr 				= mar;//DMA ´æ´¢Æ÷0µØÖ·
  DMA_InitStructure.DMA_DIR 										= dir;//direction of transmit.
  DMA_InitStructure.DMA_BufferSize 							= ndtr;//Êý¾Ý´«ÊäÁ¿ 
  DMA_InitStructure.DMA_PeripheralInc					  = DMA_PeripheralInc_Disable;//ÍâÉè·ÇÔöÁ¿Ä£Ê½
  DMA_InitStructure.DMA_MemoryInc 							= DMA_MemoryInc_Enable;//´æ´¢Æ÷ÔöÁ¿Ä£Ê½
  DMA_InitStructure.DMA_PeripheralDataSize 			= DMA_PeripheralDataSize_Byte;//ÍâÉèÊý¾Ý³¤¶È:8Î»
  DMA_InitStructure.DMA_MemoryDataSize 					= DMA_MemoryDataSize_Byte;//´æ´¢Æ÷Êý¾Ý³¤¶È:8Î»
  DMA_InitStructure.DMA_Mode 										= DMA_Mode_Normal;// Ê¹ÓÃÆÕÍ¨Ä£Ê½ 
  DMA_InitStructure.DMA_Priority 								= DMA_Priority_High;//ÖÐµÈÓÅÏÈ¼¶
  DMA_InitStructure.DMA_FIFOMode 								= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 					= DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst 						= DMA_MemoryBurst_Single;//´æ´¢Æ÷Í»·¢µ¥´Î´«Êä
  DMA_InitStructure.DMA_PeripheralBurst 				= DMA_PeripheralBurst_Single;//ÍâÉèÍ»·¢µ¥´Î´«Êä
  DMA_Init(DMA_Streamx, &DMA_InitStructure);
	DMA_Cmd(DMA_Streamx,ENABLE);
} 
//¿ªÆôÒ»´ÎDMA´«Êä
//DMA_Streamx:DMAÊý¾ÝÁ÷,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:Êý¾Ý´«ÊäÁ¿  
void DMA_Transfer_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //¹Ø±ÕDMA´«Êä 
	if(DMA_GetCmdStatus(DMA_Streamx) != DISABLE)	//È·±£DMA¿ÉÒÔ±»ÉèÖÃ  
		return;
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //Êý¾Ý´«ÊäÁ¿  
	DMA_Cmd(DMA_Streamx, ENABLE);                      //¿ªÆôDMA´«Êä 
}	  
