#include "CAN.h"



void CAN_GPIO (void)
	
{
	
	
if (CAN_Base == CAN0)
	
{
	
	

	SET_BIT(SYSCTL -> RCGC0,24); //CAN0 CLock gating enable

	SET_BIT(SYSCTL ->RCGC2 ,5); //ENABlE a module recieving  clock 

	SET_BIT(SYSCTL ->RCGC2 ,4);

	SET_BIT(SYSCTL ->RCGC2 ,1);

	SET_BIT(GPIOF->AFSEL,0); //Enable alternate function for PF0

	GPIOF->PCTL =0x00000003;	//Specify which alternate function we want to enable , assign the CAN0 signals to the appropriate pins

	SET_BIT(GPIOB->AFSEL,4); // Enable alternate function for PB4

	GPIOB->PCTL =0x00080000;//Specify which alternate function we want to enable , assign the CAN0 signals to the appropriat pins
	
 SET_BIT(GPIOE->AFSEL,4); //Enable alternate function for PE4

 GPIOE->PCTL =0x00080000;//Specify which alternate function we want to enable , assign the CAN0 signals to the appropriat pins
	
	
	/*the previous was for CAN0 Module recieve, The following will be for CAN0 Module transmit */
	
	
  SET_BIT(GPIOF->AFSEL,3); //Enable alternate function for PF0
  
	GPIOF->PCTL =0x00000003;	//Specify which alternate function we want to enable , assign the CAN1 signals to the appropriate pins
	
  SET_BIT(GPIOB->AFSEL,5); // Enable alternate function for PB4
 
  GPIOB->PCTL =0x00800000;//Specify which alternate function we want to enable , assign the CAN1 signals to the appropriat pins
	
  SET_BIT(GPIOE->AFSEL,5); //Enable alternate function for PE4
  
	GPIOE->PCTL =0x008000;//Specify which alternate function we want to enable , assign the CAN1 signals to the appropriat pins

}

else if (CAN_Base == CAN1)
	
{
	
	SET_BIT(SYSCTL -> RCGC0,25);               // CAN1 clock gating enable 
	
	SET_BIT(SYSCTL ->RCGC2 ,0);               //ENABlE a module recieving a clock 
	
	SET_BIT(GPIOA->AFSEL,0);                 //Enable alternate function for PA0
	
	SET_BIT(GPIOA->AFSEL,1);                 //Enable alternate function for PA1
	
	GPIOA -> PCTL =0x00000088;              //Specify which alternate function we want to enable , CAN1 Module recieve and Transmit

}

}


void CAN_INIT(void)
	
{

	void CAN_GPIO(void);

	SET_BIT( CAN_Base->CTL ,0);               // initializarion started , SET INIT bit 
	
	if (CAN_Mode == Test_Mode)
		
	{
		
		SET_BIT( CAN_Base->CTL ,7);             //Enable Test Mode
		
		SET_BIT( CAN_Base->TST ,4);             // Enable loop back
		
	}
	
	else 
		
	{
		
			CLEAR_BIT( CAN_Base->CTL ,7);      //Disable Test Mode
		 
			CLEAR_BIT( CAN_Base->TST ,4);          // Disable loop back
	}
	

}



void CAN_BIT_TIMING(Can_bits*Bits)
	
{
	
	SET_BIT( CAN_Base->CTL ,6) ;    //CCE bit is set
	
	//INIT and CCE bits is SET in Order to Access CANBIT register to Set CAN Timing

	CAN_Base->BIT |= Bits->BRP -1;                     //Set Baud rate prescaler

	CAN_Base->BIT |= (Bits->TSEG1 -1) <<8;             //Prop_SEG + Phase1_SEG SET
	
	CAN_Base ->BIT |= (Bits ->TSEG2 -1 ) <<12;          //Prop_SEG + Phase1_SEG SET
	
	CAN_Base ->BIT |= (Bits ->Sjw -1) <<6;              //Phase2_SEG SET 
	
	CAN_Base -> BRPE |= Baud_rate_extention;           //setting Baud Rate Prescaler Extension,Extend the BRP bit in the CANBIT register to values up to 1023. The actual interpretation by the hardware is one more than thevalue programmed by BRPE (MSBs) and BRP (LSBs).

  CLEAR_BIT(CAN_Base->CTL,6);               //Disable acces to CAN bit register

}




void Enable_CAN (void)
	
{
	
	CLEAR_BIT(CAN_Base->CTL,0); //CLear INIT bit , leave initialization state to start transmition
	
}


void Disable_CAN (void )
	
{
	
	if (CAN_Base == CAN0)
		
	{

	CLEAR_BIT(SYSCTL -> RCGC0,24);           //CAN0 CLock gating Disable
	
	}
	
	else if (CAN_Base == CAN1)
		
	{
		
		SET_BIT(SYSCTL -> RCGC0,25);               // CAN1 clock gating Disable 
	
	}

}




	
void CAN_Set_Transmit_object(uint32_t msg_obj_id,msgobjct*message)
	
