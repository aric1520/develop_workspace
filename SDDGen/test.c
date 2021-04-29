#include "DSP2833x_Device.h"			// 外设地址定义
#include "TMS320F28335.h"				// 变量和函数外部声明
#include "1553RT.h"
#include "DSP2833x_EPwm.h"
#include "add.h"
#include <stdio.h>



/**************************************************************************
 * 函数名称: _Filter_Id()
 * 函数功能: Id控制调节
 * 输入参数:
 * 输出参数:
 * 处理过程:
 @process
 * 1) 当Id数值超过限度范围时进行限幅处理;
 * 2) 当上一状态的数值为正数,且大于当前值时,将fStepId设置为0.05.
 @process
 **************************************************************************/
float32 Filter_Id(float32 m_fId_mea,float32 m_fOutId)
{
	float32 	m_fIdError = 0.0f;
	float32 	m_fPreSaturatedId = 0.0f;
	float32 	m_fStepId = 0.05f;
	float32		m_ftemp1 = 0.0f;
	float32		m_fOutput_Id = 0.0f;
	/****************************************************************/
	m_fIdError = 0.0f - m_fId_mea;
	if( m_fIdError > 0.45f)
	{
		m_fIdError = 0.45f;
	}
	else if( m_fIdError < -0.45f )
	{
		m_fIdError = -0.45f;
	}
	else
	{
		;
	}
	/****************比例调节***************************************/
	m_fPreSaturatedId = 1.7f * m_fIdError;
	/****************************************************************/
	if( m_fOutId > 0.0f )
	{
		if( m_fPreSaturatedId > m_fOutId )
		{
			m_fStepId = 0.05f;
		}
		else if( m_fPreSaturatedId < m_fOutId )
		{
			m_fStepId = 0.05f;
		}
		else
		{
			m_fStepId = 0.005f;
		}
	}
	else if( m_fOutId < 0.0f )
	{
		if( m_fPreSaturatedId > m_fOutId )
		{
			m_fStepId = 0.05f;
		}
		else if( m_fPreSaturatedId < m_fOutId )
		{
			m_fStepId = 0.05f;
		}
		else
		{
			m_fStepId = 0.005f;
		}
	}
	else
	{
		m_fStepId = 0.005f;
	}
    /****************************************************************/
	m_ftemp1 = m_fPreSaturatedId - m_fOutId;
	/****************************************************************/
	if( m_ftemp1 > m_fStepId )
	{
		m_fPreSaturatedId = m_fOutId + m_fStepId;
	}
	else if( m_ftemp1 < -m_fStepId )
	{
		m_fPreSaturatedId = m_fOutId - m_fStepId;
	}
	else
	{
		;
	}
	/****************************************************************/
	if( m_fPreSaturatedId > 1.0f )
	{
		m_fOutput_Id = 1.0f;
	}
	else if( m_fPreSaturatedId < -1.0f )
	{
		m_fOutput_Id = -1.0f;
	}
	else
	{
		m_fOutput_Id = m_fPreSaturatedId;
	}
	return m_fOutput_Id;
}
/*end-func*/


/*****************************main函数**********************************
 * 函数名称: main()
 * 函数功能: 主函数
  * 处理过程:
 @process
 初始化SysCtrl、初始化外设；
 初始化flash及程序搬运、密码区读写；
 初始化1553B控制芯片、中断配置、上电自检；
 @process
 * 输入参数: void
 * 输出参数: void
 ******************************************************************************/
void main(void)
{
	/****************************
      关中断
	*****************************/
	DINT;

	/****************************
      初始化看门狗为279ms
	*****************************/
	/*func*/InitDog();

	// 初始化28335

    /*func*/Init_F28335();


	// 使能看门狗
	/*func*/EnableDog();

	/****************************
      初始化1553B
	*****************************/
	/*func*/Init1553RT();
	/*func*/KickDog();

	/****************************
      初始化驱动器状态，开管准备动作
	*****************************/
	/*func*/InitDriver();

	/****************************
      初始化全局变量
	*****************************/
	/*func*/InitGlobalVar();
	/*func*/KickDog();
	/*初始化程序装订*/
	/*func*/ProgramLoadInit(0x100000);
	/****************************
      上电自检
	*****************************/
	/*func*/PWRONTEST();
	/*func*/KickDog();

	/****************************
      开中断 INT3.1--ePWM 和 INT12.1--XINT3
	*****************************/
	IER |= 0x0804;				// 1000 0000 0100b
   	EINT;    					// Enable Global interrupt INTM

 	/*while(1)
	{
			KickDog();
	}*/
}
/*end-func*/


/**************************************************************************
 * 函数名称: _Filter_Iq()
 * 函数功能: Iq控制调节
 * 输入参数:
 * 输出参数:
 * 处理过程：@process @process
 **************************************************************************/
float32 Filter_Iq(float32 m_fIq_ins, float32 m_fIq_mea,float32 m_fOutIq)
{
	float32 	m_fIqError = 0.0f;
	float32 	m_fPreSaturatedIq = 0.0f;
	float32 	m_fStepIq = 0.05f;
	float32		m_fTemp1 = 0.0f;
	float32		m_fOutput = 0.0f;
	/****************************************************************/
    if(m_fOutIq > 1.0f)
	{
		m_fOutIq = 1.0f;
	}
	else if(m_fOutIq < -1.0f)
	{
		m_fOutIq = -1.0f;
	}
	else
	{
		;
	}

    if(m_fIq_ins > 1.35f)
	{
		m_fIq_ins = 1.35f;
	}
	else if(m_fIq_ins < -1.35f)
	{
		m_fIq_ins = -1.35f;
	}
	else
	{
		;
	}
    /****************************************************************/
	m_fIqError = m_fIq_ins - m_fIq_mea;
	if( m_fIqError > 0.45f)
	{
		m_fIqError = 0.45f;
	}
	else if( m_fIqError < -0.45f )
	{
		m_fIqError = -0.45f;
	}
	else
	{
		;
	}
    /********************** 比例调节*********************************/
	m_fPreSaturatedIq = 1.7f * m_fIqError;
	/****************************************************************/
	if( m_fOutIq > 0.0f )
	{
		if( m_fPreSaturatedIq > m_fOutIq )
		{
			m_fStepIq = 0.05f;
		}
		else if( m_fPreSaturatedIq < m_fOutIq )
		{
			m_fStepIq = 0.14f;
		}
		else
		{
       		m_fStepIq = 0.005f;
		}
	}
	else if( m_fOutIq < 0.0f )
	{
		if( m_fPreSaturatedIq > m_fOutIq )
		{
			m_fStepIq = 0.14f;
		}
		else if( m_fPreSaturatedIq < m_fOutIq )
		{
			m_fStepIq = 0.05f;
		}
		else
		{
			m_fStepIq = 0.005f;
		}
	}
	else
	{
		m_fStepIq = 0.005f;
	}
    /****************************************************************/
	m_fTemp1 = m_fPreSaturatedIq - m_fOutIq;
	/****************************************************************/
	if( m_fTemp1 > m_fStepIq )
	{
		m_fPreSaturatedIq = m_fOutIq + m_fStepIq;
	}
	else if( m_fTemp1 < -m_fStepIq )
	{
		m_fPreSaturatedIq = m_fOutIq - m_fStepIq;
	}
	else
	{
		;
	}
	/****************************************************************/
	if( m_fPreSaturatedIq > 1.0f )
	{
		m_fOutput = 1.0f;
	}
	else if( m_fPreSaturatedIq < -1.0f )
	{
		m_fOutput = -1.0f;
	}
	else
	{
		m_fOutput = m_fPreSaturatedIq;
	}
	return 	m_fOutput;
}
/*end-func*/


/**************************************************************************
 * 函数名称: Pwm_Tabc()
 * 函数功能: 计算PWM开关时间
 * 输参数:
 * 输出参数:
 * 处理过程：@process @process
 **************************************************************************/