{

CLEAR_BIT(CAN_Base->IF2ARB2,15);

SET_BIT(CAN_Base->IF1CMSK ,7); //SET WRNRD to specify write to CANIFnCMASK register , to write from CANIF to message object

SET_BIT(CAN_Base->IF1CMSK ,6) ; //Transfer IDMASK + DIR + MXTD of the message object into the Interface registers,

SET_BIT (CAN_Base ->IF1CMSK ,5) ;  //Transfer ID + DIR + XTD + MSGVAL of the message object into the Interface registers,Set ARB bit to enable access arbitration bits

SET_BIT (CAN_Base ->IF1CMSK ,4); //Transfer control bits from the CANIFnMCTL register into the Interface registers, to enable access control bits.

SET_BIT (CAN_Base ->IF1MCTL ,12) ;  //Set UMASK bit in CANIFnMCTL to enable MSK bits in CANIFnMSK[1-2] which identify which message IDs will pass through acceptance filtering

//SET_BIT (CAN_Base ->IF1MCTL,8);    //SET TXRQST Access Transmision request
	
SET_BIT(CAN_Base ->IF1MCTL,1) ;   //Access Data byte 0-3, Tranfer Data bytes 0-3 in message object to CANIFnDA1 and CANIFnDA2 registers
	
SET_BIT (CAN_Base ->IF1MCTL,0);      //Access Data byte 4-7, Transfer Date bytes 4-7 in message object to CANIFnDA1 and CANIFnDA2 registers

	if (message->msgid <= CAN_STANDARD_ID	)
		
	{ 
			// Standard Frame
		
			CLEAR_BIT(  CAN_Base ->IF1ARB2 ,14);  //An 11-bit Standard Identifier is used for this message  object
			
	    CAN_Base->IF1ARB1|=(message->msgid & (0x00007FFF) <<2);  // 11 bits [0-10] is configured from bit 2 to 12 in CANIFnARB2 register .
	}
		
	else if(message->msgid >CAN_STANDARD_ID && message->msgid <= CAN_EXTENDED_ID)
		
	{
		//Extended Frame
			
	 SET_BIT(CAN_Base	->	IF1MSK2 ,15);    //Enable Msk which identify which message IDs will pass through acceptance filter incase of 29 bit id 
		
		SET_BIT(  CAN_Base ->IF1ARB2 ,14);    //A 29-bit Extended Identifier is used for this message object.
   
		CAN_Base->IF1ARB1|=(message->msgid & (0x0000FFFF));    // Write first 16 bits of ID
		
		CAN_Base->IF1ARB2 |=(message->msgid &(0x1FFF0000>>16));       // write rest of ID bits
	}
	
	
	SET_BIT( CAN_Base -> IF1ARB2 ,13);           //Set DIR to transmit
	
	SET_BIT(CAN_Base ->IF1ARB2, 15);              // Set message Valid bit
	
	SET_BIT (CAN_Base ->IF1MCTL ,7);              //Set EOB bit for a single message object.

	
	CAN_Base -> IF1MCTL &= 0xFFF0;           //Clear DLC
	
	CAN_Base ->IF1MCTL |=message->msglength;     // Set Message Length
	
	
 
  CAN_Base ->IF1CRQ =msg_obj_id;
 
 
 

}





void CAN_Transmit(uint32_t msg_obj_id,msgobjct*message){

SET_BIT(CAN_Base->IF1CMSK ,7); //SET WRNRD to specify write to CANIFnCMASK register , to write from CANIF to message object
	
SET_BIT(CAN_Base ->IF1MCTL,1) ;   //Access Data byte 0-3, Tranfer Data bytes 0-3 in message object to CANIFnDA1 and CANIFnDA2 registers
	
SET_BIT (CAN_Base ->IF1MCTL,0);      //Access Data byte 4-7, Transfer Date bytes 4-7 in message object to CANIFnDA1 and CANIFnDA2 registers
	
CAN_Base -> IF1MCTL &= 0xFFF0;           //Clear DLC
	
CAN_Base ->IF1MCTL |=message->msglength;     // Set Message Length
	
CAN_Base ->IF1DA1 |=(uint32_t )(message->Data[0] | (message->Data[1] <<8)); // load first two bytes
	
CAN_Base ->IF1DB1 |=(uint32_t )((message ->Data[4]) |(message->Data[5] << 8));  //load fifth and six byte 
	
CAN_Base ->IF1DB2 |=(uint32_t )((message ->Data[6]) |(message->Data[7] << 8));   //load seventh and last byte
	
SET_BIT (CAN_Base ->IF1MCTL,8);    //SET TXRQST Access Transmision request
	
 SET_BIT(CAN_Base->IF1MCTL,15);     //The message handler or the CPU has written new data into the data portion of this message object, The NEWDAT bit in CANIFnMCTL is set to indicate that new data is written in corresponding message object 

//	Selects one of the 32 message objects in the message RAM for datatransfer. The message objects are numbered from 1 to 32.
 
 CAN_Base ->IF1CRQ =msg_obj_id;
	
while ((CAN_Base ->IF1CRQ) &(1<<15));        // Polling till transmition is done As busy bit is clear when read or write has finished
 

}



	
	
void CAN_Set_Recieve_object ( uint32_t msg_obj_id,msgobjct*message_Recieve)
	
{
	
	CLEAR_BIT(CAN_Base->IF2ARB2,15);    // The MSGVAL bit must also be cleared before modifing some registers
	
	CLEAR_BIT(CAN_Base->IF1CMSK ,7);    //  Clear WRNRD bit In CANIFnCMSK to read from message object to CANIF registers.
	
	if (message_Recieve->msgid <= CAN_STANDARD_ID	)
		
	{ //Standard Frame	
		
		SET_BIT (CAN_Base ->IF1MCTL ,12) ; //Set UMASK bit in CANIFnMCTL to enable MSK bits in CANIFnMSK[1-2] which identify which message IDs will pass through acceptance filtering
		
		CLEAR_BIT(  CAN_Base ->IF1ARB2 ,14);  //An 11-bit Standard Identifier is used for this message  object
		
		CAN_Base->IF1ARB1|=(message_Recieve->msgid & (0x00007FFF) <<2);  // 11 bits [0-10] is configured from bit 2 to 12 in CANIFnARB2 register .
	}
	
	else if(message_Recieve->msgid >CAN_STANDARD_ID && message_Recieve->msgid <= CAN_EXTENDED_ID)
		
		{
		//Extended Frame
	 
		SET_BIT(CAN_Base	->	IF1MSK2 ,15);    //Enable Msk which identify which message IDs will pass through acceptance filter incase of 29 bit id 
	 
		SET_BIT(  CAN_Base ->IF1ARB2 ,14);    //A 29-bit Extended Identifier is used for this message object.
		
		CAN_Base->IF1ARB1|=(message_Recieve->msgid & (0x0000FFFF));    // Write first 16 bits of ID
		
		CAN_Base->IF1ARB2 |=(message_Recieve->msgid &(0x1FFF0000>>16));       // write rest of ID bits
    
		}
	

CLEAR_BIT( CAN_Base -> IF1ARB2 ,13);           //Set DIR to Recieve

SET_BIT(CAN_Base ->IF1ARB2, 15);            //Set MSGVAL in CANIFnARB2 to indicate that this message object of MNUM configured in CANIFnCRQ is valid.
	
SET_BIT (CAN_Base ->IF1MCTL ,7);          //Set EOB bit for a single message object.

CAN_Base ->IF1MCTL &= 0xFFF0;            //Clear DLC to set mesage length

CAN_Base ->IF1MCTL |= message_Recieve->msglength	;   // Set Message Length

CAN_Base ->IF1CRQ =msg_obj_id;        // program which message object to be addressed in MNUM field in CANIFnCRQ

		
		
}




void  CAN_Recieve ( uint32_t msg_obj_id,msgobjct*message_Recieve)
	{
	
   CLEAR_BIT(CAN_Base->IF1CMSK ,7);    //  Clear WRNRD bit In CANIFnCMSK to read from message object to CANIF registers.
	 CAN_Base ->IF1MCTL &= 0xFFF0;            //Clear DLC to set mesage length

   CAN_Base ->IF1MCTL |= message_Recieve->msglength	;   // Set Message Length

   CAN_Base ->IF1CRQ =msg_obj_id;        // program which message object to be addressed in MNUM field in CANIFnCRQ

		
		
   while (CAN_Base->IF1MCTL &(1<<15));

/* polling till the NEWDAT bit is cleared AS 
On reading this data from message object this bit is cleared ,
The NEWDAT bit in CANIFnMCTL is set to indicate that new data is written in corresponding message object.
On reading this data from message object the NEWDAT bit is cleared.The message object's NEWDAT bitin the CANNWDAn register is cleared. After a successful transmission*/

    message_Recieve->Data[0] = (CAN_Base ->IF1DA1 ) & 0x00FF;

    message_Recieve ->Data[1] =(CAN_Base->IF1DA1) &0xFF00 >>8;       //Get first and second byte

    message_Recieve->Data[2] = (CAN_Base ->IF1DA2 ) & 0x00FF;

    message_Recieve ->Data[3] =(CAN_Base->IF1DA2) &0xFF00 >>8;      // Get 3rd and fourth byte 


    message_Recieve->Data[4] = (CAN_Base ->IF1DB1 ) & 0x00FF;

    message_Recieve ->Data[5] =(CAN_Base->IF1DB1) &0xFF00 >>8;          // Get fifth and six byte 


    message_Recieve->Data[6] = (CAN_Base ->IF1DB2 ) & 0x00FF;           // Get Seveth and eigth byte
  
    message_Recieve ->Data[7] =(CAN_Base->IF1DB2) &0xFF00 >>8;

}

	
	
	
	
	
	

	
	
	