void	Pwm_Tabc(float32 m_fIdOut,float32 m_fIqOut,float32 m_fThetaElec)
{
	float32 	m_fVa = 0.0f,		m_fVb = 0.0f,	 m_fVc = 0.0f;
	float32 	m_fTa = 0.0f, 		m_fTb = 0.0f,	 m_fTc = 0.0f;
	float32 	m_fT1 = 0.0f, 		m_fT2 = 0.0f;
	Uint16 		m_uiSector = 0;
	int16 		Theta = 0;
	float32		m_fNalfa = 0.0f;
    float32		m_fNbeta = 0.0f;
	float32		m_fSin = 0.0f;
    float32		m_fCos = 0.0f;
	/*****************************************************************/
    if ( m_fThetaElec > 1.0f )
	{
        m_fThetaElec = 1.0f;
	}
	else if ( m_fThetaElec < 0.0f )
	{
	   m_fThetaElec = 0.0f;
	}
	else
	{
		;
	}
	/*****************************************************************/
	Theta = (int16)(m_fThetaElec * 256);
	m_fSin = sin_tab[Theta];
	if( ( m_fThetaElec + 0.25f ) > 1.0f )
	{
		m_fCos = sin_tab[Theta - 192];
	}
	else
	{
		m_fCos = sin_tab[Theta + 64];
	}
	/****************************************************************/
    if ( m_fIdOut > 1.0f )
	{
        m_fIdOut = 1.0f;
	}
	else if ( m_fIdOut < -1.0f )
	{
	   m_fIdOut = -1.0f;
	}
	else
	{
		;
	}
	if ( m_fIqOut > 1.0f )
	{
        m_fIqOut = 1.0f;
	}
	else if ( m_fIqOut < -1.0f )
	{
	   m_fIqOut = -1.0f;
	}
	else
	{
		;
	}
	/****************************************************************/
	m_fNalfa = m_fIdOut * m_fCos - m_fIqOut * m_fSin;
    m_fNbeta = m_fIqOut * m_fCos + m_fIdOut * m_fSin;
    /****************************************************************/
    if ( m_fNalfa > 1.5f )
	{
        m_fNalfa = 1.5f;
	}
	else if ( m_fNalfa < -1.5f )
	{
	   m_fNalfa = -1.5f;
	}
	else
	{
		;
	}
	if ( m_fNbeta > 1.5f )
	{
        m_fNbeta = 1.5f;
	}
	else if ( m_fNbeta < -1.5f )
	{
	   m_fNbeta = -1.5f;
	}
	else
	{
		;
	}
	/****************************************************************/
	m_fVa = m_fNbeta;
	m_fVb = (-0.5f * m_fNbeta) + 0.8660254f * m_fNalfa;
	m_fVc = (-0.5f * m_fNbeta) - 0.8660254f * m_fNalfa;
	/*****************************************************************/
	if( m_fVa > 0.0f )
	{
		m_uiSector = 1;
	}
	if( m_fVb > 0.0f )
	{
		m_uiSector = m_uiSector + 2;
	}
	if( m_fVc > 0.0f )
	{
		m_uiSector = m_uiSector + 4;
	}
	/*****************************************************************/
	m_fVa = m_fNbeta;
	m_fVb = 0.5f * m_fNbeta + 0.8660254f * m_fNalfa;
	m_fVc = 0.5f * m_fNbeta - 0.8660254f * m_fNalfa;
	/*****************************************************************/
	switch( m_uiSector )
	{
		case 0:
		{
			m_fTa = 0.5f;
			m_fTb = 0.5f;
			m_fTc = 0.5f;
			break;
		}
		/*****************************************************************/
		case 1:
		{
			m_fT1 = m_fVc;
			m_fT2 = m_fVb;
			m_fTb = 0.5f * ( 1 - m_fT1 - m_fT2 );
			m_fTa = m_fTb + m_fT1;
			m_fTc = m_fTa + m_fT2;
			break;
		}
		/*****************************************************************/
		case 2:
		{
			m_fT1 = m_fVb;
			m_fT2 = -m_fVa;
			m_fTa = 0.5f * ( 1 - m_fT1 - m_fT2 );
			m_fTc = m_fTa + m_fT1;
			m_fTb = m_fTc + m_fT2;
			break;
		}
		/*****************************************************************/
		case 3:
		{
			m_fT1 = -m_fVc;
			m_fT2 = m_fVa;
			m_fTa = 0.5f * ( 1 - m_fT1 - m_fT2 );
			m_fTb = m_fTa + m_fT1;
			m_fTc = m_fTb + m_fT2;
			break;
		}
		/*****************************************************************/
		case 4:
		{
			m_fT1 = -m_fVa;
			m_fT2 = m_fVc;
			m_fTc = 0.5f * ( 1 - m_fT1 - m_fT2 );
			m_fTb = m_fTc + m_fT1;
			m_fTa = m_fTb + m_fT2;
			break;
		}
		/*****************************************************************/
		case 5:
		{
			m_fT1 = m_fVa;
			m_fT2 = -m_fVb;
			m_fTb = 0.5f * ( 1 - m_fT1 - m_fT2 );
			m_fTc = m_fTb + m_fT1;
			m_fTa = m_fTc + m_fT2;
			break;
		}
		/*****************************************************************/
		case 6:
		{
			m_fT1 = -m_fVb;
			m_fT2 = -m_fVc;
			m_fTc = 0.5f * ( 1 - m_fT1 - m_fT2 );
			m_fTa = m_fTc + m_fT1;
			m_fTb = m_fTa + m_fT2;
			break;
		}
		/*****************************************************************/

	}
	/*****************************************************************/
	g_fTa = 2.0f *  m_fTa - 1;
	g_fTb = 2.0f *  m_fTb - 1;
	g_fTc = 2.0f *  m_fTc - 1;
	/*********************************************************************/
}
/*end-func*/

